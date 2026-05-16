#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <Arduino.h>

// Conecta ao Wi-Fi (Wokwi-GUEST é a rede padrão do simulador Wokwi)
void connectWiFi();

// Busca os dados de clima e retorna a probabilidade de precipitação (%)
int fetchWeather();

#endif // WEATHER_API_H
