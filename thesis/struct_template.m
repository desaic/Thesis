%\param rparam row vector of 2 reduced parameters in the range [-1,1].
%\param family integer family index
%\param res integer of 3D grid resolution
%e.g. st = struct_template([0.1 0.2], 1, 64);
function st = struct_template(rparam, family, res)
  st = zeros(res, res, res);
  %full parameter
  %assume a param.txt included in the same directory.
  [params, withCap]= loadParams('param.txt');
  startIdx = (family-1)*5;
  param = params{startIdx + 1};
  for j = 1:size(rparam,2)
    r = rparam(j);
    if(r>0)
        param = (1-r)*param + r*params{startIdx+2*j};
    else
        param = (1+r)*param - r*params{startIdx+2*j+1};
    end
  end
  nParam = size(param,2);
  paramPerBeam=9;
  nBeam = nParam/paramPerBeam;
  for i = 1:nBeam
      beamParam = [param(1+6*(i-1) : 6*i) param(nBeam*6+1+(i-1)*3 : nBeam*6+i*3)];
      st = drawCuboid(st, beamParam, withCap(family));
  end
  %enforce cubic symmetry
  %can uncomment to preview a mirrored structure.
  %st = mirrorCubicStructure(st);
end

%\param param parameters for a single cuboid. 6 coordinates followed
%cross-section size and orientation.
function o = drawCuboid(arr, param, withCap)
  v0 = param(1:3)';
  v1 = param(4:6)';
  r0= param(7);
  r1 = param(8);
  theta = param(9);
  x = v1-v0;
  len = norm(x);
  o = arr;
  if (len < 1e-10)
    return;
  end
  gridSize = size(arr);
  x = (1.0 / len) * x;
  %default y axis.
  y=[0,1,0]';
  if ( abs(x(2)) > 0.9 )
    y = [1 0 0]';
  end
  z = cross(x, y);
  z=(1/norm(z))*z;
  y = cross(z,x);
  R = vrrotvec2mat([x;theta]);
  y = R*y;
  z = R*z;

  r = sqrt(r0^2 + r1^2);
  r_avg = 0.5 * (r0 + r1);
  r_min = min( r0+r1 * 0.05, r0 * 0.05 + r1);
  
  %bounding box of the cuboid.
  fl=zeros(3,1);
  fu=zeros(3,1);

  il=zeros(3,1);
  iu=zeros(3,1);
  for i = 1:3
    fl(i) = min(v0(i), v1(i)) - r;
    fl(i) = max(0.0, fl(i));
    il(i) = floor(fl(i) * gridSize(i) + 0.5);
    fu(i) = max(v0(i), v1(i)) + r;
    fu(i) = min(1.0, fu(i));
    iu(i) = floor(fu(i) * gridSize(i) + 0.5);
  end

  for i = il(1): iu(1)+1
    for j = il(2): iu(2)+1
        for k = il(3):iu(3)+1
        if ~inbound(i, j, k, gridSize)
          continue;
        end
        coord=[(i+0.5)/gridSize(1); (j+0.5)/gridSize(2); (k+0.5)/gridSize(3)];
        t = 0;
        [dist, t]= ptLineDist(coord, v0, v1);
        disp = coord - v0;
        ycoord = abs(dot(disp,y));
        zcoord = abs(dot(disp,z));        
        if ( ((t >= 0 && t <= 1) || (dist <= r_min && withCap) )&& ycoord <= r0 && zcoord <= r1)
          %1-based index for matlab
          arr(i+1,j+1,k+1) = 1;
        end
        end
    end
  end
  o = arr;
end

%check if zero-based index within gridSize
function ret = inbound(i,j,k,s)
 ret = (i>=0 && i<s(1) && j>=0 && j<s(2) && k>=0 && k<s(3));
end

function [dist,t] = ptLineDist(pt, x0, x1)
  v0 = pt - x0;
  dir = (x1 - x0);
  len = norm(dir);
  dir = (1.0 / len)*dir;
  %component of v0 parallel to the line segment.
  t = v0'*dir;
  a = t * dir;
  t = t / len;
  b = v0 - a;
  if (t < 0)
    dist = norm(v0);
  elseif (t > 1)
    dist = norm(pt - x1);
  else
    dist = norm(b);
  end
end

%\return param cell array of row vectors of parameters.
function [param, has_cap] = loadParams(filename)
  IN = fopen(filename);
  nRows = fscanf(IN,'%d',1);
  param = cell(nRows,1);
  nTemplate = nRows/5;
  has_cap=fscanf(IN,'%d',nTemplate);
  for i = 1:nRows
      nCol = fscanf(IN,'%d',1);
      arr = fscanf(IN,'%f',[1 nCol]);
      param{i} = arr;
  end
  fclose(IN);
end
