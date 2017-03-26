


#include <CurieBLE.h>
#include <Servo.h>

#define trigPin 7
#define echoPin 8
#define trigPinTwo 4
#define echoPinTwo 5
#define Valve 13
#define LEDONE  1
#define LEDTWO  2
#define LEDTHREE 10
#define LEDFOUR  11
#define LEDFIVE  12
#define BUZZER_PIN A0 /* sig pin of the buzzer */

int LEDcounter = 0;
int calibrationInitial=1385;
int drinkySpeed= 1427;            //ADD 42 TO CALIBINIT
int timeSpeed= 691;
bool servothing=false;
Servo drinkTray;

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service     

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

int i=0;
long duration,distance;
const int minRang = 0;    //minimum rang or detection of the sensor 
const int maxRang = 4;
const int minRangTwo = 2;    //minimum rang or detection of the sensor 
const int maxRangTwo = 6;
int delaytime;


int length = 15; /* the number of notes */
char notes[] = "ccggaagffeeddc ";
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

//fundtion prototypes 
void LEDlight(int);       //function to light up LED
void reinicialize();      //function to trun off LED
bool checkglass();        //return true of false if a glass is detected
bool checkglassTwo();
void stop();              //stops the rotation to poor liquid into glass

//main setup
void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(3,OUTPUT);          //set pin 3 as output
  pinMode(Valve, OUTPUT);     //set pin 13 as output
  pinMode(trigPin, OUTPUT);   //declairing output mode for pin7
  pinMode(echoPin, INPUT);    //declairing input mode for pin8
  pinMode(trigPinTwo, OUTPUT);   //declairing output mode for pin7
  pinMode(echoPinTwo, INPUT);
  pinMode(LEDONE, OUTPUT);    //declaire pins for LED's 
  pinMode(LEDTWO,OUTPUT);
  pinMode(LEDTHREE, OUTPUT);
  pinMode(LEDFOUR,OUTPUT);
  pinMode(LEDFIVE,OUTPUT);

  drinkTray.attach(3);    //declaire servo as digital pin 3

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("ShotBOT");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());        

  // add service and characteristic:
  blePeripheral.addAttribute(ledService);           
  blePeripheral.addAttribute(switchCharacteristic);

  // set the initial value for the characeristic:
  switchCharacteristic.setValue(0);

  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println("BLE LED Peripheral");
  drinkTray.write(1427);
  while(true){
  if (checkglassTwo()) break;
  }
  stop();
}

//main loop
void loop() {
  
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

     LEDcounter = 0;
    void reinicialize();

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      while (switchCharacteristic.written()) {
        Serial.print("written");
        if (switchCharacteristic.value() == 1) {   // any value other than 0
            //pinMode(3,OUTPUT);
            //drinkTray.attach(3);
            delaytime = timeSpeed;
            drinkTray.write(1427);
            while(true){
            if (checkglassTwo()) break;
            }
            stop();
            for(i=0; i<6; i++){                 // keep track of how many glasses are filled
              drinkTray.writeMicroseconds(drinkySpeed);
                delay(delaytime);
                stop();
              if(checkglass()){             //fill glass only if the glasse is present
                Serial.print("FOUND\n");
                delay(200);
                digitalWrite(Valve,HIGH);
                delay(1800);
                digitalWrite(Valve,LOW);
                delay(1200);
                LEDcounter++;
                LEDlight(LEDcounter);
              }else{                          //do nothing if there is no glass
                Serial.print("not detected \n");
              }
            }
            //play the song
              for(int i = 0; i < length; i++) {
                if(notes[i] == ' ') {
                  delay(beats[i] * tempo);
                } else {
                   playNote(notes[i], beats[i] * tempo);
                }
                 delay(tempo / 2); /* delay between notes */
               }
              //delaytime-=5;      // always delay
              delay(1000);
        }
        if (switchCharacteristic.value() == 0){   // a 0 value
          void reinicialize();
          central.disconnect();                   //connection disconected if 0 is sent
          Serial.print("ShotBot disconnected");
        }
      }
    }
  }
}

//function to light up LED
void LEDlight(int numLED){
  switch(numLED){
    case 2:
      digitalWrite(LEDONE,HIGH);
      break;
    case 4:
      digitalWrite(LEDTWO,HIGH);
      break;
    case 6:
      digitalWrite(LEDTHREE,HIGH);
      break;
    case 8:
      digitalWrite(LEDFOUR,HIGH);
      break;
    case 10:
      digitalWrite(LEDFIVE,HIGH);
      
  }
} 
      
      /* play tone */
      void playTone(int tone, int duration) {
      for (long i = 0; i < duration * 1000L; i += tone * 2) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(tone);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(tone);
       }
      }
      void playNote(char note, int duration) {
      char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'z'};
      int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 1479 };
       // play the tone corresponding to the note name
      for (int i = 0; i < 8; i++) {
         if (names[i] == note) {
         playTone(tones[i], duration);
         }
      }
    }
 
 

//inicialize the function to reinicialize
void reinicialize(){
      digitalWrite(LEDONE,LOW);
      digitalWrite(LEDTWO,LOW);
      digitalWrite(LEDTHREE,LOW);
      digitalWrite(LEDFOUR,LOW);
      digitalWrite(LEDFIVE,LOW);
}

//function to stop the sorvo for turning
void stop(){
  drinkTray.writeMicroseconds(calibrationInitial);
}

//function to detect if a glass is present or not return true or false
bool checkglass(){
  //find the distance to the glass and find if there is a glass
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
 
  if (distance <= minRang || distance >= maxRang){
    return false;
  }else { 
    return true;
  }
}

bool checkglassTwo(){
  //find the distance to the glass and find if there is a glass
  digitalWrite(trigPinTwo, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(trigPinTwo, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPinTwo, LOW);
  duration = pulseIn(echoPinTwo, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
 
  if (distance <= minRangTwo || distance >= maxRangTwo){
    return false;
  }else { 
    return true;
  }
}




