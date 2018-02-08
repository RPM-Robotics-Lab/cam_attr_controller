function [ idx, psi_update, acquisition_func ] = gpmi_optim( y_pred, var_pred, alpha, psi)
%GPMI_OPTIM Summary of this function goes here
%   Detailed explanation goes here
var_diag = diag(var_pred);
var_eval = sqrt(alpha) * (sqrt(diag(var_pred).^2 + psi) - sqrt(psi));
acquisition_func = y_pred + var_eval;

[val, idx] = max(acquisition_func);
psi_update = psi + var_diag(idx);

end

