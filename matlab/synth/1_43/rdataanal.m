clear all
close all
% mxn = 13 x 39
data = csvread('rSNR.csv')

for i = 1:length(data(:,1))
%     figure(i)
h(i)= plot(data(i,:))
hold on
[maxval maxloc] = max(data(i,:))
% scatter(maxloc, maxval)
xlabel('exposure time [1ms : 0.5ms : 20ms]' )
ylabel('Signal to Noise Ratio [dB]')
legend('gain level [0dB]', 'gain level [1dB]', 'gain level [2dB]','gain level [3dB]','gain level [4dB]','gain level [5dB]','gain level [6dB]', 'gain level [7dB]', 'gain level [8dB]', 'gain level [9dB]', 'gain level [10dB]', 'gain level [11dB]', 'gain level [12dB]')
end