#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//variables
const char* ssid = "CESJT";
const char* password = "itisjtsmg";

String modo;
String objetivo;
String estado;

const int l1 = 33;
const int l2 = 34;
const int l3 = 35;

volatile int config = 0;

// Servidor Asíncrono 
AsyncWebServer server(80);

// HTML
const char pagina_template[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Sistema de Refrigeración</title>
    <style>
        body {
            background-color: #ecf0f1;
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 0;
            padding: 30px;
        }
        h1 {
            font-family: "Arial Black", sans-serif;
            font-size: 34px;
            text-align: center;
            margin-bottom: 30px;
        }
        .panel-temp {
            width: 550px;
            padding: 30px;
            border-radius: 20px;
            box-sizing: border-box;
        }
        .contenedor-botones {
            display: flex;
            gap: 15px;
            margin-top: 25px;
        }
        .btn {
            padding: 15px 20px;
            font-size: 15px;
        }

        .btn-detenido {
            background-color: #7f8c8d;
            color: white;
        }

        .btn-ventilacion {
            background-color: #3498db;
            color: white;
        }

        .btn-intensivo {
            background-color: #154360;
            color: white;
        }
        .detenido {
            background-color: #bdc3c7;
            color: black;
        }

        .ventilacion {
            background-color: #85c1e9;
            color: black;
        }

        .intensivo {
            background-color: #154360;
            color: white;
        }
        .panel-temp {
                width: 100%;
                padding: 20px;
        }
    </style>
</head>
<body>
    <h1>SISTEMA DE REFRIGERACIÓN</h1>
    <div class="panel-temp detenido">
        <p>MODO ACTUAL: __TEXTO_MODO__</p>
        <p>TEMPERATURA OBJETIVO: __TEXTO_OBJETIVO</p>
        <p>ESTADO DEL VENTILADOR: __TEXTO_ESTADO__</p>
    </div>
    <div class="contenedor-botones">
        <a href="/detenido" class="btn btn-detenido">DETENIDO</a>
        <a href="/ventilacion" class="btn btn-ventilacion">VENTILACIÓN</a>
        <a href="/intensivo" class="btn btn-intensivo">ENFRIAMIENTO INTENSIVO</a>
    </div>
</body>
</html>

)rawliteral";
//setup
void setup() {
  Serial.begin(115200);
  //declaro pines
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);

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
    pagina.replace("__TEXTO_MODO__", modo);
    pagina.replace("__TEXTO_OBJETIVO__", objetivo);
    pagina.replace("__TEXTO_ESTADO__", estado);
    request->send(200, "text/html", pagina);
  });

  server.on("/detenido", HTTP_GET, [](AsyncWebServerRequest *request) {
    modo = "DETENIDO";
    objetivo = "--°C";
    estado = "APAGADO";
    config = 0;
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/ventilacion", HTTP_GET, [](AsyncWebServerRequest *request) {
    modo = "VENTILACION";
    objetivo = "25°C";
    estado = "VELOCIDAD MEDIA";
    config = 1;
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/intensivo", HTTP_GET, [](AsyncWebServerRequest *request) {
    modo = "ENFRIAMIENTO INTENSIVO";
    objetivo = "18°C";
    estado = "VELOCIDAD ALTA";
    config = 2;
    request->redirect("/"); //lo mando a la ruta (/)
  });
  // Iniciar servidor
  server.begin();
}

void loop() {
    switch(config){
        case 0:
            digitalWrite(l1,HIGH);
            digitalWrite(l2,LOW);
            digitalWrite(l3,LOW);
            break;
        case 1:
            digitalWrite(l1,LOW);
            digitalWrite(l2,HIGH);
            digitalWrite(l3,LOW);
            break;
        case 2:
            digitalWrite(l1,LOW);
            digitalWrite(l2,LOW);
            digitalWrite(l3,HIGH);
            break;
        default:
            break;
    }
}