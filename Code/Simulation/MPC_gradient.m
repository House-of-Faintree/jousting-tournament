%% Model Predictive Control for Jousting Robot- Take 2
% by Lydia Drabsch
% 18/10/15
% other functions required: ControlInputs.m path.m rk4step_sattes.m
% rk4step_lambda.m

% Edit 19/10: better integration over sub timesteps, included acceleration
% in displacement variables. Fixed reference vector. Included control in
% cost. Working much better but still not 'optimal' control, just best random values.

% NOTES: control selection random over all timesteps, is there a way to
% make it more uniform? This random selection results in the cost being
% massive. 
% Idea: Maybe the total displacement between timesteps of U can only be
% so much? making a more continuous control. How to randomise to find global minima?
% not so random initial guess?

% Idea: control is still being set at extremes? not random fn but a gaussian?
% might get stuck at local minima. need to keep in mind C conversion, no
% guassian fn? maybe control spread more than displacement in
% ControlInputs.m

% Idea: change cost algorithm, cost is the change of state from current? and the 
% final cost from the desired state? would it take the whole time to get there then?
% maybe keep as is and then it wants to stay there - current control randomisation 
% doesnt work that way

% - Only the optimisation code, feedback has not been added.
% - Absolute constraints on state has not been added.
% - make better plotting code to see the xy plane of the robot? vector sizes
% for velocity vector?
% - how to decide on convergence without running all the way though

% Analysis:
% xdot ydot are approaching reference point well
% x and theta are consistantly too large, even with weighted cost fn. Maybe
% something wrong with integration? the x, y and theta do look very smooth with
% seemingly constant acceleration

clear
clc
%% EOM
Iw = 1;
It = 1;
R = 1;
L = 1;
mt = 1;
mc = 1;
alpha = Iw*mt*L^2+It*mt*R^2+It*Iw;
beta = It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2;
gamma = It-L^2*mt;
%syms theta mt mc L R It Iw c thetad xd yd Tr Tl Rphid Lphid Rphi Lphi

% M = [mt,   0,  -mc*c*sin(theta),   0,  0;...
%     0,      mt, mc*c*cos(theta),    0,  0;...
%     -mc*c*sin(theta), mc*c*cos(theta), It,0,0;...
%     0,      0,  0,                  Iw, 0;...
%     0,      0,  0,                  0,  Iw]

M = [mt,   0,  0,   0,  0;...
    0,      mt, 0,    0,  0;...
    0,      0, It,  0,  0;...
    0,      0,  0,                  Iw, 0;...
    0,      0,  0,                  0,  Iw];


C = @(X) [-sin(X(3)), cos(X(3)), 0, 0, 0;...
    cos(X(3)), sin(X(3)), L, -R,0;...
    cos(X(3)), sin(X(3)), -L,0,-R];



Cdqd = @(X) [-X(8)*(X(6)*cos(X(3))+X(7)*sin(X(3)));...
        -X(8)*(X(6)*sin(X(3))-X(7)*cos(X(3)));...
        -X(8)*(X(6)*sin(X(3))-X(7)*cos(X(3)))];

Tau = @(U) [0,0,0,U(1),U(2)]';

%lambda = @(X) -inv(C*inv(M)*transpose(C))*(C*inv(M)*T +Cdqd);

lambda_s =@(X,U) [mt*X(8)*(X(6)*cos(X(3))+X(7)*sin(X(3)));...% simplified sin^2+cos^2 = 1;
            ((Iw^2*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3)))) - R*U(2)/Iw * (It-L^2*mt)) - Iw*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3))) - R*U(1)/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2);...
            ((Iw^2*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3)))) - R*U(1)/Iw * (It-L^2*mt)) - Iw*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3))) - R*U(2)/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2)];

dXd1_dtheta =@(X,U) 2*X(8)*(Iw-Iw^2)*(X(7)*sin(2*X(3))+X(6)*cos(2*X(3)))/(mt*beta) - sin(X(3))*R*(U(1)+U(2))*(alpha+gamma/Iw)/(mt*beta) - X(8)*(X(6)*cos(2*X(3))+X(7)*sin(2*X(3)));

dXd1_dxd =@(X,U) -X(8)*cos(X(3))*sin(X(3))*(2*(Iw^2-Iw)/(mt*beta) +1);

dXd1_dyd =@(X,U) 2*X(8)*cos(X(3))^2/(mt*beta) - X(8)*sin(X(3))^2;

dXd1_dthetad =@(X,U) 2*(Iw^2-Iw)*(X(7)*cos(X(3))^2-X(6)*cos(X(3))*sin(X(3)))/(mt*beta) - (X(7)*sin(X(3))^2 + X(6)*cos(X(3))*sin(X(3)));

