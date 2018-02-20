function [best_x, best_y, k] = golddiv(matrix, a, b, epsilon)
%UNTITLED2 Summary of this function goes here
% 本函数实现黄金分割算法
% matrix是数据点矩阵（2*n，第一行是自变量x，第二行是函数值y）,a,b为区间范围, epsilon为精度
% best_x为黄金分割算法找到的最优点
% best_y为最优点处的函数值
% k为算法的迭代次数

for i=1:1:length(epsilon)
    x1 = round(a(i)+0.382*(b(i)-a(i)));   %根据黄金分割比例确定搜索点
    f1 = matrix(2,x1);           %函数y在x1处的值
    x2 = round(a(i)+0.618*(b(i)-a(i)));   %根据黄金分割比例确定搜索点
    f2 = matrix(2,x2);           %函数y在x2处的值
    k(i)=1;
    while(abs(b(i)-a(i))>epsilon(i))
        if f1 > f2      %如果f1大于f2
            b(i) = x2;     %b为新的右边界
            x2 = x1;    %更新x2值
            f2 = f1;
            x1 = round(a(i)+0.382*(b(i)-a(i)));   %更新x1值
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
    %在收敛区间内进行遍历
    x=round((a(i)+b(i))/2);
    best_x(i) = matrix(1,x); %最优解
    best_y(i) = matrix(2,x); 
end
end

