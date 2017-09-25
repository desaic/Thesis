function y = mirrorSeqUD(x)
s = size(x);
y = zeros( 2*s(1),s(2),s(3),s(4));
for k = 1:s(3)
    for l = 1:s(4)
      y( 1:s(1), :, k, l) = x(:, :, k, l);
      y((s(1)+1):(2*s(1)),:,k,l) = flipud(x(:,:,k,l));
    end
end
end