dXd2_dtheta =@(X,U) 2*X(8)*(Iw^2-Iw)*(X(7)*cos(2*X(3))-X(6)*sin(2*X(3)))/(mt*beta) + cos(X(3))*R*(U(1)+U(2))*(alpha+gamma/Iw)/(mt*beta) + X(8)*(-X(6)*sin(2*X(3))+X(7)*cos(2*X(3)));

dXd2_dxd =@(X,U) -2*X(8)*(Iw^2-Iw)/(mt*beta)*sin(X(3))^2 + X(8)*cos(X(3))^2;

dXd2_dyd =@(X,U) 2*X(8)*(Iw^2-Iw)*cos(X(3))*sin(X(3))/(mt*beta) + X(8)*cos(X(3))*sin(X(3));

dXd2_dthetad =@(X,U) 2*(Iw^2-Iw)/(mt*beta)*(X(7)*cos(X(3))*sin(X(3))-X(6)*sin(X(3))^2) + (X(6)*cos(X(3))^2+X(7)*cos(X(3))*sin(X(3)));

dXd4_dtheta =@(X,U) -R*X(8)*(1-Iw)*(X(7)*sin(X(3))+X(6)*cos(X(3)))/beta;

dXd4_dxd =@(X,U) -R*X(8)*sin(X(3))*(1-Iw)/beta;

dXd4_dyd =@(X,U) R*X(8)*cos(X(3))*(1-Iw)/beta;

dXd4_dthetad =@(X,U) R*(X(7)*cos(X(3))-X(6)*sin(X(3)))*(1-Iw)/beta;

% horizontal = constant differential variable, vertical = equation
LdXd_dX =@(X,U) [0,0,0,0,0;...
            0,0,0,0,0;...
            dXd1_dtheta(X,U),dXd2_dtheta(X,U),0,dXd4_dtheta(X,U),dXd4_dtheta(X,U);...
            0,0,0,0,0;...
            0,0,0,0,0;...
            dXd1_dxd(X,U),dXd2_dxd(X,U),0,dXd4_dxd(X,U),dXd4_dxd(X,U);...
            dXd1_dyd(X,U),dXd2_dyd(X,U),0,dXd4_dyd(X,U),dXd4_dyd(X,U);...
            dXd1_dthetad(X,U),dXd2_dthetad(X,U),0,dXd4_dthetad(X,U),dXd4_dthetad(X,U);...
            0,0,0,0,0;...
            0,0,0,0,0];
        
HdXd_dX = [eye(5);zeros(5)]    ;

dXd_dX =@(X,U) [HdXd_dX,LdXd_dX(X,U)]        
        
dXd1_dTr =@(X) cos(X(3))*R*(alpha - gamma/Iw)/(beta*mt );

dXd1_dTl =@(X) cos(X(3))*R*(alpha - gamma/Iw)/(mt*beta);

dXd2_dTr =@(X) sin(X(3))*R*(alpha-gamma/Iw)/(mt*beta);

dXd2_dTl =@(X) sin(X(3))*R*(alpha-gamma/Iw)/(mt*beta);

dXd3_dTr = L*R/(beta*It)*(gamma/It+alpha);

dXd3_dTl = -L*R/(beta*It)*(gamma/It+alpha);

dXd4_dTr = - R^2*alpha/(Iw*beta);

dXd4_dTl = R^2*gamma/(Iw^2*beta);

dXd5_dTr = R^2*gamma/(Iw^2*beta);

dXd5_dTl = - R^2*alpha/(Iw*beta);

dXd_dU = @(X)[zeros(5,2);...
        dXd1_dTr(X),dXd1_dTl(X);...
        dXd2_dTr(X),dXd2_dTl(X);...
        dXd3_dTr,dXd3_dTl;...
        dXd4_dTr,dXd4_dTl;...
        dXd5_dTr,dXd5_dTl];

%lagrange =@(X) inv(M)*transpose(C)*lambda_s;
%subs(lagrange, [mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL])

%lambdaA = subs(lambda,[cos(theta),sin(theta)],[0,1]); % theta = 90, equilibrium cond, due to floating points cannot sub in for theta directly

%% Dynamics
% X = x,y,theta,Rphi,Lphi,xd,yd,thetad,Rphid,Lphid     u = Tr,Tl

qdd =@(X,U) inv(M)*transpose(C(X))*lambda_s(X,U)+inv(M)*Tau(U);
%qdd =@(X) subs(qdd,[mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL]);
%qdd = subs(qdd,[theta,xd,yd,thetad],[@X(3),@X(6),@X(7),@X(8)])

Xdot =@(X,U) [X(6:10);qdd(X,U)]
%  initial x at zero
X_init = zeros(10,1);
X_init(3) = pi/2; % travelling in y direction start orientated


dt = 0.1; % time step (seconds?)
subdt = dt/10;

T = 5; % horizion
N = 10; % number of controls to try
spread = 1;
spreadset = 1;  % 10^-spreadset decrement
Cost_old = 100000; % infinite inital cost

