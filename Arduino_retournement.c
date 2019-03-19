/* *
 * Exemple de code pour un servomoteur, il fait faire des va-et-vient à la tête du servomoteur.
 */

 /* Inclut la lib Servo pour manipuler le servomoteur */
#include <Servo.h>

/* Créer un objet Servo pour contrôler le servomoteur */
Servo monServomoteur;

// pins utilisés
int pinServo = 9;
int pinLED = 13;
int pinInterrupt = 2;

// autres variables
int delayRotation = 500;
unsigned long delayBetweenRotation = 3600*3*1000; // délai entre les retournement (3h)
int initServoPositionValue = 90;  // position initiale du servo désirée (position neutre : couveuse à plat)
int ServoPositionValue1 = initServoPositionValue + 45 ;
int ServoPositionValue2 = initServoPositionValue - 45 ;
volatile bool boolInterrupt = false;


// fonctions générales


void setup() {
    
  // Attache le servomoteur à la broche D9
  monServomoteur.attach(pinServo);

  pinMode(pinLED, OUTPUT); 

  attachInterrupt(0, interrupt0, RISING); //attache l'interruption externe n°0 à la fonction "blink"

  clignLED(10,200);

  initServoPosition(initServoPositionValue);
  delay(5000);
}


void loop() {

  // cas interruption : on reste en position neutre
  if (boolInterrupt) 
  {
    clignLED(4,250);

    initServoPosition(initServoPositionValue);
    delay(delayBetweenRotation);
  }
  // cas fonctionnement normal : on change de position après un délai
  else
  {
    clignLED(2,750);

    /* Fait bouger le bras de 0° à 90° /!\ valeurs à modifier pour 90 / -90 /!\ */
    initServoPosition(ServoPositionValue1);

    delay(delayBetweenRotation);

    clignLED(2,750);

    // Fait bouger le bras de 90° à 0°
    initServoPosition(ServoPositionValue2);

    delay(delayBetweenRotation);
  }
}



// déclaration des fonctions


void initServoPosition(int positionValue) {
  int initialValue = monServomoteur.read();

  if (initialValue > positionValue) {

    for (int position = initialValue; position >= positionValue; position--) {
      monServomoteur.write(position);
      
      delay(delayRotation);
      digitalWrite(pinLED, 0);
      delay(delayRotation);
      digitalWrite(pinLED, 1);
    }
  }
  else if (initialValue < positionValue) {
    for (int position = initialValue; position <= positionValue; position++) {
      monServomoteur.write(position);
      
      delay(delayRotation);
      digitalWrite(pinLED, 0);
      delay(delayRotation);
      digitalWrite(pinLED, 1);
    }
  }

}

// fonction liée à l'interruption (appui du bouton) : on change l'état de booléen et on fait clignoter la LED
void interrupt0() {
  boolInterrupt = !boolInterrupt;
}


void clignLED(int n, int delayClign){
  for (int i=0; i <=n; i++){
    delay(delayClign);
    digitalWrite(pinLED, 0);
    delay(delayClign);
    digitalWrite(pinLED, 1);
  }
}




