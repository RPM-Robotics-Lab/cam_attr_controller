function Kij = gp_cov_fcns (Xi, Xj, cfg)
%
%    Xi = 3xN
%    Xj = 3xN
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.08.28      ak          created and written
%    2013.12.15      ak          change to param struct
%    2013.12.26      ak          run each cov fcns from mfile

param = cfg.gp.param;

switch cfg.gp.k
case cfg.gp.k_SE
    Kij = gp_cov_k_SE_param (Xi,Xj,param, cfg);
case cfg.gp.k_NN
    Kij = gp_cov_k_NN_param (Xi,Xj,param, cfg);
case cfg.gp.k_MEL
    Kij = gp_cov_k_MEL_param (Xi,Xj,param, cfg);
    %C = cov_fcns_melkumyan_mahal (Xi,Xj,param);
case cfg.gp.k_POLY
    Kij = gp_cov_k_poly_param (Xi, Xj, param, cfg);
case cfg.gp.k_MATERN
    Kij = gp_cov_k_matern_param (Xi, Xj, param, cfg);
case cfg.gp.k_EXP
    Kij = gp_cov_k_exp_param (Xi, Xj, param, cfg);
case cfg.gp.k_GEXP
    Kij = gp_cov_k_gexp_param (Xi, Xj, param, cfg);
case cfg.gp.k_RQ
    Kij = gp_cov_k_RQ_param (Xi, Xj, param, cfg);
end

% add noise K term
% s_n = cfg.gp.param.s_n;
% Kij = Kij + gp_cov_k_noise (Xi, Xj, s_n);

%----------------------------------------------------
function Kij = gp_cov_k_SE_param (Xi,Xj,param, cfg)
% a squared exponential in GPML book

l = param.l;
s_f = param.s_f;

Kij = gp_cov_k_SE (Xi, Xj, l, s_f, cfg);


function Kij = gp_cov_k_NN_param (Xi,Xj,param, cfg)
% Neural network covariance function

s0 = param.s_o; % covariance of NN
s = param.s;

Kij = gp_cov_k_NN (Xi, Xj, s0, s, cfg);


function Kij = gp_cov_k_MEL_param (Xi,Xj,param, cfg)
% a simple stationay cov function used by Melkumyan and Barkby

l = param.l;
s = param.s;

Kij = gp_cov_k_MEL (Xi, Xj, l, s, cfg);

%function C = cov_fcns_melkumyan_mahal (Xi,Xj,l,s,Cij)
% a simple stationay cov function used by Melkumyan and Barkby

%[xij,J]=head2tail(Xi,Xj);
%d = norm (xij(1:3));
%l = 1;
% Sii = Cij(1:6,1:6);
% Sjj = Cij(7:12,7:12);
% e=Xi-Xj;
% e1 = e'*inv(Sii)*e;
% e2 = e'*inv(Sjj)*e;
% d=sqrt(min(e1,e2));
% thresh = chi2inv(1-2*normcdf(-l),6);
% 
% if (d < thresh) % if distance is smaller than l
%     term1 = ((2+cos(2*pi*d/thresh))/3)*(1-d/thresh);
%     term2 = 1/(2*pi)*sin(2*pi*d/thresh);
%     C = s*(term1 + term2);
% else
%     C = 0;
% end


function Kij = gp_cov_k_poly_param (Xi, Xj, param, cfg)
% polynomial covariance function

p = param.p;
s0 = param.s_o;

Kij = gp_cov_k_POLY (Xi, Xj, p,s0, cfg);


function Kij = gp_cov_k_matern_param (Xi, Xj, param, cfg)
% matern family covariance function

l = param.l;
nu = param.nu;

Kij = gp_cov_k_MATERN (Xi, Xj, l, nu, cfg);


function Kij = gp_cov_k_exp_param (Xi, Xj, param, cfg)

l = param.l;

Kij = gp_cov_k_EXP (Xi, Xj, l, cfg);


function Kij = gp_cov_k_gexp_param (Xi, Xj, param, cfg)

l = param.l;
g = param.g;

Kij = gp_cov_k_GEXP (Xi, Xj, l, g, cfg);


function Kij = gp_cov_k_RQ_param (Xi, Xj, param, cfg)

a = param.a;
l = param.l;

Kij = gp_cov_k_RQ (Xi, Xj, a, l, cfg);