% Desired X
% Ref = [0.05;NaN;0;NaN;NaN;0.4;0;NaN;NaN;NaN];    % travelling in x direction
Ref = [0.2;NaN;pi/2;NaN;NaN;0;0.2;0;NaN;NaN];    % travelling in y direction, x displacement, theta=90

Q = [300;NaN;100;NaN;NaN;10;30;30;NaN;NaN];   % weighted cost
R = [1;1];

% cost gradient in state space
%Hx = Q.*2*(X-Ref)+dV_dX'*dXd_dX(X,U)
% Hy = R.*2.*U + dV_dU'*dXd_dU(X)+dV_dU'*[0;0;0;0;0;0;0;0;1/Iw;1/Iw];

torques = linspace(-1.7,1.7,101);    % potential torque values
X(:,1) = X_init;

% random control?
% U_init = randi(round(200),2,T)-round(100)
U_init = 70*ones(2,T);
centre = U_init;
STOP = 0;

while(STOP==0)       % or a while loop? until convergance?
    %Upot = ControlInputs(centre,spread,N,T);  % structure of controls
    Cost = zeros(1,N);
for j = 1:1:N % over each control
    Uc = U_init;
    %eval(['Uc = Upot.i' num2str(j) ';'])
    for i = 1:1:T      % over time for control inputs
%        for t = 1:1:10
        % calculate cost using least squares
        Cost(j) = Cost(j) + nansum(Q.*(X(:,i)-Ref).^2) + sum((torques(Uc(:,i))).^2);   % maybe weighted costs?    
        % integrate over time step
        
        X(:,i+1)=rk4step_states(X(:,i),Uc(:,i),dt,Xdot,torques);
%        end
    end
    Xfinal = X(:,i+1);
    
    dV_dX = 2*(Xfinal-Ref);  % final cost partial derivative wrt X - replace Nans with 0's?
    dV_dX(isnan(dV_dX)) = 0;
    dlambdaf = -((Q.*2.*(Xfinal-Ref))'+dV_dX'*transpose(dXd_dX(Xfinal,Uc)))  % wrote the matrix wrong, must be transpose
    dlambda = @(X,lambda,U)  -((Q.*2.*(X-Ref))'+lambda'*transpose(dXd_dX(X,U)))
    
%     Hy = R.*2.*U + dV_dU'*dXd_dU(X)+dV_dU'*[0;0;0;0;0;0;0;0;1/Iw;1/Iw];
    Search = @(X,lambda,U) -((R.*2.*U)' + lambda'*dXd_dU(X)+lambda'*[0,0;0,0;0,0;0,0;0,0;0,0;0,0;0,0;1/Iw,0;0,1/Iw])
    
    % runge kutta not exactly time reversable? Leapfrog method instead?
    % integrate dlambda backwards in time and store lambda for each
    % timestep
    lambda = zeros(10,T);
    lambda(:,T) = Q.*(Xfinal-Ref).^2; % final cost
    lambda = noNaN(lambda);
    
    for ib = T:-1:2
        % search direction
        s(:,ib) = Search(X(:,ib),lambda(:,ib),Uc(:,ib))
        
        lambda(:,ib-1) = rk4step_lambda(lambda(:,ib),-dt,dlambda,X(:,ib),Uc(:,ib))   % need to pass Ref,dXd_dX? are NAN's going to be a problem?
       
    end
    s(:,1) = Search(X(:,1),lambda(:,1),Uc(:,1)) % final search
%     dV_dU = 2.*R.*U;
%     Hy = R.*2.*U + dV_dU'*dXd_dU(X)+dV_dU'*[0;0;0;0;0;0;0;0;1/Iw;1/Iw];
    
       Cost(j) = Cost(j) + 10*nansum((X(:,T*10)-Ref).^2);  % add final cost NOTE: no cost on control
                                             % final cost is weighted??
   % integrate backwards in time
   %Hx = Q.*2*(X-Ref)+dV_dx'*dXd_dX(X,U)
   
   
   
   
end

 [Cost_new,index] = min(Cost);
   
   if Cost_new < Cost_old
       Cost_old = Cost_new;
        eval(['centre = Upot.i' num2str(index) ';'])
        %spread = spread-10^(-spreadset);  % error if less than 0?
        frac = 0.5;
   else
       frac = 0.2;
       %spread = spread-0.5*10^(-spreadset);  % error if less than 0?
       % try again? minimise spread?
   end

   if spread <= 0.15
            spread = spread-frac*0.01;
            %spreadset = 2;
   elseif spread > 0.15
            spread = spread-frac*0.1;
   end
   
   if spread < 0.01 && Cost_new < 600
       STOP = 1;
   elseif spread <= 0.01
       spread = 0.01;
       
   end
   
end

 Xfinal = path(centre,X_init,Ref,Xdot,dt,subdt,T)