function Kij = gp_cov_k_RQ (Xi, Xj, l, a, cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

r = norm (Xi-Xj);

Kij = (1+r^2/(2*a*l^2))^(-a);
