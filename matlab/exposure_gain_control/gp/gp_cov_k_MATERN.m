function Kij = gp_cov_k_MATERN (Xi, Xj, l, nu, cfg)
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.26      ak          created and written

r = norm (Xi-Xj);

%term1 = 1/(gamma(nu)*2^(nu-1));
%term2 = (sqrt(2*nu)*r/l);
%term3 = term2^nu;
%C =  term1 * term2 * Knu *term3;

% from GPML nu = 3/2 or 5/2 suggested

if nu == 3/2
    Kij = (1+sqrt(3)*r/l)*exp(-sqrt(3)*r/l);
elseif nu == 5/2 
    Kij = (1+sqrt(5)*r/l+5*r^2/(3*l^2))*exp(-sqrt(5)*r/l);
else
    fprintf (1,'nu = 3/2 or 5/2 suggestedn');
end

