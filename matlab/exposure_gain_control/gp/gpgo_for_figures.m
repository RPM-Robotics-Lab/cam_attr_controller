clear all;  close all;

%datanum = 2;
%data = csvread(sprintf('../dat/d%d_Ours.csv',datanum));

% data = csvread('../dat/090823f.csv');
% data = csvread('../dat/37datar.csv');1
data = csvread('../data/redata_d4.csv');

% exposure = data(:,1);   % ms
% metric = data(:,2); % sum of w grad

[exposure, gain] = meshgrid(1:size(data,2),1:size(data,1));
data = data - mean(data(:));
metric = data;

%% figure
fig = figure(1);
% plot(time, metric, 'b.','LineWidth', 5);
hold on;
grid on;
[val,topt_idx] = max(metric(:)); 
metric_arr = metric(:);
exp_arr = exposure(:);
gain_arr = gain(:);
mesh(data); xlabel('exposure'); ylabel('gain'); hold on;
plot3(exp_arr(topt_idx), gain_arr(topt_idx), metric_arr(topt_idx), 'ro');


% plot(exposure(topt_idx), val, 'ro', 'LineWidth', 2)
% title(sprintf('Exposure Control using GPGO \n t opt = %d [ms]\n' , exposure(topt_idx)))
% xlabel('Exposue time');

% font size for axis, label, and title
axis_fontsize = 10;
label_fontsize = 10;
title_fontsize = 16;



%% train
fidx = length(exp_arr(:));
% next_in(1) = round(fidx*.05);
next_in(1) = 1;
% next_in(1) = 1;
psi = 0;
for i = 1:10
%     idx = [next_in,  round(fidx*0.95)] % 38,50,26,8
%     idx = [next_in,  next_in(1)+5] % 38,50,26,8 
    idx_train = [next_in];
    t_train = [exp_arr(idx_train), gain_arr(idx_train)]';
    y_train = metric_arr(idx_train);
    disp([num2str(i) ' train vals: ' num2str(exp_arr(idx_train(end))) ' / ' num2str(gain_arr(idx_train(end)))]);

    tic;
    cfg = gp_cov_init ();
    K = gp_train (t_train, y_train, cfg);

    %% predict
%     idx = next_in(1):1:round(fidx*0.95);
    idx_pred = next_in(1):1:fidx;
    t_pred = [exp_arr(idx_pred), gain_arr(idx_pred)]';
%     y_true = metric_arr(idx);

    [y_pred, var_pred] = gp_predict (t_pred, t_train, K, y_train, cfg);
    [vals, optimal_id] = max(y_pred);
    toc;
    t_pred

% %     selection by GPMI
%     alpha = 0.5;
%     max_var = max(diag(var_pred));
%     [next_in(i+1), psi, acq_func] = gpmi_optim(y_pred, var_pred, alpha, psi);


    % selection by var max
    acq_func = diag(var_pred);
    max_var = max(diag(var_pred));
    [va, in] = max(acq_func);
    in(i) = in;
    next_in(i+1) = in(i);
    t_pred(next_in(i+1));

    
    % plot graph    
    [val,topt_idx] = max(metric(:)); 
    figure(2); clf; 
    mesh(data); xlabel('exposure'); ylabel('gain'); hold on; grid on; colormap(jet);
    plot3(exp_arr(topt_idx), gain_arr(topt_idx), metric_arr(topt_idx), 'ro');
    
    mesh(reshape(y_pred, size(metric))); colormap(jet);
    plot3(t_train(1,:), t_train(2,:), y_train, 'rx');
    view(-3, 18);
    
    figure(3);
    mesh(reshape(acq_func-50, size(metric))); colormap(jet);
    view(-3, 18);

%     pause(1);
    
    
%     title(sprintf('Exposure Control using GPGO \n t opt = %d [ms]\n' , time(topt_idx)))
%     xlabel('Exposure time'); ylabel('EWG (Metric)');
%     plot(t_train, y_train,'kx', 'LineWidth', 3);
%     step_size = 5;
%     plot(t_pred(1:step_size:end), y_pred(1:step_size:end), 'r:', 'LineWidth', 5)
    
