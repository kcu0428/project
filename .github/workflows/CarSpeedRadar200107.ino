#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>

const int sensor_light = 2;
const int led_backlight = 3;
SoftwareSerial mySerial(4, 5); // RX, TX

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xD2, 0x20 }; // RESERVED MAC ADDRESS
IPAddress ip(192, 168, 30, 220);
EthernetClient client;
IPAddress server(192,168,30,224);
String data;


int Car_Speed = 0;

void setup()
{
	pinMode(sensor_light, INPUT);
	pinMode(led_backlight, OUTPUT);
    digitalWrite(led_backlight, LOW);
	
  Serial.begin(38400);

   // while (!Serial) {
   // ; // wait for serial port to connect. Needed for native USB port only
   // }
   // Serial.println("Serial Monitoring......");
  Ethernet.begin(mac,ip);
  mySerial.begin(38400);
  mySerial.setTimeout(20);
 
  


}


void loop(){
  
  char temp[300];
  int ave_speed = 0;
  Car_Speed = 0;
  int Sum_speed = 0;
  int read_Count = 0;
  if(mySerial.available())
  {
    byte leng = mySerial.readBytes(temp, 300);

    if(temp[0]== 0x02 && temp[leng - 1] == 0x03)
    {
    read_Count = leng/14;
    for(int b = 0; b < read_Count; b++) {
      for(int i = 0; i < 14; i++) {
     //   Serial.print(i);
     //   Serial.print(" : ");
         if( i == 4){ Sum_speed += int(temp[i]); }
     //   Serial.println(temp[i],HEX);
      }
     //  Serial.print("Stream Packet Conunt: ");
    //   Serial.println(b);
    }
    ave_speed = Sum_speed/read_Count;
    Car_Speed = ave_speed;
   // Serial.print("Input data Lenght : ");
  //  Serial.println(leng);
   // Serial.print("Input data SUM : ");
    //Serial.println(Sum_speed);
   // Serial.print("Read Copunt : ");
   // Serial.println(read_Count);
   // Serial.print("Ave Speed  : ");
   // Serial.println(ave_speed);
  }
  else if( leng > 56 && temp[0] == 0x02)
  {
  //Serial.println("Data Byte Currupt");
   for(int b = 0; b < 4; b++) {
    for(int i = 0; i < 14; i++){
     // Serial.print(i);
     // Serial.print(" : ");
       if( i == 4){
      Sum_speed += int(temp[i]);
     
      //Serial.print(temp[i],HEX);
     // Serial.print("  ");
     
         }
     }
    ave_speed = Sum_speed/4;
    Car_Speed = ave_speed;
    //Serial.print("Input data Lenght : ");
   // Serial.println(leng);
    //Serial.print("Input data SUM : ");
    //Serial.println(Sum_speed);
    //Serial.print("Ave Speed  : ");
   // Serial.println(ave_speed);
      }
  }
  else	
  {
  //Serial.println("DFDFDFDFDFDFDFDFDF");
  // Serial.print("Input data Lenght : ");
   // Serial.println(leng);
    for(int b = 0; b < leng/14; b++){
     for(int i = 0; i < 14; i++){
    //  Serial.print(i);
    //  Serial.print(" : ");
      if( i == 4){ Sum_speed += int(temp[i]);}
     
     // Serial.println(temp[i],HEX);
      }
      return;
      }
    }
    //delay(2000);
    ledtrans();
 }
  if(digitalRead(sensor_light)) {
   // Serial.println("light sensor on ..");
  digitalWrite(led_backlight,HIGH);
  
  delay(100); // Allow hardware to stabilize
  }
  else
  digitalWrite(led_backlight,LOW);
}

void ledtrans()
{
 
 
  data = "![000/C2����ӵ�?;
 
   if(Car_Speed>=50){ data = data + "/C1" + Car_Speed  + "!]";}
  if(Car_Speed<50) { data = data + "/C2" + Car_Speed  + "!]";}
 if(Car_Speed > 0 ){
       if (client.connect(server,5000)) { // REPLACE WITH YOUR SERVER ADDRESS
    
       // Serial.println("Server Ready:   "); 
    client.print( data); 
       } 
       if (client.connected()) { 
          client.stop();  // DISCONNECT FROM THE SERVER
      }
       //delay(2000); 
  }
  else 
    {
      data = "![000/C2���Ѽӵ�50Km!]";
       if (client.connect(server,5000)) { // REPLACE WITH YOUR SERVER ADDRESS
    
       // Serial.println("Server Ready:   "); 
    client.print( data); 
       } 
       if (client.connected()) { 
          client.stop();  // DISCONNECT FROM THE SERVER
      }
       //delay(1000); 
  }
}
