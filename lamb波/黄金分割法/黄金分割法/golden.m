clear;
clc;
%�ƽ�ָ
tic
amp_fre=(xlsread('test.xlsx'))';%Lamb������A0ģʽ�����ݵ�
fre_center=[10.74 109.26];%Lamb������A0��S0ģʽ������Ƶ��(MHz)
bandwidth=[0.1 1];%Lamb������A0��S0ģʽ������Ƶ��(MHz)
fre_start=[1 1];%��¼������Ƶ����Χ
fre_stop=[2 2];
max_peak=[0 0;0 0];%��¼Lamb������A0��S0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
k=[0 0];%������ŵ����Ĵ���
%������ʼ
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
    %�ƽ�ָ��Ƶ����Ѱ�Ҽ���ֵ
    a=fre_start(i);%��ʼ����  
    b=fre_stop(i);
    Err=2;%����
    T=0.618;%�ƽ�ָ�  
    x1=round(a+(1-T)*(b-a));  
    x2=round(b-(1-T)*(b-a));  
    y1=amp_fre(2,x1);  
    y2=amp_fre(2,x2);  
    while(1)
        k(i)=k(i)+1;%��������
        if(abs(b-a)<=Err)  
            x=fix(0.5*(a+b));%�õ������Ž�  
            break;  
        end  
        if(y1>y2)%����ֵƫ��  
            b=x2;  
            x2=x1;  
            y2=y1;  
            x1=round(a+(1-T)*(b-a));  
            y1=amp_fre(2,x1);  
        elseif(y1<y2) %����ֵƫ��  
            a=x1;  
            x1=x2;  
            y1=y2;  
            x2=round(b-(1-T)*(b-a));   
            y2=amp_fre(2,x2);
        else %y1==y2������ֵ���м�
            a=x1;
            b=x2;
            x1=round(a+(1-T)*(b-a));  
            x2=round(b-(1-T)*(b-a));  
            y1=amp_fre(2,x1);  
            y2=amp_fre(2,x2);
        end      
    end 
    %�����������ڽ��б���
    x=a;
    for t=(a+1):1:b
        if amp_fre(2,t)>amp_fre(2,a)
            x=t;
        end
    end
    if amp_fre(2,x)>=amp_fre(2,x+1)&&amp_fre(2,x)>=amp_fre(2,x-1)
        max_peak(2,i)=amp_fre(2,x);%��¼����ֵ
        max_peak(1,i)=amp_fre(1,x);%��¼����ֵ��Ӧ��Ƶ��ֵ
    else
        disp('��ֵѰ��ʧ�ܣ������¶����������䣡');
        max_peak(2,i)=amp_fre(2,fre_start);
        max_peak(1,i)=amp_fre(1,fre_start);
    end
    %��ͼ
    figure();
    hold on;
    plot(amp_fre(1,fre_start(i):fre_stop(i)),amp_fre(2,fre_start(i):fre_stop(i)),'k');%����Lamb�������ķ�Ƶ��������
    xlim([amp_fre(1,fre_start(i)) amp_fre(1,fre_stop(i))]);%�����귶Χ
    plot(max_peak(1,i),max_peak(2,i),'ro');%�������ֵ��
    xlabel('Ƶ��(MHz)','fontsize',10.5,'fontname','Times');%�������10.5��С��12
    ylabel('��ֵ��(dB)','fontsize',10.5,'fontname','Times');
    hold off;
end
%��������
toc
disp('k =');%�����������
disp(k);
disp('max_peak =');%���Lamb������A0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
disp(max_peak);

%������ͼ
amp_fre_all=(xlsread('24k-120M.xlsx'))';%Lamb�������ķ�Ƶ�������ݵ�
figure();
hold on;
plot(amp_fre_all(1,1:end),amp_fre_all(2,1:end),'k');%����Lamb�������ķ�Ƶ��������
xlim([amp_fre_all(1,1) amp_fre_all(1,end)]);%�����귶Χ
plot(max_peak(1,1:end),max_peak(2,1:end),'ro');%�������ֵ��
xlabel('Ƶ��(MHz)','fontsize',10.5,'fontname','Times');
ylabel('��ֵ��(dB)','fontsize',10.5,'fontname','Times');
hold off;

