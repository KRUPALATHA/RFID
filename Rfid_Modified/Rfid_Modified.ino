#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9
#define SS_PIN          10
String value = " ";
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

unsigned long timer = 0;
unsigned long maxTimer = 50; // Max time the button will stay on after pressing


int ledpin1 = 3;
int ledpin2 = 5;
int ledpin3 = 6;


int pushbutton1 = 7;
int pushbutton2 = 4;
int pushbutton3 = 2;

int button1 = 0;
int button2 = 0;
int button3 = 0;

bool isDiffucltySelected = false;
bool canStartTimer = false;
int flag = 1;
bool b1 = false, b2 = false, b3 = false;

void setup()
{
  Serial.begin(9600);
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  pinMode(ledpin3, OUTPUT);

  pinMode(pushbutton1, INPUT);
  pinMode(pushbutton2, INPUT);
  pinMode(pushbutton3, INPUT);

  digitalWrite(ledpin1, HIGH);
  digitalWrite(ledpin2, HIGH);
  digitalWrite(ledpin3, HIGH);

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
    mfrc522.PCD_Init();

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
      buffer1[i] = 0;
    }

    buffer1[18] = 0;

  }
  Serial.print(value);
  Serial.print(",");


  value = "";
  button1 = digitalRead(pushbutton1);
  button2 = digitalRead(pushbutton2);
  button3 = digitalRead(pushbutton3);

  /* Serial.print( button1);
    Serial.print(",");
    Serial.print( button2);
    Serial.print( ",");
    Serial.println( button3);
  */


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
    //5Serial.print(",");
  }
  else
  {
    Serial.println("0");

  }
  if (!isDiffucltySelected)
  {

    button1 = digitalRead(pushbutton1);
    button2 = digitalRead(pushbutton2);
    button3 = digitalRead(pushbutton3);

    if (button1 == HIGH)
    {

      // Serial.println("Button 1 is pressed ");
      digitalWrite(ledpin3, LOW);
      digitalWrite(ledpin2, LOW);
      digitalWrite(ledpin1, HIGH);
      isDiffucltySelected = true;
      canStartTimer = true;
      b1 = true;

    }

    if (button2 == HIGH)
    {

      // Serial.println("Button 1 is pressed ");
      digitalWrite(ledpin3, LOW);
      digitalWrite(ledpin1, LOW);
      digitalWrite(ledpin2, HIGH);
      isDiffucltySelected = true;
      canStartTimer = true;
      b2 = true;
    }

    if (button3 == HIGH)
    {


      // Serial.println("Button 1 is pressed ");
      digitalWrite(ledpin1, LOW);
      digitalWrite(ledpin2, LOW);
      digitalWrite(ledpin3, HIGH);
      isDiffucltySelected = true;
      canStartTimer = true;
      b3 = true;
    }

  }

  if (canStartTimer)
  {
    timer++;
    //Serial.println(timer);
    if (timer > maxTimer)
    {
      // Serial.println("Time Reached turning all on now ");
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

  Serial.flush();
}
