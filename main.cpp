#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>


EEPROMClass  CHECKBIT("eeprom0", 1);
EEPROMClass  SSID1("eeprom1", 32 );
EEPROMClass  PASSWORD("eeprom2", 32);

String ssidt ;
String passwordt;

const char * ssid;
const char * password ;

WiFiServer server (80);

void getUspws (void)                // a function to creat a html bage and get the ssid and password through it 
{
    server.begin();                 
    bool flag1 = 0, flag2 = 0 , flag3  = 0  ; //flags 1 & 2 used to help geting  ssid and password   // flag3 the proccess of geting ssid and password is success   
    while (!flag3)
    {
        ssidt = "";
        passwordt = "";
        WiFiClient client = server.available();
        if (client)
        {
            String currentLine ="";
            while (client.connected())
            {
                if (client.available())
                {
                    char c = client.read();
                    Serial.write(c);        // just for debug
                    if (c == '\n')
                    {
                        if (currentLine.length() == 0 )         // html code 
                        {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();

                            client.print ("<form method=\"get\">");

                            client.print("<div class=\"container\">");
                            client.print("<label for=\"uname\"><b>Username</b></label>");
                            client.print("<input type=\"text\" placeholder=\"Enter Username\" name=\"uname\" required>");

                            client.print("<label for=\"psw\"><b>Password</b></label>");
                            client.print("<input type=\"text\" placeholder\"Enter Password\" name=\"psw\" required>");

                            client.print("<button type=\"submit\">Login</button>");
                            client.print("<label>");
                            client.print("</div>");
                            client.print("</form>");
   

                            client.println();
                            break;

                        }
                        else  { currentLine = "";}
                    
                    }
                    else if (c != '\r') { currentLine += c ; }
                
                    if (currentLine.endsWith ("GET /?uname="))      //searching for ssid 
                    {
                        flag1 = 1;
                        c = client.read ();
                        Serial.write(c);                    // just for debug
                    }
                    if (currentLine.endsWith("&psw="))    // searching for password 
                    {
                        flag1 = 0 ; 
                        flag2 = 1 ;
                        c = client.read();
                        Serial.write(c);
                    }
                
                    if (flag1) {    ssidt += c;}
                
                    if (flag2) {    passwordt += c;}

                    if (currentLine.endsWith(" HTTP/1.1") && ssidt != "")    // the condition to Exit the while loop 
                    {
                        flag1 = 0 ; flag2 = 0 ;  flag3 = 1  ;
                    }
                }
           }
            client.stop();
            ssidt.remove(ssidt.length()-4);             //delet the unneeded parts form ssid 
            passwordt.remove(passwordt.length ()-9);    //delet the unneeded parts form password

            Serial.println (ssidt);    // just for debug                 
            Serial.println (passwordt);// just for debug
        }
    }
}



void Set_ssid_pass()                            // a function to open APmode and save the ssid and password 
{
    WiFi.softAP("esp32ap","123456789",6,0,1 );   //choose channel 6 and show the ssid and only one device connected
    delay (2000);
    Serial.println ("ap is on ....");            //just for debug 
    getUspws();                                 // get ssid and password from the ESP server 
        
    SSID1.writeString (0,ssidt);                // save the ssid to eeprom 
    SSID1.commit();

    PASSWORD.writeString (0,passwordt);         // save the password to eeprom 
    PASSWORD.commit();

    CHECKBIT.writeBool(0,1);                    // raise the flag in the eeprom
    CHECKBIT.commit();

    WiFi.softAPdisconnect();    
}




void  connectToWifi ()
{
    int coun = 0;       // just for debug
    ssidt = SSID1.readString(0);
    ssid  = ssidt.c_str();
    passwordt = PASSWORD.readString (0);
    password = passwordt.c_str();
    WiFi.begin( ssid , password );
     
    while  (WiFi.status() != WL_CONNECTED)
    {
           delay (2000);
           Serial.print (".");          // just for debug
           coun ++;                     // just for debug
           if (coun == 10 )             // just for debug
           {
               CHECKBIT.write(0,0);     // just for debug
               CHECKBIT.commit();       // just for debug
               ESP.restart();           // just for debug
           }
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

    Serial.println ("hhhahahahahahhah");    // just for debug
    delay (5000);                           // just for debug
}

