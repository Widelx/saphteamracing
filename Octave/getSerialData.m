%function dataArray = getSerialData(serialData, dataArray)
%  distance = serialData(1,1); %Récupère la donnée distance
%  ensemble = serialData(1,4); %Récupère la donnée ensemble
%  angle1 = serialData(1,2);
%  angle2 = serialData(1,3)
%  angle = uint16(angle1) + uint16(angle2*256); %Récupère la donnée angle
%  newDataLine = [uint16(distance), uint16(angle), uint16(ensemble)]; %Génère un tableau de 1x3 avec les données traitées
%  dataArray = cat(1, dataArray, newDataLine); %Ajoute une ligne avec les données traitées aux anciennes données
%  return
%endfunction


function dataArray = getSerialData(serialData, dataArray)
  %Récupère les données
  distance1 = uint16(serialData(1,1));
  distance2 = uint16(serialData(1,2));
  angle = double(serialData(1,3));
  
  %Reconstruit la distance
  distance = uint16(distance1 + distance2*256);
  distance = double(distance)/10;
  %Adapte l'angle
  if angle < 90
    angle += 270;
  else 
    angle -= 90;
  endif
  
  
  %Stocke les informations dans un tableau et ajoute à l'ancien tableau
  newDataLine = [double(angle), double(distance)];
  dataArray = cat(1, dataArray, newDataLine);
  return
endfunction