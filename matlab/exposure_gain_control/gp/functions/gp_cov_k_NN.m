function Kij = gp_cov_k_NN (xi,xj,s0,s, cfg)
%    gp covariance function for NN
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.20      ak          created and written

[d, N] = size(xi);
Xi = [ones(N,1); xi];
Xj = [ones(N,1); xj];
S = diag([s0,ones(1,d)*s].^2);

num = 2*Xi'*S*Xj;
den = sqrt((1+2*Xi'*S*Xi)*(1+2*Xj'*S*Xj));

Kij = asin (num/den);
