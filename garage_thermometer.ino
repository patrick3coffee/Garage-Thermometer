#include <DHT.h>

int latchPin = 5;
int clockPin = 4;
int dataPin = 3;
int pinDHT = 2;
DHT dht(pinDHT, 22,1);

byte numbers[10];

void setup() {
  numbers[0] = 0b00000011;
  numbers[1] = 0b10011111;
  numbers[2] = 0b00100101;
  numbers[3] = 0b00001101;
  numbers[4] = 0b10011001;
  numbers[5] = 0b01001001;
  numbers[6] = 0b01000001;
  numbers[7] = 0b00011111;
  numbers[8] = 0b00000001;
  numbers[9] = 0b00001001;
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  showTemp();
  showHumidity();  
}

void showTemp(){
  float temperature = dht.readTemperature(true,true);
  if(temperature == NULL){printError();}
  byte leader;
  if( temperature > 99 ){
    int one = temperature / 100;
    leader = numbers[one]; 
  }
  else{
    leader = 0b11111111; 
  }
  printNumber(temperature, leader);
  delay(3000);
}

void showHumidity(){
  float humidity = dht.readHumidity(true);
  if(humidity == NULL){printError();}
  printNumber((int)humidity, 0b10010001);
  delay(3000);
}

void printNumber(int number, byte leader){
   digitalWrite(latchPin, LOW);
  int two = (number / 10) % 10;
  int three = number % 10;
  shiftOut(dataPin, clockPin, LSBFIRST, leader); 
  shiftOut(dataPin, clockPin, LSBFIRST, numbers[two]); 
  shiftOut(dataPin, clockPin, LSBFIRST, numbers[three]);  
  digitalWrite(latchPin, HIGH);
}

void printError(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, 0b11111101); 
  shiftOut(dataPin, clockPin, LSBFIRST, 0b11111101); 
  shiftOut(dataPin, clockPin, LSBFIRST, 0b11111101); 
  digitalWrite(latchPin, HIGH);
  return;
}

