function K_t = img_synth_factor_exp (crf, target_dt)
% function K_t = img_synth_factor_exp (crf, target_dt)
% Input:
%  crf = computed CRF
%  target_dt = target exposure time (ms)
% Output:
%  K_t = scaling factor for image synthesis 

global E

K_t = -1;

B = log(E* (0.00005+ (1.* 0.00005)));

% find point on CRF for initial exposure time B(1)
c_in = find (crf >= B(1));
if (isempty(c_in)),
    display('[ERROR: img_synth_factor_exp] No value found in CRF correponding to init exposure time')
    return;
end

c_int = c_in(1);
n_exp = B(1) + log(target_dt);

% find point on CRF for target exposure time
n_in = find (crf >= n_exp); 
if (isempty(n_in)),
    display('[ERROR: img_synth_factor_exp] No value found in CRF correponding to target exposure time')
    return;
end
n_int = n_in(1);

% compute ratio and return
K_t = (n_int) / (c_int); 