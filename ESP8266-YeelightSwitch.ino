#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#define button 12        //Pin for the button (D6)
#define LED_BUILTIN 2    //LED_BUILTIN for the notificationLed
WiFiManager wifiManager;

const char *H0 = "192.168.0.50"; // Ip or Hostname from your yeelight

/*
 * If you want to make this please make sure to add a PULL-UP resistor 
 * between the pin 12(D6) and the 3.3v rail (then add the button 12(D6) to GND), 
 * so that your light does not go full stroboscopic mode :)
*/

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(button, INPUT);
  Serial.begin(115200);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("LightSwitch");
  Serial.println("Started!");
  digitalWrite(LED_BUILTIN, HIGH);
}

void SendInfo(const char *IpAddress)
{
  WiFiClient client;                                                       // Use WiFiClient class to create TCP connections
  String TcpMethod = "{\"id\":1,\"method\":\"toggle\",\"params\":[]}\r\n"; // Toggle method for the Yeelight more at https://www.yeelight.com/download/Yeelight_Inter-Operation_Spec.pdf
  int port = 55443;                                                        // Lights port
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Creating a TCP connection");
  if (!client.connect(IpAddress, port))
  {
    Serial.println("connection failed");
    digitalWrite(LED_BUILTIN, HIGH);
    client.stop();
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.println("Toggle: " + String(IpAddress));
    client.print(TcpMethod);
    client.stop();
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

// void OnDemandConfig()
// {
//   wifiManager.setConfigPortalTimeout(180);
//   wifiManager.startConfigPortal("LightSwitch");
// }

void loop()
{
  if (digitalRead(button) == LOW)
  {
    if (!wifiManager.autoConnect("LightSwitch"))
    {
      delay(3000);
      ESP.reset();
    }
    Serial.println("Button 12 fired");
    delay(500);
    SendInfo(H0);
  }
}
