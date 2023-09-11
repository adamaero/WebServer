/*

  Questions

  https://forum.arduino.cc/t/how-to-create-minimal-web-server-example-with-a-dropdown-menu/1167272

  https://arduino.stackexchange.com/questions/94323/how-to-create-minimal-web-server-example-with-a-dropdown-menu






  Based from
  https://randomnerdtutorials.com/esp8266-web-server

  HTML tool
  https://www.w3schools.com/html/tryit.asp?filename=tryhtml_elem_select
*/

#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
//String output5State = "off";
//String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 14;    // D5 verizon
const int output6 = 12;    // D6 tmobile
const int output7 = 13;    // D7 atnt

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output6, OUTPUT);
  pinMode(output7, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output6, LOW);
  digitalWrite(output7, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();







            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<title>Phone carrier selection</title>");

            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // HERE
            // ↓
            // ↓
            // ↓
            // ↓

            //const char page_before_provider[] = R"rawstr(<!DOCTYPE html>
            //<p>Provider Selected: )rawstr";
            //const char page_after_provider[] = "</p>\n</body>\n</html>\n";

            // At the place where the page has to be sent.
            client.print(page_before_provider);
            if (provider == "verizon")
              client.print("Verizon");
            else if (provider == "tmobile")
              client.print("T-Mobile");
            else if (provider == "atnt")
              client.print("AT&amp;T");
            else
              client.print(provider);
            client.print(page_after_provider);



            // Web Page Heading
            client.println("<form action='/?.php'>");
            client.println("  <select id='carrier' name='carrier'>");
            client.println("    <option value='verizon'>Verizon</option>");
            client.println("    <option value='tmobile'>T-Mobile</option>");
            client.println("    <option value='atnt'>AT&amp;T</option>");
            client.println("  </select>");
            client.println("  <input type='submit'>");
            client.println("</form>");

            if (carrier == verizon)
              client.println("<p>carrier Selected: Verizon</p>");
            else if (carrier == tmobile)
              client.println("<p>carrier Selected: T-Mobile" < / p > ");
                             else if (carrier==atnt)
                             client.println("<p>carrier Selected: AT & T"</p>");

            // ↑
            // ↑
            // ↑
            // ↑

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
