Práctica de laboratorio: Control inalámbrico de un LED con ESP32

CORPORACIÓN UNIVERSITARIA AMERICANA
Asignatura: Fundamentos de Mecatrónica

Estudiante: 
Angel Martinez 
Andres Bruges 
Jesus Aguilar


1. Introducción

En esta práctica se desarrolló un sistema de control inalámbrico utilizando un ESP32, el cual permite encender y apagar un LED desde un teléfono celular mediante una conexión Wi-Fi.

El ESP32 funciona como un punto de acceso y servidor web, permitiendo que el celular se conecte directamente al dispositivo sin necesidad de utilizar internet. Desde una página web se pueden enviar órdenes para controlar el estado del LED.

2. Objetivo general

Implementar un sistema de control inalámbrico mediante un ESP32, utilizando una conexión Wi-Fi y una interfaz web para encender y apagar un LED.

3. Materiales utilizados
1 ESP32 (DevKit / WROOM-32).
LED rojo.
Resistencia de 220 ohmios.
Protoboard.
Cables Dupont.
Cable USB de datos.
Computador con Arduino IDE.
Teléfono celular.
4. Diagrama de conexiones

El LED se conecta en serie con la resistencia limitadora entre el GPIO18 y GND:

GPIO18 del ESP32 --- [ 220 ohmios ] --- |>| --- GND del ESP32
                                         LED
                              anodo (pata larga) -> catodo (pata corta)

Corriente estimada: (3,3 V − 2,0 V) / 220 Ω ≈ 5,9 mA, un valor seguro para el LED y para el GPIO.

5. Procedimiento realizado
Se realizó el montaje del ESP32 en la protoboard.
Se conectaron el LED, la resistencia y los cables correspondientes.
Se conectó el ESP32 al computador mediante un cable USB.
Se utilizó Arduino IDE para programar el microcontrolador.
Se configuró el ESP32 para crear su propia red Wi-Fi.
Se conectó el teléfono celular a la red creada por el ESP32.
Se ingresó desde el navegador a la interfaz de control 
Se realizaron pruebas para comprobar el funcionamiento del sistema mediante los botones de la página web.
6. Funcionamiento del proyecto

El funcionamiento consiste en enviar órdenes desde el teléfono celular hacia el ESP32 mediante una conexión Wi-Fi.

Cuando el usuario pulsa el botón de la página, el navegador envía una solicitud HTTP al ESP32, que cambia el estado de su salida digital para encender o apagar el LED. De esta manera se controla un componente electrónico sin manipular directamente el circuito.

Flujo de control: celular → Wi-Fi creada por el ESP32 → solicitud HTTP → función C++ → GPIO18 → resistencia → LED → GND.

7. Conceptos aprendidos
ESP32: microcontrolador que permite controlar componentes electrónicos y conectarse a redes Wi-Fi.
Wi-Fi: tecnología que permite la comunicación inalámbrica entre dispositivos.
GPIO: pines que permiten recibir o enviar señales digitales.
LED: componente electrónico que emite luz cuando circula corriente en el sentido adecuado.
Servidor web: programa que muestra una página desde la cual se pueden enviar instrucciones al ESP32.
HTTP: protocolo utilizado para enviar solicitudes entre el navegador y el servidor.
8. Respuestas a las preguntas de la guía

1. ¿Por qué funciona sin internet?
Porque el ESP32 crea su propia red Wi-Fi y el teléfono se conecta directamente a ella. La comunicación se realiza dentro de la red local (IP 192.168.4.1), sin router ni proveedor de internet.

2. ¿Qué hace la función handleClient()?
Revisa si hay solicitudes pendientes enviadas desde el navegador del celular y ejecuta la función asociada a cada ruta (por ejemplo, mostrarPagina() o alternarLED()). Se llama continuamente dentro de loop(); sin ella, el botón de la página no tendría efecto.

3. ¿Qué riesgo hay en conectar el LED sin resistencia?
Sin resistencia nada limita la corriente: puede circular demasiada por el LED y por el pin del ESP32, quemando el LED y pudiendo dañar el GPIO o el microcontrolador.

4. ¿Qué diferencia existe entre el estado físico y la variable ledEncendido?
El estado físico es si realmente circula corriente y el LED está iluminado; la variable ledEncendido solo guarda en el programa lo que este cree que ocurre. Normalmente coinciden, pero pueden diferir ante un error de cableado, un pin equivocado, un LED invertido o dañado: la página diría ENCENDIDO y el LED seguiría apagado.

9. Resultados obtenidos

Se realizó el montaje del circuito utilizando el ESP32 y una protoboard, y se usó un teléfono celular para acceder a la interfaz de control inalámbrico.

La evidencia fotográfica (sección 14) corresponde a la versión base del programa: se observa el circuito conectado al computador y el teléfono mostrando la interfaz «Control de LED por WiFi» con el estado ENCENDIDO y el LED iluminado. Esta fotografía documenta el montaje y la interfaz de control; los retos 1, 2 y 3 se desarrollaron a nivel de código (secciones 10 y 11) y no están registrados en ella.

10. Desarrollo de los tres retos de programación
10.1. Reto 1. Identidad propia de la red y personalización

Objetivo: personalizar el nombre de la red Wi-Fi, modificar el título de la página y cambiar el GPIO23 por GPIO18.

Solución: se modificó el nombre de la red para identificar el equipo y se cambió el pin que controla el LED.

cpp
const char* NOMBRE_RED = "ESP32_LED_G01";
const char* CLAVE_RED = "Mecatronica2026";

const int PIN_LED = 18;

También se modificó el título de la página web:

html
<title>Control LED - Grupo 01</title>
<h1>Control de LED por Wi-Fi</h1>
<h2>Grupo 01</h2>

