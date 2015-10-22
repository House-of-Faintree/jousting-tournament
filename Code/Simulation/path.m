function X = path(Uc,X_init,Ref,Xdot,dt,T)
    torques = linspace(-1.7,1.7,100);    % potential torque values
    X(:,1) = X_init;
    
    for i = 1:1:T      % over the time        
         X(:,i+1)=rk4step_states(X(:,i),Uc(:,i),dt,Xdot);
    end
    time = 0:dt:dt*T;
    figure(1)
    clf
    
    subplot(4,1,1), plot(time,X(1,:),'c')
    hold on
    plot(time,X(2,:),'g')
    plot(time,X(3,:)-pi/2,'m')  %dev from 90 deg
    legend('x','y','theta from 90','Location','Best')
    plot(T*dt,Ref(1),'co')
    plot(T*dt,Ref(2),'go')
    plot(T*dt,Ref(3)-pi/2,'mo')
    
    subplot(4,1,2), plot(time,X(6,:),'c')
    hold on
    plot(time,X(7,:),'g')
    plot(time,X(8,:),'m')
    legend('xdot','ydot','thetadot','Location','Best')
    plot(T*dt,Ref(6),'co')
    plot(T*dt,Ref(7),'go')
    plot(T*dt,Ref(8),'mo')
    
    subplot(4,1,3), plot(time,X(4,:),'c')
    hold on
    plot(time,X(5,:),'g')
    plot(time,X(9,:),'m')
    plot(time,X(10,:),'r')
    legend('Rphi','Lphi','Rphidot','Lphidot','Location','Best')
    
    %subplot(4,1,4), bar(time(1:end),[Uc';0,0],'grouped')
    subplot(4,1,4), bar(0:dt:dt*(T-1),Uc','grouped')

    %hold on
    %bar(time(1:end-1),Uc(2,:),'g','grouped')
    legend('R tau','L tau','Location','Best')

end
