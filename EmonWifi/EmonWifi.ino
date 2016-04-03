#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include "EmonLib.h"             // Include Emon Library

EnergyMonitor emon1;             // Create an instance
const int ANALOG_PIN = A0; // The only analog pin on the Thing

void setup()
{
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");

  emon1.current(A0, 29.1);       // Current: input pin, calibration.
}

const char* host = "192.168.1.20";
const int port = 3000;

void loop()
{

  delay(1000);

  long vcc = emon1.readVcc();
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only

  WiFiClient client;
  if (!client.connect(host, port))
  {
   return;
  }

  String url = "/puissance/" + String(Irms * 1.414235 * 235 / 10 );
  //- See more at: http://www.esp8266.com/viewtopic.php?f=29&t=3375#sthash.AgUTirw2.dpuf;
  String request =  "GET " +
                     url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n";

  client.print(request);

  delay(1000);

  while(client.available()){
   String line = client.readStringUntil('\r');
   //Serial.print(line); // Trying to avoid using serial
  }

}
