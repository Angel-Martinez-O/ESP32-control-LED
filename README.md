Práctica de laboratorio: Control inalámbrico de un LED con ESP32

CORPORACIÓN UNIVERSITARIA AMERICANA  
Asignatura: Fundamentos de Mecatrónica

Estudiantes: Angel Martinez, Andres Bruges, Jesus Aguilar

1. Introducción

En esta práctica desarrollamos un sistema de control inalámbrico utilizando un ESP32, el cual nos permitió encender y apagar un LED desde un teléfono celular mediante una conexión Wi-Fi.

El ESP32 funciona como un punto de acceso y servidor web, permitiendo que el celular se conecte directamente al dispositivo sin necesidad de utilizar internet. Desde una página web podemos enviar órdenes para controlar el estado del LED.

2. Objetivo general

Implementar un sistema de control inalámbrico mediante un ESP32, utilizando una conexión Wi-Fi y una interfaz web para encender y apagar un LED.

3. Materiales utilizados

- 1 ESP32 (DevKit / WROOM-32).
- LED rojo.
- Resistencia de 220 ohmios.
- Protoboard.
- Cables Dupont.
- Cable USB de datos.
- Computador con Arduino IDE.
- Teléfono celular.

4. Diagrama de conexiones

El LED se conecta en serie con la resistencia limitadora entre el GPIO18 y GND:

**GPIO18 del ESP32 --- [220 ohmios] --- LED --- GND del ESP32**

Ánodo (pata larga) hacia el GPIO18 a través de la resistencia; cátodo (pata corta) hacia GND.

Corriente estimada: (3,3 V − 2,0 V) / 220 Ω ≈ 5,9 mA, un valor seguro para el LED y para el GPIO.

## 5. Procedimiento realizado

- Realizamos el montaje del ESP32 en la protoboard.
- Conectamos el LED, la resistencia y los cables correspondientes.
- Conectamos el ESP32 al computador mediante un cable USB.
- Utilizamos Arduino IDE para programar el microcontrolador.
- Configuramos el ESP32 para crear su propia red Wi-Fi.
- Conectamos el teléfono celular a la red creada por el ESP32.
- Ingresamos desde el navegador a la interfaz de control.
- Realizamos pruebas para comprobar el funcionamiento del sistema mediante los botones de la página web.

6. Funcionamiento del proyecto

El funcionamiento consiste en enviar órdenes desde el teléfono celular hacia el ESP32 mediante una conexión Wi-Fi.

Cuando presionamos el botón de la página, el navegador envía una solicitud HTTP al ESP32, que cambia el estado de su salida digital para encender o apagar el LED. De esta manera podemos controlar un componente electrónico sin manipular directamente el circuito.

Flujo de control: celular → Wi-Fi creada por el ESP32 → solicitud HTTP → función C++ → GPIO18 → resistencia → LED → GND.

## 7. Conceptos aprendidos

- ESP32: microcontrolador que nos permite controlar componentes electrónicos y conectarnos a redes Wi-Fi.
- Wi-Fi: tecnología que permite la comunicación inalámbrica entre dispositivos.
- GPIO: pines que permiten recibir o enviar señales digitales.
- LED: componente electrónico que emite luz cuando circula corriente en el sentido adecuado.
- Servidor web: programa que muestra una página desde la cual podemos enviar instrucciones al ESP32.
- HTTP: protocolo utilizado para enviar solicitudes entre el navegador y el servidor.

## 8. Respuestas a las preguntas de la guía

¿Por qué funciona sin internet?

Porque el ESP32 crea su propia red Wi-Fi y el teléfono se conecta directamente a ella. La comunicación se realiza dentro de la red local (IP 192.168.4.1), sin router ni proveedor de internet.

¿Qué hace la función handleClient()?

Revisa si hay solicitudes pendientes enviadas desde el navegador del celular y ejecuta la función asociada a cada ruta (por ejemplo, mostrarPagina() o alternarLED()). Se llama continuamente dentro de loop(); sin ella, el botón de la página no tendría efecto.

¿Qué riesgo hay en conectar el LED sin resistencia?

Sin resistencia nada limita la corriente: puede circular demasiada por el LED y por el pin del ESP32, quemando el LED y pudiendo dañar el GPIO o el microcontrolador.

¿Qué diferencia existe entre el estado físico y la variable ledEncendido?

El estado físico es si realmente circula corriente y el LED está iluminado; la variable ledEncendido solo guarda en el programa lo que este cree que ocurre. Normalmente coinciden, pero pueden diferir ante un error de cableado, un pin equivocado, un LED invertido o dañado: la página diría ENCENDIDO y el LED seguiría apagado.

 9. Resultados obtenidos

Realizamos el montaje del circuito utilizando el ESP32 y una protoboard, y utilizamos un teléfono celular para acceder a la interfaz de control inalámbrico.

La evidencia fotográfica corresponde a la versión base del programa: se observa el circuito conectado al computador y el teléfono mostrando la interfaz «Control de LED por WiFi» con el estado ENCENDIDO y el LED iluminado. Esta fotografía documenta el montaje y la interfaz de control; los retos 1, 2 y 3 se desarrollaron a nivel de código y no están registrados en ella.

10. Desarrollo de los tres retos de programación

