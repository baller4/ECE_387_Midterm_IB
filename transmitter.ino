#include <VirtualWire.h> //for RF communication

int transPin = 7;
int buttonPinVU = 2;
int buttonPinVD = 4;

int buttonOutVU = 0; //stores the output of the increase volume button (VU: volume up)
int buttonOutVD = 0; //stores the output of the decrease volume button (VD: volume down)

boolean volumeUp = false;
boolean volumeDown = false;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(buttonPinVU, INPUT);
  pinMode(buttonPinVD, INPUT);

  //RF Transmitter setup, from virtualwire library
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_tx_pin(transPin);
  analogReference(DEFAULT);//optional,not needed
}


void loop() 
{
  
  int volumeUpCode[] = {2}; //signal that sends to increase volume
  int volumeDownCode[] = {3}; //signal that sends to decrease volume
  
  buttonOutVU = digitalRead(buttonPinVU); //reads the value of the increase volume button and stores it
  buttonOutVD = digitalRead(buttonPinVD); //reads the value of the decrease volume button and stores it

  if(buttonOutVU == HIGH) { //if the volume up button is pressed, sets volumeUp true
    volumeUp = true;
  }
  if(buttonOutVD == HIGH) { //if the volume down button is pressed, sets volumeDown true
    volumeDown = true;
  }

  if(volumeUp) { //if volume up button was pressed
    vw_send((uint8_t*)volumeUpCode, 1); //sending data through RF transimission (virtualwire function)
    vw_wait_tx(); //waits until the message is sent (virtualwire function)
    Serial.println("Sent"); //print "Sent" when the message has been sent
    volumeUp = false; //reset the volumeUp boolean for the next loop
    delay(500);
  }
  
  if(volumeDown) {
    vw_send((uint8_t*)volumeDownCode, 1); //sending data through RF transimission (virtualwire function)
    vw_wait_tx(); //wait until the message is sent
    Serial.println("Sent"); //prints "Sent" to confirm
    volumeDown = false; //reset boolean value for next loop
    delay(500);
  }
}


