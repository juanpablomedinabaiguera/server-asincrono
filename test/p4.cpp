#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//variables
const char* ssid = "CESJT";
const char* password = "itisjtsmg";

String textoEstado1;
String textoEstado2;
String textoEstado3;

String color1;
String color2;
String color3;

const int p1 = 33;
const int p2 = 32;
const int p3 = 35;

// Servidor Asíncrono 
AsyncWebServer server(80);

// HTML
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="refresh" content="1">

    <title>Monitor de entradas digitales</title>
    <style>
        body {
            background-color: #f0f0f0;
        }
        h1 {
            font-family: Consolas, monospace;
            text-align: center;
        }
        ul {
            list-style-type: none;
            padding: 0;
        }
        .linea_de_estado {
            width: 300px;
            margin: 10px auto;
            padding: 15px;
            border: 2px solid black;
        }
        .a {
            background-color: __COLOR_A__;
        }
        .b {
            background-color: __COLOR_B__;
        }
        .c {
            background-color: __COLOR_C__;
        }
    </style>
</head>
<body>
    <h1>MONITOR DE ENTRADAS DIGITALES</h1>
    <ul>
        <li class="linea_de_estado a">
            ENTRADA 1: __TEXTO_ESTADO_1__
        </li>
        <li class="linea_de_estado b">
            ENTRADA 2: __TEXTO_ESTADO_2__
        </li>
        <li class="linea_de_estado c">
            ENTRADA 3: __TEXTO_ESTADO_3__
        </li>
    </ul>
</body>
</html>
)rawliteral";
//setup
void setup() {
  Serial.begin(115200);
  //declaro pines
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  pinMode(p3, INPUT);
  
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
    String pagina = pagina_template;
    pagina.replace("__TEXTO_ESTADO_1__", textoEstado1);
    pagina.replace("__TEXTO_ESTADO_2__", textoEstado2);
    pagina.replace("__TEXTO_ESTADO_3__", textoEstado3);
    pagina.replace("__COLOR_A__", color1);
    pagina.replace("__COLOR_B__", color2);
    pagina.replace("__COLOR_C__", color3);
    request->send(200, "text/html", pagina);
  });
  // Iniciar servidor
  server.begin();
}

void loop() {
    if(digitalRead(p1) == 0){
        textoEstado1 = "SUELTO";
        color1 = "#e74c3c";
    } else {
        textoEstado1 = "PRESIONADO";
        color1 = "#2ecc71";
    }

    if(digitalRead(p2) == 0){
        textoEstado2 = "SUELTO";
        color2 = "#e74c3c";
    } else {
        textoEstado2 = "PRESIONADO";
        color2 = "#2ecc71";
    }

    if(digitalRead(p3) == 0){
        textoEstado3 = "SUELTO";
        color3 = "#e74c3c";
    } else {
        textoEstado3 = "PRESIONADO";
        color3 = "#2ecc71";
    }
}