


#include <CurieBLE.h>
#include <Servo.h>

#define trigPin 7
#define echoPin 8
#define Valve 13
int calibrationInitial=1385;
int drinkySpeed= 1427;
int timeSpeed= 690;
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
int delaytime;
//fundtion prototypes 
bool checkglass();        //return true of false if a glass is detected
void stop();              //stops the rotation to poor liquid into glass

//main setup
void setup() {
  Serial.begin(9600);
  
  pinMode(3,OUTPUT);          //set pin 3 as output
  pinMode(Valve, OUTPUT);     //set pin 13 as output
  pinMode(trigPin, OUTPUT);   //declairing output mode for pin7
  pinMode(echoPin, INPUT);    //declairing input mode for pin8

  drinkTray.attach(3);    //declaire servo as digital pin 3

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("MyBitch");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());        

  // add service and characteristic:
  blePeripheral.addAttribute(ledService);           
  blePeripheral.addAttribute(switchCharacteristic);

  // set the initial value for the characeristic:
  switchCharacteristic.setValue(0);

  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println("BLE LED Peripheral");
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
                delay(1800);
              }else{                          //do nothing if there is no glass
                Serial.print("not detected \n");
              }
              delaytime-=5;      // always delay
              delay(1000);
            }
        }
        if (switchCharacteristic.value() == 0){   // a 0 value
          central.disconnect();                   //connection disconected if 0 is sent
          Serial.print("ShotBot disconnected");
        }
      }
    }
  }
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



