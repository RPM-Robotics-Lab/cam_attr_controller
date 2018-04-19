close all;
clear all; 
    for k = 1+13*5: 13*6
        realimg= imread(sprintf('3_750/%d.png', k)); 
        subplot(1,2,1)
        imshow(realimg)
        subplot(1,2,2)
        imhist(realimg)
        pause(.1)
end

for i = 6: 6
    img = imread(sprintf('ss%d.png',i)); 

   
%         for ck = 1.5: .125 : 3
%             dbl = 5 * log10(ck) 
            for ck = 1:0.157:3
                dbl = ck;
            gsyn = img * dbl;
            figure(2)
            subplot(1,2,1)
            imshow(gsyn)
            subplot(1,2,2)
            imhist(gsyn)
            pause(.1)
        end
%     for jk = 1: 1 :234
%    
%         histi = imread(sprintf('3_750/%d.png', jk));
%         imhist(histi)
%     end    
    end


