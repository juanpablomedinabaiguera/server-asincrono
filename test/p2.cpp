#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";

String textoBoton;  //porcentaje
String notoBotxeT;  //color

const int ldr = 23;

int valorldr = 0;
int mapeo = 0;
int porcentaje;

// Servidor Asíncrono 
AsyncWebServer server(80);

// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="refresh" content="5">
    <title>Luz ambiental :></title>

    <style>
        body {
            background-color: __COLOR__;        
            color: white;                       
            text-align: center;                 
            font-family: 'Segoe UI';
        }
/*#2c3e50 (azul oscuro) para noche
#f1c40f (amarillo) para día
#e67e22 (naranja) para atardecer
*/
        h1 {
            font-family: 'Segoe UI';
        }

        .lectura {
            border: 3px dotted white;
            border-radius: 20px;
            padding: 25px;
        }

        .porcentaje {
            font-weight: bold;
            font-size: 40px;
        }
    </style>
</head>

<body>

    <h1>Monitor de Luz Ambiental</h1>

    <div class="lectura">
        <span>Nivel de Luz: <span class="porcentaje">__PORCENTAJE_LUZ__</span></span>
    </div>

</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(ldr, INPUT);

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
    pagina.replace("__PORCENTAJE_LUZ__", textoBoton);
    pagina.replace("__COLOR__", textoBoton);
    
    request->send(200, "text/html", pagina);
  });

  // Iniciar servidor
  server.begin();

}

void loop() {
    valorldr = analogRead(ldr);
    mapeo = map(valorldr, 0, 4095, 0, 100);
    textoBoton = mapeo;
    if(mapeo<=25) notoBotxeT = "#2c3e50";
    else if(mapeo>50&&mapeo<75) notoBotxeT = "#f1c40f";
    else if(mapeo>=75) notoBotxeT = "#e67e22";
}