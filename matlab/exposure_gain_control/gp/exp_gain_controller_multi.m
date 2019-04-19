clear; close all;


%% HDR CRF curve fitting from a set of images
global E;   % irradiance
global B;   % sample time array for crf curve fitting
global time_itv;
global is_indoor;

is_indoor = 0;  % 0 for outdoor
E = 100;  %mean(mean(img_series{1}))/2;-exclude saturated region\TODO

%% Configuration, Data load
addpath('../../synthetic');
addpath('functions');   

datapath = '/mnt/data2/exposure_control/201903_exp_gridsearch/';
seqs = 1:66;

attr_list = zeros(66, 2);

for i =  1:length(seqs)
    o_img = imread(strcat(datapath, num2str(seqs(i)), '/0001.png'));
    
    [opt_expo, opt_gain, opt_img] = gp_expo_grain_controller(o_img, true);
    
    figure(1);
    clf;
    imshow([o_img, opt_img]);
    title(strcat('frame: ', num2str(i), ' expo/gain: ', num2str(opt_expo), ' / ', num2str(opt_gain)));
    
    attr_list(i, :) = [opt_expo, opt_gain];
   
    drawnow;
    
end