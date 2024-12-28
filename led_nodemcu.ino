#include <ESP8266WiFi.h>

const char* ssid = "Galaxy A05";    
const char* password = "12345678";  

WiFiServer server(80);  
const int ledPin = D1;  

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  

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

  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
  } else if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(ledPin, LOW);
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
  client.println("<a href=\"/LED=ON\">Hidupkan LED</a>");
  client.println("<a href=\"/LED=OFF\">Matikan LED</a>");
  client.println("</body>");
  client.println("</html>");
}
