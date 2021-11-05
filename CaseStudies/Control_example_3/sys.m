%The following is the photosynthesis model composed of 7 state variables, after Feng et al. (2018).


function dy=sys(y,b,u)
dy=zeros(6,1);
dy(1)=u*b(1)*(290-y(1))-b(2)*y(1)-b(3)*(1+y(6))*y(1)-b(4)*(1-y(2))*y(1)+b(5)*y(2)*(290-y(1));
dy(2)=b(4)*y(1)*(1-y(2))-b(5)*y(2)*(290-y(1))-b(6)*y(2)*(1-y(3)-y(4))+b(7)*(1-y(2))*y(3)-b(8)*y(2)*y(3)+b(9)*(1-y(2))*y(4);
dy(3)=b(6)*y(2)*(1-y(3)-y(4))-b(7)*(1-y(2))*y(3)-b(8)*y(2)*y(3)+b(9)*(1-y(2))*y(4);
dy(4)=b(8)*y(2)*y(3)-b(9)*y(4)*(1-y(2))-b(10)*y(4)*y(5);
dy(5)=-b(10)*y(4)*y(5)+b(11)*(b(14)-y(5));
dy(6)=b(12)*y(7)*(1-y(6))-b(13)*y(6);
dy(7)=b(4)*y(1)*(1-y(2))+b(11)*(b(14)-y(5));
