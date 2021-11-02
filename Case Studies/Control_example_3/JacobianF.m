function J=JacobianF(u,b,x)
y=zeros(7,7);
J=zeros(7,7);
for i=1:7
    y(:,i)=x;
    y(i,i)=x(i)+0.000000001;
end

for i=1:7
    k1=sys(y(:,i),b,u);
    k2=sys(y(:,i)+0.00001*k1/2,b,u);
    k3=sys(y(:,i)+0.00001*k2/2,b,u);
    k4=sys(y(:,i)+0.00001*k3,b,u);
    J(:,i)=(k1+2*k2+2*k3+k4)/6;
end


