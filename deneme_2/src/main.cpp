#include <Arduino.h>
#include <Servo.h>
#include <HX711_ADC.h>


const int ESC_PIN = PB0; 
const int HX711_dout = PA1; 
const int HX711_sck = PA0;  
HX711_ADC LoadCell(HX711_dout, HX711_sck);


Servo myservo;  
int pwm = 1500; 
int karar = 1;  


unsigned long lastUpdate = 0; 
const unsigned long pwm_interval = 2000;  // PWM değişim aralığı

unsigned long lastOutput = 0; 
const unsigned long loadcell_interval = 100; // Seri çıktı aralığı 


float calibrationValue = 69.6; 

void setup() {
  Serial.begin(9600);
  
  pinMode(PC13, OUTPUT); 
  myservo.attach(ESC_PIN);  


  LoadCell.begin();
  LoadCell.setSamplesInUse(4);

  unsigned long stabilizingtime = 2000;
  bool _tare = true;
  Serial.println("LoadCell hazırlanıyor...");
  LoadCell.start(stabilizingtime, _tare);
  
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("timeout hatası");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); 
    Serial.println("Zaman (ms) | PWM (us) | Yön | Load (g)");
  }
}

void loop() {
  
  if (millis() - lastOutput >= loadcell_interval) {
    lastOutput = millis();
    
    myservo.writeMicroseconds(pwm);

    LoadCell.update(); 
    

    float loadCellVal = LoadCell.getData();

    
    char direction;
    if (pwm > 1500) {
        direction = '+';
    } else if (pwm < 1500) {
        direction = '-'; 
    } else {
        direction = '0'; 
    }
    
    digitalToggle(PC13); 
    Serial.print(millis()); 
    Serial.print(" ms | ");
    Serial.print(pwm);
    Serial.print(" us | ");
    Serial.print(direction); 
    Serial.print(" | ");
    Serial.println(loadCellVal ); 
  }
  
  
  if(millis() - lastUpdate >= pwm_interval){
    lastUpdate = millis();
    
    pwm += 50 * karar;
    
    
    if(pwm >= 1900){
      karar = -1; 
    }
    
    
    if(pwm <= 1100){
      karar = 1; 
    }
    
    
  }
}






