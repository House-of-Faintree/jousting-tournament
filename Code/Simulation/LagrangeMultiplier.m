clear
%% EOM
vIw = 1;
vIt = 1;
vR = 1;
vL = 1;
vmt = 1;
vmc = 1;

syms theta mt mc L R It Iw c thetad xd yd Tr Tl

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

lambda_s = [mt*thetad*(xd*cos(theta)+yd*sin(theta));...% simplified sin^2+cos^2 = 1;
            ((Iw*(thetad*(yd*cos(theta)-xd*sin(theta))) - R*Tl/Iw * (It-L^2*mt)) - Iw*(thetad*(yd*cos(theta)-xd*sin(theta)) - R*Tr/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2);...
            ((Iw*(thetad*(yd*cos(theta)-xd*sin(theta))) - R*Tr/Iw * (It-L^2*mt)) - Iw*(thetad*(yd*cos(theta)-xd*sin(theta)) - R*Tl/Iw * (Iw*mt*L^2+It*mt*R^2+It*Iw)))/(It*R^4*mt+4*Iw^2*L^2+2*Iw*L^2*R^2*mt+2*It*Iw*R^2)]
    
lagrange = inv(M)*transpose(C)*lambda_s;
%subs(lagrange, [mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL])

%lambdaA = subs(lambda,[cos(theta),sin(theta)],[0,1]); % theta = 90, equilibrium cond, due to floating points cannot sub in for theta directly

%% Dynamics

qdd = transpose(C)*lambda_s+T
subs(qdd,[mt,mc,It,Iw,R,L],[vmt,vmc,vIt,vIw,vR,vL])

