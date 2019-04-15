% clear; close all;

%% Configuration, Data load
addpath('../../synthetic');
addpath('functions');

datapath = '../../synthetic/indoor_sample/';

img_list = dir(strcat(datapath, '*.png'));

img = imread(strcat(img_list(1).folder, '/', img_list(1).name));
crf = csvread(strcat(datapath, 'crf.csv'));

%%

gain_roi = 1:13; % 
expo_roi = 1:39; % 

% indoor: 1000~20000 us (500us)
% outdoor: 100~2000 us (50us)
% gain: 0~12

[expos, gains] = meshgrid(expo_roi, gain_roi);

gain_arr = gains(:);
expo_arr = expos(:);

% initial_points =  
