%[Note: JH is the Jacobian matrix of the measurement equation.]

function J=JacobianH(b)
J=zeros(1,7);
J(1)=114.881863962529*b(2);