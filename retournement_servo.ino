/*################################################################
##  Script Arduino retournement couveuse Brinsea Octagon        ##
##################################################################
## ce script a pour objectif de controler un servomoteur pour   ##
## le retuornement automatique de la couveuse Brinsea Octagon.  ##
## L'appui sur le bouton d'interruption permet de passer du     ##
## mode retournement automatique au mode "pause" où la couveuse ##
## est en position horizontale pour une periode indéterminée.   ##
##                                                              ##
##################################################################
##  Développeur:  Henri DEWILDE                                 ##
##          h****.d******@gmail.com                             ##
##################################################################
##  Version 1  :    avril 2019                                  ##
################################################################*/


// Inclut la lib Servo pour manipuler le servomoteur 
#include <Servo.h>

// Créer un objet Servo pour contrôler le servomoteur
Servo theServo;

// pins utilisés
int pinServo = 9;
int pinLED = 12;
//int pinInterrupt = 2;

// autres variables
int delayRotation = 20;
unsigned long delayBetweenRotation = 3600*3*1000; // délai entre les retournement (3h)
//unsigned long delayBetweenRotation = 20*1000; // délai entre les retournement (20 sec pour test)

int initServoPositionValue = 105;  // position initiale du servo désirée (position neutre : couveuse à plat)
int ServoPositionValue1 = initServoPositionValue + 58 ;
int ServoPositionValue2 = initServoPositionValue - 50 ;

volatile bool boolInterrupt = false;
volatile bool IsJustInterrupted = false;

// variable utiles à la fonction de debouncing
long debouncing_time = 500; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;


// fonctions générales

void setup() {
    
  // Attache le servomoteur à la broche D9
  theServo.attach(pinServo,560,2000);
  
  pinMode(pinLED, OUTPUT); 

  attachInterrupt(0, debounceInterrupt, RISING); // attache l'interruption externe n°0 à la fonction "interrupt0"

  blinkLED(5,200);  // on fait clognoter la LED pour signaler le lancement du programme

  initServoPosition(initServoPositionValue); // initialise la position du servomoteur vers la position neutre
  delayUntilInterrupt(5000);
}


void loop() {

  IsJustInterrupted = false;

  // cas interruption : on reste en position neutre
  if (boolInterrupt) {
    blinkLED(4,250);

    initServoPosition(initServoPositionValue);
    delayUntilInterrupt(delayBetweenRotation);
  }
  // cas fonctionnement normal : on change de position après un délai
  else {
    // On vérifie qu'il n'y a pas eu d'interruption
    if (IsJustInterrupted == false) {      
      blinkLED(2,750);

      // Fait bouger le bras jusque la position +45° 
      initServoPosition(ServoPositionValue1);
      delayUntilInterrupt(delayBetweenRotation);
    }

    // On vérifie qu'il n'y a pas eu d'interruption
    if (IsJustInterrupted == false) {      
      blinkLED(2,750);

      // Fait bouger le bras jusque la position -45° 
      initServoPosition(ServoPositionValue2);
      delayUntilInterrupt(delayBetweenRotation);
    }
  }
}



// déclaration des fonctions

// fonction pour modifier la position du servomoteur peu importe sa position initiale
void initServoPosition(int positionValue) {
  int initialValue = theServo.read();

  if (initialValue > positionValue) {
    for (int position = initialValue; position >= positionValue; position--) {
      theServo.write(position);
      
      delay(delayRotation);
      digitalWrite(pinLED, 0);
      delay(delayRotation);
      digitalWrite(pinLED, 1);
    }
  }
  else if (initialValue < positionValue) {
    for (int position = initialValue; position <= positionValue; position++) {
      theServo.write(position);
      
      delay(delayRotation);
      digitalWrite(pinLED, 0);
      delay(delayRotation);
      digitalWrite(pinLED, 1);
    }
  }
}


// fonction permettant d'éviter le rebond du bouton poussoir
void debounceInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    interrupt0();
    last_micros = micros();
  }
}


// fonction liée à l'interruption (appui du bouton) : on change l'état de booléen et on fait clignoter la LED
void interrupt0() {
  boolInterrupt = !boolInterrupt;
  IsJustInterrupted = true;
}

// fonction permettant de faire clignoter la LED n fois
void blinkLED(int n, int delayBlink){
  for (int i=0; i <=n; i++){
    delay(delayBlink);
    digitalWrite(pinLED, 0);
    delay(delayBlink);
    digitalWrite(pinLED, 1);
  }
}

// fonction permettant d'attendre jusqu'à la fin d'un délai donné, ou jusqu'à l'interruption
void delayUntilInterrupt(unsigned long delayToWait){
  unsigned long delayWaited = 0;

  while (delayWaited < delayToWait && !IsJustInterrupted) {
    delay(1000);
    delayWaited += 1000;
  }
}
