close all;
clear all;
clc; 

%% Configuration, Data load
datdir = '../data/';

%% time_array : image sequence along each exposure time interval , ex) indoor : 500us interval
%% target_dt : desired image sequence, ex) 18 = initial_exp_t + (17 * exp_itv)



%% HDR CRF curve fitting from a set of images
global E;   % irradiance
global B;   % sample time array for crf curve fitting
global time_itv;
global target_exp_index;
global is_indoor;
%% options
plot_on = 1;
is_indoor = 1;  % 0 for outdoor



E = 100;  %mean(mean(img_series{1}))/2;-exclude saturated region\TODO

% Desired exposure time and gain for synthesis (0.5 + 0.05*i) ms 

target_exp_index = 1;
target_gain = 1;   %[d3]


if (is_indoor)
    time_itv = 0.0005; 
else 
    time_itv = 0.00005; 
end


%% hdr curve fitting
[time_array,images_names,o_img] = deal([]);

if (is_indoor)
    % time array samples (we use four samples) for CRF curve 
    time_array = [1, 18, 28, 38];   % [ms]
    % B = log(E*dt) for time array samples
    B = log(E* (0.001+ ((time_array-1).* time_itv)))
    % Names of the images to be used, 1000, 9500, 14500, 195000 us
    images_names = {'indoor_sample/1.png' 
                    'indoor_sample/18.png'
                    'indoor_sample/28.png'
                    'indoor_sample/38.png'};

             
else
    time_array = [1, 15, 70, 90];   % [ms]
    % B = log(E*dt) for time array samples
    B = log(E* (0.00005+ ((time_array-1).* time_itv)));
    
    % Names of the images to be used, 50, , 750 ,  us
    images_names = {'outdoor_sample/1.png' 
                    'outdoor_sample/15.png'
                    'outdoor_sample/70.png'
                    'outdoor_sample/90.png'};
end

% Prepare image series for CRF curve fitting
img_amount = length(images_names);
img_series = {};

for img_nus_imger = 1:img_amount
    img_series{img_nus_imger} = imread([datdir,images_names{img_nus_imger}]);
%     img_container{img_nus_imger} = rgb2gray(img_container{img_nus_imger});
    img_series{img_nus_imger} = (img_series{img_nus_imger});
end

crf = hdr_curve_fitting (img_series);

if(plot_on)
    % plot
    figure(1)
    for img_nus_imger = 1:img_amount
        subplot(2,2,img_nus_imger); imshow(img_series{img_nus_imger}); title(sprintf('%d',time_array(img_nus_imger)));
    end

    figure(2);
    intensity_range = 1:256;
    plot(intensity_range, crf);
    grid on;
    xlabel('Intensity'); ylabel('log(E*dt)')
end

%% Using the fitted curve, image synthesizing
if (is_indoor)
    o_img = imread([datdir,'indoor_sample/1.png']);
    o_img = imresize(o_img, 0.25);
    if (size(o_img,3) > 1)
        o_img = rgb2gray(o_img);

    end
else
    o_img = imread([datdir,'outdoor_sample/1.png']);
    o_img = imresize(o_img, 0.25);

end

[s_img,K_t,K_g] = img_synth (o_img, target_exp_index, target_gain, crf);

if (plot_on)
    figure(3)
    subplot(1,2,1); imshow(o_img); title(sprintf('Original [index = %d], 0 dB', time_array(1)))
    subplot(1,2,2); imshow(s_img); title(sprintf('Synthetic [index = %d] , %d dB', target_exp_index, target_gain))
end

if (target_gain < 1)
    Nsnr =0;
else 
    Nsnr = calc_img_snr (s_img, o_img);
end


%% image metric evaluation
EWG = calc_img_newg (s_img);
%% image metric evaluation with snr considered 
NEWG = EWG - Nsnr;

fprintf('Synth_exp: %.2f [ms] \n' , (0.04 + 0.005*target_exp_index)  )
fprintf('synth_gain: %.1f [dB] \n', target_gain)
fprintf('EWG : %.2f \n', EWG )
fprintf('NEWG : %.2f \n', NEWG )
