function st = mirrorCubicStructure(st_in)
  st = st_in;
  gridSize = size(st_in);
  for i = 0:gridSize(1)-1
    for j = 0:gridSize(2)-1
      for k = 0:gridSize(3)-1
        si = i;
        sj = j;
        sk = k;
        tmp=0;
        if si >= floor(gridSize(1) / 2)
          si = gridSize(1) - i - 1;
        end
        if sj >= floor(gridSize(2)/2)
          sj = gridSize(2) - j - 1;
        end
        if (sk >= floor(gridSize(3)/2))
          sk = gridSize(3) - k - 1;
        end
        if (si < sj)
          tmp = si; 
          si = sj;
          sj = tmp;
        end
        if (si < sk)
          tmp = sk;
          sk = si;
          si = tmp;
        end
        if (sj < sk)
          tmp = sk;
          sk = sj;
          sj = tmp;
        end
        st(i+1,j+1,k+1) = st_in(si+1,sj+1,sk+1);
      end
    end
  end
end