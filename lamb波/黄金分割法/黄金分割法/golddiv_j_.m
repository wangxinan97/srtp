clear;
clc;
%黄金分割法
amp_fre=(xlsread('original.xlsx'))';%Lamb波器件A0模式的数据点
%初始化设置
fre_center=[10.65 108.85];%搜索中心频率(MHz)
fre_initial=[1001 2501];%搜索初始点
step=[-100 -50];
max_peak=[0 0;0 0];%记录Lamb波器件A0和S0模式的最大峰值和对应的频率值（第一行为频率值）
%搜索开始
%确定搜索区间
[left,right] = jintuifa(amp_fre,fre_initial,step);
%调用黄金分割法子程序
epsilon=[120,50];%收敛精度
tic
[max_peak(1,1:2),max_peak(2,1:2),k]=golddiv(amp_fre,left,right,epsilon);
toc
%搜索结束
disp('k =');%输出迭代次数
disp(k);
disp('max_peak =');%输出Lamb波器件A0模式的最大峰值和对应的频率值（第一行为频率值）
disp(max_peak);
for i=1:1:length(fre_center) %模式选择
    figure();
    hold on;
    plot(amp_fre(1,left(i):right(i)),amp_fre(2,left(i):right(i)),'k','linewidth',1);%画出Lamb波器件的幅频特性曲线
    %xlim([amp_fre(1,left(i)) amp_fre(1,right(i))]);%横坐标范围
    plot(max_peak(1,i),max_peak(2,i),'ro','linewidth',1);%标出最大峰值点
    xlabel('频率(MHz)','fontsize',10.5,'fontname','Times');%五号字体10.5，小四12
    ylabel('幅值比(dB)','fontsize',10.5,'fontname','Times');
    hold off;
end



