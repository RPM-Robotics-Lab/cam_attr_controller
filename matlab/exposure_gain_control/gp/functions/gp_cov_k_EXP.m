function Kij = gp_cov_k_EXP (Xi, Xj, l, cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

r = norm (Xi-Xj);
Kij = exp (-r/l);