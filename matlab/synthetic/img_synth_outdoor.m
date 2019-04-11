function [synth_img,K_t,K_g] = img_synth(o_img, target_dt, target_gain, crf)
%function [synth_img,K_t,K_g] = img_synth(o_img, target_dt, target_gain, crf)
% 
% Input:
%  o_img = original input seed image
%  g = gain
%  t = exposure time
% Output:
%  synth_img = output synthesized image

% We compute two scaling factors for target exposure time and gain
K_t = img_synth_factor_exp_outdoor (crf, target_dt);   % factor from exposure time
K_g = img_synth_factor_gain (target_gain);     % factor from gain

if (K_t < 0 || K_g < 0)
	disp('[ERROR: img_synth] No scale factor K found');
    synth_img = [];
    return;
end

K = K_t * K_g;
synth_img = o_img * K;
