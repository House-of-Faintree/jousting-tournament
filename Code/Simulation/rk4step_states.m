function Xout=rk4step_states(X,U,dt,Xdot,torques)

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

f1= Xdot(X,torques(U));  % eval at t

Xtemp=X+htau*f1;
f2=Xdot(Xtemp,torques(U));  % eval at t+0.5tau

Xtemp=X+htau*f2;
f3=Xdot(Xtemp,torques(U));  % eval at t+0.5tau

Xtemp=X+dt*f3;
f4=Xdot(Xtemp,torques(U)); % eval at t+tau

Xout=X+dt*(f1+2*f2+2*f3+f4)/6;

end