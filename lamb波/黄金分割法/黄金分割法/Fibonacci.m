clear;
clc;
%Fibonacci法
amp_fre=(xlsread('10M-120M_0.1_1k_local.xlsx'))';%Lamb波器件A0模式的数据点
fre_center=[10.74 109.26];%Lamb波器件A0和S0模式的中心频率(MHz)
bandwidth=[0.1 1];%Lamb波器件A0和S0模式的搜索频宽(MHz)
fre_start=[1 1];%记录搜索的频带范围
fre_stop=[2 2];
max_peak=[0 0;0 0];%记录Lamb波器件A0和S0模式的最大峰值和对应的频率值（第一行为频率值）
k=[1 1];%用来存放迭代的次数
%搜索开始
tic
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
    %Fibonacci法在频带内寻找极大值
    a=fre_start(i);%初始区间  
    b=fre_stop(i);
    L=50;%精度
    fib=[1 1];
    t=2;
    while(fib(t)<(b-a)/L)
        fib(t+1)=fib(t)+fib(t-1);
        t=t+1;
    end
    n=t;%迭代次数
    x1=round(a+fib(n-2)*(b-a)/fib(n));  
    x2=round(a+fib(n-1)*(b-a)/fib(n));  
    y1=amp_fre(2,x1);  
    y2=amp_fre(2,x2); 
    while(k(i)<(n-2))
        k(i)=k(i)+1;%迭代次数  
        if(y1>y2)%最大峰值偏左  
            b=x2;  
            x2=x1;  
            y2=y1;  
            x1=round(a+fib(n-k(i)-1)*(b-a)/fib(n-k(i)+1));  
            y1=amp_fre(2,x1);  
        elseif(y1<y2) %最大峰值偏右  
            a=x1;  
            x1=x2;  
            y1=y2;  
            x2=round(a+fib(n-k(i))*(b-a)/fib(n-k(i)+1));   
            y2=amp_fre(2,x2);
        else %y1==y2，最大峰值在中间
            a=x1;
            b=x2;
            x1=round(a+fib(n-k(i)-1)*(b-a)/fib(n-k(i)+1));  
            x2=round(a+fib(n-k(i))*(b-a)/fib(n-k(i)+1));  
            y1=amp_fre(2,x1);  
            y2=amp_fre(2,x2);
        end 
    end
    k(i)=k(i)+1;
    x2=x1+1;  
    y2=amp_fre(2,x2);
    if(y1<y2)
        a=x1;  
    else
        b=x2;
    end
    x=fix(0.5*(a+b));%得到的最优解
    if amp_fre(2,x)>=amp_fre(2,x+1)&&amp_fre(2,x)>=amp_fre(2,x-1)
        max_peak(2,i)=amp_fre(2,x);%记录最大峰值
        max_peak(1,i)=amp_fre(1,x);%记录最大峰值对应的频率值
    else
        disp('峰值寻找失败，请重新定义搜索区间！');
        max_peak(2,i)=amp_fre(2,fre_start);
        max_peak(1,i)=amp_fre(1,fre_start);
    end
    %画图
    %figure();
    %hold on;
    %plot(amp_fre(1,fre_start(i):fre_stop(i)),amp_fre(2,fre_start(i):fre_stop(i)));%画出Lamb波器件的幅频特性曲线
    %xlim([amp_fre(1,fre_start(i)) amp_fre(1,fre_stop(i))]);%横坐标范围
    %plot(max_peak(1,i),max_peak(2,i),'ro');%标出最大峰值点
    %xlabel('频率(MHz)','fontsize',14,'fontname','Times');
    %ylabel('幅值(dB)','fontsize',14,'fontname','Times');
    %hold off;
end
toc
%搜索结束
disp('k =');%输出迭代次数
disp(k);
disp('max_peak =');%输出Lamb波器件A0模式的最大峰值和对应的频率值（第一行为频率值）
disp(max_peak);
%画整体图
%amp_fre_all=(xlsread('24k-120M.xlsx'))';%Lamb波器件的幅频特性数据点
%figure();
%hold on;
%plot(amp_fre_all(1,1:end),amp_fre_all(2,1:end));%画出Lamb波器件的幅频特性曲线
%xlim([amp_fre_all(1,1) amp_fre_all(1,end)]);%横坐标范围
%plot(max_peak(1,1:end),max_peak(2,1:end),'ro');%标出最大峰值点
%xlabel('频率(MHz)','fontsize',14,'fontname','Times');
%ylabel('幅值(dB)','fontsize',14,'fontname','Times');
%hold off;

