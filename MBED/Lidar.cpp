#include "Lidar.h"
#include <cstdio>
#include <string>

// Instances du programme
RawSerial SLidar(TXLIDAR, RXLIDAR, 115200);
RawSerial BLE(TXBLE, RXBLE, 115200);
PwmOut PWMLidar(PA_0);

// Variable globale
bool fullturn;

// Buffers globaux
CircularBuffer<dataPoints, POINT_BUFFER_SIZE> pointBuffer;
CircularBuffer<char, RAW_BUFFER_SIZE> rawBuffer;
CircularBuffer<char, BLE_BUFFER_SIZE> PcBuffer;

void getLidarData(float tab[], bool enableBluetooth) {
  int pointNumber = 0;
  // Etabli la connection
  relink();
  fullturn = false;
  // Je traite les points tant que je n'ai pas fait un tour en
  while (!(fullturn)) {
    // Récupère le nombre de points décodables
    pointNumber = (int)rawBuffer.size() / 5;
    // Décode les points disponibles
    if (pointNumber >= 1) {
      convertData(pointNumber);
    }
  }
  // Pré-process les données pour correspondre au format demandé
  sendToProcess(tab);

  // Envoie via la liaison bluetooth si activé
  if (enableBluetooth) {
    sendOverBluetooth(tab);
  }
  // Les données stockées dans le tableau donné en entrée sont analysables
}

/******************Init*********************/
void initLidar(void) {
  // Fait tourner le Lidar
  PWMLidar.period(4e-5);
  PWMLidar.write(PWMLIDAR);

  // Défini le formats de la liaison série
  SLidar.format(8, SerialBase::None, 1);
  // Défini l'interruption sur reception de caractères
  SLidar.attach(&getChar, Serial::RxIrq);
}

void initBLE(void) {
  // Défini le formats de la liaison série
  BLE.format(8, SerialBase::None, 1);
  // Défini l'interruption sur reception de caractères
  BLE.attach(&putData, Serial::TxIrq);
}
/******************Init*********************/

/*****************Interrupts****************/
void getChar(void) {
  // Récupère un caractère et le stock dans le buffer circulaire
  // prévu à cet effet
  while (SLidar.readable()) {
    rawBuffer.push((char)SLidar.getc());
  }
}

void putData(void) {
  char ConsumeChar;
  while (BLE.writeable() && (!(PcBuffer.empty()))) {
    // envoie un caractère si c'est possible et que le buffer n'est pas vide
    PcBuffer.pop(ConsumeChar);
    BLE.putc(ConsumeChar);
  }
  if (PcBuffer.empty()) {
    // Si le buffer est vide (plus de données) j'arrête les interruptions sur TX
    BLE.enable_output(false);
  }
}
/*****************Interrupts****************/

/*******************Relink******************/
void relink(void) {
  // Etabli une nouvelle connection
  // via les fonctions suivantes
  stopConnection();
  emptyBuffers();
  initConnection();
}

void emptyBuffers(void) {
  // Vide les buffers (head = tail => reset)
  rawBuffer.reset();
  pointBuffer.reset();
}

void initConnection(void) {
  char char1 = 0;
  char char2 = 0;
  char consumeChar;
  int i = 0;
  emptyBuffers();
  // Envoie la trame de start
  SLidar.putc(PRE_TRAME);
  SLidar.putc(TRAME_START);
  // Consomme les caractères envoyés par le LIDAR tant que le
  // Début de la trame n'est pas détecté (probablement des caractères
  // qui correspondent au modèle etc...)
  do {
    char1 = char2;
    rawBuffer.pop(char2);
  } while (!(char1 == TRAME_RECEP1 && char2 == TRAME_RECEP2));
  // Consomme les 5premiers caractères de la trame (le premier point)
  // Qui ne correspondent pas à une réelle mesure (poubelle)
  do {
    while (!(SLidar.readable()))
      ;
    rawBuffer.pop(consumeChar);
    i++;
  } while (i != TAILLE_TRAME);
}

void stopConnection(void) {
  // Envoie les caractères de fin de trame
  SLidar.putc(PRE_TRAME);
  SLidar.putc(TRAME_STOP);
  // Attend une milliseconde (cf datasheet)
  wait_us(999);
}
/*******************Relink******************/

