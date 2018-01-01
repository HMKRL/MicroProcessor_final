#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  Serial1.begin(9600);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Relay ready!");
}

void loop() {}

// callback for received data
void receiveData(int byteCount) {
  Serial.print("Data received: ");
  while (Wire.available()) {
    number = Wire.read();
    Serial.write(number);
    Serial1.write(number);
  }
  Serial.println("");
}

// callback for sending data
void sendData() {
  Wire.write(number);
}
