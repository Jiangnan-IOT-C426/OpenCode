
% [Note: ¡°Select_Parameter¡± and :Control_Operator¡± -- the data query command and control command
% T is Chlorophyll a fluorescence control interval ; 
% x is the total experiment time and y is the target chlorophyll a fluorescence signal ]

T = 30;
x=0:T:T*40+T;
for i=1:42
    if i<=21
        y(i)=abs(200*sin(pi*(x(i))/(T*10)))+100; % Target fluorescence signal  
    else
        y(i)=abs(300*sin(pi*(x(i))/(T*10)))+100;
    end
end

kp=0.3; ki=0.2;      % PID parameters 
e_1=0; c_1=5;        % Error and duty cycle initialization
u_max=100;           % integral windup protection parameter
Control_Operator(['Lig_',num2str(c_1)]);%Initialize excitation light intensity
pause(30);

for i=1:42
    t1=clock;           % Timer start
    NowValue(i)=str2double(Select_Parameter("Flour"));%get value
    error = y(i) - NowValue(i);
    if(error>u_max)     % Integral separation
        c = kp * error;
    else
        c = kp * error  + ki * e_1;            % Position PID adjustment
    end
    
    if c<0
        c=0;
    end
    if c>=100
        c=99;
    end  
    Control_Operator(['Lig_',num2str(c)]);
    e_1 = e_1 + error;                  %Error accumulation and integration
    t2=clock;                           % Timer ends
    time_has = etime(t2,t1);
    pause(T-time_has);                  % Timer ends
end