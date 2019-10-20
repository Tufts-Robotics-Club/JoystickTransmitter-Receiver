#include <AFMotor.h>

/*
  nRF24L01+ Joystick Receiver Demo
  nrf24l01-joy-rcv-demo.ino
  nRF24L01+ Receiver with Joystick Decode
  Use with Joystick Transmitter Demo
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
 
// Include dependant SPI Library 
#include <SPI.h>
 
// Define Joystick Connections
#define JoyStick_X_PIN     A0 
#define JoyStick_Y_PIN     A1
#define JoyStick1_X_PIN     A2
#define JoyStick1_Y_PIN     A3
 
// Define addresses for radio channels
#define CLIENT_ADDRESS 1   
#define SERVER_ADDRESS 2
#define CLIENT_ADDRESS1 3   
#define SERVER_ADDRESS1 4

 
// Create an instance of the radio driver
RH_NRF24 RadioDriver;
RH_NRF24 RadioDriver1;
 
// Sets the radio driver to NRF24 and the server address to 2
RHReliableDatagram RadioManager(RadioDriver, SERVER_ADDRESS);
RHReliableDatagram RadioManager1(RadioDriver1, SERVER_ADDRESS1);
 
// Define a message to return if values received
uint8_t ReturnMessage[] = "JoyStick Data Received"; 
 
// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

//DC Motors:
//extern HardwareSerial Serial;
//
int BIN_1 = 3;
int BIN_2 = 5;
int AIN_1 = 6;
int AIN_2 = 9;
//int MAX_PWM_VOLTAGE = 240;
 
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
  
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init()){
    Serial.println("init failed");
  }

  //DC Motors:
  pinMode(BIN_1, OUTPUT);
  pinMode(BIN_2, OUTPUT);
  pinMode(AIN_1, OUTPUT);
  pinMode(AIN_2, OUTPUT);
} 
 
void loop()
{
  int x, y, x1, y1;
  if (RadioManager.available())
  {
 // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAck(buf, &len, &from))
 //Serial Print the values of joystick
    {
      x = buf[0];
      y = buf[1];
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": X = ");
      Serial.print(buf[0]);
      Serial.print(" Y = ");
      Serial.println(buf[1]);
 
      // Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }
    if (y > 140) {
      digitalWrite(AIN_2, LOW);
      digitalWrite(AIN_1, HIGH);
    }
    else if (y < 115) {
      digitalWrite(AIN_2, HIGH);
      digitalWrite(AIN_1, LOW);
    }
    else {
      digitalWrite(AIN_2, LOW);
      digitalWrite(AIN_1, LOW);
    }
  }

  if (RadioManager1.available())
  {
 // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager1.recvfromAck(buf, &len, &from))
 //Serial Print the values of joystick
    {
      x1 = buf[0];
      y1 = buf[1];
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": X = ");
      Serial.print(buf[0]);
      Serial.print(" Y = ");
      Serial.println(buf[1]);
 
      // Send a reply back to the originator client, check for error
      if (!RadioManager1.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }
    if (y1 > 140) {
      digitalWrite(BIN_2, LOW);
      analogWrite(BIN_1, buf[1]);
    }
    else if (y1 < 115) {
      digitalWrite(BIN_2, HIGH);
      digitalWrite(BIN_1, LOW);
    }
    }
    else {
      digitalWrite(BIN_2, LOW);
      digitalWrite(BIN_1, LOW);
    }
  }
}
