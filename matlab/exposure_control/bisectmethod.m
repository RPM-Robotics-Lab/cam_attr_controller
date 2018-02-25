clc; clear all;  close all;

data = csvread('dat/38datar.csv');
data = [data(:,1)/10 data(:,2)];


%% figure
figure(1)
plot(data(:,1), data(:,2));
hold on;
TrueVal = max(data(:,2)); 
True = find(data(:,2) == TrueVal);
plot(True, data(True,2), 'ro', 'MarkerSize', 15)
hold on
fprintf('estimated exposure time is %d [ms]\n' , True) 

%% init search
% reference 2 exp times 
min_v = min(data(:,1)); max_v = max(data(:,1)); 
x_l = min_v:1 :max_v ;
x_r = max_v:-1:min_v;

init = 2;
%% initial      ()
data_l = [data(init,:); data(init+1,:)] ;
data_r = [data(end-(init),:); data(end-(init-1),:)];
a_l = (data_l(2,2)-data_l(1,2))  /  (data_l(2,1) - data_l(1,1)) ;
a_r = (data_r(2,2)-data_r(1,2))  /  (data_r(2,1) - data_r(1,1)) ;

y_l = a_l * (x_l- data_l(1,1)) + data_l(1,2);
y_r = a_r * (x_r- data_r(2,1)) + data_r(2,2);

x_next = round ((a_r * data_r(2,1) - a_l* data_l(1,1) + data_l(1,2) - data_r(2,2)) / (a_r-a_l));
% plot two lines
plot(x_l, y_l, ':k')
plot(x_r, y_r,':k')
pause(.2)


%%
seq = 30;
for i = 1 : seq
direction = data(x_next(i)+1,2) - data(x_next(i),2) ;  % 

    s_th = 2;
    if (direction < 0 )
    data_l = [data(i+2,:); data(i+3,:)] ;
    data_r = [data((x_next(i)),:); data(x_next(i)-1,:)];
        if (mean([data_r(2,1); data_r(1,1)] -data_l(:,1)) < s_th)
        fprintf('%d is opt by bisect method \n' , data_r(1,1))      
        break;
        end
    a_l = (data_l(2,2)-data_l(1,2))  /  (data_l(2,1) - data_l(1,1)) ;
    a_r = (data_r(2,2)-data_r(1,2))  /  (data_r(2,1) - data_r(1,1)) ;
    y_l = a_l * (x_l- data_l(1,1)) + data_l(1,2);
    y_r = a_r * (x_r- data_r(2,1)) + data_r(2,2);
    x_next(i+1) = round ((a_r * data_r(2,1) - a_l* data_l(1,1) + data_l(1,2) - data_r(2,2)) / (a_r-a_l));
    plot(x_l, y_l, ':k')
    plot(x_r, y_r,':k')
    plot(x_next, data(x_next,2), 'r*');
    pause(.2)

    else 
    data_l = [data((x_next(i)),:); data(x_next(i)+1,:)];
    data_r = [data((end-i-1),:); data((end-i-2), :)] ;
        if ( mean([data_r(2,1); data_r(1,1)] -data_l(:,1)) <s_th )
        fprintf('%d is opt by bisect method \n' , data_l(1,1))      

        break;
        end
    
    a_l =(data_l(2,2)-data_l(1,2))  /  (data_l(2,1) - data_l(1,1)) ;
    a_r = (data_r(2,2)-data_r(1,2))  /  (data_r(2,1) - data_r(1,1)) ;
    y_l = a_l * (x_l- data_l(1,1)) + data_l(1,2);
    y_r = a_r * (x_r- data_r(2,1)) + data_r(2,2);
    x_next(i+1) = round ((a_r * data_r(2,1) - a_l* data_l(1,1) + data_l(1,2) - data_r(2,2)) / (a_r-a_l));
    plot(x_l, y_l, ':k')
    plot(x_r, y_r,':k')
    plot(x_next, data(x_next,2), 'g*');
    pause(.2)
    end
    
    % solution 
%      if (a_l < 0)    
%      fprintf('%d is local optimal \n' , data_l(1,1))
%      break;
%      end
% 
%      if (a_r > 0)    
%      fprintf('%d is opt \n' , data_r(1,1))
%      break;
%      end
end






% find intersection
eval = abs(y_r(data(end,1):-1:1)-y_l);
[v, next_exp] = min(eval);
t1 = data_l(1);
t2 = data_r(1);
t3 = next_exp;% plot two lines
plot(x_l, y_l, ':k')
plot(x_r, y_r,':k')
pause(.1)
plot(t1, data(t1,2), 'go');
plot(t2, data(t2,2), 'go');



%fprintf('estimated exposure time is %d [ms]\n' , x_next) 
