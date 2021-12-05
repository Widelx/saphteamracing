#include "Lidar.h"
#include "mbed.h"
#include <cstdio>
#include <string>

int main() {
  float data[181];
  initLidar();
  initBLE();
  while (1) {
    getLidarData(data, BLE_ENABLE);
  }
}
