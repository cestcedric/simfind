function c = cross_efficient(a,b)
%gives a column vector

c(1,1)= a(2)*b(3)-a(3)*b(2);
c(2,1)= a(3)*b(1)-a(1)*b(3);
c(3,1)= a(1)*b(2)-a(2)*b(1);