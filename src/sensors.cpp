#include "sensors.h"

void initSensors(DHT& dht) {
    pinMode(PIN_BUTTON_N, INPUT_PULLUP);
    pinMode(PIN_BUTTON_P, INPUT_PULLUP);
    pinMode(PIN_BUTTON_K, INPUT_PULLUP);
    
    pinMode(PIN_LDR, INPUT);
    
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_RELAY, LOW);
    
    dht.begin();
}

void readButtons(bool& n, bool& p, bool& k) {
    // Lógica de toggle no momento em que o botão é solto (RISING edge)
    static bool last_n = HIGH;
    static bool last_p = HIGH;
    static bool last_k = HIGH;

    bool current_n = digitalRead(PIN_BUTTON_N);
    bool current_p = digitalRead(PIN_BUTTON_P);
    bool current_k = digitalRead(PIN_BUTTON_K);

    // Ocorre o toggle quando o estado anterior era pressionado (LOW) e o atual é solto (HIGH)
    if (last_n == LOW && current_n == HIGH) n = !n;
    if (last_p == LOW && current_p == HIGH) p = !p;
    if (last_k == LOW && current_k == HIGH) k = !k;

    last_n = current_n;
    last_p = current_p;
    last_k = current_k;
}

float analogToPh(int ldrValue) {
    // Definindo os limites (esp32 ADC é 0-4095)
    float min_origem = 32.0;
    float max_origem = 4095.0;
    float min_destino = 14.0;
    float max_destino = 0.0;
    
    // Garante que o valor esteja dentro do intervalo de entrada
    if (ldrValue < min_origem) ldrValue = min_origem;
    if (ldrValue > max_origem) ldrValue = max_origem;
    
    float resultado = (ldrValue - min_origem) * (max_destino - min_destino) / (max_origem - min_origem) + min_destino;
    float resultado_arredondado = std::round(resultado * 10.0f) / 10.0f;
    return resultado_arredondado;
}
