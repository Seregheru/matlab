function [ x ] = mwisr_approx( rec, nn, display, im )

% Find an approximate solution of the Maximum Weight Independent Set of Rectangles (MWISR) problem by linear programming
% Building the constrains matrix A is the bottleneck of the approach. This is done in C++ using a sparse matrix and a hash
% function to avoid duplicated line
% INPUT
% rec is a nx5 matrix. Each line is a rectangle (xmin,ymin,xmax,ymax,w) with its associated weight
% nn is the number of rectangles (it must be less than 128)
% display is a diplay flag
% im is the image where the rectangles are detected
% OUTPUT
% x is a value for each rectangle 0<x<1. Ideally the solution is the set of rectangles with x=1

  f = -double(rec(1:nn,5));
  A = mwisr_sparse_uniq_long(int32(rec(1:nn,1:4)));
  b = ones(size(A,1),1);
  lb = zeros(nn,1);
  ub = ones(nn,1);
  x = linprog(f,A,b,[],[],lb,ub);

  if((nargin>2) && (display==1))
    figure();
    imshow(im);
    hold on;
    recds = rec(1:nn,1:4);
    display_rec(recds(x>0.5,:),[1,0,0]);
  end

end
