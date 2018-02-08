function Kij = gp_cov_k_POLY (Xi, Xj, p, s0, cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

Kij = (Xi'*Xj + s0*s0).^p;