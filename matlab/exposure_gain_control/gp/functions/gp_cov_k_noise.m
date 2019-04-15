function Kij = gp_cov_k_noise (Xi, Xj, s_n)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

del = 0; if (norm(Xi-Xj)==0), del = 1; end
Kij = s_n.^2*del;
