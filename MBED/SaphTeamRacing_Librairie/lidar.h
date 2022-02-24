#ifndef _lidar_h_
#define _lidar_h_

#include "mbed.h"
#include "pinmapF746NG.h"

/*****Paramètres pouvant être modifiés*****/
// Taille des buffers,
#define POINT_BUFFER_SIZE 5000
#define RAW_BUFFER_SIZE 5000

// Réglgage acquisition du LiDAR
#define POINT_QUALITY 10 // sans unité
#define MAX_RANGE 2500   // en mm

// Vitesse de rotation du LiDAR
#define PWMLIDAR 0.3
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
#define PRECISION 180
#define NULL_VALUE MAX_RANGE + 500

// Paramètre de la vision du lidar
#define ANGLE_AVANT_1 270
#define ANGLE_AVANT_2 90
/*****NE DEVRAIT PAS ETRE MODIFIE*****/

struct dataPoints {
  float angle;
  float range;
};

/********Fonctions publiques**********/
// Prend un tableau en entrée et le complète
void getLidarData(float tab[]);
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
void emptyBuffers(void);

// TreatData
void convertData(int pointReceived);
float arrayToAngle(char char1, char char2);
float arrayToRange(char char3, char char4);
bool isQualityEnough(char charQ);
float convertAngle(float angle);

// SendData
void sendToProcess(float *tab_data);
/*******Fonctions PRIVEES******/

#endif