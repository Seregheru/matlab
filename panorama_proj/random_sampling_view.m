function [ res, resgeo ] = random_sampling_view( I, labels, normals, display )

% Sample a perspective view from Google Street View equirectangular pano image I and normal map (labels,normals)
% INPUT
% I 832x1664x3 matrix - the Google Street View equirectangular pano color image
% labels 256x512 matrix - the Google Street View equirectangular pano plane segmentation
% normals 3xnb_planes matrix - the associated normal for each plane (index of labels)
% OUTPUT
% res 480x640x3 matrix - the sampled perspective view
% resgeo 480x640x3 matrix - the associated normal map of the view res

f = 320;
phi0 = randn(1)*20*pi/180;
lambda0 = rand(1)*360*pi/180;
rotl = [cos(lambda0),-sin(lambda0),0;sin(lambda0),cos(lambda0),0;0,0,1];
rotp = [cos(phi0),0,-sin(phi0);0,1,0;sin(phi0),0,cos(phi0)];
rot = rotp*rotl;
m_normals = rot*normals;
nb_planes = size(normals,2);
mask = double(imresize(labels,[size(I,1),size(I,2)],'nearest'));

[res,resmask] = proj(double(rot),double(permute(I,[3,1,2])),int32(size(I,1)),int32(size(I,2)),double(f),mask);
res = uint8(permute(res,[2,3,1]));

Igeo = zeros(480,640,3);
for j=2:nb_planes
    mask = resmask==(j-1);
    n = m_normals(:,j);
    Igeo(:,:,1) = Igeo(:,:,1) + n(1)*mask; %R = X
    Igeo(:,:,2) = Igeo(:,:,2) + n(2)*mask; %G = Z
    Igeo(:,:,3) = Igeo(:,:,3) + n(3)*mask; %B = Y
end

resgeo = Igeo./repmat(sqrt(Igeo(:,:,1).^2+Igeo(:,:,2).^2+Igeo(:,:,3).^2),1,1,3);
resgeo(isnan(resgeo)) = 0;

if((nargin>1) && (display==1))
  Igeo = (Igeo*0.5+0.5).*double(repmat(resmask>0,1,1,3));
  figure()
  imshow(uint8(Igeo));
end

end
