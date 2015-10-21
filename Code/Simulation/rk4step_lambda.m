function Lout=rk4step_lambda(lambda,dt,dlambda,X,U)

% Perform one step of RK4 for the ODE dx/dt=f(x,t).  
% Routine adapted from Garcia, Numerical Methods in Physics.
%
% Inputs:
% X - current state of dependent variables
% U - current index of controls which are constant over the time interval
% dt - step size
% Xdot - anonomus function of the rate of change of variables, fn of (X,U)
% torques - transform index of controls to torque

% Outputs:
% Xout - new value of x after a step of size tau

htau=0.5*dt;

f1= dlambda(X,lambda,U);  % eval at t    NOTE: X not time invarient??
f1 = noNaN(f1');

Ltemp=lambda+htau*f1;
f2=dlambda(X,Ltemp,U);  % eval at t+0.5tau
f2 = noNaN(f2');

Ltemp=lambda+htau*f2;
f3=dlambda(X,Ltemp,U);  % eval at t+0.5tau
f3 = noNaN(f3');

Ltemp=lambda+dt*f3;
f4=dlambda(X,Ltemp,U); % eval at t+tau
f4 = noNaN(f4');

Lout=lambda+dt*(f1+2*f2+2*f3+f4)/6;

end