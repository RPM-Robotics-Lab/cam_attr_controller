function Kij = gp_cov_k_GEXP (Xi, Xj, l, g, cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

r = norm (Xi-Xj);
Kij = exp (-r/l)^g;
