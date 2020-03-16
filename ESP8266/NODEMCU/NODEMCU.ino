#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* ssid  = "";
const char* password = "";
const String passwordGate = "123abc";
const String gateInfos = "[{\"id\": 1, \"name\": \"Portão 1\"},{\"id\": 2, \"name\": \"Portão 2\"},{\"id\": 3, \"name\": \"Portão 3\"},{\"id\": 4, \"name\": \"Portão 4\"},{\"id\": 5, \"name\": \"Portão 5\"},{\"id\": 6, \"name\": \"Portão 6\"},{\"id\": 7, \"name\": \"Portão 7\"},{\"id\": 8, \"name\": \"Portão 8\"}]";

IPAddress ip(192,168,0,90);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,0,1);

String indexFile;

ESP8266WebServer server(80);

const int builtInLed = 2; //D4
const int gate1 = 16;     //D0
const int gate2 = 5;      //D1
const int gate3 = 4;      //D2
const int gate4 = 0;      //D3
const int gate5 = 14;     //D5
const int gate6 = 12;     //D6
const int gate7 = 13;     //D7
const int gate8 = 15;     //D8

// ======================================== Inicialização do ESP

void readFiles(){
  SPIFFS.begin();

  if(SPIFFS.exists("/index.html")){
    File rIndex = SPIFFS.open("/index.html", "r");
    indexFile = rIndex.readString();
    rIndex.close();
  } else {
    indexFile = "Could not find index.html";
  } 

  SPIFFS.end();
}

void initializePins(){
  pinMode(builtInLed, OUTPUT);
  pinMode(gate1, OUTPUT);
  pinMode(gate2, OUTPUT);
  pinMode(gate3, OUTPUT);
  pinMode(gate4, OUTPUT);
  pinMode(gate5, OUTPUT);
  pinMode(gate6, OUTPUT);
  pinMode(gate7, OUTPUT);
  pinMode(gate8, OUTPUT);
  digitalWrite(builtInLed, HIGH);
  digitalWrite(gate1, HIGH);
  digitalWrite(gate2, HIGH);
  digitalWrite(gate3, HIGH);
  digitalWrite(gate4, HIGH);
  digitalWrite(gate5, HIGH);
  digitalWrite(gate6, HIGH);
  digitalWrite(gate7, HIGH);
  digitalWrite(gate8, HIGH);
}

void initializeWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(builtInLed, LOW);
    delay(250);
    digitalWrite(builtInLed, HIGH);
    delay(250);
  }
}

void initializeWebServer(){
  server.on("/", HTTP_GET, handleRoot);

  server.on("/gate", HTTP_GET, handleGateGET);

  server.on("/gate", HTTP_POST, handleGatePOST);

  server.onNotFound(handleNotFound);

  server.begin();
}

void sinalize(){
  digitalWrite(builtInLed, LOW);
}

// ======================================== Funções do servidor

void handleRoot(){
  server.send(200, "text/html", indexFile);
}

void handleGateGET(){
  server.send(200, "application/json", gateInfos);
}

void handleGatePOST(){

/*
  * 0 - Não autorizado
  * 1 - Portão não existe
  * 2 - Sinal feito
*/

  if (!isAuthenticated(server.arg("pw"))){
    server.send(401, "text/plain", "0");  // Não autenticado
    return;

  } else if (!gateExists(server.arg("gate").toInt())) {
    server.send(404, "text/plain", "1");  // Portão não existe
    return;

  } else {
    server.send(200, "text/plain", "2");  // Sinal feito
  }

  switch(server.arg("gate").toInt()){
    case 1:
      gateSignal(gate1);
      break;

    case 2:
      gateSignal(gate2);
      break;

    case 3:
      gateSignal(gate3);
      break;

    case 4:
      gateSignal(gate4);
      break;

    case 5:
      gateSignal(gate5);
      break;

    case 6:
      gateSignal(gate6);
      break;

    case 7:
      gateSignal(gate7);
      break;

    case 8:
      gateSignal(gate8);
      break;
  }
}

void handleNotFound(){
  server.send(404, "text/plain", "404");
}

// ======================================== Funções

bool isAuthenticated(String pw){
  if (pw == passwordGate){
    return true;
  } else {
    return false;
  }
}

bool gateExists(int gateID){
  if (gateID >= 1 && gateID <= 8){
    return true;
  } else {
    return false;
  }
}

void gateSignal(int gate){
  digitalWrite(gate, LOW);
  delay(200);
  digitalWrite(gate, HIGH);
}

// ======================================== Funções ESP

void setup() {
  readFiles();
  initializePins();
  initializeWiFi();
  initializeWebServer();
  sinalize();
}

void loop() {
  server.handleClient();
}
