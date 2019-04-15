function logprob = log_prob_k_SE (x,data,cfg)
%
%    log prob for SE cov function
%    x = [l
%         s_f
%         s_n]
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.20      ak          created and written

X = data(1:3,:);
y = data(4,:)';

n = length(y);
K = zeros(n,n);

l = x(1);
s_f = x(2);
s_n = cfg.gp.param.s_n;

for ii=1:n
    Xi = X(:,ii);
    for jj=1:ii
        Xj = X(:,jj);
        Kij = gp_cov_k_SE (Xi, Xj, l, s_f) + gp_cov_k_noise (Xi, Xj, s_n);
        K(ii,jj) = Kij;
        if (ii~=jj), K(jj,ii) = Kij; end
    end
end

logprob = y'/K*y+log(det(K));
