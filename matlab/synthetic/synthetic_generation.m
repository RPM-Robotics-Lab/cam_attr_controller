close all;
clear all;

%% HDR CRF curve fitting from a set of images
global E;   % irradiance

plot_on = 1;
E = 20;


% Names of the images to be used
images_names = {'sample_pic2/1.png' 
                'sample_pic2/10.png'
                'sample_pic2/70.png'
                'sample_pic2/90.png'};


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
        subplot(2,2,img_nus_imger); imshow(img_series{img_nus_imger});
    end

    figure(2);
    intensity_range = 1:256;
    plot(intensity_range, crf);
    grid on;
    xlabel('Intensity'); ylabel('log(E*dt)')
end

%% Using the fitted curve, image synthesizing
o_img = imread('sample_pic2/1.png');
target_dt = 1;     %[ms]
target_gain = 0;   %[db]
[s_img,K_t,K_g] = img_synth(o_img, target_dt, target_gain, crf);

if (plot_on)
    figure(3)
    subplot(1,2,1); imshow(o_img); title('Original')
    subplot(1,2,2); imshow(s_img); title('Synthetic')
end


s_img = im2double(s_img); % normalize
s_img = imresize(s_img,0.25); % image down scale 
% s_img = rgb2gray(s_img); 

H = entropyfilt(s_img, ones(5)); % local entropy (5x5)

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
GmagOur = sum(sum(G_weight .* Gmag - S)) 



