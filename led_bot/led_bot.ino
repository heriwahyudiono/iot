#include <ESP8266WiFi.h>  
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "";    
const char* password = ""; 

#define BOT_TOKEN ""  

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

const int ledPin = D1; 
bool ledState = false;
bool isBlinking = false;  

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

  client.setInsecure();  
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
  }

  if (isBlinking) {
    digitalWrite(ledPin, HIGH);  
    delay(500);                  
    digitalWrite(ledPin, LOW);   
    delay(500);                  
  }
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    Serial.print("Message received: ");
    Serial.println(text);

    if (text == "/start") {
      String welcomeMessage = "*Selamat Datang di Bot Kontrol LED!*\n\n"  
                              "Gunakan perintah berikut:\n"
                              "/on - Menghidupkan LED\n"
                              "/off - Mematikan LED\n"
                              "/blink - Menghidupkan lampu kelap-kelip";
      bot.sendMessage(chat_id, welcomeMessage, "Markdown");  
    } else if (text == "/on") {
      digitalWrite(ledPin, HIGH);
      ledState = true;
      isBlinking = false;  
      bot.sendMessage(chat_id, "LED dihidupkan!", "");
    } else if (text == "/off") {
      digitalWrite(ledPin, LOW);
      ledState = false;
      isBlinking = false;  
      bot.sendMessage(chat_id, "LED dimatikan!", "");
    } else if (text == "/blink") {
      isBlinking = true;  
      bot.sendMessage(chat_id, "Lampu kelap-kelip diaktifkan!", "");
    } else {
      bot.sendMessage(chat_id, "Perintah tidak dikenali. Gunakan /start untuk melihat daftar perintah.", "");
    }
  }
}
