function display_2D(xPhys)
colormap(gray);
imagesc(1-xPhys);
caxis([0 1]);
axis equal;
axis off;
drawnow;
end