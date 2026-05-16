#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>

// Definições de Pinos
#define PIN_BUTTON_N 32
#define PIN_BUTTON_P 33
#define PIN_BUTTON_K 21
#define PIN_DHT 14
#define PIN_LDR 34
#define PIN_RELAY 23

#define DHTTYPE DHT22

// Função para inicializar os sensores e atuadores
void initSensors(DHT& dht);

// Função para ler os botões (estado togglado)
void readButtons(bool& n, bool& p, bool& k);

// Função para converter o valor do LDR para PH
float analogToPh(int ldrValue);

#endif // SENSORS_H
