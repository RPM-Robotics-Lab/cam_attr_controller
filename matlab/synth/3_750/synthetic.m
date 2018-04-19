for j = 1:0.2:3
    im=imread('144.png');
%     im= rgb2gray(im);
    im2 =imread('131.png');
%     im2= rgb2gray(im2);
    Linear = j;
    Lin = im * Linear;
    figure(3)
    imshow(Lin)
%     imwrite(Lin, sprintf('im2_lin%d.png',j*10))
%     pause(0.1)
%     saveas(gcf,['im2_lin' num2str(j) '.png']);
%     Linh = imread(sprintf('im2_lin%d.png',j*10));
%     Linh = rgb2gray(Linh);
%     figure()
%     imhist(Linh)    
    pause(0.1)
    title( ['Linear * ' num2str(j) ]);
%     imwrite(Linhh, sprintf('im2_li_h%d.png',j*10))
%     saveas(gcf,['im2_li_h' num2str(j) '.png']);


end