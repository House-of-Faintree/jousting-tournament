clear
%% EOM
% http://www.omicsgroup.org/journals/dynamic-modelling-of-differentialdrive-mobile-robots-using-lagrange-and-newtoneuler-methodologies-a-unified-framework-2168-9695.1000107.pdf
% http://robotics.caltech.edu/~me72/class/DiffDrive2.pdf
% http://web.mit.edu/jlramos/www/Arquivos/ReportDifferentialDrive.pdf
% http://www.intechopen.com/books/frontiers-of-model-predictive-control/a-real-time-gradient-method-for-nonlinear-model-predictive-control
vIw = 1;
vIt = 1;
vR = 1;
vL = 1;
vmt = 1;
vmc = 1;

syms theta mt mc L R It Iw c thetad xd yd Tr Tl Rphid Lphid Rphi Lphi alpha beta gamma

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


C = [-sin(theta), cos(theta), 0, 0, 0;...
    cos(theta), sin(theta), L, -R,0;...
    cos(theta), sin(theta), -L,0,-R];

Cdqd = [-thetad*(xd*cos(theta)+yd*sin(theta));...
        -thetad*(xd*sin(theta)-yd*cos(theta));...
        -thetad*(xd*sin(theta)-yd*cos(theta))];

T = transpose([0,0,0,Tr,Tl]);

lambda = -inv(C*inv(M)*transpose(C))*(C*inv(M)*T +Cdqd);

