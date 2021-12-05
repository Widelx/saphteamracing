%function dataArray = getSerialData(serialData, dataArray)
%  distance = serialData(1,1); %R�cup�re la donn�e distance
%  ensemble = serialData(1,4); %R�cup�re la donn�e ensemble
%  angle1 = serialData(1,2);
%  angle2 = serialData(1,3)
%  angle = uint16(angle1) + uint16(angle2*256); %R�cup�re la donn�e angle
%  newDataLine = [uint16(distance), uint16(angle), uint16(ensemble)]; %G�n�re un tableau de 1x3 avec les donn�es trait�es
%  dataArray = cat(1, dataArray, newDataLine); %Ajoute une ligne avec les donn�es trait�es aux anciennes donn�es
%  return
%endfunction


function dataArray = getSerialData(serialData, dataArray)
  %R�cup�re les donn�es
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
  
  
  %Stocke les informations dans un tableau et ajoute � l'ancien tableau
  newDataLine = [double(angle), double(distance)];
  dataArray = cat(1, dataArray, newDataLine);
  return
endfunction