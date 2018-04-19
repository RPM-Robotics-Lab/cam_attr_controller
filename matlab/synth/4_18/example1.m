
clear all;
close all;
N = 507;
Low = 1;

for q = Low:1:N 
 
mb=im2double(imread(sprintf('%d.png', q)));

%% compute gain snr
img=mb;
img=double(img(:));
ima=max(img(:));
imi=min(img(:));
mse=std(img(:));
snr=20*log10((ima-imi)./mse);
SNR(q,:) = [q, snr];

%% Convert  image ( entropy, Gaussian) 
% mb = rgb2gray(mb);
J = entropyfilt(mb); 
[rs, cs] = find(J<=0.01); % Saturated region 
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
S(M(:,1), M(:,2)) = -mean(mean(Gmag));
SVal = -mean(mean(Gmag));

% k = 2;
% if i = 1: 0.5*752 
% WR = sin((pi / 2*(0.5 * 752)) * i).^k
% 
% else j = 0.5*752 : 752
% WR(:,j) = sin (pi/2 - pi*(j-(0.5*752) / 2*(752- (0.5*752)))).^k 
% end
alpha = 3;
GVal =  alpha / snr ; 
G =  -GVal * mean(mean(Gmag))



GmagOur = sum(GF .* Gmag + S + G) ;
GmagS = sum(real(log(lambda*(GmagSim-delta)+1))/N);
GmagR = sum(GFR.* Gmag);

SG = sqrt(sum(GmagS)); 
RG = sqrt(sum(GmagR));
WG = (sum(W.*GmagOur));

% WG = sum(G);
SG2(q,:) = [q,SG];
RG2(q,:) = [q, RG];
WG2(q, :) = [q, WG];

    if ( q >= 20) 
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
figure(1)
subplot(1,3,1); plot(SG2(:,1), SG2(:,2))
% figure(4); plot(SG2(:,1), SG2(:,2))
hold on 
[maxval maxloc] = max(SG2(:,2));
% scatter(maxloc, maxval)
xlabel('Exposure Time [ms]'); 
ylabel(' Gradient'); 

subplot(1,3,2); plot(RG2(:,1), RG2(:,2))
% figure(5); plot(RG2(:,1), RG2(:,2))
hold on 
[maxval maxloc] = max(RG2(:,2));
% scatter(maxloc, maxval)
xlabel('Exposure Time [ms]'); 
ylabel( 'Gradient(Percentile)'); 

subplot(1,3,3); plot(WG2(:,1), WG2(:,2))
% figure(6); plot(WG2(:,1), WG2(:,2))
hold on 
[maxval maxloc] = max(WG2(:,2));
% scatter(maxloc, maxval)
xlabel('Exposure Time [ms]'); 
ylabel('Weighted Gradient based Entropy '); 


% pause(0.0000001)

    end 
    
end

data = WG2(:,2)
redata = reshape(data, 13,39)

csvwrite('d1_Ours.csv' , redata)
csvwrite('d1_Shim.csv' , SG2)
csvwrite('d1_Chang.csv' , RG2)
csvwrite('SNR.csv' , SNR)



