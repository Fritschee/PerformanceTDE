#include <WiFi.h>           // Biblioteca para conexão Wi-Fi
#include <WebServer.h>      // Biblioteca para criação do servidor HTTP
#include <DHT.h>            // Biblioteca para ler o sensor DHT

// Definições para o sensor DHT
#define DHTPIN 4            // Pino do ESP32 onde o sensor está conectado
#define DHTTYPE DHT22       // Tipo do sensor (DHT22)
DHT dht(DHTPIN, DHTTYPE);

// Credenciais para conexão Wi-Fi
const char* ssid = "Wokwi-GUEST";  // Configurado para usar a rede de teste do Wokwi
const char* password = "";   

// Criação do servidor HTTP na porta 80
WebServer server(80);

// Função para tratar a rota raiz "/"
void handleRoot() {
  // Leitura dos valores de umidade e temperatura
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verificação se a leitura falhou
  if (isnan(humidity) || isnan(temperature)) {
    server.send(500, "text/plain", "Falha na leitura do sensor DHT!");
    return;
  }

  // Construindo a página HTML com os valores lidos
  String html = "<html><head><meta http-equiv='refresh' content='5'/><title>Monitoramento DHT</title></head><body>";
  html += "<h1>Monitoramento de Temperatura e Umidade</h1>";
  html += "<p><strong>Temperatura:</strong> " + String(temperature) + " &deg;C</p>";
  html += "<p><strong>Umidade:</strong> " + String(humidity) + " %</p>";
  html += "</body></html>";

  // Envia o código 200 (OK) e o conteúdo HTML para o cliente
  server.send(200, "text/html", html);
}

void setup() {
  // Inicializa a comunicação serial para debug
  Serial.begin(115200);
  delay(1000); // Pequena pausa para inicializar a serial

  // Inicializa o sensor DHT
  dht.begin();

  // Conecta ao Wi-Fi
  Serial.println("Conectando-se ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado! Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configura a rota "/" para o servidor HTTP
  server.on("/", handleRoot);

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  // Mantém o servidor processando as requisições
  server.handleClient();
}
