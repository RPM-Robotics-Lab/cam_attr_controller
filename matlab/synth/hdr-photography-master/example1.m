
clear all;
close all;
N = 18;
Low = 1;

for q = Low:1:N 
 
mb=im2double(imread(sprintf('ss%d.png', q)));

%% Convert  image ( entropy, Gaussian) 
% mb = rgb2gray(mb);
J = entropyfilt(mb); 
[rs, cs] = find(J<=0.05); % Saturated region 
K = sum(J); % entropy row sum 
% Th = find(K < mean(K));
% K(1,Th)= 0;
mu = mean(K); % entropt median
sigma=std2(K);
GP = normpdf(K, mu, sigma);
W = GP / sum(GP);
S = zeros(size(mb));
M = [rs, cs]; 

%% Gradient image
[Gmag, Gdir] = imgradient(mb,'prewitt'); 
% dr.Shim method 
delta = 0.001;
lambda = 0.1;
GmagSim = (Gmag-min(Gmag(:)))/(max(Gmag(:))-min(Gmag(:)));
N = log(lambda*(1-delta)+1);

% Percentile (RPG)
GFR = Gmag > (median(mean(Gmag)));

% Ours
GF= Gmag > (mean(mean(Gmag))) *0.5 ;
CountG = size(find(GF==1)');

% Saturation weight
S(M(:,1), M(:,2)) = -mean(mean(Gmag)) * 0.86;
SVal = -mean(mean(Gmag))* 0.86;

% k = 2;
% if i = 1: 0.5*752 
% WR = sin((pi / 2*(0.5 * 752)) * i).^k
% 
% else j = 0.5*752 : 752
% WR(:,j) = sin (pi/2 - pi*(j-(0.5*752) / 2*(752- (0.5*752)))).^k 
% end

GmagOur = sum(GF .* Gmag + S) ;
GmagS = sum(real(log(lambda*(GmagSim-delta)+1))/N);
GmagR = sum(GFR.* Gmag);

SG = sqrt(sum(GmagS)); 
RG = sqrt(sum(GmagR));
WG = (sum(W.*GmagOur));

% WG = sum(G);
SG2(q,:) = [q,SG];
RG2(q,:) = [q, RG];
WG2(q, :) = [q, WG];

    if ( q >= 1) 
    figure(1)
    plot(WG2(:,1), WG2(:,2));
    hold on;
    L = max(WG2(:,2)); 
    Lp = find(WG2(:,2) == L) ;
    plot(Lp, WG2(Lp,2), 'r*')
    fprintf('measured exposure time is %d [ms]\n' , Lp) 
  
    bi = [WG2(q,:); WG2(q,:)] ;
  
    min_v = min(WG2(:,1)); max_v = max(WG2(:,1)); 
    x1 = min_v:1 :max_v ;

    a1 = (bi(2,2)-bi(1,2))  /  (bi(2,1) - bi(1,1)) ;
    y1 = a1 * (x1- bi(1,1)) + bi(1,2);

        % find solution 
        if (a1 < 0)    
        fprintf('%d is optimal !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! updated  \n' , bi(1,1))
        mp=im2double(imread(sprintf('%d.png',  bi(1,1))));
        figure(2)
        imshow(mp)

        break;
        end
        % plot two lines
        plot(x1, y1, ':k')
        pause(.1)

%% figure 2
% figure(1)
% subplot(1,3,1); plot(SG2(:,1), SG2(:,2))
% % figure(4); plot(SG2(:,1), SG2(:,2))
% hold on 
% [maxval maxloc] = max(SG2(:,2));
% % scatter(maxloc, maxval)
% xlabel('Exposure Time [ms]'); 
% ylabel(' Gradient'); 
% 
% subplot(1,3,2); plot(RG2(:,1), RG2(:,2))
% % figure(5); plot(RG2(:,1), RG2(:,2))
% hold on 
% [maxval maxloc] = max(RG2(:,2));
% % scatter(maxloc, maxval)
% xlabel('Exposure Time [ms]'); 
% ylabel( 'Gradient(Percentile)'); 
% 
% subplot(1,3,3); 
plot(WG2(:,1), WG2(:,2))
% figure(6); plot(WG2(:,1), WG2(:,2))
hold on 
[maxval maxloc] = max(WG2(:,2));
% scatter(maxloc, maxval)
xlabel('Exposure Time [ms]'); 
ylabel('Entropy Weighted Gradient  '); 


pause(0.0000001)

    end 
    
end


csvwrite('d2_Ours.csv' , WG2)
csvwrite('d2_Shim.csv' , SG2)
csvwrite('d2_Chang.csv' , RG2)




