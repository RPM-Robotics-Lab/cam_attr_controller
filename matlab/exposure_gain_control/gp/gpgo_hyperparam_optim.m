clear all;  close all;

datalist = dir('../data/analysis_gain/*.csv');
hyperparam_list = zeros(length(datalist), 2);

for i = 1:length(datalist)
    disp(datalist(i).name);
    
    data = csvread([datalist(i).folder '/' datalist(i).name]);
    [exposure, gain] = meshgrid(1:size(data,2),1:size(data,1));
    data = data - mean(data(:));
    metric = data;

    exp_arr = exposure(:);% ./ 39;
    gain_arr = gain(:);% ./ 13;
    metric_arr = metric(:) ./ max(metric(:));

    fidx = length(exp_arr(:));
    % next_in(1) = round(fidx*.05);
    idx_train = [1:10:fidx];
    
    t_train = [exp_arr(idx_train), gain_arr(idx_train)]';
    y_train = metric_arr(idx_train);

    idx_pred = 1:1:fidx;
    t_pred = [exp_arr(idx_pred), gain_arr(idx_pred)]';

%     gpm = fitrgp(t_train', y_train','KernelFunction','squaredexponential',...
%                 'OptimizeHyperparameters','auto','HyperparameterOptimizationOptions',...
%                 struct('AcquisitionFunctionName','expected-improvement-plus'));
    gpm = fitrgp(t_train', y_train','KernelFunction','squaredexponential',...
                'OptimizeHyperparameters','auto');
    % [y_pred, var_pred] = gpm.predict(t_pred');
    
    hyperparam_list(i,:) = gpm.KernelInformation.KernelParameters';

    

end
save('optimized_hyperparam_10_ei.mat', 'hyperparam_list');
