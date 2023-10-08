# act1_exp2_pwm
## Asignatura: 
"IC422 Sistemas Embebidos"
## Actividad: 
Actividad Experimental Nº1: Manejo de Puertos, Temporizadores e Interrupciones en Microcontroladores ATmega2560 y ATmega328p.
## Consigna:
**Experiencia 2: Arranque suave para motor de CC.** 
Una máquina herramienta posee una parte impulsada por un motor de CC. Este motor debe arrancar en forma suave para quedar en marcha a velocidad nominal. El arranque suave es realizado mediante la técnica de modulación de ancho de pulsos (PWM), donde la señal que comanda al motor (mediante etapa de potencia, Fig. 2) es una onda rectangular cuyo ciclo útil aumenta en forma progresiva hasta alcanzar el valor del 100 % (momento el cual gira a velocidad nominal). Este aumento progresivo del ciclo útil se hace en un tiempo T, el cual se pretende que sea programable por el usuario de la máquina. A partir de esto, desarrollar el prototipo inicial de un sistema embebido que cumpla con la operación indicada y considerando elementos de la Fig. 2, estos deben cumplir con las siguientes funciones:
* PB1: Este pulsador servirá para encender/apagar el motor. Cada vez que el usuario lo acciona, el circuito cambia de estado entre apagado y encendido. Al encenderse el motor, el mismo arranca en forma suave.
* PB2: Este pulsador permitirá seleccionar el tiempo de duración T del arranque suave. Existirán cuatro tiempos: T1 = 5 s, T2 = 8 s, T3 = 11 s y T4 = 14 s. Con cada pulsado de PB2, el usuario puede seleccionar el tiempo según: pulsa una vez, cambia de T1 a T2; pulsa otra vez, cambia de T2 a T3; pulsa nuevamente, cambia de T3 a T4;pulsa una vez más, cambia de T4 a T1, y así sucesivamente.
* PB3: Este pulsador servirá para encender/apagar el motor. Cada vez que el usuario lo acciona, el circuito cambia de estado entre apagado y encendido.
* LEDpwm: Está asociado a la salida de comando PWM y permite mostrar la variación del ciclo útil de esta señal durante el arranque suave del motor.
* LEDs: Los cinco LEDs conectados a uno de los puertos de E/S del microcontrolador, permitirán visualizar el progreso del arranque suave. Los mismos deben encenderse uno a uno, cubriendo todo el tiempo de duración del arranque suave del motor. Cuando el motor queda en marcha a velocidad nominal, todos los LEDs quedan encendidos.
* Salida PWM: Proporciona la señal de comando del motor y durante arranque suave del mismo, su ciclo útil cambia en forma progresiva (gradualmente, NO en saltos) hasta alcanzar el 100%.
