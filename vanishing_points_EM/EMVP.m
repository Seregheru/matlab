function [ R, likelihood, params ] = EMVP( R, data )

% Find the 3 Manhattan vanishing points by Expectation-Maximization from line intersections
% the bayesian model is a mixture of Von-Mises Fisher distribution and a uniform distribution for outliers
% INPUT
% R 3x3 rotation matrix - the initial Manhattan frame
% data Nx3 matrix - line intersections (normalized cross-product of the line representation on the gaussian sphere)
% OUTPUT
% R 3x3 rotation matrix - the final Manhattan frame
% likelihood - the value of the final likelihood
% params object - params.prior the final prior weights, params.kappa the final spreading parameters of the VM
% distributions, params.outliers the final outliers rate

Niter = 20; N = size(data,1);
prior_outliers = 0.35;
prior = (1-prior_outliers)*[0.33,0.33,0.33];
kappax = 100; kappay = 100; kappaz = 100;
maxkappa = 200;
X = [repmat([1,0,0],N,1);repmat([0,1,0],N,1);repmat([0,0,1],N,1)];
O = repmat(data',1,3);

for iter = 1:Niter

   % E step

   %D = diag([kappax,kappay,kappaz]);
   %gamma = exp(data*D*R)*diag(prior.*[kappax/(4*pi*sinh(kappax)),kappay/(4*pi*sinh(kappay)),kappaz/(4*pi*sinh(kappaz))]);
   %better suited expression for numerical evaluation
   gamma = exp((data*R-1).*[kappax*ones(N,1),kappay*ones(N,1),kappaz*ones(N,1)]);
   gamma = gamma.*[(prior(1)*kappax/(2*pi*(1-exp(-2*kappax))))*ones(N,1),(prior(2)*kappay/(2*pi*(1-exp(-2*kappay))))*ones(N,1),(prior(3)*kappaz/(2*pi*(1-exp(-2*kappaz))))*ones(N,1)];
   gamma_outliers = prior_outliers/(4*pi);
   denom = repmat(nansum(gamma,2)+gamma_outliers,1,3);
   gamma = gamma./denom;
   gamma(isnan(gamma)) = 0;
   gamma_outliers = repmat(gamma_outliers,N,1)./denom(:,1);
   gamma_outliers(isnan(gamma_outliers)) = 0;

   % M step

   %prior
   prior = sum(gamma,1)/N;
   prior_outliers = sum(gamma_outliers)/N;
   %rotation
   W = repmat(gamma(:)',3,1);
   M = (W.*O)*X;
   [u,~,v] = svd(M);
   R = u*v';
   if det(R)<0
      R = u*diag([1,1,-1])*v';
   end
   %kappa
   r1 = norm(sum(repmat(gamma(:,1),1,3).*data))/sum(gamma(:,1));
   r2 = norm(sum(repmat(gamma(:,2),1,3).*data))/sum(gamma(:,2));
   r3 = norm(sum(repmat(gamma(:,3),1,3).*data))/sum(gamma(:,3));
   kappax = min(r1*(3-r1^2)/(1-r1^2),maxkappa);
   kappay = min(r2*(3-r2^2)/(1-r2^2),maxkappa);
   kappaz = min(r3*(3-r3^2)/(1-r3^2),maxkappa);

end

params.prior = prior;
params.kappa = [kappax,kappay,kappaz];
params.outliers = prior_outliers;

a = log([(prior(1)*kappax/(2*pi*(1-exp(-2*kappax)))),(prior(2)*kappay/(2*pi*(1-exp(-2*kappay)))),(prior(3)*kappaz/(2*pi*(1-exp(-2*kappaz))))]);
a(isinf(a)) = 0;
a = repmat(a,N,1);
likelihood = sum(sum(gamma.*((data*R-1).*[kappax*ones(N,1),kappay*ones(N,1),kappaz*ones(N,1)]+a)));
likelihood = likelihood + sum(gamma_outliers*log(prior_outliers/(4*pi)));

end
