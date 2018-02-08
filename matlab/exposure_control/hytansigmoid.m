clear all;
close all;

%% control level
N = 9;
print_pdf = 0;

% font size for axis, label, and title
axis_fontsize = 10;
label_fontsize = 14;
title_fontsize = 16;

%% plot
figure(1); 
%subplot(2,1,1);
hold on;

for j = 0:2:N
    i =2^j;
    x = -0.2:0.005:1;
    a = (2./(1+ exp(-1 * i.*x + 2^2 )))-1;
    subplot(2,1,1)
    hold on;
    plot(x,a, 'LineWidth', 2)    
    %pause(1)
end

grid on 
title('Hyperbolic tangent sigmoid transfer function')
xlabel('Entropy'); 
ylabel('Activation function');
legend('\alpha = 2^0, \tau=2^2' , '\alpha = 2^2, \tau=2^2', '\alpha = 2^4, \tau=2^2', '\alpha = 2^6, \tau=2^2', '\alpha = 2^8, \tau=2^2','Location','southeast')
axis([-0.2,1,-1,1])

%% Adjust font size
set(gca,'FontSize',axis_fontsize);
xlable_h = get(gca, 'xlabel'); set(xlable_h, 'FontSize', label_fontsize);
ylable_h = get(gca, 'ylabel'); set(ylable_h, 'FontSize', label_fontsize);

set(gcf, 'PaperPosition', [-0.1 -0.25 16 15.0]); %Position the plot further to the left and down. Extend the plot to fill entire paper.
set(gcf, 'PaperSize', [15.5 15]); %Keep the same paper size
%% Change file save option

if (print_pdf)
    print -dpdf ../figs/weightf.pdf
end