#include "webhook_api.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const char* urlWebhook = "https://hook.us2.make.com/6ap7p29fuyrktts5t2ik3w767p5gfky5";

bool sendWebhookData(float h, float ph, bool n, bool p, bool k, float precipitation, bool bombaLigada) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("⚠️  Wi-Fi desconectado. Não foi possível enviar os dados para o webhook.");
        return false;
    }

    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();
    http.begin(urlWebhook);
    http.addHeader("Content-Type", "application/json");

    JsonDocument doc;
    doc["umidade"] = h;
    doc["ph"] = ph;
    doc["n"] = n;
    doc["p"] = p;
    doc["k"] = k;
    doc["precipitacao"] = precipitation;
    doc["irrigacao"] = bombaLigada;

    String payload;
    serializeJson(doc, payload);

    Serial.println("Enviando dados para o Make (Webhook)...");
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
        Serial.printf("Status: %d", httpResponseCode);
        Serial.println();
        Serial.println("Dados enviados com sucesso!");
        
        http.end();
        return true;
    } else {
        Serial.print("Erro no envio POST: ");
        Serial.println(httpResponseCode);
        http.end();
        return false;
    }
}
