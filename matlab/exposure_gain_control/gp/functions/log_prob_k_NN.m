function logprob = log_prob_k_NN (x,data,cfg)
%
%    log prob for NN cov function
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.20      ak          created and written
%    2013.12.26      ak          added noise term to k_NN

X = data(1:3,:);
y = data(4,:)';

n = length(y);
K = zeros(n,n);

s0 = x(1);
s = x(2);
s_n = cfg.gp.param.s_n;

for ii=1:n
    Xi = X(:,ii);
    for jj=1:ii
        Xj = X(:,jj);
        Kij = gp_cov_k_NN (Xi,Xj,s0,s) + gp_cov_k_noise (Xi, Xj, s_n);
        K(ii,jj) = Kij;
        if (ii~=jj), K(jj,ii) = Kij; end
    end
end

logprob = y'/K*y+log(det(K));
