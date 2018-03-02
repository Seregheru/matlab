clear all;
close all;
clc;

% Learn the optimal octree that subdivides the color space so as to maximize the discrimination between the likelihoods of % the color contrast cue for facade and non-facade rectangles (this feature is defined by Bogdan Alexe, Thomas Deselaers
% and Vittoria Ferrari in groups.inf.ed.ac.uk/calvin/objectness/)

load('/home/fond/facadeness/learning_set.mat','filenames');

octree = '()()()()()()()()';
ldp = 112290;
ldn = 112290;
dp = zeros(ldp,1);
cp = zeros(ldn,1);
se = strel('diamond',16);

u = linspace(0.00001,0.000001,10);
T = repmat(u',[1,200])';
T = T(:);
res = cell(1000,2);
old_cost = 0.01;

for t=1:1000

    disp(t);
    octree_mutated = mutate(octree);
    disp(octree_mutated);

    p = 1; n = 1;
    for i=1:size(filenames,1)
        %disp(i);
        im = imread(filenames{i,1});
        img_size = [size(im,1),size(im,2)];
        load(filenames{i,2},'recpos','recneg');
        p = p + size(recpos,1);
        n = n + size(recneg,1);
        mask = double(imerode(~(im(:,:,1)==0 & im(:,:,2)==0 & im(:,:,3)),se));
        maskint = integralImage(double(mask));
        color = fast_color_octree_rgb( im,mask,octree_mutated );
        cp = color_ring_contrast_norm_octree(color, img_size, maskint, int32(size(recpos,1)), recpos(:,1),recpos(:,2),recpos(:,3),recpos(:,4));
        cn = color_ring_contrast_norm_octree(color, img_size, maskint, int32(size(recneg,1)), recneg(:,1),recneg(:,2),recneg(:,3),recneg(:,4));
        dp(p:(p+length(cp)-1)) = cp;
        dn(n:(n+length(cn)-1)) = cn;
        p = p + length(cp);
        n = n + length(cn);
    end

    x = linspace(0,1,200);
    hp = hist(dp,x); hp = hp/sum(hp);
    hn = hist(dn,x); hn = hn/sum(hn);

    cost = -sum((hp(2:end) - hn(2:end)).^2);
    res{t,1} = octree_mutated;
    res{t,2} = cost;
    if ((cost<old_cost) || (rand()<exp(-(cost-old_cost)/T(t))));
        octree = octree_mutated;
        old_cost = cost;
    end
    save('res_simu_new_rgb.mat','res');

end
