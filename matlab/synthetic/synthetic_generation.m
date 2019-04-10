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

for img_number = 1:img_amount
    img_series{img_number} = imread(images_names{img_number});
    % img_container{img_number} = rgb2gray(img_container{img_number});
    img_series{img_number} = (img_series{img_number});
end

crf = hdr_curve_fitting (img_series);

if(plot_on)
    % plot
    figure(1)
    for img_number = 1:img_amount
        subplot(2,2,img_number); imshow(img_series{img_number});
    end

    figure(2);
    intensity_range = 1:256;
    plot(intensity_range, crf);
    grid on;
    xlabel('Intensity'); ylabel('log(E*dt)')
end

%% Using the fitted curve, image synthesizing
o_img = imread('sample_pic2/1.png');
target_dt = 5;     %[ms]
target_gain = 2;   %[db]
[s_img,K_t,K_g] = img_synth(o_img, target_dt, target_gain, crf);

if (plot_on)
    figure(3)
    subplot(1,2,1); imshow(o_img); title('Original')
    subplot(1,2,2); imshow(s_img); title('Synthetic')
end

% pause(.001)
% imwrite(s_img, sprintf('syn%d.png' , in_exp ) );

