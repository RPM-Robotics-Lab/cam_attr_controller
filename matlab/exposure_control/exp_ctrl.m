clear all;  close all;

global time metric
eps = 0.01;
s_th = 3;

%datanum = 2;
%data = csvread(sprintf('../dat/d%d_Ours.csv',datanum));

data = csvread('./dat/38datar.csv');

time = data(:,1)/10;   % ms
metric = data(:,2); % sum of w grad

%% figure
figure(1)
plot(time, metric);
hold on;
[val,topt_idx] = max(metric); 
plot(time(topt_idx), val, 'ro')
title(sprintf('t opt = %d [ms]\n' , time(topt_idx)));

%% init evaluation points
init_t = [400, 3400]; % ms
val1 = metric(time == init_t(1));
val2 = metric(time == init_t(2));

if (val1 < val2)
    curr_t = init_t(1);
else
    curr_t = init_t(2);
end

%% iterate to find the optimal exp
itr = 1;

while(1)
    %next_t = compute_next_exp_binary (curr_t);
    next_t = compute_next_exp_gpgo (curr_t);
    
    % increase iteration count
    itr = itr+1;
    curr_t = next_t;
    
    fprintf (1,sprintf('itr = %d\n', itr));
    
    % termination criteria check
   if (abs(curr_t - next_t) < 0), break; end;    
   

    
    
end

