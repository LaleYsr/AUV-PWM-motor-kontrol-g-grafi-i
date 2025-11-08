#include <Arduino.h>
#include <Servo.h>
#include <HX711_ADC.h>


const int ESC_PIN = PB0; 
const int HX711_dout = PA1; 
const int HX711_sck = PA0;  
HX711_ADC LoadCell(HX711_dout, HX711_sck);

int pwm = 1500;


Servo myservo;  

//unsigned long currentTime = millis();
//unsigned long lastPWMtime = 0;


/*unsigned long lastUpdate = 0; 
const unsigned long pwm_interval = 2000;  // PWM değişim aralığı */

unsigned long lastOutput = 0; 
const unsigned long loadcell_interval = 100; // Seri çıktı aralığı 


float calibrationValue = 69.6; 

void setup() {
  Serial.begin(9600);
  
  pinMode(PC13, OUTPUT); 
  myservo.attach(ESC_PIN);  


  LoadCell.begin();
  LoadCell.setSamplesInUse(1);

  

  unsigned long stabilizingtime = 2000;
  bool _tare = true;
  
  LoadCell.start(stabilizingtime, _tare);
  
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("timeout hatası");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); 
    
  }
}

void loop() {
  if (Serial.available() > 0) {
    int pwm = Serial.parseInt();}
    
  if (millis() - lastOutput >= loadcell_interval) { //100 ms de bir loadcell verisi alıyoruz 
    
    myservo.writeMicroseconds(pwm);

    LoadCell.update(); 

    float loadCellVal = LoadCell.getData();
    Serial.println(loadCellVal);

    digitalToggle(PC13); 
    lastOutput = millis();

    /*char direction;
    if (pwm > 1500) {
        direction = '+';
    } else if (pwm < 1500) {
        direction = '-'; 
    } else {
        direction = '0'; 
    }*/
    

  }
  
 }
  







