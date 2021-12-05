function polaire(a, d)
  subplot(2,1,1);
  for i=1:size(a,1)
    if a(i,1)>90
      a(i,1) = a(i,1) - 360;
    endif
  endfor
  plot(a, d, 'or', 'markersize', 3, 'markerfacecolor', 'r');
  title('Coordonnées polaires','fontsize',16);
  xlabel('Angle','fontsize',16);
  ylabel('Distance','fontsize',16);
  grid('on');
  axis ([-90, 90, 0, 250], "square");
endfunction