clear all;  close all;

%datanum = 2;
%data = csvread(sprintf('../dat/d%d_Ours.csv',datanum));

data = csvread('../dat/090823f.csv');
% data = csvread('../dat/37datar.csv');

time = data(:,1);   % ms
metric = data(:,2); % sum of w grad

%% figure
fig = figure(1);
% plot(time, metric, 'b.','LineWidth', 5);
hold on;
grid on;
[val,topt_idx] = max(metric); 
plot(time(topt_idx), val, 'ro', 'LineWidth', 2)
title(sprintf('Exposure Control using GPGO \n t opt = %d [ms]\n' , time(topt_idx)))
xlabel('Exposue time');
% font size for axis, label, and title
axis_fontsize = 10;
label_fontsize = 10;
title_fontsize = 16;



%% train
fidx = length(time);
% next_in(1) = round(fidx*.05);
next_in(1) = 1;
% next_in(1) = 1;
psi = 0;
for i = 1:20
    i
%     idx = [next_in,  round(fidx*0.95)] % 38,50,26,8
%     idx = [next_in,  next_in(1)+5] % 38,50,26,8
    idx = [next_in];
    t_train = time(idx);
    y_train = metric(idx);

    tic;
    cfg = gp_cov_init ();
    K = gp_train (t_train, y_train, cfg);

    %% predict
%     idx = next_in(1):1:round(fidx*0.95);
    idx = next_in(1):1:fidx;
    t_pred = time(idx);
    y_true = metric(idx);

    [y_pred, var_pred] = gp_predict (t_pred, t_train, K, y_train, cfg);
    [vals, optimal_id] = max(y_pred);
    toc;

% %     selection by GPMI
%     alpha = 7;
%     max_var = max(diag(var_pred));
%     [next_in(i+1), psi, acq_func] = gpmi_optim(y_pred, var_pred, alpha, psi);


    % selection by var max
    acq_func = diag(var_pred);
    max_var = max(diag(var_pred));
    [va, in] = max(acq_func);
    in(i) = in;
    next_in(i+1) = in(i);
    t_pred(next_in(i+1))

    
    % plot graph    
    figure(1); clf;
    plot(time, metric, 'LineWidth', 5);
    hold on;
    grid on;
    [val,topt_idx] = max(metric); 
%     title(sprintf('Exposure Control using GPGO \n t opt = %d [ms]\n' , time(topt_idx)))
    xlabel('Exposure time'); ylabel('EWG (Metric)');
    plot(t_train, y_train,'kx', 'LineWidth', 3);
    step_size = 5;
    plot(t_pred(1:step_size:end), y_pred(1:step_size:end), 'r:', 'LineWidth', 5)
    var_diag = diag(var_pred);
    errorbar (t_pred(1:step_size:end), y_pred(1:step_size:end), var_diag(1:step_size:end), 'm', 'LineWidth',.1);
    axis([0 20000 -2000 4000]);
    
    % plot acquisition function
%     figure(2);
%     subplot(2,1,2);
%     plot(t_pred, acq_func, 'b');
%     xlabel('Exposure time'); ylabel('Acqusition Func.');
%     grid on;

    pause(1)
    %% Adjust font size
    set(gca,'FontSize',axis_fontsize);
    xlable_h = get(gca, 'xlabel'); set(xlable_h, 'FontSize', label_fontsize);
    ylable_h = get(gca, 'ylabel'); set(ylable_h, 'FontSize', label_fontsize);

    %% Save figure
    % font setup
    set(gca, 'FontName', 'Arial')  
    set(gca, 'FontSize', 10);
    % paper setup
    set(fig, 'PaperUnits', 'centimeters');
    set(fig, 'PaperSize', [15 10]);
    pause(0.4);
%     print('-fillpage',['GPGO' num2str(i)],'-dpdf')
    
    % stop criteria
    if abs(t_pred(next_in(i+1))-t_pred(next_in(i))) < 15 || max_var < 500
        [max_val, optimal_id] = max(y_pred);
        fprintf('break, Optimal exposure time is %d \n', t_pred(optimal_id));
        break;
    end

end

figure(1); 
t_selected = t_pred(optimal_id);
y_selected = y_pred(optimal_id);
% subplot(2,1,1); hold on;
plot(time(topt_idx), val, 'bo', 'LineWidth', 3)
plot(t_selected, y_selected,'go', 'LineWidth', 3);
legend_h = legend('GT' , 'Training Points','GP-Mean', 'GP-Cov', 'Optimal-GT', 'Optimal-Pred',  'Location','southwest');


%% Save figure
% font setup
set(gca, 'FontName', 'Arial')  
set(gca, 'FontSize', 10);
% paper setup
set(fig, 'PaperUnits', 'centimeters');
set(fig, 'PaperSize', [15 10]);
pause(0.4);
% print('-fillpage',['GPGO' num2str(i+1)],'-dpdf');


%% Change file save option
print_pdf = 0;

if (print_pdf)
    print -dpdf ../figs/GPGO6.pdf
end