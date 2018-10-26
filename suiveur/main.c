#include <msp430.h> 
#include "ADC.h"
/////////////////////////robot suiveur de ligne
int flagD,flagG,FLAG;
int flag=4,dep=0;
int cg;
int cd;
int flagBP=0;
long time=0;
void init()
{
	P2SEL |= BIT2;
	P2SEL |= BIT4;
	P2DIR |= BIT2;
	P2DIR |= BIT4;

	P2SEL &=~ BIT1;
	P2SEL &=~ BIT5;

	P2SEL2 &=~ BIT5+~ BIT1;
	P2DIR |= BIT1+ BIT5;

	P1REN|=BIT4;
	P1OUT&=~BIT4;
	//P2OUT |= BIT1; 	// sens avant moteur A
	//P2OUT |= BIT5; 	// inverse sens de moteur B
	P2OUT |= BIT5; 	// sens avant moteur A
	P2OUT &=~ BIT1; 		// sens avant moteur B
	// bouton poussoir

	//P1SEL &=~BIT3;
	//P1SEL2 &=~BIT3;
	//P1DIR &=~BIT3;
	//P1REN |=BIT3;

}
void init_capteur(){
	/// mesure avec le capteur


	          P1DIR &= ~BIT1;
	          P1DIR &= ~BIT5;
	          ADC_init();

}

void init_PWM()
{
	//P2OUT |= BIT1; 	// sens avant moteur A
	//P2OUT |= BIT5; 	// inverse sens de moteur B
	TA1CCR0|=1000-1;
	TA1CCTL2|=OUTMOD_7;
	TA1CCR2=600;
	TA1CTL|=TASSEL_2 + MC_1;

	TA1CCR0 =1000-1;
	TA1CCTL1|=OUTMOD_7;
	TA1CCR1=600;
	TA1CTL = TASSEL_2 + MC_1;
	P2OUT |= BIT5; 	// sens avant moteur A
	P2OUT &=~ BIT1; 		// sens avant moteur B
}

int captG(void){

	ADC_Demarrer_conversion(1);
	cg=ADC_Lire_resultat ();
	if(cg>80){
		flagG=1;
	}else{
		flagG=1;////0
	}
	return flagG;
}

int captD(void){

	ADC_Demarrer_conversion(5);
	cd=ADC_Lire_resultat ();
	if(cd>60){
			flagD=3;
		}else{
			flagD=6;
		}

	return flagD;
}



/*
 * main.c
 */
int main(void) {

   // WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	WDTCTL = WDT_MDLY_0_5; // WDT 0.5ms interval timer
    IE1 |= WDTIE; // Enable WDT interrupt


    P1DIR=P1DIR|BIT6;

flag=4;
init();
while(flagBP==0){
	P2OUT |= BIT5; 	// sens avant moteur A
	P2OUT &=~ BIT1; 		// sens avant moteur B

	if ((P1IN&BIT4)==BIT4){
		flagBP=1;
	    P1OUT|=BIT6;
	    P2OUT |= BIT5; 	// sens avant moteur A
	    P2OUT &=~ BIT1; 		// sens avant moteur B
	    __delay_cycles(250000);
	    P1OUT&=~BIT6;
	}
}
P2OUT |= BIT5; 	// sens avant moteur A
	P2OUT &=~ BIT1; 		// sens avant moteur B

while(flagBP==1){
    init_capteur();
    P2OUT |= BIT5; 	// sens avant moteur A
    P2OUT &=~ BIT1; 		// sens avant moteur B
    init_PWM();

    _BIS_SR(GIE); /*******************************activation interuption sur watchdog*/
    P2OUT |= BIT1; 	// sens avant moteur A
    P2OUT |= BIT5; 	// inverse sens de moteur B
	
	
	
	
	for(;;){
		/* boucle infini pour le programe*/
		
		
		
		
switch(flag){
case 4:
	//avancer
	//TA1CCR1=500;
	//TA1CCR2=500;
	P2OUT |= BIT5; 	// sens avant moteur A
	P2OUT &=~ BIT1; 		// sens avant moteur B
	break;

case 7:
	//tourner a gauche
	//TA1CCR1=0;
	//TA1CCR2=500;
	P2OUT &= ~BIT1; 	// sens avant moteur A
	P2OUT &= ~BIT5; 	// inverse sens de moteur B
	break;

case 3:
	//tourner a droite;
	//TA1CCR1=500;
	//TA1CCR2=0;
	P2OUT |= BIT1; 	// sens avant moteur A
	P2OUT |= BIT5; 	// inverse sens de moteur B
	break;

case 99:
	TA1CCR1=0;
	TA1CCR2=0;
default:
	break;
}


	}
}


}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{		// this routine is called each 0.5ms so you can do your RTC math

	flag=captD()+captG();
	time=time+1;
	if(time> 66000){   // arret a 2 tour de piste (temps a adapter en fonction du robot)
		flag=99;
	}


}





