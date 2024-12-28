#include <ESP8266WiFi.h>

const char* ssid = "";    
const char* password = "";  

WiFiServer server(80);  

const int led1Pin = D1;  
const int led2Pin = D2;  

void setup() {
  Serial.begin(115200);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/LED1=ON") != -1) {
    digitalWrite(led1Pin, HIGH);
    delay(1000); 
    digitalWrite(led1Pin, LOW);
  } else if (request.indexOf("/LED1=OFF") != -1) {
    digitalWrite(led1Pin, LOW);
  }

  if (request.indexOf("/LED2=ON") != -1) {
    digitalWrite(led2Pin, HIGH);
    delay(1000); 
    digitalWrite(led2Pin, LOW);
  } else if (request.indexOf("/LED2=OFF") != -1) {
    digitalWrite(led2Pin, LOW);
  }

  if (request.indexOf("/LEDALL=ON") != -1) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
  } else if (request.indexOf("/LEDALL=OFF") != -1) {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("body { font-family: Arial; text-align: center; margin-top: 50px; }");
  client.println("a { text-decoration: none; display: inline-block; margin: 10px; padding: 10px 20px; color: white; background-color: blue; border-radius: 5px; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>Kontrol LED</h1>");
  client.println("<a href=\"/LED1=ON\">Hidupkan LED 1</a>");
  client.println("<a href=\"/LED1=OFF\">Matikan LED 1</a>");
  client.println("<br>");
  client.println("<a href=\"/LED2=ON\">Hidupkan LED 2</a>");
  client.println("<a href=\"/LED2=OFF\">Matikan LED 2</a>");
  client.println("<br>");
  client.println("<a href=\"/LEDALL=ON\">Hidupkan Kedua LED</a>");
  client.println("<a href=\"/LEDALL=OFF\">Matikan Kedua LED</a>");
  client.println("</body>");
  client.println("</html>");
}