% lambda_s = [mt*thetad*(xd*cos(theta)+yd*sin(theta));...% simplified sin^2+cos^2 = 1;
%             ((Iw*(thetad*(yd*cos(theta)-xd*sin(theta))) - R*Tl/Iw * (It-L^2*mt)) - Iw*(thetad*(yd*cos(theta)-xd*sin(theta)) - R*Tr/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2);...
%             ((Iw*(thetad*(yd*cos(theta)-xd*sin(theta))) - R*Tr/Iw * (It-L^2*mt)) - Iw*(thetad*(yd*cos(theta)-xd*sin(theta)) - R*Tl/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2)];
%     
%alpha = Iw*mt*L^2+It*mt*R^2+It*Iw;
%beta = It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2;
% gamma = It-L^2*mt
lambda_s = [mt*thetad*(xd*cos(theta)+yd*sin(theta));...% simplified sin^2+cos^2 = 1;
            ((Iw^2*(thetad*(yd*cos(theta)-xd*sin(theta))) - R*Tl/Iw * (gamma)) - Iw*(thetad*(yd*cos(theta)-xd*sin(theta)) - R*Tr/Iw * (alpha)))/(beta);...
            ((Iw^2*(thetad*(yd*cos(theta)-xd*sin(theta))) - R*Tr/Iw * (gamma)) - Iw*(thetad*(yd*cos(theta)-xd*sin(theta)) - R*Tl/Iw * (alpha)))/(beta)];        
        
lagrange = transpose(C)*lambda_s;

lagrange_s = [2*thetad*(yd*cos(theta)^2-xd*cos(theta)*sin(theta))*(Iw^2-Iw)/beta+cos(theta)*R*(Tl+Tr)*(alpha-gamma/Iw)/beta-mt*thetad*(xd*sin(theta)*cos(theta)+yd*sin(theta)^2);...
            mt*thetad*(xd*cos(theta)^2+yd*cos(theta)*sin(theta))+2*thetad*(yd*cos(theta)*sin(theta)-xd*sin(theta)^2)/beta*(Iw^2-Iw)+sin(theta)*R*(Tr+Tl)*(alpha-gamma/Iw)/beta;...
            L*R*(Tr-Tl)/beta*(gamma/Iw+alpha);...
            R*thetad*(yd*cos(theta)-xd*sin(theta))/beta*(Iw-Iw^2)-R^2*Tr*alpha/beta+R^2*Tl*gamma/(Iw*beta);...
            R*thetad*(yd*cos(theta)-xd*sin(theta))/beta*(Iw-Iw^2)-R^2*Tl*alpha/beta+R^2*Tr*gamma/(Iw*beta)]
%subs(lagrange, [mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL])

%lambdaA = subs(lambda,[cos(theta),sin(theta)],[0,1]); % theta = 90, equilibrium cond, due to floating points cannot sub in for theta directly

%% Dynamics
% X = x,y,theta,Rphi,Lphi,xd,yd,thetad,Rphid,Lphid     u = Tr,Tl

qdd = inv(M)*transpose(C)*lambda_s+inv(M)*T;
%qdd = subs(qdd,[mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL]);
%qdd = subs(qdd,[theta,xd,yd,thetad],[@X(3),@X(6),@X(7),@X(8)])

Xdots = [xd;yd;thetad;Rphid;Lphid;]


%% gradient method MPC

dXd1_dtheta = 2*thetad*(Iw-Iw^2)*(yd*sin(2*theta)+xd*cos(2*theta))/beta - sin(theta)*R*(Tr+Tl)*(alpha+gamma/Iw)/beta - mt*thetad*(xd*cos(2*theta)+yd*sin(2*theta));

dXd1_dxd = -thetad*cos(theta)*sin(theta)*(2*(Iw^2-Iw)/beta +mt);

dXd1_dyd = 2*thetad*cos(theta)^2/beta - mt*thetad*sin(theta)^2;

dXd1_dthetad = 2*(Iw^2-Iw)*(yd*cos(theta)^2-xd*cos(theta)*sin(theta))/beta - mt*(yd*sin(theta)^2 + xd*cos(theta)*sin(theta));

dXd2_dtheta = 2*thetad*(Iw^2-Iw)*(yd*cos(2*theta)-xd*sin(2*theta))/beta + cos(theta)*R*(Tr+Tl)*(alpha+gamma/Iw)/beta + mt*thetad*(-xd*sin(2*theta)+yd*cos(2*theta));

dXd2_dxd = -2*thetad*(Iw^2-Iw)/beta*sin(theta)^2 + mt*thetad*cos(theta)^2;

dXd2_dyd = 2*thetad*(Iw^2-Iw)*cos(theta)*sin(theta)/beta + mt*thetad*cos(theta)*sin(theta);

dXd2_dthetad = 2*(Iw^2-Iw)/beta*(yd*cos(theta)*sin(theta)-xd*sin(theta)^2) + mt*(xd*cos(theta)^2+yd*cos(theta)*sin(theta));

dXd4_dtheta = -R*thetad*(Iw-Iw^2)*(yd*sin(theta)+xd*cos(theta))/beta;

dXd4_dxd = -R*thetad*sin(theta)*(Iw-Iw^2)/beta;

dXd4_dyd = R*thetad*cos(theta)*(Iw-Iw^2)/beta;

dXd4_dthetad = R*(yd*cos(theta)-xd*sin(theta))*(Iw-Iw^2)/beta;

% horizontal = constant differential variable, vertical = equation
LdXd_dX = [0,0,0,0,0;...
            0,0,0,0,0;...
            dXd1_dtheta,dXd2_dtheta,0,dXd4_dtheta,dXd4_dtheta;...
            0,0,0,0,0;...
            0,0,0,0,0;...
            dXd1_dxd,dXd2_dxd,0,dXd4_dxd,dXd4_dxd;...
            dXd1_dyd,dXd2_dyd,0,dXd4_dyd,dXd4_dyd;...
            dXd1_dthetad,dXd2_dthetad,0,dXd4_dthetad,dXd4_dthetad;...
            0,0,0,0,0;...
            0,0,0,0,0];
        
HdXd_dX = [eye(5);zeros(5)]    ;

dXd_dX = [HdXd_dX,LdXd_dX]

%% Convert to X vector




