clear all
close all
% mxn = 13 x 39
data = xlsread('redata.csv')

for i = 1:length(data(:,1))
%     figure(i)
h(i)= plot(data(i,:))
hold on
[maxval maxloc] = max(data(i,:))
scatter(maxloc, maxval)
xlabel('exposure time [1ms : 0.5ms : 20ms]' )
ylabel('metric')
legend('gain level [0:12 dB]')
end