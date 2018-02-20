function [left,right] = jintuifa(matrix,x0,step)
%���˷�������ȷ���µ�������.�������Ż�����������ѧ�����磩20ҳ�㷨1.4.3��д��
%v1.0 author: liuxi BIT
%[left right]Ϊ�µ������䣬yΪ������xΪ����y�ı�����x0Ϊ��ʼ�㣨Ĭ��Ϊ0��,step��>0��Ϊ��ʼ������Ĭ��Ϊ0.01��

for i=1:1:length(x0)
    
f0=matrix(2,x0(i)); %step1 ��f(x0) ������y�б���x�滻Ϊx0
x1(i)=x0(i)+step(i);%step2
f1=matrix(2,x1(i));

if (f1>=f0)%step3 step4    % ������ȷ�����²��Ͻ������һ�Ųȹ�����
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

