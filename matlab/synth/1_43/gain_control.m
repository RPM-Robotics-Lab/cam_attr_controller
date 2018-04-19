clc;        % Clear the command window.
close all;  % Close all figures (except those of imtool.)
clear;      % Erase all existing variables.
workspace;  % Make sure the workspace panel is showing.
fontSize = 22;

%Load single  image
I = imread('e1g6.bmp'); 

% addition of graininess (i.e. noise)
% I_noise = imnoise(I, 'gaussian', 0.09); 

% the average of 3^2, or 9 values(filters the multidimensional array A with the multidimensional filter h)
%  h = ones(3,3) / 2.5^2; 
%  I2 = imfilter(I_noise,h); 
% Measure signal-to-noise ratio
% img=I2;
img=I;
img=double(img(:));
ima=max(img(:));
imi=min(img(:));
mse=std(img(:));
snr=20*log10((ima-imi)./mse)


% Measure Peak SNR
% [peaksnr, snr] = psnr(I_noise, I);
% fprintf('\n The Peak-SNR value is %0.4f', peaksnr);
fprintf('\n The SNR value is %0.4f \n', snr);
fprintf('\n The MSE value is %0.4f \n', mse);

   %Plot original & filtered figure
%    subplot(1,2,1), imshow(I_noise), title('Original image') 
%    subplot(1,2,2), imshow(I2), title('Filtered image')
%     text(size(I,2),size(I,1)+15, ...
%       'Gaussian = 0.09', ...
%       'FontSize',10,'HorizontalAlignment','right');

