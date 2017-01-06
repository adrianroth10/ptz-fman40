close all
nbrnoise = 300;
nbrline = 120;
noise1 = rand(2, nbrnoise) * 10;
noise2 = rand(1, nbrline);
x = linspace(0, 10, nbrline);
y = x + noise2(1, :);
plot(x, y, 'b*');
hold on;
plot(noise1(1, :), noise1(2, :), 'b*');
ylim([0, 10]);

print('../results/ransac.png', '-dpng');
close all
