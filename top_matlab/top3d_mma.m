% AN 169 LINE 3D TOPOLOGY OPITMIZATION CODE BY LIU AND TOVAR (JUL 2013)
function xPhys=top3d_mma(nelx,nely,nelz,volfrac,penal,rmin)
% USER-DEFINED LOOP PARAMETERS
maxloop = 100;    % Maximum number of iterations
tolx = 0.01;      % Terminarion criterion
displayflag = 1;  % Display structure flag
% USER-DEFINED MATERIAL PROPERTIES
E0 = 1;           % Young's modulus of solid material
Emin = 1e-9;      % Young's modulus of void-like material
nu = 0.4;         % Poisson's ratio
% USER-DEFINED LOAD DOFs
%[il,jl,kl] = meshgrid(nelx, 0, 0:nelz);                 % Coordinates
%loadnid = kl*(nelx+1)*(nely+1)+il*(nely+1)+(nely+1-jl); % Node IDs
%loaddof = 3*loadnid(:) - 1;                             % DOFs
il = 1;
jl = 1;
%kl = nelz/2;
xwidth = floor(0.1*nelx);
x1 = min(nelx, il+xwidth);
loadnid = zeros( x1 * (nelz+1),1);
cnt = 1;
for i = 1:x1
for k = 1: (nelz+1)
  loadnid(cnt,1) = (k-1)*(nelx+1)*(nely+1) + (i-1)*(nely+1)+(jl-1) + 1;
  cnt = cnt + 1;
end
end
%loadnid = kl*(nelx+1)*(nely+1)+il*(nely+1)+(nely+1-jl);
loaddof = 3*loadnid(:) - 1;
% USER-DEFINED SUPPORT FIXED DOFs
%[iif,jf,kf] = meshgrid(0,0:nely,0:nelz);                  % Coordinates
%fixednid = kf*(nelx+1)*(nely+1)+iif*(nely+1)+(nely+1-jf); % Node IDs
%fixeddof = [3*fixednid(:); 3*fixednid(:)-1; 3*fixednid(:)-2]; % DOFs
%iif = [0 0 nelx nelx]; jf = [0 0 0 0]; kf = [0 nelz 0 nelz];
fixWidth = max(2,1+xwidth);
fixednid = zeros(1, fixWidth*(1+nelz));
cnt = 1;
for i = 1:fixWidth
    for k = 1: (nelz+1)
        iif = i-1;
        kf = k-1;
        jf = nely+1;
        fixednid(cnt) = kf*(nelx+1)*(nely+1)+iif*(nely+1)+jf;
        cnt = cnt + 1;
    end
end

fixeddof = [3*fixednid(:); 3*fixednid(:)-1; 3*fixednid(:)-2];
% PREPARE FINITE ELEMENT ANALYSIS
nele = nelx*nely*nelz;
ndof = 3*(nelx+1)*(nely+1)*(nelz+1);
F = sparse(loaddof,1,-0.0001,ndof,1);
U = zeros(ndof,1);

Uhat = U;
D = U;
KinvU = U;
xwidth = max(1,floor(0.2 * nelx));
ydist = 1 + floor(0.1*nely);
for i = (nelx-xwidth+1) : nelx+1
    for k = 0:nelz
      y = min(nely, floor(nely/2) + ydist);
      nid = k*(nelx+1)*(nely+1)+ (i-1)*(nely+1)+y+1;
      dof = 3*nid-1;
      Uhat(dof,1) = 0.5;
      D(dof, 1) = 1;
      
      y = max(1, floor(nely/2) - ydist);
      nid = k*(nelx+1)*(nely+1)+ (i-1)*(nely+1)+y+1;
      dof = 3*nid-1;
      Uhat(dof,1) = -0.5;
      D(dof, 1) = 1;
    end
end

% U(loaddof)=0.5;
% uy = U(2:3:length(U));
% uy = reshape(uy, [nelx+1 nely+1 nelz+1]);

freedofs = setdiff(1:ndof,fixeddof);
KE = lk_H8(nu);
nodegrd = reshape(1:(nely+1)*(nelx+1),nely+1,nelx+1);
nodeids = reshape(nodegrd(1:end-1,1:end-1),nely*nelx,1);
nodeidz = 0:(nely+1)*(nelx+1):(nelz-1)*(nely+1)*(nelx+1);
nodeids = repmat(nodeids,size(nodeidz))+repmat(nodeidz,size(nodeids));
edofVec = 3*nodeids(:)+1;
edofMat = repmat(edofVec,1,24)+ ...
    repmat([0 1 2 3*nely + [3 4 5 0 1 2] -3 -2 -1 ...
    3*(nely+1)*(nelx+1)+[0 1 2 3*nely + [3 4 5 0 1 2] -3 -2 -1]],nele,1);
