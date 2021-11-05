%[ Note: T is for QA- control interval;
%xx is the total experiment time; 
%y_SetValue is the target QA- signal; 
%b is the photosynthesis model parameter;
%Q, R and P0 are the Extended Kalman filter initial values; 
%x(:,1) is the photosynthesis model initial value; 
%and x_ekf is used to store the state estimate value at all control moments.]



T = 30;
xx=0:T:T*41;
for i=1:42
    if i<=21
         y(i)=0.3+abs(0.6*sin(pi*(xx(i))/(T*10)));
    elseif i>=22 && i<=42
         y(i)=0.3+abs(0.7*sin(pi*(xx(i))/(T*10)));
    end
end
b=[1.26063,0.0770650,6506.235084,27425.6909792,14.3284301,666.6491,803.8881,15307,195.18555,...
   9.8449800,9.30757433229272e-05,0.00626064729861041,581.772636716396,1.20227403646062];      % Photosynthesis model parameters
Q=(2.5000e-08)*eye(7);                       % Extended Kalman filter 
R=2.5000e-08;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      ; 
P0=1e-7*eye(7);
x(:,1)=[0 0 0 0 1.20227403646062 0 0];      
x_ekf=zeros(7,43);                         
kp=21;ki=21;                                            % PID parameters
e_1=0;u=1;                                              % Error and duty cycle initialization
u_max=100;                                              % integral windup protection parameter
[x_ekf(:,1),F_pred]=kutta_predict(u,b,x(:,1));         % Initial value of the prediction output
for ii=2:43
    t1=clock;       					               % Timer start
    [x_pred,F_pred]=kutta_predict(u,b,x_ekf(:,ii-1));  % Predictive output
    F=JacobianF(u,b,x_ekf(:,ii-1));       		       % Jacobian matrix of state equation
    P=F*P0*F'+Q;                                       % Process variance prediction    
    H=JacobianH(b);          		                   % Jacobian matrix of measurement equation
    S=H*P*H'+R;
    K=P*H'/S;                                           % Kalman gain    
    F_obs(ii) = str2double (Select_Parameter("Flour"))/1000; % Measured fluorescence
    x_ekf(:,ii)=x_pred+K*(F_obs(ii)-F_pred);       % State estimate of EKF
    P0=(eye(7)-K*H)*P;                              % Variance of EKF
    y_QA(ii-1)=x_ekf(2,ii); 					    % Estimated QA       
    error=y(ii-1)-x_ekf(2,ii);
        if(error>u_max)                             % Integral separation
            u = kp * error;
        else
            u = kp * error  + ki * e_1;            % Position PID adjustment
        end
    if u<=0
        u=1;
    end
    if u>=100
        u=99;
    end  
    Control_Operator(['Lig_',num2str(u)]);
    e_1 = e_1 + error;                      %Error accumulation and integration
    t2=clock; 						        % Timer ends
    time_has = etime(t2,t1);
    pause(T-time_has);                      % Time interval 30s
end