Explicación: se cambió el GPIO23 por GPIO18 en el programa. Para que funcione, el cable del LED también debe pasarse físicamente a GPIO18; en la fotografía de evidencia el montaje corresponde a la versión base. No bastaba modificar solo el software porque PIN_LED únicamente indica al programa qué pin manejar; el LED está conectado físicamente a un pin concreto. Si el cable seguía en GPIO23 mientras el código escribía en GPIO18, el LED nunca recibiría la señal. El nombre de la red se personalizó para identificar el equipo entre varios grupos.

10.2. Reto 2. Un solo botón para alternar

Objetivo: reemplazar los botones Encender y Apagar por un único botón llamado Cambiar estado.

Solución: se utiliza la variable booleana ledEncendido para conocer el estado actual del LED y una nueva ruta POST /alternar.

cpp
void alternarLED() {

  ledEncendido = !ledEncendido;

  if (ledEncendido) {
    digitalWrite(PIN_LED, HIGH);
    tiempoEncendido = millis();
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  servidor.sendHeader("Location", "/");
  servidor.send(303, "text/plain", "");
}

Explicación: cada vez que se presiona el botón, el programa invierte el estado del LED: si está apagado se enciende y si está encendido se apaga. El operador ! invierte el valor de una variable booleana.

10.3. Reto 3. Autoapagado sin bloquear el servidor

Objetivo: hacer que el LED se apague automáticamente después de 10 segundos, sin detener el funcionamiento del servidor web.

Solución: se utiliza millis() para medir el tiempo transcurrido desde que se encendió el LED.

cpp
unsigned long tiempoEncendido = 0;

void verificarTiempo() {

  if (ledEncendido &&
      millis() - tiempoEncendido >= 10000UL) {

    digitalWrite(PIN_LED, LOW);
    ledEncendido = false;

    Serial.println("LED apagado automaticamente");
  }
}

En loop() se llama constantemente:

cpp
void loop() {

  servidor.handleClient();

  verificarTiempo();
}

Explicación: al encender el LED se registra el tiempo inicial con millis(). Después de 10 segundos el programa apaga el LED automáticamente. Si el usuario vuelve a encenderlo antes del vencimiento, se guarda una nueva marca temporal y el plazo se reinicia. Se usa millis() en lugar de delay(10000) porque así el servidor sigue atendiendo las solicitudes del celular durante esos 10 segundos.

11. Código completo con los tres retos integrados

Nombre del archivo: Control_LED_ESP32_WiFi.ino

cpp
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

Nota sobre el montaje: para ejecutar este programa, el LED debe estar conectado al GPIO18 mediante una resistencia de 220 ohmios, usando una placa ESP32 clásica compatible con ese pin.

12. Pruebas de funcionamiento
Prueba	Acción	Resultado esperado	Resultado obtenido
T1	Conectar el celular a la red	Aparece la red personalizada	El celular se conectó a la red creada por el ESP32 (versión base).
T2	Abrir la página web	Aparece el botón Cambiar estado	La página «Control de LED por WiFi» cargó en el celular (versión base, con dos botones).
T3	Primera pulsación	LED encendido	El LED se encendió y la página mostró ENCENDIDO.
T4	Segunda pulsación	LED apagado	No realizada con la versión final.
T5	Tercera pulsación	LED encendido	No realizada con la versión final.
T6	Esperar 10 segundos	LED se apaga automáticamente	No realizada con la versión final.
T7	Encender nuevamente	Comienza un nuevo conteo de 10 segundos	No realizada con la versión final.

Las pruebas T1 a T3 se hicieron con la versión base del programa. Las pruebas T4 a T7 corresponden a los retos 2 y 3, que se desarrollaron en el código pero no se verificaron físicamente.

13. Explicación de los conceptos del reto 3

¿Para qué sirve millis()?
Permite conocer el tiempo transcurrido desde que se inició el ESP32, sin detener el funcionamiento del programa.

¿Qué es una marca temporal?
Es el valor del tiempo que se guarda cuando ocurre un evento; en este caso, cuando se enciende el LED. Restando esa marca al valor actual de millis() se sabe cuánto tiempo ha pasado.

¿Cuál es la diferencia entre código bloqueante y no bloqueante?

Código bloqueante: detiene la ejecución de otras tareas mientras espera, como delay(10000); durante ese tiempo handleClient() no se ejecutaría y la página no respondería.
Código no bloqueante: permite seguir ejecutando otras instrucciones mientras se controla el tiempo, como ocurre con millis().
14. Evidencia fotográfica

Evidencia 1. Montaje del ESP32 y control inalámbrico del LED.

[PEGA AQUÍ LA FOTOGRAFÍA Y BORRA ESTA LÍNEA]

Fotografía del ESP32 conectado a la protoboard y al computador, junto con el teléfono celular mostrando la interfaz de control de LED por Wi-Fi (versión base del programa, con el LED encendido). No corresponde a la versión final con los tres retos.

15. Conclusión

Con esta práctica aprendí cómo utilizar un ESP32 para controlar componentes electrónicos mediante una conexión inalámbrica. También comprendí cómo funciona una red Wi-Fi creada por el propio microcontrolador y cómo se pueden enviar instrucciones desde una página web.

Con el desarrollo de los tres retos se trabajó en la personalización de la red Wi-Fi, la implementación de un único botón para cambiar el estado del LED y el apagado automático después de 10 segundos. La verificación física se realizó con la versión base del programa; los retos quedaron implementados en el código para su posterior prueba.

Esta actividad me permitió relacionar la programación con la electrónica y comprender cómo se pueden desarrollar sistemas de control remoto utilizando tecnologías como Wi-Fi y HTTP. Además, aprendí la importancia de utilizar millis() para controlar el tiempo sin bloquear el servidor web.
