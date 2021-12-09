#include "mbed.h"
#include <cstdio>
#include <string>

#include "bluetooth.h"
#include "car.h"
#include "lidar.h"

int main() {
  float data[181];
  initLidar();
  initBLE();
  while (1) {
    getLidarData(data);
    sendOverBluetooth(data);
  }
}
