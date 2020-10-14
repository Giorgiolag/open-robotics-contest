
 
#include <SPI.h> 
#include "SX1272.h"
#include "Arduino.h"
#include "my_temp_sensor_code.h"
#define ETSI_EUROPE_REGULATION
#define BAND433
#define PABOOST
#define node_addr 5
#define LORAMODE  1
uint8_t message[50];

#define PRINTLN                   Serial.println("")
#define PRINT_CSTSTR(fmt,param)   Serial.print(F(param))
#define PRINT_STR(fmt,param)      Serial.print(param)
#define PRINT_VALUE(fmt,param)    Serial.print(param)
#define FLUSHOUTPUT               Serial.flush();

#define MAX_DBM 14
const uint32_t DEFAULT_CHANNEL=CH_00_433;
#define DEFAULT_DEST_ADDR 1
unsigned long nextTransmissionTime=0L;
int loraMode=LORAMODE;
long previous_transmission_time = 0;
char nomenclature_str[4]="TC";

// -------------- variables and constants added by RAIN SPOTTERS -------------

const int RAIN_INT=3;
int my_total = 0;
int flag;
int pin3_current_value;
int pin3_previous_value = 0;

//--------------------------------------------------------------------------
char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 *a++ = '.';
 long desimal = abs((long)((f - heiltal) * p[precision]));
 if (desimal < p[precision-1]) {
  *a++ = '0';
 } 
 itoa(desimal, a, 10);
 return ret;
}
//---------------------------------------------------------------------------------

void setup()
{
  int e;
  delay(3000);
  Serial.begin(38400);    
  sx1272.ON();
  e = sx1272.setMode(loraMode);
  PRINT_CSTSTR("%s","Setting Mode: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  
  e = sx1272.setChannel(DEFAULT_CHANNEL);  
  PRINT_CSTSTR("%s","Setting Channel: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  // enable carrier sense
  sx1272._enableCarrierSense=true;
  
 
  sx1272._needPABOOST=true; 

  e = sx1272.setPowerDBM((uint8_t)MAX_DBM);
  PRINT_CSTSTR("%s","Setting Power: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  
  // Set the node address and print the result
  e = sx1272.setNodeAddress(node_addr);
  PRINT_CSTSTR("%s","Setting node addr: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  // Print a success message
  PRINT_CSTSTR("%s","SX1272 successfully configured\n");

  delay(500);
 
}

/*****************************
 _                       
| |                      
| |     ___   ___  _ __  
| |    / _ \ / _ \| '_ \ 
| |___| (_) | (_) | |_) |
\_____/\___/ \___/| .__/ 
                  | |    
                  |_|    
*****************************/

void loop(void)
{
  
  uint8_t app_key_offset=0;

  int e;
  float temp;
  pin3_current_value = digitalRead(RAIN_INT);
  if (pin3_current_value ==1 and pin3_previous_value == 0)
  {
    my_total+=1;
    Serial.println(my_total);
  }
      
      
  if (millis()-previous_transmission_time>60000)
  {
      previous_transmission_time = millis();
      uint8_t r_size;
      char float_str[10];
      ftoa(float_str,my_total,2);
      r_size=sprintf((char*)message+app_key_offset,"\\!%s/%s",nomenclature_str,float_str);

      PRINT_CSTSTR("%s","Sending ");
      PRINT_STR("%s",(char*)(message+app_key_offset));
      PRINTLN;
      
      PRINT_CSTSTR("%s","Real payload size is ");
      PRINT_VALUE("%d", r_size);
      PRINTLN;
      
      int pl=r_size+app_key_offset;
      sx1272.CarrierSense();
      sx1272.setPacketType(PKT_TYPE_DATA);
      
      // Send message to the gateway and print the result
      // with the app key if this feature is enabled

      e = sx1272.sendPacketTimeout(DEFAULT_DEST_ADDR, message, pl);

      PRINT_CSTSTR("%s","Packet sent, state ");
      PRINT_VALUE("%d", e);
      PRINTLN;
      my_total = 0;
    
  }   
  pin3_previous_value = pin3_current_value;
}
