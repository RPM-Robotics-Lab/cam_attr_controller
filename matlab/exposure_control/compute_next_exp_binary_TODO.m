function next_t = compute_next_exp_binary (curr_t)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2017.08.28      ak          created and written
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

curr_t = round ((a_r * data_r(2,1) - a_l* data_l(1,1) + data_l(1,2) - data_r(2,2)) / (a_r-a_l))
% plot two lines
plot(x_l, y_l, ':r')
plot(x_r, y_r,':k')
pause(.2)


%%
for itr = 1: 10
    direction = data(curr_t+1,2) - data(curr_t,2) ;  % 

    s_th = 2;
    if (direction < 0 )
    data_l = [data(itr+2,:); data(itr+3,:)] ;
    data_r = [data(curr_t,:); data(curr_t-1,:)];
      
    a_l = (data_l(2,2)-data_l(1,2))  /  (data_l(2,1) - data_l(1,1)) ;
    a_r = (data_r(2,2)-data_r(1,2))  /  (data_r(2,1) - data_r(1,1)) ;
    y_l = a_l * (x_l- data_l(1,1)) + data_l(1,2);
    y_r = a_r * (x_r- data_r(2,1)) + data_r(2,2);
    next_t = round ((a_r * data_r(2,1) - a_l* data_l(1,1) + data_l(1,2) - data_r(2,2)) / (a_r-a_l))
    plot(x_l, y_l, ':g')
    plot(x_r, y_r,':g')
    plot(next_t, data(next_t,2), 'r*');
    pause(.2)
    curr_t = next_t;

    else 
    data_l = [data(curr_t,:); data(curr_t+1,:)];
    data_r = [data((end-itr-1),:); data((end-itr-2), :)] ;

    
    a_l =(data_l(2,2)-data_l(1,2))  /  (data_l(2,1) - data_l(1,1)) ;
    a_r = (data_r(2,2)-data_r(1,2))  /  (data_r(2,1) - data_r(1,1)) ;
    y_l = a_l * (x_l- data_l(1,1)) + data_l(1,2);
    y_r = a_r * (x_r- data_r(2,1)) + data_r(2,2);
    next_t = round ((a_r * data_r(2,1) - a_l* data_l(1,1) + data_l(1,2) - data_r(2,2)) / (a_r-a_l))
    plot(x_l, y_l, ':b')
    plot(x_r, y_r,':b')
    plot(next_t, data(next_t,2), 'g*');
    pause(.2)
    curr_t = next_t;
    end
end
    
end


