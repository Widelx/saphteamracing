function rangeArray = getRange(array)
  rangeArray = 0;
  %rangeArray = cast(array(2:end,2), 'double');
  rangeArray = array(2:end, 2);
  return
endfunction