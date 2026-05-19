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

    int h2 = std::round(h);
    int ph2 = std::round(ph * 10);
    int precipitation2 = std::round(precipitation);
    int n2 = n ? 1 : 0;
    int p2 = p ? 1 : 0;
    int k2 = k ? 1 : 0;
    int bombaLigada2 = bombaLigada ? 1 : 0;

    JsonDocument doc;
    doc["umidade"] = h2;
    doc["ph"] = ph2;
    doc["n"] = n2;
    doc["p"] = p2;
    doc["k"] = k2;
    doc["precipitacao"] = precipitation2;
    doc["irrigacao"] = bombaLigada2;

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