iK = reshape(kron(edofMat,ones(24,1))',24*24*nele,1);
jK = reshape(kron(edofMat,ones(1,24))',24*24*nele,1);
% PREPARE FILTER
iH = ones(nele*(2*(ceil(rmin)-1)+1)^2,1);
jH = ones(size(iH));
sH = zeros(size(iH));
k = 0;
for k1 = 1:nelz
    for i1 = 1:nelx
        for j1 = 1:nely
            e1 = (k1-1)*nelx*nely + (i1-1)*nely+j1;
            for k2 = max(k1-(ceil(rmin)-1),1):min(k1+(ceil(rmin)-1),nelz)
                for i2 = max(i1-(ceil(rmin)-1),1):min(i1+(ceil(rmin)-1),nelx)
                    for j2 = max(j1-(ceil(rmin)-1),1):min(j1+(ceil(rmin)-1),nely)
                        e2 = (k2-1)*nelx*nely + (i2-1)*nely+j2;
                        k = k+1;
                        iH(k) = e1;
                        jH(k) = e2;
                        sH(k) = max(0,rmin-sqrt((i1-i2)^2+(j1-j2)^2+(k1-k2)^2));
                    end
                end
            end
        end
    end
end
H = sparse(iH,jH,sH);
Hs = sum(H,2);
% INITIALIZE ITERATION
x = repmat(volfrac,[nely,nelx,nelz]);
xPhys = x; 
loop = 0; 
change = 1;
% INITIALIZE MMA OPTIMIZER
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
% START ITERATION
w_rho = 0.00001;
while change > tolx && loop < maxloop
    loop = loop+1;
    % FE-ANALYSIS
    sK = reshape(KE(:)*(Emin+xPhys(:)'.^penal*(E0-Emin)),24*24*nele,1);
    K = sparse(iK,jK,sK); K = (K+K')/2;
    U(freedofs,:) = K(freedofs,freedofs)\F(freedofs,:);
    %(u-hat)*D
    uuD = (U-Uhat).*D;
    KinvU(freedofs, :) = K(freedofs,freedofs)\uuD(freedofs,:);
    
    %numerical differencing debug
%     dx = 0.01;
%     dc_num=zeros(nelx,nely,nelz);
%     for i = 1:nelx
%         for j = 1:nely
%             for k = 1:nelz
%                 xPhys(i,j,k) = xPhys(i,j,k) + dx;
%                 sK = reshape(KE(:)*(Emin+xPhys(:)'.^penal*(E0-Emin)),24*24*nele,1);
%                 K = sparse(iK,jK,sK); K = (K+K')/2;
%                 U(freedofs,:) = K(freedofs,freedofs)\F(freedofs,:);
%                 cp = 0.5*((U-Uhat).*D)'*(U-Uhat);
%                 
%                 xPhys(i,j,k) = xPhys(i,j,k) - 2*dx;
%                 sK = reshape(KE(:)*(Emin+xPhys(:)'.^penal*(E0-Emin)),24*24*nele,1);
%                 K = sparse(iK,jK,sK); K = (K+K')/2;
%                 U(freedofs,:) = K(freedofs,freedofs)\F(freedofs,:);
%                 cm = 0.5*((U-Uhat).*D)'*(U-Uhat);
%                 
%                 dc_num(i,j,k) = (cp-cm)/(2*dx);
%                 
%                 xPhys(i,j,k) = xPhys(i,j,k) + dx;
%             end
%         end
%     end
    
    % OBJECTIVE FUNCTION AND SENSITIVITY ANALYSIS
 %   ce = reshape(sum((U(edofMat)*KE).*U(edofMat),2),[nely,nelx,nelz]);
 %   c = sum(sum(sum((Emin+xPhys.^penal*(E0-Emin)).*ce)));
 %   dc = -penal*(E0-Emin)*xPhys.^(penal-1).*ce;
    ce = reshape(sum((KinvU(edofMat)*KE).*U(edofMat),2),[nely,nelx,nelz]);
    totalMass = sum(sum(sum(xPhys)))/(nelx*nely*nelz);
    c = 0.5*uuD'*(U-Uhat) + w_rho * 0.5 * totalMass * totalMass;
    dc = -penal*(E0-Emin)*xPhys.^(penal-1).*ce + w_rho * totalMass * ones(nely,nelx,nelz);
    dv = 0*ones(nely,nelx,nelz);
    % FILTERING AND MODIFICATION OF SENSITIVITIES
    dc(:) = H*(dc(:)./Hs);  
    %dv(:) = H*(dv(:)./Hs);
    % METHOD OF MOVING ASYMPTOTES
    xval  = x(:);
    f0val = c;
    df0dx = dc(:);
    fval  = 0*sum(xPhys(:))/(volfrac*nele) - 1;
    dfdx  = dv(:)';
    [xmma, ~, ~, ~, ~, ~, ~, ~, ~, low,upp] = ...
    mmasub(m, n, loop, xval, xmin, xmax, xold1, xold2, ...
    f0val,df0dx, 0*df0dx, fval, dfdx,0*dfdx, low,upp,a0,a,c_MMA,d);
    % Update MMA Variables
    xnew     = reshape(xmma,nely,nelx,nelz);
    xPhys(:) = (H*xnew(:))./Hs;
    xold2    = xold1(:);
    xold1    = x(:);
    change = max(abs(xnew(:)-x(:)));
    x = xnew;
    % PRINT RESULTS
    fprintf(' It.:%5i Obj.:%11.4f Vol.:%7.3f ch.:%7.3f\n',loop,c,mean(xPhys(:)),change);
    % PLOT DENSITIES
    if displayflag, clf; display_3D(xPhys); end 
end
clf; display_3D(xPhys);
end


% === GENERATE ELEMENT STIFFNESS MATRIX ===
function [KE] = lk_H8(nu)
A = [32 6 -8 6 -6 4 3 -6 -10 3 -3 -3 -4 -8;
    -48 0 0 -24 24 0 0 0 12 -12 0 12 12 12];
k = 1/144*A'*[1; nu];

K1 = [k(1) k(2) k(2) k(3) k(5) k(5);
    k(2) k(1) k(2) k(4) k(6) k(7);
    k(2) k(2) k(1) k(4) k(7) k(6);
    k(3) k(4) k(4) k(1) k(8) k(8);
    k(5) k(6) k(7) k(8) k(1) k(2);
    k(5) k(7) k(6) k(8) k(2) k(1)];
K2 = [k(9)  k(8)  k(12) k(6)  k(4)  k(7);
    k(8)  k(9)  k(12) k(5)  k(3)  k(5);
    k(10) k(10) k(13) k(7)  k(4)  k(6);
    k(6)  k(5)  k(11) k(9)  k(2)  k(10);
    k(4)  k(3)  k(5)  k(2)  k(9)  k(12)
    k(11) k(4)  k(6)  k(12) k(10) k(13)];
K3 = [k(6)  k(7)  k(4)  k(9)  k(12) k(8);
    k(7)  k(6)  k(4)  k(10) k(13) k(10);
    k(5)  k(5)  k(3)  k(8)  k(12) k(9);
    k(9)  k(10) k(2)  k(6)  k(11) k(5);
    k(12) k(13) k(10) k(11) k(6)  k(4);
    k(2)  k(12) k(9)  k(4)  k(5)  k(3)];
K4 = [k(14) k(11) k(11) k(13) k(10) k(10);
    k(11) k(14) k(11) k(12) k(9)  k(8);
    k(11) k(11) k(14) k(12) k(8)  k(9);
    k(13) k(12) k(12) k(14) k(7)  k(7);
    k(10) k(9)  k(8)  k(7)  k(14) k(11);
    k(10) k(8)  k(9)  k(7)  k(11) k(14)];
K5 = [k(1) k(2)  k(8)  k(3) k(5)  k(4);
    k(2) k(1)  k(8)  k(4) k(6)  k(11);
    k(8) k(8)  k(1)  k(5) k(11) k(6);
    k(3) k(4)  k(5)  k(1) k(8)  k(2);
    k(5) k(6)  k(11) k(8) k(1)  k(8);
    k(4) k(11) k(6)  k(2) k(8)  k(1)];
K6 = [k(14) k(11) k(7)  k(13) k(10) k(12);
    k(11) k(14) k(7)  k(12) k(9)  k(2);
    k(7)  k(7)  k(14) k(10) k(2)  k(9);
    k(13) k(12) k(10) k(14) k(7)  k(11);
    k(10) k(9)  k(2)  k(7)  k(14) k(7);
    k(12) k(2)  k(9)  k(11) k(7)  k(14)];
KE = 1/((nu+1)*(1-2*nu))*...
    [ K1  K2  K3  K4;
    K2'  K5  K6  K3';
    K3' K6  K5' K2';
    K4  K3  K2  K1'];
end

% =========================================================================
% === This code was written by K Liu and A Tovar, Dept. of Mechanical   ===
% === Engineering, Indiana University-Purdue University Indianapolis,   ===
% === Indiana, United States of America                                 ===
% === ----------------------------------------------------------------- ===
% === Please send your suggestions and comments to: kailiu@iupui.edu    ===
% === ----------------------------------------------------------------- ===
% === The code is intended for educational purposes, and the details    ===
% === and extensions can be found in the paper:                         ===
% === K. Liu and A. Tovar, "An efficient 3D topology optimization code  ===
% === written in Matlab", Struct Multidisc Optim, 50(6): 1175-1196, 2014, =
% === doi:10.1007/s00158-014-1107-x                                     ===
% === ----------------------------------------------------------------- ===
% === The code as well as an uncorrected version of the paper can be    ===
% === downloaded from the website: http://www.top3dapp.com/             ===
% === ----------------------------------------------------------------- ===
% === Disclaimer:                                                       ===
% === The authors reserves all rights for the program.                  ===
% === The code may be distributed and used for educational purposes.    ===
% === The authors do not guarantee that the code is free from errors, a