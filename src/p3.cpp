#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";

String textoBoton;

const int r = 23;
const int g = 24;
const int b = 25;

volatile int config = 0;

// Servidor Asíncrono 
AsyncWebServer server(80);

// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <title>Selector de Color RGB</title>
    <style>
        body {
                background-color: black;
                text-align: center;
                font-family: Arial;
        }
        h1 {
                color: white;
        }

        .color_actual {
                        color: white;
                        font-size: 20px;
        }

        .contenedor {
                        display: flex;
                        justify-content: center;
        }

        .btn_color {
                    width: 80px;
                    height: 80px;
                    border: 3px solid white;
                    border-radius: 50%;
        }

        .rojo {
                background-color: red;
        }

        .verde {
                background-color: green;
        }

        .azul {
                background-color: blue;
        }

        .blanco {
                background-color: white;
        }

        .apagado {
                background-color: #34495e;
        }
    </style>
</head>
<body>
    <h1>Selector de Color RGB</h1>
    <span class="color_actual">Color Actual: <p>__TEXTO_BOTON__</p></span>
    <div class="contenedor">
        <button class="btn_color rojo"></button>
        <button class="btn_color verde"></button>
        <button class="btn_color azul"></button>
        <button class="btn_color blanco"></button>
        <button class="btn_color apagado"></button>
    </div>

</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

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
    pagina.replace("__TEXTO_BOTON__", textoBoton);
    request->send(200, "text/html", pagina);
  });

  server.on("/rojo", HTTP_GET, [](AsyncWebServerRequest *request) {        //rojo
    config = 1;
    textoBoton = "ROJO";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/verde", HTTP_GET, [](AsyncWebServerRequest *request) {        //verde
    config = 2;
    textoBoton = "VERDE";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/azul", HTTP_GET, [](AsyncWebServerRequest *request) {     //azul
    config = 3;
    textoBoton = "AZUL";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/blanco", HTTP_GET, [](AsyncWebServerRequest *request) {       //blanco
    config = 4;
    textoBoton = "BLANCO";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {       //apagado
    config = 0;
    textoBoton = "APAGADO";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  // Iniciar servidor
  server.begin();
}

void loop() {
    switch(config){
        case 0:
                digitalWrite(r,LOW);
                digitalWrite(g,LOW);
                digitalWrite(b,LOW);
                break;
        case 1:
                digitalWrite(r,HIGH);
                digitalWrite(g,LOW);
                digitalWrite(b,LOW);
                break;
        case 2:
                digitalWrite(r,LOW);
                digitalWrite(g,HIGH);
                digitalWrite(b,LOW);
                break;
        case 3:
                digitalWrite(r,LOW);
                digitalWrite(g,LOW);
                digitalWrite(b,HIGH);
                break;
        case 4:
                digitalWrite(r,HIGH);
                digitalWrite(g,HIGH);
                digitalWrite(b,HIGH);
                break;
        default:
                break;
    }
}