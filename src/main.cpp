#include <Arduino.h>
#include <SPIFFS.h>
#include "sensors.h"
#include "weather_api.h"

// Limites definidos
const int precipitation_limit = 60; // Não liga a bomba se previsão de chuva for >= 60%
const float h_limit = 50.0;         // Liga a bomba se umidade for < 50%
const float ph_limit = 5.5;         // Liga a bomba se o PH for < 5.5

// Variáveis de estado
bool n = false;
bool p = false;
bool k = false;

DHT dht(PIN_DHT, DHTTYPE);

void setup() {
    Serial.begin(115200);
    
    // Inicializa Wi-Fi
    connectWiFi();
    
    // Inicializa os sensores e atuadores
    initSensors(dht);

    // Inicializa o sistema de arquivos para salvar o CSV
    if (!SPIFFS.begin(true)) {
        Serial.println("Erro ao inicializar o SPIFFS para salvar CSV");
    }
}

void loop() {
    // Leitura da umidade (DHT22)
    float h = dht.readHumidity();
    
    // Leitura dos botões (NPK)
    readButtons(n, p, k);
    
    // Lê o valor analógico do LDR
    int ph_value = analogRead(PIN_LDR);
    float ph = analogToPh(ph_value);

    Serial.println("\n================= STATUS =================");
    
    // Se a leitura da umidade falhar
    if (isnan(h)) {
        Serial.println("⚠️  Falha na leitura do sensor de umidade!");
    } else {
        Serial.println("");
        Serial.printf("🌡️  Umidade do Solo: %.1f%%", h);
        Serial.println("");
    }
    Serial.println("");
    Serial.printf("🧪 PH do Solo:      %.1f", ph);
    Serial.println("");
    Serial.println("");
    bool alterando_npk = true;
    while (alterando_npk) {
        Serial.println("🌱 Nutrientes NPK:");
        Serial.printf("   [N] Nitrogênio: %s %s", n ? "Ativo" : "Inativo", n ? "🟢" : "🔴");
        Serial.println("");
        Serial.printf("   [P] Fósforo:    %s %s", p ? "Ativo" : "Inativo", p ? "🟢" : "🔴");
        Serial.println("");
        Serial.printf("   [K] Potássio:   %s %s", k ? "Ativo" : "Inativo", k ? "🟢" : "🔴");
        Serial.println("");

        Serial.println("\nDeseja alterar o valor dos elementos NPK?");
        Serial.println("1 - Sim");
        Serial.println("2 - Não");
        Serial.print("Sua escolha: ");

        while (!Serial.available()) {
            delay(100);
        }
        int npk_choice = Serial.parseInt();

        // Limpa o buffer do Serial
        while (Serial.available()) {
            Serial.read();
        }
        Serial.println(npk_choice);

        if (npk_choice == 1) {
            Serial.println("\nAguardando pressionar e soltar um dos botoes (N, P ou K)...");
            bool old_n = n;
            bool old_p = p;
            bool old_k = k;
            
            // Aguarda até que algum dos valores seja alterado
            while (n == old_n && p == old_p && k == old_k) {
                readButtons(n, p, k);
                delay(50);
            }
            Serial.println("Valor alterado!\n");
        } else if (npk_choice == 2) {
            alterando_npk = false; // Sai do loop
        } else {
            Serial.println("Opcao invalida. Tente novamente.\n");
        }
    }

    // Menu de Escolha da Precipitação
    Serial.println("");
    Serial.println("Escolha como obter a previsao de precipitacao:");
    Serial.println("1 - Atraves da API de Clima (Proxy HTTP para Google)");
    Serial.println("2 - Digitar o valor diretamente");
    Serial.print("Sua escolha: ");
    
    while (!Serial.available()) {
        delay(100);
    }
    int choice = Serial.parseInt();
    
    // Limpa o buffer do Serial
    while (Serial.available()) {
        Serial.read();
    }
    
    Serial.println(choice); // Exibe a escolha do usuário
    
    int precipitation = 0;
    if (choice == 1) {
        precipitation = fetchWeather();
    } else if (choice == 2) {
        Serial.print("Digite o valor da precipitacao (0 a 100): ");
        while (!Serial.available()) {
            delay(100);
        }
        precipitation = Serial.parseInt();
        
        // Limpa o buffer do Serial
        while (Serial.available()) {
            Serial.read();
        }
        Serial.println(precipitation);
    } else {
        Serial.println("Opcao invalida. Usando API por padrao.");
        precipitation = fetchWeather();
    }
    Serial.println("");
    Serial.printf("☁️  Precipitação:    %d%%", precipitation);
    Serial.println("");
    Serial.println("");

    // Condições de NPK (Prioridade Máxima)
    bool acidoForte = (n == true && p == true && k == false);
    bool muitoAcido = (n == true && p == true && k == true);

    // Relé: Irrigação do Solo (Bomba d'água)
    if (acidoForte || muitoAcido) {
        digitalWrite(PIN_RELAY, HIGH); // Liga a bomba independente das outras condições
        if (acidoForte) {
            Serial.println("💧 Bomba de Água: LIGADA (Ácido Forte: N e P ativos, K inativo)");
        } else {
            Serial.println("💧 Bomba de Água: LIGADA (Muito Ácido: N, P e K ativos)");
        }
    } else if (precipitation < precipitation_limit) { // Previsão de chuva < 60%
        if (h < h_limit) { // Umidade < 50%
            digitalWrite(PIN_RELAY, HIGH); // Liga a bomba
            Serial.println("💧 Bomba de Água: LIGADA (Baixa Umidade)");
        } else if (ph < ph_limit) { // PH < 5.5
            digitalWrite(PIN_RELAY, HIGH); // Liga a bomba
            Serial.println("💧 Bomba de Água: LIGADA (PH Baixo)");
        } else {
            digitalWrite(PIN_RELAY, LOW); // Desliga a bomba
            Serial.println("🛑 Bomba de Água: DESLIGADA");
        }
    } else { // Previsão de chuva >= 60%
        digitalWrite(PIN_RELAY, LOW); // Desliga a bomba
        Serial.println("🛑 Bomba de Água: DESLIGADA (Chuva Prevista)");
    }
    Serial.println("");

    // Salvar dados no arquivo CSV
    File file = SPIFFS.open("/dados.csv", FILE_APPEND);
    if (!file) {
        Serial.println("⚠️  Erro ao abrir arquivo CSV para escrita.");
    } else {
        // Se o arquivo estiver vazio, insere o cabeçalho
        if (file.size() == 0) {
            file.println("Umidade,PH,N,P,K,Precipitacao,Irrigacao");
        }
        bool bombaLigada = digitalRead(PIN_RELAY) == HIGH;
        file.printf("%.1f,%.1f,%d,%d,%d,%d,%d\n", h, ph, n, p, k, precipitation, bombaLigada);
        file.close();
        Serial.println("📝 O arquivo CSV foi atualizado com sucesso!");
        Serial.println("📂 Local: /dados.csv (Memória Flash Interna do ESP32)");
    }
    
    Serial.println("");

    delay(3000); // Intervalo de 3 segundos
}
