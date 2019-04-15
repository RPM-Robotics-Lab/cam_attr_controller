function logprob = log_prob_k_POLY (x,data,cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

X = data(1:3,:);
y = data(4,:)';

n = length(y);
K = zeros(n,n);

so = x(1);
p = x(2);
s_n = cfg.gp.param.s_n;

for ii=1:n
    Xi = X(:,ii);
    for jj=1:ii
        Xj = X(:,jj);
        Kij = gp_cov_k_POLY (Xi, Xj, p, so) + gp_cov_k_noise (Xi, Xj, s_n);
        K(ii,jj) = Kij;
        if (ii~=jj), K(jj,ii) = Kij; end
    end
end

logprob = y'/K*y+log(det(K));
