#include <ESP8266WiFi.h>

const char* ssid = "";    
const char* password = "";  

WiFiServer server(80);  

const int motor1Pin = D1; 
const int motor2Pin = D2;  

void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, LOW);

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

  if (request.indexOf("/MOTOR1=ON") != -1) {
    digitalWrite(motor1Pin, HIGH);
    delay(1000); 
    digitalWrite(motor1Pin, LOW);
  } 

  if (request.indexOf("/MOTOR2=ON") != -1) {
    digitalWrite(motor2Pin, HIGH);
    delay(1000); 
    digitalWrite(motor2Pin, LOW);
  } 

  if (request.indexOf("/ALLMOTORS=ON") != -1) {
    digitalWrite(motor1Pin, HIGH);
    digitalWrite(motor2Pin, HIGH);
  } else if (request.indexOf("/ALLMOTORS=OFF") != -1) {
    digitalWrite(motor1Pin, LOW);
    digitalWrite(motor2Pin, LOW);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("body { font-family: Arial; text-align: center; margin-top: 50px; }");
  client.println(".button { text-decoration: none; display: inline-block; margin: 10px; padding: 10px 20px; color: white; border-radius: 5px; }");
  client.println(".button-green { background-color: green; }");
  client.println(".button-red { background-color: red; }");
  client.println(".button-blue { background-color: blue; }");
  client.println(".container { display: flex; flex-direction: column; align-items: center; }");
  client.println(".row { display: flex; justify-content: space-between; width: 50%; margin: 10px 0; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>SMART CAR</h1>");
  client.println("<div class=\"container\">");
  client.println("<a href=\"/ALLMOTORS=ON\" class=\"button button-green\">MAJU</a>");
  client.println("<div class=\"row\">");
  client.println("<a href=\"/MOTOR2=ON\" class=\"button button-blue\">BELOK KIRI</a>");
  client.println("<a href=\"/MOTOR1=ON\" class=\"button button-blue\">BELOK KANAN</a>");
  client.println("</div>");
  client.println("<a href=\"/ALLMOTORS=OFF\" class=\"button button-red\">BERHENTI</a>");
  client.println("</div>");
  client.println("</body>");
  client.println("</html>");
}
