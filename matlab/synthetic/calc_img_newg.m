function EWG = calc_img_newg (o_img)
% function NEWG = calc_img_newg ()

global is_indoor;

tmp_img = im2double(o_img); % normalize
img = imresize(tmp_img,0.25); % image down scale 

if (size(img,3) > 1)
img = rgb2gray(img);
end

% local entropy mask size = (3x3)
H = entropyfilt(img, ones(1)); 
mu = mean(sum(H)); 
sigma=std2(sum(H));
norm_entropy = normpdf(H, mu, sigma);

% generate saturation mask
[rs, cs] = find(H <= 0.01); % Saturated region  
M = [rs, cs]; 

% entropy weight
H_weight = norm_entropy / sum(sum(norm_entropy)); % optional 

% image Gradient
[Gmag, Gdir] = imgradient(img,'Sobel'); 
Gmag = Gmag / 4.4867;

% Ours
G_weight= Gmag >= (mean(mean(Gmag))) * 0.01 ;

% Saturation weight
S = zeros(size(img));
if (is_indoor)
S(M(:,1), M(:,2)) = (mean(mean(Gmag)))/sqrt(2) ;    
else
S(M(:,1), M(:,2)) = (mean(mean(Gmag)))/sqrt(2) ;  
end    

saturation = sum(sum(S));
gradient = sum(sum(Gmag));

% Metric
EWG = sum(sum(G_weight .* Gmag - S)) ;



