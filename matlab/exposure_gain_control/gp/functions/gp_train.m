function K = gp_train (x_train, y_train, cfg)
% 
%    x_train: x data to train
%    cfg: contain cov function params
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2017.08.16      ak          created and written

num = size(x_train, 2);
K = zeros(num,num);

for ii=1:num
    for jj=ii:num
        Xii = x_train(:, ii);
        Xjj = x_train(:, jj);
        Kij = gp_cov_fcns (Xii, Xjj, cfg);
        K(ii,jj) = Kij;
        if (ii~=jj), K(jj,ii) = Kij; end;
    end
end
