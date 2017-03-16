#include <VirtualWire.h> //for RF communication
#include <SPI.h> //for Digital Potentiometer

//Iniitializes message which will store data recieved from trasmitter
byte message[VW_MAX_MESSAGE_LEN];
//messageLength is to assist in recieving data
byte messageLength = VW_MAX_MESSAGE_LEN;

const int ledPin = 4; //an LED is used to confirm that a signal has been recieved
const int recPin = 7; //data line from RF reciever
const int potPin = 10; //CS port on the digital Potentiometer
const int speakerPin = 8;

//Integer volume represents the steps (out of 256) of the digital potentiometer
int volume = 10;

void setup()
{
  Serial.begin(9600);

  //RF Reciever setup, from virtualwire library
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_rx_pin(recPin);
  vw_rx_start();
  
  pinMode(ledPin, OUTPUT);
  
  pinMode(potPin, OUTPUT);
  SPI.begin(); //begin transmitting data to the digital potentiometer
}


void loop()
{
  tone(speakerPin, 1000); //begin outputing a tone from the speaker
  
  if (vw_get_message(message, &messageLength)) //the function "vw_get_message()" verifies a message has been recieved
                                               // and stores the message in a variable (from virtualwire library)
  {
    for (int i = 0; i < messageLength; i++)//printing data recieved from RF transission
    {
      Serial.print(message[i]); //print the message
      Serial.print(" ");
      if(message[i] == 2) {
        volume = volume + 10;
        volume = correctVolume(volume); //see correctVolume below
        digitalPotWrite(1, volume); //see digitalPotWrite below
        blink();
      }
      if(message[i] == 3) {
        volume = volume - 10;
        volume = correctVolume(volume);
        digitalPotWrite(1, volume);
        blink();
        blink();
      }
    }
    Serial.println("Found"); //"Found" prints if a message was recieved
  }
  Serial.println("Loop\n"); //"Loop" prints after a cycle of void loop() is completed
  delay(500);
}

//blink(): a blink method
void blink()
{
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}

/* digitalPotWrite: sets the CS pin of the digital potentiometer high to send in data
 * the first value sent is a two bit address that specifies which channel of the DP to talk with,
 * and the following value is the "position" of the wiper that variates the resistence between
 * ports A and B of the specified channel
 * Borrowed from: https://www.arduino.cc/en/Tutorial/DigitalPotControl
 */
void digitalPotWrite(int address, int value) {
  digitalWrite(potPin, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(potPin, HIGH);
}

/*the correctVolume method makes sure the volume is not adjusted beyond the bounds
 * of the digital potentiometer (0 - 256)
 */
int correctVolume(int vol) {
  int volume = vol;
  while(volume >= 255) {
    volume--;
  }
  while(volume <= 1) {
    volume++;
  }
  return volume;
}

