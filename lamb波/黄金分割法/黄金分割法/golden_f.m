%黄金分割法
%clear;
%clc;
tic
%初始化设置
amp_fre=(xlsread('original'))';%Lamb波器件A0模式的数据点
fre_center=[10.74 109.26];%Lamb波器件A0和S0模式的中心频率(MHz)
bandwidth=[0.1 1];%Lamb波器件A0和S0模式的搜索频宽(MHz)
fre_start=[1 1];%记录搜索的频带范围
fre_stop=[2 2];
max_peak=[0 0;0 0];%记录Lamb波器件A0和S0模式的最大峰值和对应的频率值（第一行为频率值）
k=[0 0];%用来存放迭代的次数
%搜索开始
for i=1:1:length(fre_center) %模式选择
    fre_start(2)=fre_start(1);
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
epsilon=[2,2];%收敛精度
[max_peak(1,1:2),max_peak(2,1:2),k]=golddiv(amp_fre,a,b,epsilon);
%搜索结束
toc
%画图
for i=1:1:length(fre_center) %模式选择
    figure();
    hold on;
    plot(amp_fre(1,fre_start(i):fre_stop(i)),amp_fre(2,fre_start(i):fre_stop(i)),'k','linewidth',1);%画出Lamb波器件的幅频特性曲线
    xlim([amp_fre(1,fre_start(i)) amp_fre(1,fre_stop(i))]);%横坐标范围
    plot(max_peak(1,i),max_peak(2,i),'ro','linewidth',1);%标出最大峰值点
    xlabel('频率(MHz)','fontsize',10.5,'fontname','Times');%五号字体10.5，小四12
    ylabel('幅值比(dB)','fontsize',10.5,'fontname','Times');
    hold off;
end
disp('k =');%输出迭代次数
disp(k);
disp('max_peak =');%输出Lamb波器件A0模式的最大峰值和对应的频率值（第一行为频率值）
disp(max_peak);

%画整体图
amp_fre_all=(xlsread('24k-120M.xlsx'))';%Lamb波器件的幅频特性数据点
figure();
hold on;
plot(amp_fre_all(1,1:end),amp_fre_all(2,1:end),'k');%画出Lamb波器件的幅频特性曲线
xlim([amp_fre_all(1,1) amp_fre_all(1,end)]);%横坐标范围
plot(max_peak(1,1:end),max_peak(2,1:end),'ro');%标出最大峰值点
xlabel('频率(MHz)','fontsize',10.5,'fontname','Times');
ylabel('幅值比(dB)','fontsize',10.5,'fontname','Times');
hold off;

