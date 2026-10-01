#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";

String textoBoton;

const int ventilador = 23;
const int apagado = 0;
const int bajo = 80;
const int media = 175;
const int alta = 255;

volatile int velocidad;

// Servidor Asíncrono 
AsyncWebServer server(80);

// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Control ventilador</title>
  <style>
    body {  background-color: #ecf0f1;
            text-align: center;
            padding-top: 50px;
        }
    .titulo {   font-family: 'Arial Black';
                font-weight: bold;
                color: #2c3e50; 
            }
    .boton {    width: 180px;
                height: 60px;
                border: 1px solid black;
                border-radius: 15px;
                display: block;
                margin: 10px auto;
                cursor: pointer;
        }
    .A {
            background-color: #c0392b;
        }
    .B {
            background-color: #2980b9;
        }
    .M {
            background-color: #27ae60;
        }
    .H {
            background-color: #f39c12;
        }
  </style>
</head>
<body>
  <h1 class='titulo'>CONTROL DE VENTILADOR</h1>
  <p>Velocidad Actual: <span><p>TEXTO_VELOCIDAD</p></span></p>

    <a href='/off'><button class="boton A">Apagado</button></a>
    <a href='/low'><button class="boton B">Baja</button></a>
    <a href='/mesium'><button class="boton M">Media</button></a>
    <a href='/high'><button class="boton H">Alta</button></a>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(ventilador, OUTPUT);

  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Conexion wifi
 int timeout = 20; 
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    //parte para cambiar lo que dice el boton una vez que se prende o apaga
    pagina.replace("__TEXTO_VELOCIDAD__", textoBoton);
    request->send(200, "text/html", pagina);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {        //apagado
    digitalWrite(ventilador,LOW);
    textoBoton = "APAGADO";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/low", HTTP_GET, [](AsyncWebServerRequest *request) {        //lento
    analogWrite(ventilador,bajo);
    textoBoton = "BAJA";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/mesium", HTTP_GET, [](AsyncWebServerRequest *request) {     //maomeno
    analogWrite(ventilador,media);
    textoBoton = "MEDIA";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/high", HTTP_GET, [](AsyncWebServerRequest *request) {       //rapido
    analogWrite(ventilador,alta);
    textoBoton = "ALTA";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  // Iniciar servidor
  server.begin();
}

void loop() {}