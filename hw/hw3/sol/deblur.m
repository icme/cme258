%% Load image data

H = imread('jetplane_blur2.tif');
H = double(H);
figure;
imshow(H,[]);

[m,n] = size(H);

%% deblur using iterative method
tau = 1; % Blurring width
lambda = 1e-1; % Regularization level

% define linear operator
A = @(X, t) vec(blur(reshape(X,m,n), tau));

% Run iterative method
tol = 1e-6;
conlim = 1e8;
maxit = 100;

[R, istop, itn, normr, normAr] = lsmr(A, vec(H), lambda, tol, tol, conlim, maxit);

%% Show deblurred image
R = reshape(R, m, n);
figure;
imshow(R, []);
