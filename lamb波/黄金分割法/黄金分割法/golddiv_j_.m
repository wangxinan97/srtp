clear;
clc;
%�ƽ�ָ
amp_fre=(xlsread('original.xlsx'))';%Lamb������A0ģʽ�����ݵ�
%��ʼ������
fre_center=[10.65 108.85];%��������Ƶ��(MHz)
fre_initial=[1001 2501];%������ʼ��
step=[-100 -50];
max_peak=[0 0;0 0];%��¼Lamb������A0��S0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
%������ʼ
%ȷ����������
[left,right] = jintuifa(amp_fre,fre_initial,step);
%���ûƽ�ָ�ӳ���
epsilon=[120,50];%��������
tic
[max_peak(1,1:2),max_peak(2,1:2),k]=golddiv(amp_fre,left,right,epsilon);
toc
%��������
disp('k =');%�����������
disp(k);
disp('max_peak =');%���Lamb������A0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
disp(max_peak);
for i=1:1:length(fre_center) %ģʽѡ��
    figure();
    hold on;
    plot(amp_fre(1,left(i):right(i)),amp_fre(2,left(i):right(i)),'k','linewidth',1);%����Lamb�������ķ�Ƶ��������
    %xlim([amp_fre(1,left(i)) amp_fre(1,right(i))]);%�����귶Χ
    plot(max_peak(1,i),max_peak(2,i),'ro','linewidth',1);%�������ֵ��
    xlabel('Ƶ��(MHz)','fontsize',10.5,'fontname','Times');%�������10.5��С��12
    ylabel('��ֵ��(dB)','fontsize',10.5,'fontname','Times');
    hold off;
end



