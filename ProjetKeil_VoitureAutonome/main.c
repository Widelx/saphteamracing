#include <LPC17xx.h>
#include "GPIO.h"
#include "LPC_PWM.h"
#include "circular_buffer.h"


void write(char data);
void read(void);
void Init_BT (void);
void Init_Lidar (void);


int main(void) {
	initPWM();
	initPWMMoteur();
	initPWMServo();
	setPWMServo(50);
	setPWMServo(60);
	Init_BT;
	Init_Lidar;
	return 0;
}





void Init_BT (void) // broches P0.2 pour TX et P0.3 pour RX
{
	LPC_SC->PCONP |= (1<<3)//Active la sortie UART0 (active de base sur un reset)
	LPC_PINCON->PINSEL0 |= 0x5 << 4 ; // broches P0.2 pour TX et P0.3 pour RX
	LPC_UART0->FCR |= 1|(0x02<<6) //Active le FIFO et lance une interruption à la réception de 8 caractères
	LPC_UART0->LCR = 0x03 ; // 8 bits sans parité, 1 bit de stop
	// Réglage de la vitesse de transmission à 115200
	LPC_UART0->LCR |= 0x80 ; // Forçage bit DLAB=1 (Demande autorisation de modification)
	LPC_UART0->DLM = 0 ; // Pas de sur-division de PCLK
	LPC_UART0->DLL = 9 ; // Division principale par 9 de PLCK
	LPC_UART0->FDR = 0x21 ; // Division fractionnaire de 1,5 (DIVAddVAL=1 et MULVAL=2)
	LPC_UART0->LCR &= 0x7F ; // Forçage bit DLAB=0 (Fin d'autorisation de modification)
}

void Init_Lidar (void) // broches P0.2 pour TX et P0.3 pour RX
{
	LPC_SC->PCONP |= (1<<24)//Active la sortie UART2 (non active de base sur un reset)
	LPC_PINCON->PINSEL0 |= 0x5 << 20 ; // broches P0.10 pour TX et P0.11 pour RX
	LPC_UART2->FCR |= 1|(0x02<<6) //Active le FIFO et lance une interruption à la réception de 8 caractères
	LPC_UART0->LCR = 0x03 ; // 8 bits sans parité, 1 bit de stop
	// Réglage de la vitesse de transmission à 115200
	LPC_UART2->LCR |= 0x80 ; // Forçage bit DLAB=1 (Demande autorisation de modification)
	LPC_UART2->DLM = 0 ; // Pas de sur-division de PCLK
	LPC_UART2->DLL = 9 ; // Division principale par 9 de PLCK
	LPC_UART2->FDR = 0x21 ; // Division fractionnaire de 1,5 (DIVAddVAL=1 et MULVAL=2)
	LPC_UART2->LCR &= 0x7F ; // Forçage bit DLAB=0 (Fin d'autorisation de modification)
}

void read{
	static unsigned int i;
	while(!(UART0->LSR & (1<<0)){
		rawData[i] = UART0->RBR;
		i++;
	}
}

void write(data){
	while(!(UART0->LSR & (1<<5)){}
	UART0->THR = data;
}