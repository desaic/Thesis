function [U, C] = homogenize(nelx, nely, xPhys)
%% MATERIAL PROPERTIES
E0 = 1;
nu = 0.45;
maxIter = 100;
dim = 3;
%% PREPARE FINITE ELEMENT ANALYSIS
nele = nelx*nely*nelz;
ndof = dim*(nelx+1)*(nely+1)*(nelz+1);
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
change = 1;
loop = 0;

%% FE-ANALYSIS
sK = reshape(KE(:) * xPhys(:)', 64*nelx*nely,1);
K = sparse(iK,jK,sK); K = (K+K')/2;
max(diag(K))

Kr = [K(d2,d2), K(d2,d3)+K(d2,d4); K(d3,d2)+K(d4,d2), K(d3,d3)+K(d4,d3)+K(d3,d4)+K(d4,d4)];
U(d1,:) = ufixed;
U([d2,d3],:) = Kr\(-[K(d2,d1); K(d3,d1)+K(d4,d1)]*ufixed-[K(d2,d4); K(d3,d4)+K(d4,d4)]*wfixed);
U(d4,:) = U(d3,:)+wfixed;
%% OBJECTIVE FUNCTION AND SENSITIVITY ANALYSIS
for i = 1:3
for j = 1:3
  U1 = U(:,i); U2 = U(:,j);
  qe{i,j} = reshape(sum((U1(edofMat)*KE).*U2(edofMat),2),nely,nelx)/(nelx*nely);
  Q(i,j) = sum(sum((xPhys).*qe{i,j}));
end
end
C=Q;
nu21 = Q(1,2)/Q(1,1)
nu12 = Q(2,1)/Q(2,2)
E1 = Q(1,1) * (1-nu12*nu21)
E2 = Q(2,2) * (1-nu12*nu21)
end