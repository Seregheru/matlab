function [ histos,nnfaces ] = display_histogram( dir, display )

% Compute and display a spherical histogram of the normals distribution dir
% The computation is speed-up by a multi-resolution scheme. Indeed the sphere mesh is subdivided several times from an
% icosedreon
% INPUT
% dir is a nx3 matrix. Each line is a 3d normalized vector
% display is a diplay flag
% OUTPUT
% histos is nx1 matrix corresponding to the spherical histogram of the distribution dir
% nnfaces is cell array associating to each face its neighboors (useful for a interpolated view of the histogram)

load('semiregular_sphere.mat','vert','ind','ressize');
histos = intersection3Dmultires(dir',size(dir,1),4,vert,ind,int32(ressize));

if((nargin>1) && (display==1))

    load('semiregular_sphere.mat','vertices','faces');
    vert0 = vertices(faces(:,1),:); vert1 = vertices(faces(:,2),:); vert2 = vertices(faces(:,3),:);
    nnfaces = cell(size(faces,1),1);
    for f=1:size(faces,1)
       x = faces(f,1); y = faces(f,2); z = faces(f,3);
       [nnx,~] = ind2sub(size(faces),find(faces==x));
       [nny,~] = ind2sub(size(faces),find(faces==y));
       [nnz,~] = ind2sub(size(faces),find(faces==z));
       nnfaces{f} = unique([nnx;nny;nnz]);
       nnfaces{f}(nnfaces{f}==f) = [];
    end

    figure();
    trisurf(faces,vertices(:,1),vertices(:,2),vertices(:,3),histos);
    axis equal

end

end
