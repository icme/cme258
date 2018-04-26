%% Load image data

H = imread('jetplane_blur2.tif');
% H = imread('jetplane_blur.tif');
H = double(H);
figure;
imshow(H,[]);

[m,n] = size(H);

%% deblur using iterative method
tau = 1; % Blurring width (should be ~O(1 to 10))
lambda = 1e-6; % Regularization level (should be ~O(1e-2 to 1e-3))

% define linear operator
% Note that A = A', so computing products with the transpose is the same as
% just computing products.
% TODO: implement the linear operator using the mex interface
% A = @(X) X % Use this if method requires only products with A
A = @(X, t) X; % Use this if method requires transposes

% Run iterative method
tol = 1e-6;
conlim = 1e8;
maxit = 10; % This is okay for smaller picture, use 10 for larger one

% TODO:
% Choose appropriate iterative solver for regularized least-squares
R = H;

%% Show deblurred image
R = reshape(R, m, n);
figure;
imshow(R, []);
