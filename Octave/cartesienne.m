function cartesienne(a, d) 
  %subplot(2,1,2)
  [x1,y1] = pol2cart(deg2rad(a) , d);
  plot(y1, x1, 'or', 'markersize', 3, 'markerfacecolor', 'r');
  title('Coordonnées cartésiennes','fontsize',16);
  xlabel('Distance','fontsize',16);
  ylabel('Distance','fontsize',16);
  axis ([-2500, 2500, 0, 2500], "equal");
  grid('on');
endfunction