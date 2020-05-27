function EWG = calc_img_newg (img)
% function NEWG = calc_img_newg ()

global is_indoor;
img = im2double(img); % normalize
% img = im2double(o_img); % normalize
img = imresize(img,0.25); % image down scale 

if (size(img,3) > 1)
img = rgb2gray(img);
end

% local entropy mask size = (3x3)
H = entropyfilt(img, ones(3)); 
mu = mean(sum(H)); 
sigma=std2(sum(H));
norm_entropy = normpdf(H, mu, sigma);

% generate saturation mask
[rs, cs] = find(H <= 0.01); % Saturated region  

M = [rs, cs]; 


% entropy weight
H_weight = norm_entropy / sum(sum(norm_entropy)); % optional 

% image Gradient
[Gmag, Gdir] = imgradient(img,'sobel'); 

% Ours
G_weight= Gmag >= (mean(mean(Gmag))) * 0.01 ;

% Saturation weight
Smask = uint8(zeros(size(img))) ;
Sval = (mean(mean(Gmag))) ;
Smask(sub2ind([size(H)], rs, cs)) =255;

if (is_indoor)
    Sval = (mean(mean(Gmag))) ;
    Smask = Smask.*Sval;
else
    Sval = (mean(mean(Gmag))) ;
    Smask = Smask.*Sval;
end    


saturation = sum(sum(Smask)) / 255;
gradient = sum(sum(Gmag));

% Metric
EWG = sum(sum(G_weight .* Gmag )) - saturation; 

GsE= [gradient saturation  EWG];


