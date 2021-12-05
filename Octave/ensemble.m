function ensemble(a, d, e, c)
  subplot(2,2,2);
  for i=1:size(d,1)
    if d(i,1)>250
      d(i,1) = -1;
    endif
  endfor
  for i=1:size(a,1)
    if a(i,1)>90
      a(i,1) = a(i,1) - 360;
    endif
  endfor
  scatter(a, d,13, c(e+1, :), 20,"filled", 'o');
  subplot(2,2,2)
  title('Ensembles','fontsize',16)
  xlabel('Angle','fontsize',16)
  ylabel('Distance','fontsize',16)
  axis ([-90, 90, -1, 250], "square")
  grid('on')
endfunction