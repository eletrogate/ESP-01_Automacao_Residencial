#include <ESP8266WiFi.h>
#define LoadPin 2// GPIO2
const char* ssid = "SuaRedeWiFI";//type your ssid
const char* password = "SuaSenha";//type your password
WiFiServer server(80);//Service Port
int value = LOW;
void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(LoadPin, OUTPUT);
  digitalWrite(LoadPin, LOW);
  // Connect to WiFi network
  Serial.println();
  Serial.print("Conectando na rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  delay(500);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado!");
  //Inicializa o servidor
  server.begin();
  Serial.println("Servidor Inicializado");
  // Imprime o endereco IP
  Serial.print("Utilize essa URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop()
{
  // Verifica se o cliente está conectado
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  //Aguarda envio de dados pelo cliente
  while (!client.available())
  {
    delay(1);
  }
  //LE a primeira linha da requisicao
  String request = client.readStringUntil('\r');
  client.flush();
  //aciona a luminaria led de acordo com os dados recebidos
  
  if (request.indexOf("/Carga=ON") != -1)
  {
    Serial.println(request);
    digitalWrite(LoadPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/Carga=OFF") != -1)
  {
    Serial.println(request);
    digitalWrite(LoadPin, LOW);
    value = LOW;
  }
  //envia mensagem de resposta
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Status da carga: ");
  if (value == HIGH)
  {
    client.print("On");
  }
  else
  {
    client.print("Off");
  }
  //Mensagem que aparece no site
  client.println("<br><br>");
  client.println("Clique <a href=\"/Carga=ON\">here</a>para ligar<br>");
  client.println("Clique <a href=\"/Carga=OFF\">here para desligar<br>");
  client.println("</html>");
  delay(1);
  Serial.println("");
}