%�ƽ�ָ
%clear;
%clc;
tic
%��ʼ������
amp_fre=(xlsread('original'))';%Lamb������A0ģʽ�����ݵ�
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
end
%���ûƽ�ָ�ӳ���
a=[fre_start(1),fre_start(2)];%��������  
b=[fre_stop(1),fre_stop(2)];
epsilon=[2,2];%��������
[max_peak(1,1:2),max_peak(2,1:2),k]=golddiv(amp_fre,a,b,epsilon);
%��������
toc
%��ͼ
for i=1:1:length(fre_center) %ģʽѡ��
    figure();
    hold on;
    plot(amp_fre(1,fre_start(i):fre_stop(i)),amp_fre(2,fre_start(i):fre_stop(i)),'k','linewidth',1);%����Lamb�������ķ�Ƶ��������
    xlim([amp_fre(1,fre_start(i)) amp_fre(1,fre_stop(i))]);%�����귶Χ
    plot(max_peak(1,i),max_peak(2,i),'ro','linewidth',1);%�������ֵ��
    xlabel('Ƶ��(MHz)','fontsize',10.5,'fontname','Times');%�������10.5��С��12
    ylabel('��ֵ��(dB)','fontsize',10.5,'fontname','Times');
    hold off;
end
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

