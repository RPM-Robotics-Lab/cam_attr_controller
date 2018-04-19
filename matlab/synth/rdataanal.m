clear all
close all
% mxn = 13 x 39
data = csvread('4pm_458.csv')

for i = 1:length(data(:,1))
%     figure(i)
h(i)= plot(data(i,:))
hold on
[tmaxval tmaxloc] = max(data(i,:))
scatter(tmaxloc, tmaxval)
et = [tmaxloc tmaxval]
xlabel('exposure time [1ms : 0.5ms : 20ms]' )
ylabel('metric')
legend('gain level [0:12 dB]')
pause(.5)
end

% et = 1000 : 500 : 1000+500*t_t-1
for k = 1: length(data(1,:))
[gmaxval gmaxloc] = max(data(:,k))
scatter(gmaxloc, gmaxval)
gain = [gmaxloc gmaxval]
g_t = length(data(:,k));

figure(2)
gt(k)= plot(data(:,k));
hold on
end
% gt = 0 : 1 : g_t-1
t_t = length(data(1,:));
t_ti = 1 : .5 : t_t*.5+.5;
g_t = length(data(:,length(data(1,:))));
g_ti = 0: g_t-1;

for u = 1: length(g_ti) %column gain
    for v = 1: length(t_ti) % row exp
        metric = data(u,v);
        figure(3)
        scatter3(u, v, metric, 'filled')
        hold on
    end
end
figure(4)
surf(data)

