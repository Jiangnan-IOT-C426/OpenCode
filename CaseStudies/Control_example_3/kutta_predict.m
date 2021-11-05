function [ys,F]=kutta_predict(u,b,yx)
Num=30;
y=zeros(7,100000*Num);
y(:,1)=yx;
h=0.00001;
for ii=1:100000*Num-1
    k1=feval(@sys,y(:,ii),b,u);
    k2=feval(@sys,y(:,ii)+h*k1/2,b,u);
    k3=feval(@sys,y(:,ii)+h*k2/2,b,u);
    k4=feval(@sys,y(:,ii)+h*k3,b,u); 
    y(:,ii+1)=y(:,ii)+h*(k1+2*k2+2*k3+k4)/6;            
end
ys=y(:,100000*Num);
F=114.881863962529*b(2)*ys(1)*2;              



    
