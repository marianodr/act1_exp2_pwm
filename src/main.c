// *******************************************************************//
// Rodriguez Mariano 21/09/2023
// Programa para control de Motor CC a traves de PWM
// Usa interrupcion externa INT 0 para el pulsador PB3
// *******************************************************************//
// Definiciones
// ---------------------------------------------------------------------
#define F_CPU    16000000UL
#define BOUNCE_DELAY 8 //ms
// Entradas:
#define P1      PD1           // Encendido con PWM y apagado
#define P2      PD2           // Selector de temporizacion
#define P3      PD0           // Encendido directo y apagado
// Salidas:
#define LED1      PC0
#define LED2      PC1
#define LED3      PC2
#define LED4      PC3
#define LED5      PC4
#define MOTOR     PC5

// Macros de usuario
// -------------------------------------------------------------------
#define	sbi(p,b)	p |= _BV(b)					//	sbi(p,b) setea el bit b de p.
#define	cbi(p,b)	p &= ~(_BV(b))				//	cbi(p,b) borra el bit b de p.
#define	tbi(p,b)	p ^= _BV(b)					//	tbi(p,b) togglea el bit b de p.
#define is_high(p,b)	(p & _BV(b)) == _BV(b)	//	is_high(p,b) p/testear si el bit b de p es 1.
#define is_low(p,b)		(p & _BV(b)) == 0		//	is_low(p,b) p/testear si el bit b de p es 0.

// Inclusion de archivos de cabecera
// -------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

// Variables globales
// -------------------------------------------------------------------
volatile int switchedOn = 0, FLAG = 0;
float deltaT;
int stateP2, stateP1;
int lastStateP2 = 1, lastStateP1 = 1, T=5;

// Declararacion de funciones
// -------------------------------------------------------------------
void initPorts();
void boot();
void initExternalInterrupts();
void pollingP2();                  // Consulta, antirebote y accion de PB2
void pollingP1();                  // Consulta, antirebote y accion de PB2
void turnOn();                     // Encendido directo que activa PB3
void turnOnPWM(int Ti);            // Encendido por PWM que activa PB1
void turnOff();                    // Apagado de LEDs y Motor (tanto para PB1 como PB3)
void delay(int ms);
void indicateTime();
void indicateLED(volatile uint8_t *port, uint8_t pin);

// Interrupciones externas
// -------------------------------------------------------------------
ISR(INT0_vect){
	_delay_ms(BOUNCE_DELAY);
	if (is_low(PIND,P3)){           // Comprueba si PB3 sigue en BAJO
		FLAG = 1;                    // Establecer la bandera para indicar la interrupci�n
	}
}

// Programa
// -------------------------------------------------------------------
int main(void){
	initPorts();
	boot();
	initExternalInterrupts();
	sei();							// Habilita las interrupciones globalmente.

	// Inicio
	while(1){
		pollingP2();
		pollingP1();

		if(FLAG){
			if(switchedOn){
				turnOff();
			}
			else{
				turnOn();
			}
			FLAG=0;
		}
	}
}

// Funciones
// ----------------------------------------------------------------------------------------
void initPorts(){
	// Se configura e inicializa los puertos
	DDRC = 0xFF;     // Puerto C todo como salida
	PORTC = 0x00;    // Inicializa el puerto C

	// Se configura a PD0, PD1 y PD2 como puertos de entrada con resistencia pull-up internas:
	PORTD = (1 << P1) | (1 << P2) | (1 <<P3);
}

void boot(){
	// Secuencia de Arranque
	for(int i=0; i<5; i++){
		PORTC = 0x1F;     // Encienden los 5 LEDs
		_delay_ms(400);
		PORTC = 0x00;     // Apagan los 5 LEDs
		_delay_ms(150);
	}
}

void initExternalInterrupts(){
	// Habilitacion de interrupciones externas:
	EICRA = (1<<ISC01);	            // Configura interrupcion INT0 sensible a flanco descendente
	EIMSK = (1<<INT0);				// Habilita interr. externa INT0.
	EIFR = 0x00;					// Borra flag INTF0 p/evitar alguna interrupcion espurea.
}

void pollingP2(){
    stateP2 = (PIND & (1 << P2));         // Comprueba el estado de P2

	if(stateP2 != lastStateP2){          // Comprueba cambio de estado del botón

        if(!stateP2){                    // El botón pasó de ALTO a BAJO
    	    _delay_ms(BOUNCE_DELAY);

			if(!stateP2){                // El botón todavía está en BAJO
				if(T!=14){
					T += 3;
				}
				else{
					T = 5;
				}
			}
			indicateTime();
        }
		else{
			_delay_ms(10); // Retraso para evitar cambios rápidos
		}
    }
    lastStateP2 = stateP2;
}

void pollingP1(){
	stateP1 = (PIND & (1 << P1));         // Comprueba el estado de P1

	if(stateP1 != lastStateP1){          // Comprueba cambio de estado del botón

		if(!stateP1){                    // El botón pasó de ALTO a BAJO
    	    _delay_ms(BOUNCE_DELAY);

			if(!stateP1){                // El botón todavía está en BAJO
				if(switchedOn){
					turnOff();
				}
				else{
					turnOnPWM(T);
				}
			}
        }
		else{
			_delay_ms(10); // Retraso para evitar cambios rápidos
		}
    }
    lastStateP1 = stateP1;
}

void turnOn(){
	PORTC = 0x3F;
	switchedOn = 1;
}

void turnOnPWM(int Ti){
	int highTime = 0, lowTime = 0;

	Ti *= 1000;

	deltaT = (int)(Ti/100);

	for(int n=0; n<100; n++){
		if(FLAG==0){
			highTime = (int)((deltaT/100) * (n+1));
			lowTime = (int)(deltaT - highTime);

			switch (n){
				case 19:
					sbi(PORTC,PC0);
					break;

				case 39:
					sbi(PORTC,LED2);
					break;

				case 59:
					sbi(PORTC,LED3);
					break;

				case 79:
					sbi(PORTC,LED4);
					break;

				case 99:
					sbi(PORTC,LED5);
					break;
			}

			sbi(PORTC,MOTOR);
			delay(highTime);

			cbi(PORTC,MOTOR);
			delay(lowTime);

			sbi(PORTC,MOTOR);

			switchedOn = 1;
		}
		else {
			turnOff();
			FLAG = 0;
			break;
		}
	}
}

void turnOff(){
	PORTC = 0x00;
	switchedOn = 0;
}

void delay(int ms){
	while (0 < ms){
		_delay_ms(1);
		--ms;
	}
}

void indicateTime(){

	if(T==5)
		indicateLED(&PORTC,LED1);

	if(T==8)
		indicateLED(&PORTC,LED2);

	if(T==11)
		indicateLED(&PORTC,LED3);

	if(T==14)
		indicateLED(&PORTC,LED4);
}

void indicateLED(volatile uint8_t *port, uint8_t pin){

	if(switchedOn){
			cbi(*port,pin);             // Apaga y enciende el led correspondiente
			_delay_ms(500);
			sbi(*port,pin);
	}
	else {
		sbi(*port,pin);                 // Enciende y apaga el led correspondiente
		_delay_ms(500);
		cbi(*port,pin);
	}
}