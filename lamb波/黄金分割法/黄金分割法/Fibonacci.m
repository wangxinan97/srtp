clear;
clc;
%Fibonacci��
amp_fre=(xlsread('10M-120M_0.1_1k_local.xlsx'))';%Lamb������A0ģʽ�����ݵ�
fre_center=[10.74 109.26];%Lamb������A0��S0ģʽ������Ƶ��(MHz)
bandwidth=[0.1 1];%Lamb������A0��S0ģʽ������Ƶ��(MHz)
fre_start=[1 1];%��¼������Ƶ����Χ
fre_stop=[2 2];
max_peak=[0 0;0 0];%��¼Lamb������A0��S0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
k=[1 1];%������ŵ����Ĵ���
%������ʼ
tic
for i=1:1:length(fre_center) %ģʽѡ��
    fre_start(2)=fre_start(1);
    for j=fre_start(i):1:length(amp_fre)%ȷ��������Ƶ����Χ
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
    %Fibonacci����Ƶ����Ѱ�Ҽ���ֵ
    a=fre_start(i);%��ʼ����  
    b=fre_stop(i);
    L=50;%����
    fib=[1 1];
    t=2;
    while(fib(t)<(b-a)/L)
        fib(t+1)=fib(t)+fib(t-1);
        t=t+1;
    end
    n=t;%��������
    x1=round(a+fib(n-2)*(b-a)/fib(n));  
    x2=round(a+fib(n-1)*(b-a)/fib(n));  
    y1=amp_fre(2,x1);  
    y2=amp_fre(2,x2); 
    while(k(i)<(n-2))
        k(i)=k(i)+1;%��������  
        if(y1>y2)%����ֵƫ��  
            b=x2;  
            x2=x1;  
            y2=y1;  
            x1=round(a+fib(n-k(i)-1)*(b-a)/fib(n-k(i)+1));  
            y1=amp_fre(2,x1);  
        elseif(y1<y2) %����ֵƫ��  
            a=x1;  
            x1=x2;  
            y1=y2;  
            x2=round(a+fib(n-k(i))*(b-a)/fib(n-k(i)+1));   
            y2=amp_fre(2,x2);
        else %y1==y2������ֵ���м�
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
    x=fix(0.5*(a+b));%�õ������Ž�
    if amp_fre(2,x)>=amp_fre(2,x+1)&&amp_fre(2,x)>=amp_fre(2,x-1)
        max_peak(2,i)=amp_fre(2,x);%��¼����ֵ
        max_peak(1,i)=amp_fre(1,x);%��¼����ֵ��Ӧ��Ƶ��ֵ
    else
        disp('��ֵѰ��ʧ�ܣ������¶����������䣡');
        max_peak(2,i)=amp_fre(2,fre_start);
        max_peak(1,i)=amp_fre(1,fre_start);
    end
    %��ͼ
    %figure();
    %hold on;
    %plot(amp_fre(1,fre_start(i):fre_stop(i)),amp_fre(2,fre_start(i):fre_stop(i)));%����Lamb�������ķ�Ƶ��������
    %xlim([amp_fre(1,fre_start(i)) amp_fre(1,fre_stop(i))]);%�����귶Χ
    %plot(max_peak(1,i),max_peak(2,i),'ro');%�������ֵ��
    %xlabel('Ƶ��(MHz)','fontsize',14,'fontname','Times');
    %ylabel('��ֵ(dB)','fontsize',14,'fontname','Times');
    %hold off;
end
toc
%��������
disp('k =');%�����������
disp(k);
disp('max_peak =');%���Lamb������A0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
disp(max_peak);
%������ͼ
%amp_fre_all=(xlsread('24k-120M.xlsx'))';%Lamb�������ķ�Ƶ�������ݵ�
%figure();
%hold on;
%plot(amp_fre_all(1,1:end),amp_fre_all(2,1:end));%����Lamb�������ķ�Ƶ��������
%xlim([amp_fre_all(1,1) amp_fre_all(1,end)]);%�����귶Χ
%plot(max_peak(1,1:end),max_peak(2,1:end),'ro');%�������ֵ��
%xlabel('Ƶ��(MHz)','fontsize',14,'fontname','Times');
%ylabel('��ֵ(dB)','fontsize',14,'fontname','Times');
%hold off;

