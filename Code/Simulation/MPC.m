%% Model Predictive Control for Jousting Robot- Take 1
% by Lydia Drabsch
% 18/10/15
% other functions required: ControlInputs.m path.m
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

T = @(U) [0,0,0,U(1),U(2)]';

%lambda = @(X) -inv(C*inv(M)*transpose(C))*(C*inv(M)*T +Cdqd);

lambda_s =@(X,U) [mt*X(8)*(X(6)*cos(X(3))+X(7)*sin(X(3)));...% simplified sin^2+cos^2 = 1;
            ((Iw*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3)))) - R*U(2)/Iw * (It-L^2*mt)) - Iw*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3))) - R*U(1)/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2);...
            ((Iw*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3)))) - R*U(1)/Iw * (It-L^2*mt)) - Iw*(X(8)*(X(7)*cos(X(3))-X(6)*sin(X(3))) - R*U(2)/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2)];
    
%lagrange =@(X) inv(M)*transpose(C)*lambda_s;
%subs(lagrange, [mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL])

%lambdaA = subs(lambda,[cos(theta),sin(theta)],[0,1]); % theta = 90, equilibrium cond, due to floating points cannot sub in for theta directly

%% Dynamics
% X = x,y,theta,Rphi,Lphi,xd,yd,thetad,Rphid,Lphid     u = Tr,Tl

qdd =@(X,U) inv(M)*transpose(C(X))*lambda_s(X,U)+inv(M)*T(U);
%qdd =@(X) subs(qdd,[mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL]);
%qdd = subs(qdd,[theta,xd,yd,thetad],[@X(3),@X(6),@X(7),@X(8)])

Xdot =@(X,U) [X(6:10);qdd(X,U)]
%  initial x at zero
 X_init = zeros(10,1);
dt = 0.1; % time step (seconds?)
T = 9; % horizion
N = 10; % number of controls to try
spread = 1;
spreadset = 1;  % 10^-spreadset decrement
Cost_old = 100000; % infinite inital cost

% Desired X
Ref = [0.05;NaN;0;NaN;NaN;0.4;0;NaN;NaN;NaN];    % travelling in x direction
Q = [30;NaN;1;NaN;NaN;3;1;NaN;NaN;NaN];   % weighted cost
torques = linspace(0,1.7,100);    % potential torque values
X(:,1) = X_init;

% random control?
U_init = randi(100,2,T);
centre = U_init;
STOP = 0;

while(STOP==0)       % or a while loop? until convergance?
    Upot = ControlInputs(centre,spread,N,T);  % structure of controls
    Cost = zeros(1,N);
for j = 1:1:N % over each control

    eval(['Uc = Upot.i' num2str(j) ''])
    for i = 1:1:T      % over the time
        % calculate cost using least squares
        Cost(j) = Cost(j) + nansum(Q.*(X(:,i)-Ref).^2);   % maybe weighted costs?
        
        % integrate over time step
        X(:,i+1) = X(:,i) + dt*Xdot(X(:,i),torques(Uc(:,i)));
    end
       Cost(j) = Cost(j) + nansum((X(:,T)-Ref).^2)  % add final cost NOTE: no cost on control
   
   
end

 [Cost_new,index] = min(Cost);
   
   if Cost_new < Cost_old
       Cost_old = Cost_new;
        eval(['centre = Upot.i' num2str(index) ';'])
        %spread = spread-10^(-spreadset);  % error if less than 0?
        frac = 1;
   else
       frac = 0.5;
       %spread = spread-0.5*10^(-spreadset);  % error if less than 0?
       % try again? minimise spread?
   end

   if spread<0.15
            spread = spread-frac*0.01;
            %spreadset = 2;
   elseif spread > -0.005 || spread <0.005
       STOP = 1;
   else
            spread = spread-frac*0.1;
   end
   
end

 path(centre,X_init,Ref,Xdot,dt,T)