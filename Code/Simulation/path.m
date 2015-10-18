function path(Uc,X_init,Ref,Xdot,dt,T)
    torques = linspace(0,1.7,100);    % potential torque values
    X(:,1) = X_init;
    
    for i = 1:1:T      % over the time        
        % integrate over time step
        X(:,i+1) = X(:,i) + dt*Xdot(X(:,i),torques(Uc(:,i)));
    end
    time = 1:1:(T+1);
    figure(1)
    clf
    
    subplot(4,1,1), plot(time,X(1,:),'c')
    hold on
    plot(time,X(2,:),'g')
    plot(time,X(3,:),'m')
    legend('x','y','theta','Location','Best')
    plot(T+1,Ref(1),'co')
    plot(T+1,Ref(2),'go')
    plot(T+1,Ref(3),'mo')
    
    subplot(4,1,2), plot(time,X(6,:),'c')
    hold on
    plot(time,X(7,:),'g')
    plot(time,X(8,:),'m')
    legend('xdot','ydot','thetadot','Location','Best')
    plot(T+1,Ref(6),'co')
    plot(T+1,Ref(7),'go')
    plot(T+1,Ref(8),'mo')
    
    subplot(4,1,3), plot(time,X(4,:),'c')
    hold on
    plot(time,X(5,:),'g')
    plot(time,X(9,:),'m')
    plot(time,X(10,:),'r')
    legend('Rphi','Lphi','Rphidot','Lphidot','Location','Best')
    
    subplot(4,1,4), bar(time(1:end-1),Uc','grouped')
    %hold on
    %bar(time(1:end-1),Uc(2,:),'g','grouped')
    legend('R tau','L tau','Location','Best')

end
