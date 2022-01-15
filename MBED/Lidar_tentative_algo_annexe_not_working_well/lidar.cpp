#include "lidar.h"
#include <cstdio>

// Instances du programme
RawSerial SLidar(TXLIDAR, RXLIDAR, 115200);
PwmOut PWMLidar(PIN_PWM);

// Variable globale
bool fullturn;

// Buffers globaux
CircularBuffer<char, RAW_BUFFER_SIZE> rawBuffer;
// char RawBuffer[181];
float PointBuffer[181];
enum status {
  CONNECTED = 0,
  DISCONNECTED = 1,
};
status LidarState;
bool shouldRead;

bool getLidarData(float *tab) {
  resetBuffer();
  shouldRead = true;
  if (LidarState == DISCONNECTED) {
    relink();
  }
  int pointNumber = 0;
  //   PC.printf("\n\rReset");
  // Etabli la connection
  //   PC.printf("\n\rRelink");
  fullturn = false;
  // Je traite les points tant que je n'ai pas fait un tour en
  while (!(fullturn)) {
    // Récupère le nombre de points décodables
    pointNumber = (int)rawBuffer.size() / 5;
    // Décode les points disponibles

    if (pointNumber >= 1) {
      //   PC.printf("\n\rConvert");
      if (convertData() == false) {
        stopConnection();
        return false;
      }
    }
  }
  //   PC.printf("\n\rEndConvert");
  // Pré-process les données pour correspondre au format demandé
  std::memcpy(tab, PointBuffer, 181 * sizeof(float));
  completeBuffer(tab);

  //   PC.printf("\n\rCopy");
  //   for (int i = 0; i < 181; i++) {
  //     PC.printf("\n\rA%d R%6.2f", i, tab[i]);
  //   }

  return true;
  // Les données stockées dans le tableau donné en entrée sont désormais
  // analysables
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
  shouldRead = false;
  LidarState = DISCONNECTED;
  relink();
}
/******************Init*********************/

/*****************Interrupts****************/
void getChar(void) {
  // Récupère un caractère et le stock dans le buffer circulaire
  // prévu à cet effet
  static char buffer[5];
  static int i = 0, y = 0;

  //   if (shouldRead) {
  //     rawBuffer.push((char)SLidar.getc());
  //   }
  switch (LidarState) {
  case CONNECTED:
    while (SLidar.readable()) {
      buffer[i] = SLidar.getc();
      i++;
      if (i > 4) {
        if (shouldRead) {
          for (int y = 0; y < 5; y++) {
            rawBuffer.push(buffer[y]);
          }
          i = 0;
        }
      }
    }
    break;
  case DISCONNECTED:
    while (SLidar.readable()) {
      rawBuffer.push(SLidar.getc());
    }
    break;
  }
}
/*****************Interrupts****************/

/*******************Relink******************/
void relink(void) {
  // Etabli une nouvelle connection
  // via les fonctions suivantes
  stopConnection();
  initConnection();
}

void initConnection(void) {
  char char1 = 0;
  char char2 = 0;
  char consumeChar;
  int i = 0;
  rawBuffer.reset();
  //   printf("\n\rRst %d", rawBuffer.size());
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
  // Consomme les 5premiers caractères de la trame (la réponse du Lidar à
  // l'instruction)
  do {
    if (!rawBuffer.empty()) {
      rawBuffer.pop(consumeChar);
      i++;
    }
  } while (i != TAILLE_TRAME);
  LidarState = CONNECTED;
}

void stopConnection(void) {
  // Envoie les caractères de fin de trame
  SLidar.putc(PRE_TRAME);
  SLidar.putc(TRAME_STOP);
  LidarState = DISCONNECTED;
  // Attend une milliseconde (cf datasheet)
  wait_us(999);
}
/*******************Relink******************/

/*****************TreatData*****************/
bool convertData(void) {
  // PC.printf("Valeur reçues = %d", pointReceived);
  static dataPoints dataPoint;
  static bool second_quart;
  // char qualite;
  int i = 0, y = 0;
  char data[5] = {};
  for (i = 0; i < TAILLE_TRAME; i++) {
    rawBuffer.pop(data[i]);
  }
  if (!isSync(data)) {
    return false;
  }
  dataPoint.angle = arrayToAngle(data);
  // Récupère l'angle
  if (dataPoint.angle > 250 || dataPoint.angle < 110) {
    // Il y a des sauts de valeurs, ainsi on empêche les sauts de nous géner
    // en mettant des extrêmes plus grand
    if ((dataPoint.angle >= 270 || dataPoint.angle <= 90) &&
        second_quart == 1) {
      // Je prend ce point et le décode si il concerne l'avant du lidar.
      if (isQualityEnough(data)) {
        // Si la qualité est suffisante
        // Je transforme l'angle pour qu"il soit compris entre 0 et 180 (a
        // la base entre 270 et 90)
        dataPoint.angle = convertAngle(dataPoint.angle);

        // Je décode la distance et j'ajoute au buffer
        dataPoint.range = arrayToRange(data);
        //   PC.printf("\n\rA%d R%6.2f", dataPoint.angle, dataPoint.range);
        addPoint(dataPoint);
      }
    }
  } else if (dataPoint.angle > 180) {
    second_quart = 1;

  } else if (second_quart == 1) {
    second_quart = 0;
    fullturn = true;
    rawBuffer.reset();
    shouldRead = false;
  }
  return true;
}

int arrayToAngle(char *data) {
  // Récupère la valeur entière de l'angle
  return (((int)data[2] << 1 | (int)data[1] >> 7));
}

int arrayToRange(char *data) {
  // Bride la distance
  int range = (float)((int)data[4] << 8 | (int)data[3]) / 4;
  if (range > MAX_RANGE) {
    range = MAX_RANGE;
  }
  return range;
}

bool isQualityEnough(char *data) {
  // Récupère la qualité
  return (((data[0] >> 2) & 0x3F) >= POINT_QUALITY);
}

int convertAngle(int angle) {
  // Converti les angles pour passer de [270;90] à [0;180]
  if (angle <= 90) {
    angle += 90;
  } else {
    angle -= 270;
  }
  return angle;
}
/*****************TreatData*****************/

void completeBuffer(float *data) {
  for (int i = 1; i <= 181; i++) {
    if (data[i] == NULL_VALUE) {
      data[i] = data[i - 1];
    }
  }
}

void resetBuffer(void) {
  for (int i = 0; i < 181; ++i) {
    PointBuffer[i] = NULL_VALUE;
  }
}

void addPoint(dataPoints dataPoint) {
  if (PointBuffer[dataPoint.angle] == NULL_VALUE) {
    PointBuffer[dataPoint.angle] = dataPoint.range;
  } else {
    PointBuffer[dataPoint.angle] =
        (PointBuffer[dataPoint.angle] + dataPoint.range) / 2;
  }
}

bool isSync(char *data) {
  char S_S = data[0] & 0x03;
  char C = data[1] & 0x01;
  int angle = arrayToAngle(data);
  if (S_S == 0x01 || S_S == 0x02) {
    if (C == 0x01) {
      if (angle < 360 && angle >= 0) {
        return true;
      }
    }
  }
  return false;
}