#include "LPC17xx.h"

//Change le rapport cyclique (en pourcentage) avec une précision au dixième de pourcentage
bool setPWMServo(rapport_cyclique){
	rapport_cyclique = (unsigned int) (rapport_cyclique*10);
	if(rapport_cyclique > 1000 || rapo){
		return 0;
	}
	else{
		LPC_PWM1->MR3 = (rapport_cyclique*10) - 1;
		return 1;
	}	
}

//Change le rapport cyclique (en pourcentage) avec une précision au dixième de pourcentage
bool setPWMMoteur(rapport_cyclique){
	rapport_cyclique = (unsigned int) (rapport_cyclique*10);
	if(rapport_cyclique > 1000){
		return 0;
	}
	else{
		LPC_PWM1->MR4 = (rapport_cyclique*10) - 1;
		return 1;
	}	
}

//Initialise, active et configure le PWM1 pour une fréquence de 50Hz
void initPwm(void){ // P=50Hz 
	LPC_SC->PCONP= LPC_SC->PCONP | (1<<6); //Active le PWM1
	LPC_PWM1->CTCR = 0 ; //mode timer
	LPC_PWM1->PR = 249 ; //Pas de prédivision
	LPC_PWM1->MR0 = 999 ; //valeur de N pour une fréquence de 50Hz
	//On pourra ajuster le rapport cyclique au dixième de pourcent et l'exprimer en pourcentage assez simplement
	LPC_PWM1->MCR |= (1<<1) ; //RAZ du compteur si correspondance avec MR0
}

//Initialise le PWM du servo moteur(sur P2.2) et le lance
void initPWMServo(void){ 
	LPC_PINCON->PINSEL4 |= (1<<4) ; //PWM 1.3 sur P2.2
	LPC_PWM1->PCR |= (1<<11) ; //active la sortie PWM1.3
	LPC_PWM1->MR3 = 90-1; //Rapport de 9%
	LPC_PWM1->LER |= (1<<3) //Autorise la réécriture de MR3 dynamiquement 
	LPC_PWM1->TCR = 1 ; //démarrage du timer
}

//Initialise le PWM du servo moteur(sur P2.2) et le lance
void initPWMMoteurCC(void){ // PWM1.4 sur P2.3
	LPC_PINCON->PINSEL4 |= (1<<6) ; //PWM 1.4 sur P2.3
	LPC_PWM1->PCR |= (1<<12) ; //active la sortie PWM1.4
	LPC_PWM1->MR4 = 1; //Rapport de 0.1%
		LPC_PWM1->LER |= (1<<4) //Autorise la réécriture de MR3 dynamiquement
	LPC_PWM1->TCR =1 ; //démarrage du timer
}