%% Input parameters
close all;
clear all;
% Names of the images to be used
% images_names = {
%                 'sample_pictures/StLouisArchMultExpEV-4.72.JPG' 
%                 'sample_pictures/800px-StLouisArchMultExpEV-1.82.JPG' 
%                 'sample_pictures/800px-StLouisArchMultExpEV+1.51.JPG' 
%                 'sample_pictures/StLouisArchMultExpEV+4.09.JPG'
%                };
images_names = {
                'sample_pic2/1.png' 
                'sample_pic2/10.png'
                'sample_pic2/30.png' 
                'sample_pic2/50.png' 
                'sample_pic2/70.png'
                'sample_pic2/90.png'
               };
% Natural logarithm of shutter speed for each of the above images 0.05, 7,
% 13.5, 20ms, E = 20

 o_img = imread('sample_pic2/1.png');
% o_img = imread('sample_pictures/StLouisArchMultExpEV-4.72.JPG');
% o_img = imread('sample_pictures/800px-StLouisArchMultExpEV-1.82.JPG');
% o_img = imread('sample_pictures/800px-StLouisArchMultExpEV+1.51.JPG');
% o_img = imread('sample_pictures/StLouisArchMultExpEV+4.09.JPG');
% o_img = rgb2gray(o_img);
img_bright = mean(mean(o_img))

E = img_bright/2; % 180, ? choice

B = [log(E* (0.00005+ (1 * 0.00005))) log(E* (0.00005+ (9 * 0.00005))) log(E* (0.00005+ (29 * 0.00005))) log(E* (0.00005+ (49 * 0.00005))) log(E* (0.00005+ (69 * 0.00005))) log(E* (0.00005+ (89 * 0.00005)))];

% Smoothness parameter used in equation (3). Increase it, if you have a lot
% of noise in your images.
l = 200; %200

% Amount of randomly selected sample points from each image. This amount
% will affect the quality of the approximation of the real response
% function. You need at least N(P-1) > (Z_max - Z_min)
% N - amount of sample points
% P - amount of images % Assumes:% Zmin = 0% Zmax = 255
sample_points_amount = 1000;

%Bands amount in each of the images. Usually RGB(3) or grayscale(1)
bands_amount = 1;


%% Recovering response function
img_amount = length(images_names);

% Loading images
img_container = {};

for img_number = 1:img_amount
    img_container{img_number} = imread(images_names{img_number});
%     img_container{img_number} = rgb2gray(img_container{img_number});
    img_container{img_number} = (img_container{img_number});

end

% Create random indexes to get random points from each image
row_amount = size(img_container{1}, 1);
col_amount = size(img_container{1}, 2);
point_amount = row_amount*col_amount;
random_indexes = ceil(rand(1, sample_points_amount)*point_amount);

% Load all values from random points into Z
Z = zeros(sample_points_amount, img_amount, bands_amount);

for img_number = 1:img_amount
    for band = 1:bands_amount
        Z(:, img_number, band) = img_container{img_number}(random_indexes + (band-1)*point_amount);
    end
end

% Create response function for each band
radiance_map = zeros(256, bands_amount);
radiance_map_obj = {};

for band = 1:bands_amount
    radiance_map(:, band) = gsolve(Z(:,:,band), B, l, @hat_weight_func);
    radiance_map_obj{band} = radiance_map(:, band);
end

%% Plot the response functions for each band.



%% Getting the actual radiance values of the scene.
% The most time consuming part

% Equation (6) from the debevec paper.
% This equation is used to get the natural logarithm of radiance values
% on the scene and to get rid of artifacts

%Corner case: ones() matrix to avoid dividing by 0
denominator = ones(row_amount, col_amount, bands_amount);
numerator = zeros(row_amount, col_amount, bands_amount);

response = radiance_map_obj;


    figure(3)
    imshow(o_img)
%% Generate Synthetic images 
% for in_exp = 1 : abs(log(0.0005 * 20) + 4)*2 : abs(log(0.0005 * 20) + 4)*2*39  
   for in_exp = 1 : 91
%     c_exp = B(1)* (0.95+ 0.001*in_exp) ;
    c_exp = B(1) ;

    c_in = find (radiance_map_obj{1} >= c_exp);
    c_int = c_in(1);
%     n_exp = in_exp;
    atio =1;
    n_exp = B(1) + log(atio * (in_exp)) %* abs(log(0.0005 * 20) + 4)* 1.69);
 

    n_in = find (radiance_map_obj{1} >= n_exp);
    n_int = n_in(1)

    k_exp = exp(n_exp) / exp(c_exp); % irradiance 
    k_int = (n_int) / (c_int);  % intensity
    kk_int(in_exp, :) = k_int;
    for band = 1:bands_amount
            g_zij(:,:,band) =  radiance_map_obj{band}( k_exp * o_img(:, :, band)+1);
    end
    
    E_zij =  g_zij - c_exp ;
    n_img = kk_int(in_exp) * o_img ;
    
    for band = 1:bands_amount
        band_max = max( max(g_zij(:, :, band)) );
        band_min = min( min(g_zij(:, :, band)) );
        g_zij(:, :, band) = ( g_zij(:, :, band) + abs(band_min) ) / ( band_max + abs(band_min) );
    end
    
    figure(1)
    subplot(1,2,1)
    % plot(radiance_map_obj{1}, 1:256, 'r');
    plot(radiance_map_obj{1},1:256, 'r');
    hold on
    scatter( radiance_map_obj{1}(n_int), n_int)
    hold on
    title('Camera response function');
    xlabel('Irradiance');
    ylabel('Intensity');
   
    
    subplot(1,2,2)
    plot(1:in_exp, kk_int)
    hold on
    title('Intensity Mapping function');
    xlabel('Exposure steps');
    ylabel('Ratio');
    scatter(in_exp , kk_int(in_exp))

    
    
    seq_img=im2double(imread(sprintf('sample_pic2/%d.png', in_exp)));
  
figure(3)
syn_img = im2uint8(n_img);
% imshow(tone_mapped_img_original);
%     imshow(g_zij)
    imshowpair(seq_img, syn_img, 'montage')
pause(.001)
imwrite(syn_img, sprintf('syn%d.png' , in_exp ) );

    
end

