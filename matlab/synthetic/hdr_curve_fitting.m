function crf = hdr_curve_fitting(img_series)
% function crf = hdr_curve_fitting(img_series)

% Input:
%  img_series = cell array of image series for curve fitting (we use 4)
% Output:
%  crf = 256xN_band vector containing curve information with each axis
%        N_band = # of channels (1 for grayscale)
%        x-axis [0, ..., 255]
%        y-axis [crf(1), ..., crf(256)]

global E;

% Smoothness parameter used in equation (3). Increase it, if you have a lot
% of noise in your images.
l = 200;

% time array samples (we use four samples)
time_array = [1, 15, 69, 89];   % [ms]

% B = log(E*dt) for time array samples
B = log(E* (0.00005+ (time_array.* 0.00005)));

% Number of randomly selected sample pixel points from each image. 
% This amount will affect the quality of the approximation of the 
% real response function. 
% You need at least N(P-1) > (Z_max - Z_min)
% N - amount of sample points
% P - amount of images % Assumes:% Zmin = 0% Zmax = 255
N_sample_pxpts = 1000;

% Bands (=channel) in each of the images. Usually RGB(3) or grayscale(1)
N_bands = 1;    % 3 for RGB

% Number of images for curve fitting
N_imgs = length(img_series);

% Create random indexes to get random points from each image
N_row = size(img_series{1}, 1);
N_col = size(img_series{1}, 2);
N_point = N_row*N_col;
random_indexes = ceil(rand(1, N_sample_pxpts)*N_point);

% Load all values from random points into Z
Z = zeros(N_sample_pxpts, N_imgs, N_bands);

for img_ii = 1:N_imgs
    for band_ii = 1:N_bands
        Z(:, img_ii, band_ii) = img_series{img_ii}(random_indexes + (band_ii-1)*N_point);
    end
end

% Create response function for each band
radiance_map = zeros(256, N_bands);
crf = zeros(256,N_bands);

for band = 1:N_bands
    radiance_map(:, band) = gsolve(Z(:,:,band), B, l, @hat_weight_func);
    crf(:,band) = radiance_map(:, band);
end
