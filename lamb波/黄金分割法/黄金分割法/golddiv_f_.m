clear;
clc;
%黄金分割法
amp_fre=(xlsread('original.xlsx'))';%Lamb波器件A0模式的数据点
%初始化设置
fre_center=[10.65 108.85];%Lamb波器件A0和S0模式的中心频率(MHz)
bandwidth=[0.1 1];%Lamb波器件A0和S0模式的搜索频宽(MHz)
fre_start=[1 1502];%记录搜索的频带范围
fre_stop=[1501 2752];
max_peak=[0 0;0 0];%记录Lamb波器件A0和S0模式的最大峰值和对应的频率值（第一行为频率值）
k=[0 0];%用来存放迭代的次数
%搜索开始
tic
for i=1:1:length(fre_center) %模式选择
    %fre_start(2)=1502;
    for j=fre_start(i):1:length(amp_fre)%确定搜索的频带范围
        if amp_fre(1,j)>=fre_center(i)-bandwidth(i)/2
            fre_start(i)=j;
            for p=j:1:length(amp_fre)
                if amp_fre(1,p)>=fre_center(i)+bandwidth(i)/2
                    fre_stop(i)=p;
                    break;
                end
            end
            break;
        end
    end
end
%调用黄金分割法子程序
a=[fre_start(1),fre_start(2)];%搜索区间  
b=[fre_stop(1),fre_stop(2)];
epsilon=[5,5];%收敛精度
[max_peak(1,1:2),max_peak(2,1:2),k]=golddiv(amp_fre,a,b,epsilon);
%搜索结束
toc

disp('k =');%输出迭代次数
disp(k);
disp('max_peak =');%输出Lamb波器件A0模式的最大峰值和对应的频率值（第一行为频率值）
disp(max_peak);


