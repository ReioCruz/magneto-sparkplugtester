#include <SoftwareSerial.h>

const int IR_PIN = 5;
const int BT_RX_PIN = 10;  
const int BT_TX_PIN = 11;

float rev = 0;
int rpm;
unsigned long oldtime = 0;
unsigned long time;

SoftwareSerial bluetooth(BT_RX_PIN, BT_TX_PIN);

bool rpmCalculated = false;

void isr() {
  rev++;
}

void setup() {
  pinMode(IR_PIN, INPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), isr, RISING); 
}

void loop() {
  int colorValue = digitalRead(IR_PIN); 
  
  if (colorValue == LOW && !rpmCalculated) {
    // Black color detected
    time = millis() - oldtime; 
    if (time > 0) {
      rpm = (rev / time) * 60000 * 3;
    } else {
      rpm = 0;
    }
    Serial.println(rpm);
    bluetooth.println(rpm);
    rpmCalculated = true;
    oldtime = millis(); 
    rev = 0; 
    delay(1000);
  } else if (colorValue == HIGH && rpmCalculated) {
    
    rpmCalculated = false;
  }
}
