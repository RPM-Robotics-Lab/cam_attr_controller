clc; clear all;  close all;

data = csvread('dat/38datar.csv');
data = [data(:,1)/10 data(:,2)];


%% figure
figure(1)
plot(data(:,1), data(:,2));
hold on;
L = max(data(:,2)); 
Lp = find(data(:,2) == L);
plot(Lp, data(Lp,2), 'r*')
fprintf('estimated exposure time is %d [ms]\n' , Lp) 

%% init search
% reference 2 exp times 
n = 20;
for i = 1 :1: n
bi = [data(i,:); data(i+1,:)] ;
ci = [data(end-(i-1),:); data(end-i,:)];

min_v = min(data(:,1)); max_v = max(data(:,1)); 
x1 = min_v:1 :max_v ;
x2 = max_v:-1:min_v;


a1 = (bi(2,2)-bi(1,2))  /  (bi(2,1) - bi(1,1)) ;
y1 = a1 * (x1- bi(1,1)) + bi(1,2);
a2 = (ci(2,2)-ci(1,2))  /  (ci(2,1) - ci(1,1)) ;
y2 = a2 * (x2- ci(2,1)) + ci(2,2);

x_next = round ((a2 * ci(2,1) - a1* bi(1,1) + bi(1,2) - ci(2,2)) / (a2-a1))

plot(x_next, data(x_next,2), 'ro');
hold on
% find solution 
% if (a1 < 0)    
% fprintf('%d is local optimal \n' , bi(1,1))
% break;
% end
% 
% if (a2 > 0)    
% fprintf('%d is opt \n' , ci(1,1))
% break;
% end

% plot two lines
plot(x1, y1, ':k')
plot(x2, y2,':k')
pause(.1)
end 

% find intersection
eval = abs(y2(data(end,1):-1:1)-y1);
[v, next_exp] = min(eval);
t1 = bi(1);
t2 = ci(1);
t3 = next_exp;
plot(t1, data(t1,2), 'go');
plot(t2, data(t2,2), 'go');

% plot(next_exp, data(next_exp,2), 'ro');

top = t3;
vop = data(top,2);

fprintf('estimated exposure time is %d [ms]\n' , next_exp) 
