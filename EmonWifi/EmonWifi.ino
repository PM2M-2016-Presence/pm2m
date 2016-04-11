#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <stdlib.h>
#include "EmonLib.h"             // Include Emon Library

EnergyMonitor emon1;             // Create an instance
const int ANALOG_PIN = A0; // The only analog pin on the Thing

const char host[] = "ec2-52-29-210-27.eu-central-1.compute.amazonaws.com";
const int port = 80;
const char nodename[] = "sct013";
const char apiKey[] = "&apikey=7af3b1772fa1b624ef73925ba117d562";

//const char* ssid     = "Livebox-3294";
//const char* password = "E9D45C9A35245222FDE9DCD973";

const char* ssid     = "Xperia Z1_b5d9";
const char* password = "dadoudidou";

  // Use WiFiClient class to create TCP connections
WiFiClient client;


void setup()
{
  Serial.begin(115200);
  delay(10);
  

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  emon1.current(A0, 29.1);       // Current: input pin, calibration.
}

void loop()
{
  Serial.print("waiting a delay\n");
  delay(5000);

  Serial.print("connecting to ");
  Serial.println(host);

  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  Serial.println("begin loop");

  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  const String url =  "/input/post.json?node=" + String(nodename) + "&json={power:" + String(Irms * 1.414 * 235 / 10) + "}" + String(apiKey);
  client.print("GET " + url + " HTTP/1.1\r\n" +
               "Host:" + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.print("Requesting URL: ");
  Serial.println(url);

  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}
