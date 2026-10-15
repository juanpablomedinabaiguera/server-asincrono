#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//variables
const char* ssid = "CESJT";
const char* password = "itisjtsmg";

String textoBoton;


const int buzzer = 33;

volatile int nota = 0;

// Servidor Asíncrono 
AsyncWebServer server(80);

// HTML
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <title>Generador de tonos</title>
    <style>
        body {
            background-color: #f0f0f0;
            text-align: center;
        }
        h1 {
            font-family: Verdana;
            text-align: center;
        }
        .est_nota {
            width: 400px;
            height: 80px;
            border: 2px dashed #000;
            margin: 20px auto;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .contenedor {
            display: flex;
            justify-content: center;
            gap: 10px;
            margin-top: 30px;
        }
        .btn_nota {
            width: 60px;
            height: 150px;
            background-color: white;
            border: 2px solid black;
        }
        .silencio {
            background-color: red;
            color: white;
        }
    </style>
</head>

<body>

    <h1>GENERADOR DE TONOS</h1>

    <div class="est_nota">
        Tocando: __NOTA__
    </div>

    <div class="contenedor">
        <a href="/do"><button class="btn_nota">Do</button></a>
        <a href="/re"><button class="btn_nota">Re</button></a>
        <a href="/mi"><button class="btn_nota">Mi</button></a>
        <a href="/sol"><button class="btn_nota">Sol</button></a>
        <a href="/la"><button class="btn_nota">La</button></a>
        <a href="/off"><button class="btn_nota silencio">Silencio</button></a>
    </div>

</body>
</html>
)rawliteral";
//setup
void setup() {
  Serial.begin(115200);
  //declaro pines
  pinMode(buzzer, OUTPUT);
  
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
    pagina.replace("__TEXTO_ESTADO_1__", textoBoton);
    request->send(200, "text/html", pagina);
  });

  server.on("/do", HTTP_GET, [](AsyncWebServerRequest *request) {        //do
    textoBoton = "DO";
    nota = 50;
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/re", HTTP_GET, [](AsyncWebServerRequest *request) {        //re
    textoBoton = "RE sospechoso";
    nota = 100;
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/mi", HTTP_GET, [](AsyncWebServerRequest *request) {        //mi
    textoBoton = "MI";
    nota = 150;
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/sol", HTTP_GET, [](AsyncWebServerRequest *request) {        //sol
    textoBoton = "SOL";
    nota = 200;
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/la", HTTP_GET, [](AsyncWebServerRequest *request) {        //la
    textoBoton = "LA";
    nota = 250;
    request->redirect("/"); //lo mando a la ruta (/)
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {        //silencio
    textoBoton = "SILENCIO";
    nota = 0;
    request->redirect("/"); //lo mando a la ruta (/)
  });
  // Iniciar servidor
  server.begin();
}

void loop() {
    analogWrite(buzzer, nota);
}