    function cfg = gp_cov_init (cfg)
% function cfg = gp_cov_init
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2013.12.15      ak          created and written
%    2017.08.30      ak          generalization

cfg.gp.k_SE = 1;        % Squared exponential
cfg.gp.k_NN = 2;        % neural network
cfg.gp.k_MEL = 3;       % mel
cfg.gp.k_POLY = 4;      % polynomial
cfg.gp.k_MATERN = 5;    % matern family
cfg.gp.k_EXP = 6;       % exponential
cfg.gp.k_GEXP = 7;      % gamma exponential
cfg.gp.k_RQ = 8;        % rational quadratic

% covariance function
cfg.gp.k = cfg.gp.k_SE;

switch (cfg.gp.k)
case cfg.gp.k_SE
%     % for prev dataset
%     param.l = 2;                 % length term
%     param.s_f = 3;               % exp term
    param.l = 15.5;                 % length term
    param.s_f = 4.85;               % exp term
    param.h1_range = 10:10:500;    % == l
    param.h2_range = 10:10:500;    % == s_f
    param.h1_name = 'l';
    param.h2_name = 's_f';
    param.s_n = 0.25;
    log_p = @log_prob_k_SE;
case cfg.gp.k_NN
    % NN S = diag (so,s,s,....)
    param.s_o = 20.78;
    param.s = 22.55;
    param.h1_range = 1e6:1e6:1e7;    % == s_o
    param.h2_range = 1e6:1e6:1e7;    % == s
    param.h1_name = 's_o';
    param.h2_name = 's';
    param.s_n = 0.75;
    log_p = @log_prob_k_NN;
case cfg.gp.k_MEL
    param.l = 10; %6.49;
    param.s = 10; %10.59;
    param.h1_range = 1:2:20;    % == l
    param.h2_range = 1:2:20;    % == s
    param.h1_name = 'l';
    param.h2_name = 's';
    param.s_n = 0.75;
    log_p = @log_prob_k_MEL;
case cfg.gp.k_POLY
    param.s_o = 9.0;
    param.p = 0.4;
    param.h1_range = 1:2:10;    % == s_o
    param.h2_range = 0.1:0.1:1;    % == p
    param.h1_name = 's_o';
    param.h2_name = 'p';
    param.s_n = 1.0;
    log_p = @log_prob_k_POLY;
case cfg.gp.k_MATERN
    param.l = 6.93;
    param.nu = 5/2;
    param.h1_range = 1:1:10;    % == l
    param.h2_range = [];
    param.h1_name = 'l';
    param.h2_name = '';
    param.s_n = 1.0;
    log_p = @log_prob_k_MATERN;
case cfg.gp.k_EXP
    param.l = 6;
    param.h1_range = 1:1:20;    % == l
    param.h2_range = [];    
    param.h1_name = 'l';
    param.h2_name = '';
    param.s_n = 0.5;
    log_p = @log_prob_k_EXP;
case cfg.gp.k_GEXP
    param.l = 11.0;
    param.g = 0.9;
    param.h1_range = 5:1:15;    % == l
    param.h2_range = 0.5:0.1:1.5;    % == g
    param.h1_name = 'l';
    param.h2_name = 'g';
    param.s_n = 1.0;
    log_p = @log_prob_k_GEXP;
case cfg.gp.k_RQ
    param.l = 1.6;
    param.a = 0.2;
    param.h1_range = 0.1:0.5:5;    % == l
    param.h2_range = 0.1:0.1:1;    % == a
    param.h1_name = 'l';
    param.h2_name = 'a';
    param.s_n = 1.0;
    log_p = @log_prob_k_RQ;
end

cfg.gp.k_log_p = log_p;
cfg.gp.param = param;
