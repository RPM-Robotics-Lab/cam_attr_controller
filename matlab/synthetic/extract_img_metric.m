function [ metric, s_img] = extract_img_metric(is_newg, o_img, target_exp_index, target_gain, crf )
%EXTRACT_IMG_METRIC Summary of this function goes here
%   Detailed explanation goes here


[s_img, K_t, K_g] = img_synth (o_img, target_exp_index, target_gain, crf);

if (target_gain < 1)
    Nsnr = 0;
else 
    Nsnr = calc_img_snr (s_img, o_img);
end

% figure(3);
% imshow(s_img);
% title(strcat(num2str(target_exp_index), '/', num2str(target_gain)));
% pause;

EWG = calc_img_newg (s_img);
if (is_newg)
    %% image metric evaluation with snr considered 
    NEWG = EWG - Nsnr;
    metric = NEWG;
else
    %% image metric evaluation
    metric = EWG;
end

metric = metric / 100;

end 