%     


%     var_diag = diag(var_pred);
%     errorbar (t_pred(1:step_size:end), y_pred(1:step_size:end), var_diag(1:step_size:end), 'm', 'LineWidth',.1);
%     axis([0 20000 -2000 4000]);
%     
%     % plot acquisition function
% %     figure(2);
% %     subplot(2,1,2);
% %     plot(t_pred, acq_func, 'b');
% %     xlabel('Exposure time'); ylabel('Acqusition Func.');
% %     grid on;
% 
%     pause(1)
%     %% Adjust font size
%     set(gca,'FontSize',axis_fontsize);
%     xlable_h = get(gca, 'xlabel'); set(xlable_h, 'FontSize', label_fontsize);
%     ylable_h = get(gca, 'ylabel'); set(ylable_h, 'FontSize', label_fontsize);
% 
%     %% Save figure
%     % font setup
%     set(gca, 'FontName', 'Arial')  
%     set(gca, 'FontSize', 10);
%     % paper setup
%     set(fig, 'PaperUnits', 'centimeters');
%     set(fig, 'PaperSize', [15 10]);
%     pause(0.4);
% %     print('-fillpage',['GPGO' num2str(i)],'-dpdf')
%     
%     % stop criteria
%     if abs(t_pred(next_in(i+1))-t_pred(next_in(i))) < 15 || max_var < 500
%         [max_val, optimal_id] = max(y_pred);
%         fprintf('break, Optimal exposure time is %d \n', t_pred(optimal_id));
%         break;
%     end

end

%%
figure(2); 
t_selected = t_pred(:, optimal_id);
y_selected = y_pred(optimal_id);
% subplot(2,1,1); hold on;
plot3(exp_arr(topt_idx), gain_arr(topt_idx), metric_arr(topt_idx), 'bo', 'LineWidth', 3)
plot3(t_selected(1), t_selected(2), y_selected, 'go', 'LineWidth', 3);
legend_h = legend('GT' , 'Training Points','GP-Mean', 'GP-Cov', 'Optimal-GT', 'Optimal-Pred',  'Location','southwest');


fig_true = figure(111);
s1 = surf(data, 'FaceAlpha',0.85); xlabel('exposure'); ylabel('gain'); zlabel('EWG'); hold on;
s1.EdgeColor = 'none';
plot3(exp_arr(topt_idx), gain_arr(topt_idx), metric_arr(topt_idx), 'bo', 'LineWidth', 4)

fig_estim = figure(222);
s2 = surf(reshape(y_pred, size(metric)), 'FaceAlpha',0.85); xlabel('exposure'); ylabel('gain'); zlabel('EWG'); hold on;
s2.EdgeColor = 'none';
plot3(t_selected(1), t_selected(2), y_selected, 'bo', 'LineWidth', 4); 
plot3(t_train(1,:), t_train(2,:), y_train, 'rx', 'LineWidth', 3);


%% Save figure
figure(fig_true);
% font setup
set(gca, 'FontName', 'Arial')  
set(gca, 'FontSize', 13);
% paper setup
set(fig_true, 'PaperUnits', 'centimeters');
set(fig_true, 'PaperSize', [12 8]);
pause(0.4);
print('-fillpage',['True'],'-dpdf');

figure(fig_estim);
% font setup
set(gca, 'FontName', 'Arial')  
set(gca, 'FontSize', 13);
% paper setup
set(fig_estim, 'PaperUnits', 'centimeters');
set(fig_estim, 'PaperSize', [12 8]);
pause(0.4);
print('-fillpage',['Estim'],'-dpdf');



%% Change file save option
print_pdf = 0;

if (print_pdf)
    print -dpdf ../figs/GPGO6.pdf
end