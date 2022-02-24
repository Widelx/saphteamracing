#include "bluetooth.h"

RawSerial BLE(TXBle, RXBle, 115200);

CircularBuffer<char, BLE_BUFFER_SIZE> BLEBuffer;

/******************Init*********************/
void initBLE(void) {
  // Défini le formats de la liaison série
  BLE.format(8, SerialBase::None, 1);
  // Défini l'interruption sur reception de caractères
  BLE.attach(&putData, Serial::TxIrq);
}
/******************Init*********************/

/*****************Interrupts****************/
void putData(void) {
  char ConsumeChar;
  while (BLE.writeable() && (!(BLEBuffer.empty()))) {
    // envoie un caractère si c'est possible et que le buffer n'est pas vide
    BLEBuffer.pop(ConsumeChar);
    BLE.putc(ConsumeChar);
  }
  if (BLEBuffer.empty()) {
    // Si le buffer est vide (plus de données) j'arrête les interruptions sur TX
    BLE.enable_output(false);
  }
}
/*****************Interrupts****************/

/*****************SendData******************/
void sendOverBluetooth(float tab_data[]) {
  unsigned char range1, range2;
  unsigned short range;
  // Je converti les données de distance sur un short pour ne pas trop perdre en
  // précision
  for (int i = 0; i <= PRECISION; i++) {
    range = (unsigned short)(tab_data[i] * 10);
    range1 = (unsigned char)range & 0xFF;
    range2 = (unsigned char)(range >> 8) & 0xFF;
    // J'ajoute les valeurs au buffer
    BLEBuffer.push(range1);
    BLEBuffer.push(range2);
    BLEBuffer.push((unsigned char)i);
  }
  // Fin de trame
  BLEBuffer.push(0xA5);
  BLEBuffer.push(0xA5);
  BLEBuffer.push(0xA5);
  // Autorise l'interruption sur TX
  BLE.enable_output(true);
}
/*****************SendData******************/