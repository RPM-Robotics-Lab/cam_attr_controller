function K_g = img_synth_factor_gain (target_gain)
% function K_g = img_synth_factor_gain (target_gain)
% Input:
%  target_gain = target gain (db)
% Output:
%  K_g = scaling factor for image synthesis 

K_g = (10 / sqrt(2))^(target_gain/20);