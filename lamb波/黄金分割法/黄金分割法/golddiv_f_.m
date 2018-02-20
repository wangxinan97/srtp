clear;
clc;
%�ƽ�ָ
amp_fre=(xlsread('original.xlsx'))';%Lamb������A0ģʽ�����ݵ�
%��ʼ������
fre_center=[10.65 108.85];%Lamb������A0��S0ģʽ������Ƶ��(MHz)
bandwidth=[0.1 1];%Lamb������A0��S0ģʽ������Ƶ��(MHz)
fre_start=[1 1502];%��¼������Ƶ����Χ
fre_stop=[1501 2752];
max_peak=[0 0;0 0];%��¼Lamb������A0��S0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
k=[0 0];%������ŵ����Ĵ���
%������ʼ
tic
for i=1:1:length(fre_center) %ģʽѡ��
    %fre_start(2)=1502;
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
epsilon=[5,5];%��������
[max_peak(1,1:2),max_peak(2,1:2),k]=golddiv(amp_fre,a,b,epsilon);
%��������
toc

disp('k =');%�����������
disp(k);
disp('max_peak =');%���Lamb������A0ģʽ������ֵ�Ͷ�Ӧ��Ƶ��ֵ����һ��ΪƵ��ֵ��
disp(max_peak);


