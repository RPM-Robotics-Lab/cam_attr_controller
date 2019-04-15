function K_t = img_synth_factor_exp (crf, target_dt)
% function K_t = img_synth_factor_exp (crf, target_dt)
% Input:
%  crf = computed CRF
%  target_dt = target exposure time (ms)
% Output:
%  K_t = scaling factor for image synthesis 

% B1 = log(E* 1 * exp_itv)
% target_exp = log(E* (1 * target_dt * exp_itv)) 
%       = log(E * 1 * target_dt * exp_itv) = B1 + log(target_dt) 

global B;
global E;
global time_itv;
global target_exp_index;
global is_indoor;
    
K_t = -1;


% find point on CRF for initial exposure time B(1)
init_index = find (crf >= B(1));
if (isempty(init_index))
    disp('[ERROR: img_synth_factor_exp] No value found in CRF correponding to init exposure time');
    return;
end

gi = init_index(1)


if (is_indoor)
    target_exp = log(E* (0.0001+ (target_dt.* time_itv)));
else
    target_exp = log(E* (0.00005+ (target_dt.* time_itv)));
end

% find point on CRF for target exposure time
target_index = find (crf >= target_exp); 
if (isempty(target_index))
    disp('[ERROR: img_synth_factor_exp] No value found in CRF correponding to target exposure time');
    return;
end
go = target_index(1)

% compute ratio and return
K_t = (go) / (gi)