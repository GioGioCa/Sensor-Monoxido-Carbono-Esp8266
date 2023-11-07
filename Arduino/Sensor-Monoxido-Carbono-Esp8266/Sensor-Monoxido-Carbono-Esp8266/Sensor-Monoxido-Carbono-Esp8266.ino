#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
//Para este proyecto se implemento una solucion basada en Blynk
//Creditos a colaboradores:
//Angel Javier Delgado Ibarra
//Giovanni Gomez
//Christian Rogriguez
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define ventana_open D1
#define alarma D2
#define ventana_cerrar D3
#define sensor A0
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

bool notificacionEnviada = false; 
BlynkTimer timer;

void sendSensor(){
 int nivelGas = analogRead(sensor);
 Blynk.virtualWrite(V0, nivelGas);

  // Leer el valor del sensor y tomar acciones en función de ese valor
  if(nivelGas > 500 && !notificacionEnviada){
    abrirVentana();
    Blynk.logEvent("YOUR_BLYNK_LOG_EVENT",String("Se detecto una fuga de gas!!! " ) +nivelGas + String(" PPM"));
    // Activacion de indicador de notificacion, previene desperdicio de notificaciones
    notificacionEnviada = true;  
  } else {
    cerrarVentana();
    notificacionEnviada = false;
  }

}

void abrirVentana(){
  digitalWrite(alarma, HIGH);
  digitalWrite(ventana_open, HIGH);
  digitalWrite(ventana_cerrar, LOW);
}

void cerrarVentana(){
  delay(10000);
  digitalWrite(alarma, LOW);
  digitalWrite(ventana_open, LOW);
  digitalWrite(ventana_cerrar, HIGH);
}

void setup(){
  pinMode(sensor, INPUT);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  timer.setInterval(2500L, sendSensor);
  pinMode(alarma, OUTPUT);
  pinMode(ventana_open, OUTPUT);
  pinMode(ventana_cerrar, OUTPUT);
  pinMode(sensor, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión WiFi establecida");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.SSID());
}

void loop(){
  Blynk.run();
  timer.run();
}
