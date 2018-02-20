function [best_x, best_y, k] = golddiv(matrix, a, b, epsilon)
%UNTITLED2 Summary of this function goes here
% ������ʵ�ֻƽ�ָ��㷨
% matrix�����ݵ����2*n����һ�����Ա���x���ڶ����Ǻ���ֵy��,a,bΪ���䷶Χ, epsilonΪ����
% best_xΪ�ƽ�ָ��㷨�ҵ������ŵ�
% best_yΪ���ŵ㴦�ĺ���ֵ
% kΪ�㷨�ĵ�������

for i=1:1:length(epsilon)
    x1 = round(a(i)+0.382*(b(i)-a(i)));   %���ݻƽ�ָ����ȷ��������
    f1 = matrix(2,x1);           %����y��x1����ֵ
    x2 = round(a(i)+0.618*(b(i)-a(i)));   %���ݻƽ�ָ����ȷ��������
    f2 = matrix(2,x2);           %����y��x2����ֵ
    k(i)=1;
    while(abs(b(i)-a(i))>epsilon(i))
        if f1 > f2      %���f1����f2
            b(i) = x2;     %bΪ�µ��ұ߽�
            x2 = x1;    %����x2ֵ
            f2 = f1;
            x1 = round(a(i)+0.382*(b(i)-a(i)));   %����x1ֵ
            f1 = matrix(2,x1);
        elseif f1 < f2
            a(i) = x1;
            x1 = x2;
            f1 = f2;
            x2 = round(a(i)+0.618*(b(i)-a(i)));
            f2 = matrix(2,x2);
        else 
            a(i) = x1;
            b(i) = x2;
            x1 = round(a(i)+0.382*(b(i)-a(i)));   
            f1 = matrix(2,x1);           
            x2 = round(a(i)+0.618*(b(i)-a(i)));   
            f2 = matrix(2,x2);          
        end
        k(i)=k(i)+1;
    end
    %�����������ڽ��б���
    x=round((a(i)+b(i))/2);
    best_x(i) = matrix(1,x); %���Ž�
    best_y(i) = matrix(2,x); 
end
end

