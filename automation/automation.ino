#include <WiFi.h>



// Replace with your network credentials
// to create its own wifi network
const char* ssid = "your-WIFI-username";
const char* password = "password";

// Assign output variables to GPIO pins
// GPIO Setting
static uint8_t output12 = 23;

WiFiServer server(80);

String header;
String output12State = "off";

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output12, OUTPUT);

  // Set outputs to high because we are using active low type relay module
  digitalWrite(output12, LOW);

  connectWiFi();
  while ((!(WiFi.status() == WL_CONNECTED))) {
    connectWiFi();
  }

  server.begin();
}

void loop() {
  while ((!(WiFi.status() == WL_CONNECTED))) {
    connectWiFi();
  }

  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {                     // If a new client connects,
    Serial.println("New Client.");  // print a message out in the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            //for GPIO12
            if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("RELAY 1 on");
              output12State = "on";
              digitalWrite(output12, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("RELAY 1 off");
              output12State = "off";
              digitalWrite(output12, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>SAND smart home</h1>");

            // Display current state, and ON/OFF buttons for RELAY 1
            client.println("<p>RELAY 1 - State " + output12State + "</p>");
            // If the output12State is off, it displays the ON button
            if (output12State == "off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {  // if you got a newline, then clear currentLine
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

void connectWiFi() {
  pinMode(2, OUTPUT);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to wifi......");

  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED))) {
    digitalWrite(2, HIGH);
    delay(300);
    digitalWrite(2, LOW);
    delay(200);

    Serial.println(" - ");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("ESP32 Local IP is : ");
  Serial.print((WiFi.localIP()));


  digitalWrite(2, HIGH);
  delay(400);
  digitalWrite(2, LOW);
}
