function [left,right] = jintuifa(matrix,x0,step)
%进退法，用于确定下单峰区间.根据最优化方法（天津大学出版社）20页算法1.4.3编写。
%v1.0 author: liuxi BIT
%[left right]为下单峰区间，y为函数，x为函数y的变量，x0为初始点（默认为0）,step（>0）为初始步长（默认为0.01）

for i=1:1:length(x0)
    
f0=matrix(2,x0(i)); %step1 求f(x0) 将函数y中变量x替换为x0
x1(i)=x0(i)+step(i);%step2
f1=matrix(2,x1(i));

if (f1>=f0)%step3 step4    % 两个点确定上坡不严谨，如果一脚踩沟里呢
    step(i)=2*step(i);
    x2(i)=x1(i)+step(i);
    f2=matrix(2,x2(i));
    while(f1<f2)
        x0(i)=x1(i); 
        x1(i)=x2(i);
        f0=f1;
        f1=f2;
        step(i)=2*step(i);
        x2(i)=x1(i)+step(i);
        f2=matrix(2,x2(i));
    end
    left(i)=x2(i);
    right(i)=x0(i);
else%step5 step6
    step(i)=2*step(i);
    x2(i)=x1(i)-step(i);
    f2=matrix(2,x2(i));
    while(f0<f2)
        x1(i)=x0(i); 
        x0(i)=x2(i);
        f1=f0; 
        f0=f2;
        step(i)=2*step(i);
        x2(i)=x1(i)-step(i);
        f2=matrix(2,x2(i));
    end
    left(i)=x1(i);
    right(i)=x2(i);
end
end
end

