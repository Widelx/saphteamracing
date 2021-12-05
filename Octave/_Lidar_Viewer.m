pkg load instrument-control

portCom = "COM10";

if (exist("serialport") != 3)
    disp("No Serial Support");
endif

s1 = serialport(portCom, 115200);  
flush(s1);
shouldRead = 1;
dataArray = [double(0), double(0)];
%dataArray = [uint16(0), uint16(0), uint16(0)];
while 1
  while shouldRead
    hexReaded = read(s1, 3);
    shouldRead = enableRead(hexReaded);
    if shouldRead
      dataArray = getSerialData(hexReaded, dataArray);
    endif
 endwhile
 %Aset = getSet(dataArray);
 Angle = getAngle(dataArray);
 Range = getRange(dataArray);
 cartesienne(Angle, Range);
 pause(0.001);
 shouldRead = 1; 
 clear Angle;
 clear Range;
 clear dataArray;
 dataArray = [double(0), double(0)];
 %dataArray = [0, 0, 0];
endwhile

%clearS1

#polaire(Angle, Range, Aset, colorMap);
#ensemble(Angle, Range, Aset, colorMap);