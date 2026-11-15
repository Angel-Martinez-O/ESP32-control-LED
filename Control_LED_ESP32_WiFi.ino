 #include <WiFi.h>
#include <WebServer.h>

// Configuracion de la red Wi-Fi
const char* NOMBRE_RED = "ESP32_LED_G01";
const char* CLAVE_RED = "Mecatronica2026";

// Pin del LED
const int PIN_LED = 18;

// Servidor web
WebServer servidor(80);

// Estado del LED
bool ledEncendido = false;

// Control del tiempo
unsigned long tiempoEncendido = 0;

// Mostrar pagina web
void mostrarPagina() {

  String pagina = R"HTML(
  <!DOCTYPE html>
  <html lang="es">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="refresh" content="1">
    <title>Control LED - Grupo 01</title>
  </head>

  <body style="text-align:center; font-family:Arial;">

    <h1>Control de LED por Wi-Fi</h1>
    <h2>Grupo 01</h2>

    <p>Estado actual: <strong>%ESTADO%</strong></p>

    <form action="/alternar" method="POST">
      <button type="submit">
        Cambiar estado
      </button>
    </form>

  </body>
  </html>
  )HTML";

  pagina.replace("%ESTADO%",
    ledEncendido ? "ENCENDIDO" : "APAGADO");

  servidor.send(200, "text/html", pagina);
}

// Cambiar estado del LED
void alternarLED() {

  ledEncendido = !ledEncendido;

  if (ledEncendido) {

    digitalWrite(PIN_LED, HIGH);
    tiempoEncendido = millis();

    Serial.println("LED ENCENDIDO");

  } else {

    digitalWrite(PIN_LED, LOW);

    Serial.println("LED APAGADO");
  }

  servidor.sendHeader("Location", "/");
  servidor.send(303, "text/plain", "");
}

// Apagado automatico despues de 10 segundos
void verificarTiempo() {

  if (ledEncendido &&
      millis() - tiempoEncendido >= 10000UL) {

    digitalWrite(PIN_LED, LOW);
    ledEncendido = false;

    Serial.println("LED apagado automaticamente");
  }
}

// Configuracion inicial
void setup() {

  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  WiFi.mode(WIFI_AP);

  bool redCreada = WiFi.softAP(
    NOMBRE_RED,
    CLAVE_RED
  );

  if (!redCreada) {

    Serial.println("Error al crear la red Wi-Fi");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("Red Wi-Fi creada correctamente");
  Serial.println(NOMBRE_RED);
  Serial.println(WiFi.softAPIP());

  servidor.on("/", HTTP_GET, mostrarPagina);

  servidor.on("/alternar", HTTP_POST, alternarLED);

  servidor.begin();

  Serial.println("Servidor web iniciado");
}

// Ciclo principal
void loop() {

  servidor.handleClient();

  verificarTiempo();
}
