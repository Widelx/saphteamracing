##function willRead = enableRead(serialData)
##  char1 = serialData(1,1);
##  char2 = serialData(1,2);
##  char3 = serialData(1,3) ; 
##  char4 = serialData(1,4) ;
##  if ((char1 == 0xA5) && (char2 == 0xA5) && (char3 == 0xA5) &&(char4 == 0xA5));
##    willRead = 0;
##    return
##  else
##    willRead = 1;
##    return
##  endif
##  return
##endfunction

function willRead = enableRead(serialData)
  char1 = serialData(1,1);
  char2 = serialData(1,2);
  char3 = serialData(1,3);
  if ((char1 == 0XA5) &&  (char2 == 0xA5) && (char3 = 0xA5));
    willRead = 0;
    return
  else
    willRead = 1;
    return
  endif
endfunction