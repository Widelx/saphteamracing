#include "mbed.h"
#include <cstdio>
#include <string>

#include "Car.h"
#include "Lidar.h"

int main() {
  float data[181];
  initLidar();
  initBLE();
  while (1) {
    getLidarData(data, BLE_ENABLE);
  }
}

