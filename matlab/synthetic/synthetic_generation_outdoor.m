close all;
clear all;

%% HDR CRF curve fitting from a set of images
global E time_array B;
   % irradiance

plot_on = 1;
E = 20;
% time array samples (we use four samples)
time_array = [1, 15, 69, 89];   % [ms]

% B = log(E*dt) for time array samples
B = log(E* (0.00005+ (time_array.* 0.00005)));


% Desired exposure time and gain for synthesis (0.5 + 0.05*i) ms 
target_dt = 70;     %[ms]
target_gain = 0;   %[db]

% Names of the images to be used
images_names = {'outdoor_sample/1.png' 
                'outdoor_sample/15.png'
                'outdoor_sample/70.png'
                'outdoor_sample/90.png'};


% Prepare image series for CRF curve fitting
img_amount = length(images_names);
img_series = {};

for img_nus_imger = 1:img_amount
    img_series{img_nus_imger} = imread(images_names{img_nus_imger});
    % img_container{img_nus_imger} = rgb2gray(img_container{img_nus_imger});
    img_series{img_nus_imger} = (img_series{img_nus_imger});
end

crf = hdr_curve_fitting_outdoor (img_series);

if(plot_on)
    % plot
    figure(1)
    for img_nus_imger = 1:img_amount
        subplot(2,2,img_nus_imger); imshow(img_series{img_nus_imger});
    end

    figure(2);
    intensity_range = 1:256;
    plot(intensity_range, crf);
    grid on;
    xlabel('Intensity'); ylabel('log(E*dt)')
end

%% Using the fitted curve, image synthesizing
o_img = imread('outdoor_sample/1.png');

[s_img,K_t,K_g] = img_synth_outdoor (o_img, target_dt, target_gain, crf);

if (plot_on)
    figure(3)
    subplot(1,2,1); imshow(o_img); title('Original')
    subplot(1,2,2); imshow(s_img); title('Synthetic')
end


s_img = im2double(s_img); % normalize
s_img = imresize(s_img,0.25); % image down scale 
% s_img = rgb2gray(s_img); 

% local entropy mask size = (3x3)
H = entropyfilt(s_img, ones(3)); 

[rs, cs] = find(H <= 0.01); % Saturated region  

% generate saturation mask
M = [rs, cs]; 


mu = mean(sum(H)); 
sigma=std2(sum(H));
norm_entropy = normpdf(H, mu, sigma);
% entropy weight
H_weight = norm_entropy / sum(sum(norm_entropy));
S = zeros(size(s_img));

% image Gradient
[Gmag, Gdir] = imgradient(s_img,'sobel'); 

% Ours
G_weight= Gmag >= (mean(mean(Gmag))) * 0.1 ;

% Saturation weight
S(M(:,1), M(:,2)) = (mean(mean(Gmag))) * 2;    

% Metric
EWG = sum(sum(G_weight .* Gmag - S));


fprintf('Synth_exp: %.2f [ms] \n' , (0.5 + 0.05*target_dt)  )
fprintf('synth_gain: %.1f [dB] \n', target_gain)
fprintf('Entropy Weighted Gradient metric is : %.2f \n', EWG )



