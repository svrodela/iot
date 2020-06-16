#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

//vraiables
int contconexion = 0;

const char *ssid = "LG Harmony_3341";
const char *password = "1234567890";

unsigned long previousMillis = 0;

char host[48];
String strhost = "192.168.14.226"; //IP del Server
String strurl = "/iot/insert.php"; //URL
String chipid = "";

//funcion para insertar datos

String enviardatos(String datos) {
  String linea = "error";
  WiFiClient client;
  strhost.toCharArray(host, 49);
  if (!client.connect(host, 80)) {
    Serial.println("Fallo de conexion");
    return linea;
  }

  client.print(String("POST ") + strurl + " HTTP/1.1" + "\r\n" + 
               "Host: " + strhost + "\r\n" +
               "Accept: */*" + "*\r\n" +
               "Content-Length: " + datos.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "\r\n" + datos);           
  delay(10);             
  
  Serial.print("Enviando datos a Mysql...");
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("No conectado con el cliente");
      client.stop();
      return linea;
    }
  }
  // imprime las lineas que recibe del servidor
  while(client.available()){
    linea = client.readStringUntil('\r');
  }  
  Serial.println(linea);
  return linea;
}

//-------------------------------------------------------------------------

void setup() {

  // Inicia Serial
  Serial.begin(115200);
  Serial.println("");

  Serial.print("chipId: "); 
  chipid = String(ESP.getChipId());
  Serial.println(chipid); 

  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //50 intentos para conectar  si no lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      IPAddress ip(192, 168, 14, 60); 
      IPAddress gateway(192, 168, 14, 1); 
      IPAddress subnet(255, 255, 255, 0); 
   //   WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//--------------------------LOOP--------------------------------
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
    previousMillis = currentMillis;
    int v = analogRead(A0);
   // float temp = analog*0.322265625;
    Serial.println(v);
    enviardatos("tem=" + String(v, 2));
  }
}