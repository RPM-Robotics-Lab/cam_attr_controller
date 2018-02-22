function [ idx, psi_update, acquisition_func ] = gpmi_optim( y_pred, var_pred, alpha, psi)
%GPMI_OPTIM Summary of this function goes here
%   Detailed explanation goes here

if (size(var_pred,1) == size(var_pred,2))
    var_diag = diag(var_pred);
else
    var_diag = var_pred;
end

var_eval = sqrt(alpha) * (sqrt(var_diag.^2 + psi) - sqrt(psi));
acquisition_func = y_pred + var_eval;

[val, idx] = max(acquisition_func);
psi_update = psi + var_diag(idx);

end

