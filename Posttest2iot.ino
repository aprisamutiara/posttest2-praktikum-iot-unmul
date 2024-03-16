#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const int BUZZER_PIN = D2;
const int LED_PIN = D3;
const int BUTTON_TAMBAH = D5;
const int BUTTON_KURANG = D6;
const int BUTTON_ED = D7;

const char* ssid = "vivo 2019";    
const char* password = "ikan2121";

ESP8266WebServer server(80);    
String webpage;

// Define variables
int suaraBuzzer = 100; 
int kecerahanLampu = 0; 
int tambah = 0; 
int kurang = 0; 
int isEd = 0; 
bool nyala = false;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_TAMBAH, INPUT_PULLUP);
  pinMode(BUTTON_KURANG, INPUT_PULLUP);
  pinMode(BUTTON_ED, INPUT_PULLUP);

  // Connect ke Hotspot
  Serial.println();
  Serial.print("Mengkonfigurasi akses point...");

   // Mengatur WiFi 
  WiFi.mode(WIFI_STA);                     
  WiFi.begin(ssid, password);              
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("....");
  }

  // Print status Connect 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    String html = "<html><head><meta http-equiv='refresh' content='5'></head><body><h1>Mengatur kecerahan lampu dan Tone atau nada Buzzer</h1>";
    html += "<p>LED : " + String(kecerahanLampu) + "</p>";
    html += "<p>Buzzer : " + String(suaraBuzzer) + "</p>";
    html += "<p>Status : " + String(nyala ? "Enable" : "Disable") + "</p></body></html>";
    server.send(200, "text/html", html);
  });
  server.begin();  // Memulai web server
}

void loop() {

  tambah = digitalRead(BUTTON_TAMBAH); 
  kurang = digitalRead(BUTTON_KURANG);
  isEd = digitalRead(BUTTON_ED); 

  if (isEd == LOW) {
    nyala = !nyala;
    delay(500);
  } 

  if (!nyala) {
    analogWrite(LED_PIN, 0);
    noTone (BUZZER_PIN); 
  } else {
    analogWrite(LED_PIN, kecerahanLampu);
    tone (BUZZER_PIN, suaraBuzzer); 
  }

  if (tambah == LOW){
    if (nyala){
      if (kecerahanLampu < 249 && suaraBuzzer < 2500){
        kecerahanLampu += 3;
        suaraBuzzer += 10;
      }
      refresh();
      delay (500);
      analogWrite(LED_PIN, kecerahanLampu);
      tone (BUZZER_PIN, suaraBuzzer);
    }
  }

  if (kurang == LOW){
    if (nyala){
      if (kecerahanLampu > 0 && suaraBuzzer > 100){
        kecerahanLampu -= 3;
        suaraBuzzer -= 10;
      }
      refresh();
      delay(500);
      analogWrite(LED_PIN, kecerahanLampu);
      tone (BUZZER_PIN, suaraBuzzer);
    }  
  }
  server.handleClient(); 
}

void refresh() {
  String html = "<html><head><meta http-equiv='refresh' content='1'></head><body><h1>Mengatur kecerahan lampu dan Tone atau nada Buzzer</h1>";
  html += "<p>LED : " + String(kecerahanLampu) + "</p>";
  html += "<p>Buzzer : " + String(suaraBuzzer) + "</p>";
  html += "<p>Status : " + String(nyala ? "Enable" : "Disable") + "</p></body></html>";
  server.send(200, "text/html", html);
}
