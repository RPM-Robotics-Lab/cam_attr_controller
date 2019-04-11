close all;
clear all;
clc;
%% HDR CRF curve fitting from a set of images
global E;   % irradiance
global B;   % sample time array for crf curve fitting

%% option
plot_on = 1;
is_indoor = 1;  % 0 for outdoor

E = 20;

% Desired exposure time and gain for synthesis (0.5 + 0.05*i) ms 
target_dt = 6;     %[ms]
target_gain = 1;   %[db]


%% hdr curve fitting
[time_array,images_names,o_img] = deal([]);

if (is_indoor)
    % time array samples (we use four samples)
    time_array = [1, 18, 28, 38];   % [ms]
    % B = log(E*dt) for time array samples
    B = log(E* (0.0004+ (time_array.* 0.0005)));

    % Names of the images to be used
    images_names = {'indoor_sample/1.png' 
                    'indoor_sample/18.png'
                    'indoor_sample/28.png'
                    'indoor_sample/38.png'};
else
    time_array = [1, 15, 69, 89];   % [ms]
    % B = log(E*dt) for time array samples
    B = log(E* (0.00005+ (time_array.* 0.00005)));
    
    % Names of the images to be used
    images_names = {'outdoor_sample/1.png' 
                    'outdoor_sample/15.png'
                    'outdoor_sample/70.png'
                    'outdoor_sample/90.png'};
end

% Prepare image series for CRF curve fitting
img_amount = length(images_names);
img_series = {};

for img_nus_imger = 1:img_amount
    img_series{img_nus_imger} = imread(images_names{img_nus_imger});
    % img_container{img_nus_imger} = rgb2gray(img_container{img_nus_imger});
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
    o_img = imread('indoor_sample/1.png');
else
    o_img = imread('outdoor_sample/1.png');
end

[s_img,K_t,K_g] = img_synth (o_img, target_dt, target_gain, crf);

if (plot_on)
    figure(3)
    subplot(1,2,1); imshow(o_img); title('Original')
    subplot(1,2,2); imshow(s_img); title('Synthetic')
end

if (target_gain < 1)
    Nsnr =0;
else 
    Nsnr = calc_img_snr (s_img, o_img);
end


%% image metric evaluation
EWG = calc_img_ewg (s_img);
NEWG = EWG - Nsnr;

fprintf('Synth_exp: %.2f [ms] \n' , (0.5 + 0.05*target_dt)  )
fprintf('synth_gain: %.1f [dB] \n', target_gain)
fprintf('EWG : %.2f \n', EWG )
fprintf('NEWG : %.2f \n', NEWG )
