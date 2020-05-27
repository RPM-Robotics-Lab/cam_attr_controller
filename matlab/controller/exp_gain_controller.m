clear all; close all;

%% Configuration, Data load
addpath('../data');
addpath('../fcns_img');
addpath('../fcns_gp');      

%% HDR CRF curve fitting from a set of images
global E;           % irradiance
global B;           % sample time array for crf curve fitting
global time_itv;    % exposure time interval

global is_indoor;

is_indoor = 0;  % 0 for outdoor

%% Configuration, Data load
if (is_indoor)
    datapath = '../data/indoor_sample';
    E = 18;  %mean(mean(img_series{1}))/2;-exclude saturated region\TODO

else
    datapath = '../data/outdoor_sample';
    E = 180;  %mean(mean(img_series{1}))/2;-exclude saturated region\TODO

end
seqs = 1:1;

attr_list = zeros(66, 2);

for i =  1:length(seqs)
    o_img = imread(strcat(datapath,  '/1.png'));
    if  size(o_img,3) > 1
        o_img = rgb2gray(o_img);
    end
    [opt_expo, opt_gain, opt_img] = gp_expo_gain_controller(o_img, true);
    figure(1);
    clf;
    imshow([o_img, opt_img]);
    title(strcat('frame: ', num2str(i), ' expo/gain: ', num2str(opt_expo), ' / ', num2str(opt_gain)));
    attr_list(i, :) = [opt_expo, opt_gain];
    drawnow;
    
end


