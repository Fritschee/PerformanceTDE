#include <WiFi.h>         // Biblioteca para conexão Wi-Fi
#include <WebServer.h>    // Biblioteca para criação do servidor HTTP
#include <DHT.h>          // Biblioteca para leitura do sensor DHT

// Configurações do sensor DHT22
#define DHTPIN 4          // Pino digital do ESP32 conectado ao pino de dados do DHT22
#define DHTTYPE DHT22     // Define o modelo do sensor
DHT dht(DHTPIN, DHTTYPE);

// Credenciais de acesso à rede Wi-Fi
const char* ssid     = "Wokwi-GUEST";     // Gateway Publico do Wokwi para testes
const char* password = "";     

// Cria o objeto do servidor HTTP na porta 80
WebServer server(80);

// Função que trata as requisições na rota "/"
void handleRoot() {
  // Realiza a leitura dos dados do sensor
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifica se as leituras são válidas
  if (isnan(humidity) || isnan(temperature)) {
    server.send(500, "text/plain", "Erro na leitura do sensor DHT22!");
    return;
  }

  // Cria uma página HTML simples que se atualiza a cada 5 segundos
  String html = "<html><head><meta http-equiv='refresh' content='5'/><title>Monitoramento DHT22</title></head><body>";
  html += "<h1>Monitoramento de Temperatura e Umidade</h1>";
  html += "<p><strong>Temperatura:</strong> " + String(temperature) + " &deg;C</p>";
  html += "<p><strong>Umidade:</strong> " + String(humidity) + " %</p>";
  html += "</body></html>";

  // Envia o código 200 (OK) e a página HTML para o cliente
  server.send(200, "text/html", html);
}

void setup() {
  // Inicializa o Monitor Serial para visualização dos dados
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando o sistema...");

  // Inicializa o sensor DHT22
  dht.begin();

  // Conexão com a rede Wi-Fi
  Serial.println("Conectando-se ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado! Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configura a rota da interface web
  server.on("/", handleRoot);
  // Inicia o servidor HTTP
  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  // Processa as requisições HTTP
  server.handleClient();

  // Realiza a leitura dos dados do sensor DHT22
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Exibe os dados no Monitor Serial
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erro na leitura do sensor DHT22!");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print(" °C\t");
    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Aguarda 5 segundos para a próxima leitura
  delay(5000);
}
