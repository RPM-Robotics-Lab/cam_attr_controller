clear all

data = csvread('d1_Ours.csv');
data = data(:,2)
redata = reshape(data, 13,39)
% for i = 1:13: length(data(:,1))-12
%     for j = 1: 39
% %     mndata = zeros(13,39);
%     redata(:,j) = data(i:i+12, 2);
%     plot(data(:,1), data(:,2))
%     end
%    
% end

csvwrite('r43.csv',redata)