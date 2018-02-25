function [mu, var] = gp_predict (x_test, x_train, K, y_train, cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.09.09      ak          created and written

mu = [];
var = [];

r = length(x_train);
c = length(x_test);
k_star = zeros(r,c);

for kk=1:r
    Xkk = x_train(kk);
    for ii=1:c
        Xnew = x_test(ii);
        k_star(kk,ii) = gp_cov_fcns (Xkk, Xnew, cfg);
    end
end

% A=K+sigma_n*eye(n_test); inv(A)*y
s_n = cfg.gp.param.s_n;
K_sub = K;
y_sub = y_train;
L = chol(K_sub + (s_n^2)*eye(r));
alpha = L\(L'\y_sub);

% mu prediction
mu = k_star'*alpha;

% var prediction
v = L' \ k_star;
var = gp_cov_fcns (Xnew, Xnew, cfg) - v'*v;
