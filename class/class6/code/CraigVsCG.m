% Set up least-norm problem
m = 100;
n = 300;
A = rand(m,n);
A = A*A'*A*A'*A;
b = rand(m,1);

% Compute 'exact' solution
[q,r] = qr(A',0);
xs = q*(r'\b);

maxit = 1500;
tol = 1e-18;

% Use CRAIG to solve problem
for i = 1:maxit
    [xcr,~,istop,itn,rnorm,Anorm,Acond,xnorm] = craigSOL(m,n,A,b,tol,tol,1/tol,i,false);
    err_cr(i) = norm(xs - xcr);
end

% Use CG to solve problem
AA = @(x) A*(A'*x);
for i = 1:maxit
    [xcg, flag, relres, iter, resvec] = pcg(AA, b, tol, i);
    err_cg(i) = norm(xs - A'*xcg);
end

% Plot error curves
figure;
semilogy(1:length(err_cr), err_cr, '-');
hold all;
semilogy(1:length(err_cg), err_cg, '-');