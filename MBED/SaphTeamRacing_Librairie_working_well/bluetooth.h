#ifndef _bluetooth_h_
#define _bluetooth_h_

#include "mbed.h"
#include "pinmap.h"

/*****Paramètres pouvant être modifiés*****/
#define BLE_BUFFER_SIZE 5000
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
#define PRECISION 180
/*****NE DEVRAIT PAS ETRE MODIFIE*****/

/********Fonctions publiques**********/
void initBLE(void);
void sendOverBluetooth(float tab_data[]);
/*
 *
 *
 *
 *
 *
 */
/*******Fonctions PRIVEES******/
void putData(void);
/*******Fonctions PRIVEES******/

#endif