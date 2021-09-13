//this program for difficulty level choosing  
// include libraries for RFID and SPI protocol

#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9   // reset pin for RFID
#define SS_PIN          10  // slave  select pin
String value = " ";   // string varible to read  and store rfid tag data
// initializing the RFID pins and key for communcation
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

unsigned long timer = 0;
unsigned long maxTimer = 50; // Max time the button will stay on after pressing

//led pins
int ledpin1 = 3;
int ledpin2 = 5;
int ledpin3 = 6;

//switch pins
int pushbutton1 = 7;
int pushbutton2 = 4;
int pushbutton3 = 2;

// to hold button state 
int button1 = 0;
int button2 = 0;
int button3 = 0;

// to hold button and timer statuses 
bool isDiffucltySelected = false;
bool canStartTimer = false;
int flag = 1;
bool b1 = false, b2 = false, b3 = false;

void setup()
{
  Serial.begin(9600);  // uart initialization with 9600 baudrate 
  // defining led as OUTPUT
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  pinMode(ledpin3, OUTPUT);

//Defining Switches as INPUT
  pinMode(pushbutton1, INPUT);
  pinMode(pushbutton2, INPUT);
  pinMode(pushbutton3, INPUT);

// turn onm the LEDs For the first time
  digitalWrite(ledpin1, HIGH);
  digitalWrite(ledpin2, HIGH);
  digitalWrite(ledpin3, HIGH);

// initialize spi and rfid header and key 
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
}

void loop()
{
  byte block = 0;
  byte len = 0;

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    Serial.print("0");
    mfrc522.PCD_Init();// re initialize the rfid header file 
  }
  else
  {
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    byte buffer1[18];

    block = 4;
    len = 18;
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
    if (status != MFRC522::STATUS_OK) {
      return;
    }

    status = mfrc522.MIFARE_Read(block, buffer1, &len);
    if (status != MFRC522::STATUS_OK) {
      return;
    }

    //PRINT FIRST NAME check this logic guys make it print at end
    for (byte i = 0; i < 3; i++)
    {
      mfrc522.PCD_Init();
      value += (char)buffer1[i];
      buffer1[i] = {0};
    }

    buffer1[18] = {0};  // resetting the buffer 

  }
  Serial.print(value);  //send rfid tag data
  Serial.print(",");
  value = ""; // clear the tag data
 // read the all button status 
  button1 = digitalRead(pushbutton1);
  button2 = digitalRead(pushbutton2);
  button3 = digitalRead(pushbutton3);

  if (b1 == true)
  {
    Serial.print("1");
    Serial.print(",");
  }
  else
  {
    Serial.print("0");
    Serial.print(",");
  }
  if (b2 == true)
  {
    Serial.print("1");
    Serial.print(",");
  }
  else
  {
    Serial.print("0");
    Serial.print(",");
  }
  if (b3 == true)
  {
    Serial.println("1");
  }
  else
  {
    Serial.println("0");

  }

  // check  the button status 
  if (!isDiffucltySelected)
  {

//if one of the button status is HIGH turn on the Particular LED and Turn of all other leds and also make canstatrt timer is true
    button1 = digitalRead(pushbutton1);
    button2 = digitalRead(pushbutton2);
    button3 = digitalRead(pushbutton3);

    if (button1 == HIGH)  
    {
      digitalWrite(ledpin3, LOW);
      digitalWrite(ledpin2, LOW);
      digitalWrite(ledpin1, HIGH);
      isDiffucltySelected = true;
      canStartTimer = true;
      b1 = true;
    }

    if (button2 == HIGH)
    {
      digitalWrite(ledpin3, LOW);
      digitalWrite(ledpin1, LOW);
      digitalWrite(ledpin2, HIGH);
      isDiffucltySelected = true;
      canStartTimer = true;
      b2 = true;
    }

    if (button3 == HIGH)
    {
      digitalWrite(ledpin1, LOW);
      digitalWrite(ledpin2, LOW);
      digitalWrite(ledpin3, HIGH);
      isDiffucltySelected = true;
      canStartTimer = true;
      b3 = true;
    }

  }

// if timer is true check the timer with maxtimer 
  if (canStartTimer)
  {
    timer++;
  
    if (timer > maxTimer)   // if it reaches to the max timer turn ON all the leds and make canstart timer is false
    {
      digitalWrite(ledpin1, HIGH);
      digitalWrite(ledpin2, HIGH);
      digitalWrite(ledpin3, HIGH);
      canStartTimer = false;
      isDiffucltySelected = false;
      timer = 0;
      b1 = false;
      b2 = false;
      b3 = false;
    }
  }

  Serial.flush();  // clear the transmitted data
}
