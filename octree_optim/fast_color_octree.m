function [ labhist ] = fast_color_octree( im, mask, octree )

lab = rgb2lab(im);
labp = permute(lab,[3,1,2]);
[imoctree,nbleaves] = evaluate(octree,int32(labp));

labhist = cell(nbleaves+1,1);
for k=1:(nbleaves+1)
    labhist{k} = integralImage(double(imoctree==(k-2)).*mask);
end

end

