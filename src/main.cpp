#include<Arduino.h>
#include<ESP8266WiFi.h>

#define LED LED_BUILTIN
const char* ssid = "ssid";
const char* password = "password";
unsigned char status_led=0;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);                   
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());
}


void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (! client.available())
  {
    delay (1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if (req.indexOf("/ledoff") != -1) {
    status_led=0;
    digitalWrite(LED, LOW);
    Serial.println("LED OFF");
  }
  else if(req.indexOf("/ledon") != -1)
  {
    status_led=1;
    digitalWrite(LED,HIGH);
    Serial.println("LED ON");
  }

  client.println("<!DOCTYPE HTML>");
  client.println("<HTML>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, inital-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS to style the on/off buttons
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #FF0000; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #008000; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  // Web Page Heading
  client.println("<h1>LED CONTROL<h1>");
  // Display and ON/OFF buttons
  client.println("<p><a href=\"/ledon\"><button class=\"button\">ON</button></a></p>");
  client.println("<p><a href=\"/ledoff\"><button class=\"button2\">");
  client.println("</body></html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}