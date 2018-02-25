clear; close all;

data1 = csvread('redata_d1.csv');
data2 = csvread('redata_d2.csv');
data3 = csvread('redata_d3.csv');
data4 = csvread('redata_d4.csv');


figure;
subplot(2,2,1); surf(data1); xlabel('exposure'); ylabel('gain'); view(2);
subplot(2,2,2); surf(data2); xlabel('exposure'); ylabel('gain'); view(2);
subplot(2,2,3); surf(data3); xlabel('exposure'); ylabel('gain'); view(2);
subplot(2,2,4); surf(data4); xlabel('exposure'); ylabel('gain'); view(2);
 