10.1. Reto 1. Identidad propia de la red y personalización

Objetivo: personalizar el nombre de la red Wi-Fi, modificar el título de la página y cambiar el GPIO23 por GPIO18.

Solución:modificamos el nombre de la red para identificar nuestro equipo y cambiamos el pin que controla el LED. El nombre de la red quedó como ESP32_LED_G01 y el pin configurado fue GPIO18. También modificamos el título de la página web para identificar al Grupo 01.

Explicación: cambiamos el GPIO23 por GPIO18 en el programa. Para que funcione, el cable del LED también debe pasarse físicamente a GPIO18; en la fotografía de evidencia el montaje corresponde a la versión base. No bastaba con modificar solo el software porque PIN_LED únicamente indica al programa qué pin manejar; el LED está conectado físicamente a un pin concreto. Si el cable seguía en GPIO23 mientras el código escribía en GPIO18, el LED nunca recibiría la señal. Personalizamos el nombre de la red para identificar nuestro equipo entre varios grupos.

10.2. Reto 2. Un solo botón para alternar

Objetivo: reemplazar los botones Encender y Apagar por un único botón llamado Cambiar estado.

Solución: utilizamos la variable booleana ledEncendido para conocer el estado actual del LED y una nueva ruta POST /alternar.

Explicación: cada vez que presionamos el botón, el programa invierte el estado del LED: si está apagado se enciende y si está encendido se apaga. El operador ! invierte el valor de una variable booleana.

10.3. Reto 3. Autoapagado sin bloquear el servidor

Objetivo: hacer que el LED se apague automáticamente después de 10 segundos, sin detener el funcionamiento del servidor web.

Solución: utilizamos millis() para medir el tiempo transcurrido desde que se encendió el LED.

Explicación: al encender el LED se registra el tiempo inicial con millis(). Después de 10 segundos el programa apaga el LED automáticamente. Si volvemos a encenderlo después de apagarlo, se guarda una nueva marca temporal y el plazo comienza de nuevo. Utilizamos millis() en lugar de delay(10000) porque así el servidor sigue atendiendo las solicitudes del celular durante esos 10 segundos.

11. Pruebas de funcionamiento

| Prueba | Acción | Resultado esperado | Resultado obtenido |
|---|---|---|---|
| T1 | Conectar el celular a la red | Aparece la red personalizada | El celular se conectó a la red creada por el ESP32 (versión base). |
| T2 | Abrir la página web | Aparece el botón Cambiar estado | La página «Control de LED por WiFi» cargó en el celular (versión base, con dos botones). |
| T3 | Primera pulsación | LED encendido | El LED se encendió y la página mostró ENCENDIDO. |
| T4 | Segunda pulsación | LED apagado | No realizada con la versión final. |
| T5 | Tercera pulsación | LED encendido | No realizada con la versión final. |
| T6 | Esperar 10 segundos | LED se apaga automáticamente | No realizada con la versión final. |
| T7 | Encender nuevamente | Comienza un nuevo conteo de 10 segundos | No realizada con la versión final. |

Las pruebas T1 a T3 las realizamos con la versión base del programa. Las pruebas T4 a T7 corresponden a los retos 2 y 3, que desarrollamos en el código, pero no verificamos físicamente.

 12. Explicación de los conceptos del reto 3

¿Para qué sirve millis()?

Permite conocer el tiempo transcurrido desde que se inició el ESP32, sin detener el funcionamiento del programa.

¿Qué es una marca temporal?

Es el valor del tiempo que guardamos cuando ocurre un evento; en este caso, cuando se enciende el LED. Restando esa marca al valor actual de millis() sabemos cuánto tiempo ha pasado.

¿Cuál es la diferencia entre código bloqueante y no bloqueante?**

Código bloqueante:** detiene la ejecución de otras tareas mientras espera, como delay(10000); durante ese tiempo handleClient() no se ejecutaría y la página no respondería.

Código no bloqueante:** permite seguir ejecutando otras instrucciones mientras se controla el tiempo, como ocurre con millis().

 13. Evidencia fotográfica

Evidencia.

<img width="1026" height="441" alt="image" src="https://github.com/user-attachments/assets/c9be1724-d053-42b9-abff-c20f69157884" />

Fotografía del ESP32 conectado a la protoboard y al computador, junto con el teléfono celular mostrando la interfaz de control de LED por Wi-Fi (versión base del programa).

14. Conclusión

Con esta práctica aprendimos cómo utilizar un ESP32 para controlar componentes electrónicos mediante una conexión inalámbrica. También comprendimos cómo funciona una red Wi-Fi creada por el propio microcontrolador y cómo podemos enviar instrucciones desde una página web.

Con el desarrollo de los tres retos trabajamos en la personalización de la red Wi-Fi, la implementación de un único botón para cambiar el estado del LED y el apagado automático después de 10 segundos. La verificación física la realizamos con la versión base del programa, mientras que los retos quedaron implementados en el código para su posterior prueba.

Esta actividad nos permitió relacionar la programación con la electrónica y comprender cómo se pueden desarrollar sistemas de control remoto utilizando tecnologías como Wi-Fi y HTTP. Además, aprendimos la importancia de utilizar millis() para controlar el tiempo sin bloquear el servidor web.
