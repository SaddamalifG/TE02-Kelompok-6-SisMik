
#include <Wire.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);
int fingerprintID = 00;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#include <SPI.h>
#include <RFID.h>
#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN,RST_PIN);

int serNum[5];          //Variable buffer Scan Card
int cards[10][5] = {{34, 186, 40, 52, 132 },    //SULTAN - 0
                    {85, 87, 209, 42, 248 },    //SADDAM - 1
                    {85, 87, 209, 42, 249 },    //FAZA - 2
                   };

bool access = false;
String Nama;
int count;

const int Relay_Kontak = 6;
const int Relay_Starter = 7;

const int ALARM = 2;

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);
  Wire.begin();
  SPI.begin();
  rfid.init();
  pinMode(Relay_Kontak, OUTPUT);
  pinMode(Relay_Starter, OUTPUT);
  pinMode(ALARM, OUTPUT);
  digitalWrite(Relay_Kontak, LOW);
  digitalWrite(Relay_Starter, LOW);
  digitalWrite(ALARM, LOW);
 
  if (finger.verifyPassword())
  {
    Serial.println("Sensor ditemukan");
    delay(500);
  }
  else
  {
    Serial.println("Sensor tidak ditemukan");
    delay(500);
    while (1)
    {
      delay(1);
    }
  }

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{
  fingerprintID = getFingerprintIDez();
  delay(50);
 
  if(fingerprintID == 1)
  {
    String IDname_1 = "SULTAN";
    digitalWrite(Relay_Starter, HIGH);
    delay(2500);
    digitalWrite(Relay_Starter, LOW);
    Serial.println(IDname_1);
    fingerprintID = 00;
  }  
  else if(fingerprintID == 2)
  {
    String IDname_2 = "SADDAM";
    digitalWrite(Relay_Starter, HIGH);  
    delay(2500);
    digitalWrite(Relay_Starter, LOW);
    Serial.println(IDname_2);
    fingerprintID = 00;        
  }
  else if(fingerprintID == 3)
  {
    String IDname_3 = "FAZA";
    digitalWrite(Relay_Starter, HIGH);  
    delay(2500);
    digitalWrite(Relay_Starter, LOW);
    Serial.println(IDname_3);
    fingerprintID = 00;        
  }
  if(rfid.isCard())
  {
    if(rfid.readCardSerial())
    {
      for(int x = 0; x < sizeof(cards); x++)
      {
        for(int i = 0; i < sizeof(rfid.serNum); i++ )
        {
          if(rfid.serNum[i] == cards[0][i])
          {
            access = true;
            Nama = "SULTAN";
          }
          else if(rfid.serNum[i] == cards[1][i])
          {
            access = true;
            Nama = "FAZA";
          }   
          else
          {
            access = false;
            break;
          }
        }
        if(access)break;
      }   
    }
    
    if(access)
    {
      count++;
      if(count==1)
      {
        Serial.println(Nama);
        digitalWrite(Relay_Kontak, HIGH);
      }
      if(count==2)
      {
        Serial.println("Motor Mati");
        digitalWrite(Relay_Kontak, LOW);
        count=0;
      }
    }
    else
    {
      Serial.println("Akses Ditolak");
      digitalWrite(ALARM, HIGH);
    }
    delay(1000);        
  }    
  rfid.halt();
}

int getFingerprintIDez()
{
  uint8_t x = finger.getImage();
  if (x != FINGERPRINT_OK)  return -1;
 
  x = finger.image2Tz();
  if (x != FINGERPRINT_OK)  return -1;
 
  x = finger.fingerFastSearch();
  if (x != FINGERPRINT_OK)  return -1;
 
  //menulis di komunikasi serial
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}
