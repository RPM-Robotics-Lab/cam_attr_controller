function Nsnr = calc_img_snr (img, ref_img)
% function NEWG = calc_img_snr ()

img = im2double(img); % normalize
img = imresize(img,0.25); % image down scale 
[Gmag] = imgradient(img,'sobel'); 

ref_img = im2double(ref_img); % normalize
ref_img = imresize(ref_img,0.25); % image down scale 


% Calculate snr 
[peaksnr, snr] = psnr(img, ref_img);
% acceptance snr 
ref_snr = 20; 
% calculate snr compensate
Nsnr = (1- peaksnr/ref_snr) * mean(mean(Gmag)) * (size(img,1) * size(img,2))/5^3;


