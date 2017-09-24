function saveArr4d(A, filename)
s = size(A);
OUT = fopen(filename,'w');
fprintf(OUT, '%d %d %d %d\n', s(1,1),s(1,2),s(1,3), s(1,4));
for ii = 1:s(1,1)
   for jj = 1:s(1,2)
      for kk = 1:s(1,3)
          for ll = 1:s(1,4)
            fprintf(OUT, '%f ', A(ii,jj,kk,ll));
          end
          fprintf(OUT, '\n');
      end
      fprintf(OUT, '\n');
   end
   fprintf(OUT, '\n');
end
fclose(OUT);
end