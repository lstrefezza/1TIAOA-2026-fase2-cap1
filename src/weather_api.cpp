#include "weather_api.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* API_KEY = "AIzaSyCaa5maiC35idGJ6xYuH9QKTKFo5IK5CN8";

void connectWiFi() {
    Serial.println("");
    Serial.print("📡 Conectando ao Wi-Fi [Wokwi-GUEST]");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("✅ Wi-Fi Conectado com sucesso!\n");
}

int fetchWeather() {
    int precipitation = 0;
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure *client = new WiFiClientSecure;
        client->setInsecure();
        
        HTTPClient http;
        
        // As coordenadas originais eram positivas (Hemisfério Norte/Leste). Ajustando para SP (-23, -46).
        String googleUrl = "https://weather.googleapis.com/v1/forecast/days:lookup?key=";
        googleUrl += API_KEY;
        googleUrl += "&location.latitude=-23.5614&location.longitude=-46.6564";
        
        // Usamos o CorsProxy.io via HTTPS.
        // O erro 301 ocorreu porque o ESP32 não conseguiu redirecionar de HTTP para HTTPS sem o Client Seguro.
        String url = "https://corsproxy.io/?" + googleUrl;
        
        Serial.println("");
        Serial.print("🌍 Consultando API de Clima (Proxy HTTPS)...");
        Serial.println("");
        
        client->setHandshakeTimeout(30000);
        http.setTimeout(15000);
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        http.begin(*client, url);
        
        // Adicionando headers de navegador para o firewall do Cloudflare não barrar o ESP32 (Erro 403)
        http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
        http.addHeader("Origin", "http://localhost");
        
        int httpCode = http.GET();
        Serial.printf("Status: %d", httpCode);
        Serial.println("");
        
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                JsonDocument doc; 
                DeserializationError error = deserializeJson(doc, payload);

                if (!error) {
                    precipitation = doc["forecastDays"][0]["daytimeForecast"]["precipitation"]["probability"]["percent"];
                } else {
                    Serial.print("Erro no parse JSON: ");
                    Serial.println(error.c_str());
                }
            }
        } else {
            Serial.printf("Erro na requisição: %s", http.errorToString(httpCode).c_str());
            Serial.println("");
        }
        http.end();
        delete client;
    } else {
        Serial.println("WiFi Desconectado");
        Serial.println("");
    }
    return precipitation;
}
