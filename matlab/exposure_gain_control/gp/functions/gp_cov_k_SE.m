function C = gp_cov_k_SE (Xi, Xj, l, s_f, cfg)
%    gp covariance function for SE
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.20      ak          created and written

dim = size(Xi, 1);
M = eye(dim)/(l^2);
%M = diag(1./[0.2,3.0,1.0].^2)/(l^2);
C = s_f.^2*exp(-0.5*(Xi-Xj)'*M*(Xi-Xj));
