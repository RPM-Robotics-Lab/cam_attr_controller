clear all; close all;

print_pdf = 1;
offset = 120;
fontsz = 15;

WG2 = csvread('dat/fig1.csv');
fig = figure(6); subplot(2,1,1);
plot(WG2(:,1)*100, WG2(:,2)*10, 'Linewidth', 2)
hold on 
[maxval maxloc] = max(WG2(:,2));
a = 'A';
b = num2str(a); c = cellstr(a);

% plot points as dots
plot(400,  10*WG2(4,2),  'b.', 'MarkerSize' , 20)
plot(1500, 10*WG2(15,2), 'g.', 'Markersize' , 20)
plot(2600, 10*WG2(26,2), 'r.', 'Markersize' , 20)
plot(3700, 10*WG2(37,2), 'c.', 'Markersize' , 20)
plot(4700, 10*WG2(47,2), 'm.', 'Markersize' , 20)


text(400,  10* WG2(4,2)+offset,'A', 'Color', 'b', 'Fontsize' , fontsz)
text(1500, 10* WG2(15,2)+offset,'B', 'Color', 'g', 'Fontsize' , fontsz)
text(2600, 10* WG2(26,2)+offset,' C', 'Color', 'r', 'Fontsize' , fontsz)
text(3700, 10* WG2(37,2)+offset,'D', 'Color', 'c', 'Fontsize' , fontsz)
text(4700, 10* WG2(47,2)+offset,'E', 'Color', 'm', 'Fontsize' , fontsz)

xlabel('Exposure Time [us]'); 
ylabel('Image Information'); 

%% Save figure
% % font setup
% set(gca, 'FontName', 'Arial')  
% set(gca, 'FontSize', 10);
% % paper setup
% set(fig, 'PaperUnits', 'centimeters');
% set(fig, 'PaperSize', [10 10]);

grid on;

%% Change file save option
if (print_pdf)
    print -dpdf ../figs/grappdf.pdf
end