/*****************TreatData*****************/
void convertData(int pointReceived) {
  // PC.printf("Valeur reçues = %d", pointReceived);
  dataPoints dataPoint;
  static bool second_quart;
  // char qualite;
  int i = 0, y = 0;
  char data[5] = {};
  if (pointReceived > 10) {
    pointReceived = 10;
  }
  for (y = 0; y < pointReceived; y++) {
    for (i = 0; i < TAILLE_TRAME; i++) {
      rawBuffer.pop(data[i]);
    }
    dataPoint.angle = arrayToAngle(data[1], data[2]);
    // Récupère l'angle
    if (dataPoint.angle > (ANGLE_AVANT_1 - 20) ||
        dataPoint.angle < (ANGLE_AVANT_2 + 20)) {
      // Il y a des sauts de valeurs, ainsi on empêche les sauts de nous géner
      // en mettant des extrêmes plus grand
      if (((dataPoint.angle >= ANGLE_AVANT_1 ||
            dataPoint.angle <= ANGLE_AVANT_2)) &&
          second_quart == 1) {
        // Je prend ce point et le décode si il concerne l'avant du lidar.
        if (isQualityEnough(data[0], POINT_QUALITY)) {
          // Si la qualité est suffisante
          // Je transforme l'angle pour qu"il soit compris entre 0 et 180 (a la
          // base entre 270 et 90)
          dataPoint.angle = convertAngle(dataPoint.angle);
          // Je décode la distance et j'ajoute au buffer
          dataPoint.range = arrayToRange(data[3], data[4], MAX_RANGE);
          pointBuffer.push(dataPoint);
        }
      }
    } else if (dataPoint.angle > 180) {
      second_quart = 1;
    } else {
      if (second_quart == 1 && pointBuffer.size() >= 10) {
        second_quart = 0;
        fullturn = true;
        stopConnection();
        rawBuffer.reset();
        break;
      }
    }
  }
}

float arrayToAngle(char char1, char char2) {
  // Récupère l'angle
  return (float)(char2 << 7 | char1 >> 1) / 64.0;
}

float arrayToRange(char char3, char char4, float max) {
  // Bride la distance
  char range;
  range = (float)(char4 << 8 | char3) / 4;
  if (range > max) {
    range = max;
  }
  return range;
}

bool isQualityEnough(char charQ, char qualite) {
  // Récupère la qualité
  return (((charQ >> 2) & 0x3F) >= qualite);
}

float convertAngle(float angle) {
  // Converti les angles pour passer de [270;90] à [0;180]
  if (angle <= 90.0) {
    angle += 90.0;
  } else {
    angle -= 270.0;
  }
  return angle;
}
/*****************TreatData*****************/

/*****************SendData******************/
void sendToProcess(float tab_data[]) {
  // Réinitialise les données du tableau
  for (int i = 0; i <= PRECISION; i++) {
    tab_data[i] = NULL_VALUE;
  }
  // Déclare les variables
  dataPoints dataPoint;
  pointBuffer.pop(dataPoint);
  while (!(pointBuffer.empty())) {
    if (tab_data[(unsigned int)floor(dataPoint.angle)] == NULL_VALUE) {
      // Aucun point stocké précédemment, je stock
      tab_data[(unsigned int)floor(dataPoint.angle)] = dataPoint.range;
    } else {
      // Un point existait déja pour cet angle, je fais la moyene des deux
      tab_data[(unsigned int)floor(dataPoint.angle)] =
          (tab_data[(unsigned int)floor(dataPoint.angle)] + dataPoint.range) /
          2;
    }
    pointBuffer.pop(dataPoint);
  }
  // Si il existe des valeurs non définies, je prends la valeur du point d'avant
  for (int i = 0; i <= PRECISION; i++) {
    if (tab_data[i] == NULL_VALUE) {
      tab_data[i] = tab_data[i - 1];
    }
  }
  // Reset du buffer point
  pointBuffer.reset();
}

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
    PcBuffer.push(range1);
    PcBuffer.push(range2);
    PcBuffer.push((unsigned char)i);
  }
  // Fin de trame
  PcBuffer.push(0xA5);
  PcBuffer.push(0xA5);
  PcBuffer.push(0xA5);
  // Autorise l'interruption sur TX
  BLE.enable_output(true);
}
/*****************SendData******************/