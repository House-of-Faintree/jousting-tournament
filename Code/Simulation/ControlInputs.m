function U = ControlInputs(centre,spread,N,T)
    % centre is the last minimum control 2xT matrix (indicies of torques)
    % spread is how large the purturbations are on the centre value (is
    % this approching convergence artifically?) percentage
    % N is the number of control sets
    % T is horizion
    
    for i = 1:1:N
        control = centre + randi(round(200*spread),2,T)-round(100*spread);
        control(control<1) = 1;  % max and min indicies
        control(control>100) = 100;
        eval(['U.i' num2str(i) '=control;'])
    end
    
end