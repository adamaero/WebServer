/* ***********************************************************************************************

https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/README.rst

https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS/examples/mDNS_Web_Server/mDNS_Web_Server.ino

   This sketch is an ESP8266 test of a WiFi interface

   An mDNS server is started to handle the dropdown.local URL

   A WiFi server is started to provide a user interface via a web browser
   URL handlers are provided to
      Root page
      Unknown URL

   NOTES:
      I am not a web programmer. The web interface is a WEB101 level. If you think you can
      do better you may be right.

   WARNING:
     This is proof of concept code - not for prime time.
     Error handling is sparse
*********************************************************************************************** */


#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
//  #include <WebServer.h>
//  #include <ESP32mDNS.h>  
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
#endif
#include <WiFiClient.h>



const char* ssid = "Wiki:EffectiveAltruism";
const char* password = "123454321";
const char* station_name = "dropdown"; // station name url will be dropdown.local

ESP8266WebServer server(80);


// Creates drop-down menu
void handleRoot() 
{

  if (server.hasArg("carrier")) // variable for user to select
  {
    // Handle return parameter
    Serial.print("Carrier is ");
    Serial.println(server.arg("carrier"));                    //  get request argument value
  }
  // Build html page
  String htmlPage =
    String("<!DOCTYPE HTML>\n")
    + "<html>\n"
    + "<head>\n"
    +    "<title>Dropdown Demo</title>\n"
    + "</head>\n"
    + "<body>\n\n"
    + "<h1 style='text-align: center;'>Drop Down Demo</h1>\n"
    + " <form>\n"
    + "    <label for='carriers'>Choose a carrier:</label>\n"
    + "    <select name='carrier' id='carriers'>\n"
    + "       <option value='Verizon'>Verizon</option>\n"
    + "       <option value='T-mobile'>T-Mobile</option>\n"
    + "       <option value='AT&T'>AT&T</option>\n"
    + "    </select>\n"
    + "    <input type='submit' value='Submit'>\n"
    + "  </form>\n"
    + "</body>\n" +
    "</html>\n";
  server.send(200, "text/html", htmlPage);
}



void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();                                        // Uniform Resource Identifier (e.g., URL)
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  
  server.send(404, "text/plain", message);
}




void setup() 
{
  Serial.begin(9600);

  // Set up WiFi network
  Serial.println();
  Serial.print("Creating Station ");
  Serial.println(station_name);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS responder:
  if (!MDNS.begin(station_name)) 
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
      delay(1000);
  }
  Serial.println("mDNS responder started");

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

}


void loop() 
{
  MDNS.update();          
  server.handleClient();  

}
