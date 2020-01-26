#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

EEPROMClass  CHECKBIT("eeprom0", 1);
EEPROMClass  SSID1("eeprom1", 32 );
EEPROMClass  PASSWORD("eeprom2", 32);

String ssidt ;
String passwordt;

const char * ssid;
const char * password ;

void Set_ssid_pass()
{
    WiFi.softAP("esp32ap","123465798",6 );

    ssidt = //Serial.readString();
    SSID1.writeString (0,ssidt);
    SSID1.commit();

    
    passwordt =// Serial.readString ();
    PASSWORD.writeString (0,passwordt);
    PASSWORD.commit();

    CHECKBIT.writeBool(0,1);
    CHECKBIT.commit();
    WiFi.softAPdisconnect();    
}


void  connectToWifi ()
{
    ssidt = SSID1.readString(0);
    ssid = ssidt.c_str();
    passwordt = PASSWORD.readString (0);
    password = passwordt.c_str();
    for ( int i = 0 ; i < 10 ; i++ )
    {   
        WiFi.begin( ssid , password );
        if (WiFi.status() == WL_CONNECTED)
            break;
    }

}



void setup() 
{
    Serial.begin(115200);
    
    CHECKBIT.begin(1);
    SSID1.begin(32);
    PASSWORD.begin(32);

    if (CHECKBIT.readBool(0) == 0)
    {
        Set_ssid_pass();
    }
    connectToWifi();
}

void loop() 
{

   
}

