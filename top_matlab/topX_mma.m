%% PERIODIC MATERIAL MICROSTRUCTURE DESIGN
function xPhys=topX_mma(nelx,nely,volfrac,penal,rmin,ft)
%% MATERIAL PROPERTIES
E0 = 1;
Emin = 1e-9;
nu = 0.3;
maxIter = 100;
%% PREPARE FINITE ELEMENT ANALYSIS
nele = nelx*nely;
ndof = 2*(nelx+1)*(nely+1);
A11 = [12  3 -6 -3;  3 12  3  0; -6  3 12 -3; -3  0 -3 12];
A12 = [-6 -3  0  3; -3 -6 -3 -6;  0 -3 -6  3;  3 -6  3 -6];
B11 = [-4  3 -2  9;  3 -4 -9  4; -2 -9 -4 -3;  9  4 -3 -4];
B12 = [ 2 -3  4 -9; -3  2  9 -2;  4  9  2  3; -9 -2  3  2];
KE = 1/(1-nu^2)/24*([A11 A12;A12' A11]+nu*[B11 B12;B12' B11]);
nodenrs = reshape(1:(1+nelx)*(1+nely),1+nely,1+nelx);
edofVec = reshape(2*nodenrs(1:end-1,1:end-1)+1,nelx*nely,1);
edofMat = repmat(edofVec,1,8)+repmat([0 1 2*nely+[2 3 0 1] -2 -1],nelx*nely,1);
iK = reshape(kron(edofMat,ones(8,1))',64*nelx*nely,1);
jK = reshape(kron(edofMat,ones(1,8))',64*nelx*nely,1);
%% PREPARE FILTER
iH = ones(nelx*nely*(2*(ceil(rmin)-1)+1)^2,1);
jH = ones(size(iH));
sH = zeros(size(iH));
k = 0;
for i1 = 1:nelx
  for j1 = 1:nely
    e1 = (i1-1)*nely+j1;
    for i2 = max(i1-(ceil(rmin)-1),1):min(i1+(ceil(rmin)-1),nelx)
      for j2 = max(j1-(ceil(rmin)-1),1):min(j1+(ceil(rmin)-1),nely)
        e2 = (i2-1)*nely+j2;
        k = k+1;
        iH(k) = e1;
        jH(k) = e2;
        sH(k) = max(0,rmin-sqrt((i1-i2)^2+(j1-j2)^2));
      end
    end
  end
end
H = sparse(iH,jH,sH);
Hs = sum(H,2);
%% PERIODIC BOUNDARY CONDITIONS
e0 = eye(3);
ufixed = zeros(8,3);
U = zeros(2*(nely+1)*(nelx+1),3);
alldofs = (1:2*(nely+1)*(nelx+1));
n1 = [nodenrs(end,[1,end]),nodenrs(1,[end,1])];
d1 = reshape([(2*n1-1);2*n1],1,8);
n3 = [nodenrs(2:end-1,1)',nodenrs(end,2:end-1)];
d3 = reshape([(2*n3-1);2*n3],1,2*(nelx+nely-2));
n4 = [nodenrs(2:end-1,end)',nodenrs(1,2:end-1)];
d4 = reshape([(2*n4-1);2*n4],1,2*(nelx+nely-2));
d2 = setdiff(alldofs,[d1,d3,d4]);
for j = 1:3
  ufixed(3:4,j) =[e0(1,j),e0(3,j)/2;e0(3,j)/2,e0(2,j)]*[nelx;0];
  ufixed(7:8,j) = [e0(1,j),e0(3,j)/2;e0(3,j)/2,e0(2,j)]*[0;nely];
  ufixed(5:6,j) = ufixed(3:4,j)+ufixed(7:8,j);
end
wfixed = [repmat(ufixed(3:4,:),nely-1,1); repmat(ufixed(7:8,:),nelx-1,1)];
%% INITIALIZE ITERATION
qe = cell(3,3);
Q = zeros(3,3);
dQ = cell(3,3);
x = repmat(volfrac,nely,nelx);
for i = 1:nelx
  for j = 1:nely
    if sqrt((i-nelx/2-0.5)^2+(j-nely/2-0.5)^2) < min(nelx,nely)/6
      x(j,i) = volfrac/2;
    end
  end
end
xPhys = x;
change = 1;
loop = 0;
m     = 1;                % The number of general constraints.
n     = nele;             % The number of design variables x_j.
xmin  = zeros(n,1);       % Column vector with the lower bounds for the variables x_j.
xmax  = ones(n,1);        % Column vector with the upper bounds for the variables x_j.
xold1 = x(:);             % xval, one iteration ago (provided that iter>1).
xold2 = x(:);             % xval, two iterations ago (provided that iter>2).
low   = ones(n,1);        % Column vector with the lower asymptotes from the previous iteration (provided that iter>1).
upp   = ones(n,1);        % Column vector with the upper asymptotes from the previous iteration (provided that iter>1).
a0    = 1;                % The constants a_0 in the term a_0*z.
a     = zeros(m,1);       % Column vector with the constants a_i in the terms a_i*z.
c_MMA = 1000*ones(m,1);   % Column vector with the constants c_i in the terms c_i*y_i.
d     = zeros(m,1);       % Column vector with the constants d_i in the terms 0.5*d_i*(y_i)^2.
%% START ITERATION
while ( (change > 0.001) && (loop<maxIter))
  loop = loop+1;
  %% FE-ANALYSIS
  sK = reshape(KE(:)*(Emin+xPhys(:)'.^penal*(E0-Emin)),64*nelx*nely,1);
  K = sparse(iK,jK,sK); K = (K+K')/2;
  Kr = [K(d2,d2), K(d2,d3)+K(d2,d4); K(d3,d2)+K(d4,d2), K(d3,d3)+K(d4,d3)+K(d3,d4)+K(d4,d4)];
  U(d1,:) = ufixed;
  U([d2,d3],:) = Kr\(-[K(d2,d1); K(d3,d1)+K(d4,d1)]*ufixed-[K(d2,d4); K(d3,d4)+K(d4,d4)]*wfixed);
  U(d4,:) = U(d3,:)+wfixed;
  %% OBJECTIVE FUNCTION AND SENSITIVITY ANALYSIS
  for i = 1:3
    for j = 1:3
      U1 = U(:,i); U2 = U(:,j);
      qe{i,j} = reshape(sum((U1(edofMat)*KE).*U2(edofMat),2),nely,nelx)/(nelx*nely);
      Q(i,j) = sum(sum((Emin+xPhys.^penal*(E0-Emin)).*qe{i,j}));
      dQ{i,j} =  penal*(E0-Emin)*xPhys.^(penal-1).*qe{i,j};
    end
  end
  %c = -(Q(1,1)+Q(2,2)+Q(1,2)+Q(2,1));
  %dc = -(dQ{1,1}+dQ{2,2}+dQ{1,2}+dQ{2,1});
  c = Q(1,2);
  dc = dQ{1,2};

  dv = ones(nely,nelx);
  %% FILTERING/MODIFICATION OF SENSITIVITIES
  if ft == 1
    dc(:) = H*(x(:).*dc(:))./Hs./max(1e-3,x(:));
  elseif ft == 2
    dc(:) = H*(dc(:)./Hs);
    dv(:) = H*(dv(:)./Hs);
  end
  % METHOD OF MOVING ASYMPTOTES
  xval  = x(:);
  f0val = c;
  df0dx = dc(:);
  fval  = sum(xPhys(:))/(volfrac*nele) - 1;
  dfdx  = dv(:)';
  [xmma, ~, ~, ~, ~, ~, ~, ~, ~, low,upp] = ...
  mmasub(m, n, loop, xval, xmin, xmax, xold1, xold2, ...
  f0val,df0dx, 0*df0dx, fval, dfdx,0*dfdx, low,upp,a0,a,c_MMA,d);
  % Update MMA Variables
  xnew     = reshape(xmma,nely,nelx);
  xPhys(:) = (H*xnew(:))./Hs;
  xold2    = xold1(:);
  xold1    = x(:);
  change = max(abs(xnew(:)-x(:)));
  x = xnew;

  %% PRINT RESULTS
  fprintf(' It.:%5i Obj.:%11.4f Vol.:%7.3f ch.:%7.3f\n',loop,c, mean(xPhys(:)),change);
  %% PLOT DENSITIES
  colormap(gray); imagesc(1-xPhys); caxis([0 1]); axis equal; axis off; drawnow;
end