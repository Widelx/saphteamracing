#ifndef _lidar_h_
#define _lidar_h_

#include "mbed.h"
#include <cstring>

/*****Paramètres pouvant être modifiés*****/
// Lidar TX/RX
#define TXLIDAR PF_7
#define RXLIDAR PF_6

// Taille des buffers,
#define RAW_BUFFER_SIZE 5000

// Réglgage acquisition du LiDAR
#define POINT_QUALITY 10 // sans unité
#define MAX_RANGE 2500   // en mm

// Vitesse de rotation du LiDAR
#define PWMLIDAR 0.4
#define PIN_PWM PA_0
/*****Paramètres pouvant être modifiés*****/
/*
 *
 *
 *
 *
 *
 *
 */
/*****NE DEVRAIT PAS ETRE MODIFIE*****/
// Lidar diverses infos
#define TAILLE_TRAME 5
#define TRAME_START 0x20
#define TRAME_STOP 0x25
#define PRE_TRAME 0xA5
#define TRAME_RECEP1 0xA5
#define TRAME_RECEP2 0x5A

// Précision du tableau de données renvoyées
// 180 => un point par degré, suffisant !
// si différent de 180 crash !
#define NULL_VALUE MAX_RANGE + 500

// Paramètre de la vision du lidar
/*****NE DEVRAIT PAS ETRE MODIFIE*****/

struct dataPoints {
  int angle;
  float range;
};

/********Fonctions publiques**********/
// Prend un tableau en entrée et le complète
bool getLidarData(float *tab);
// Init
void initLidar(void);
/********Fonctions publiques**********/
/*
 *
 *
 *
 *
 *
 */
/*******Fonctions PRIVEES******/
// Ne devraient pas être utilisées
// Interrupt
void getChar(void);

// Relink
void relink(void);
void initConnection(void);
void stopConnection(void);
bool isSync(char *data);

// TreatData
bool convertData(void);
int arrayToAngle(char *data);
int arrayToRange(char *data);
bool isQualityEnough(char *data);
int convertAngle(int angle);

// SendData
void completeBuffer(float* data);
void resetBuffer(void);
void addPoint(dataPoints dataPoint);
/*******Fonctions PRIVEES******/

#endif