# 1TIAOA-2026-fase2-cap1
Atividade avaliativa do curso de tecnologia em inteligência artificial online da FIAP, referente fase 2 - capítulo 1.

# FarmTech Solutions – Análise de Dados Agrícolas
Projeto desenvolvido com o objetivo de simular um sistema que avalia o PH e umidade do solo executando python em um simulador de dispositivos eletrônicos([Woki](https://wokwi.com/)), utilizando o modelo ESP32, afim de controlar a irrigação do solo conforme a necessidade. 

## Funcionalidades
O sistema permite:

* Pressionar 3 botões para simular a proporção dos elementos químicos: Nitrogênio(N), Fósforo(P), Potássio(K), que afeta o PH do solo.
* Visualizar o nível de PH do solo em tempo real.
* Visualizar a porcentagem da umidade do solo em tempo real.
* Controlar o relé azul representando uma bomba de àgua para irrigação do solo caso a umidade esteja abaixo do esperado.

## Tecnologias utilizadas
* Python → sistema principal
* ESP32 → microcontrolador que executa python para integração dos sensores do solo.
* photoresistor-sensor (LDR) → sensor de luz que irá ajudar na simulação do PH do solo.
* dht22 → sensor digital de umidade e temperatura.

## Como executar o projeto
1. Clone o repositório: git clone git@github.com:lstrefezza/1TIAOA-2026-fase2-cap1.git
2. Execute o sistema: python main.py

## Exemplo de saída
O sistema exibe o PH e a umidade do solo em tempo real como:

- PH do solo: 7.0
- Umidade do solo: 25,5%

## Autor

Lucas Ribeiro Strefezza | Projeto acadêmico desenvolvido para a FIAP.

## Obsevações:
- Identificação: No Wokwi, ao passar o mouse sobre os pinos, o nome "GPIOx" aparece. Use o número que aparece no código (ex: pinMode(2, OUTPUT) usa o GPIO 2).

- Simulação: Pinos 34-39 são ótimos para potenciômetros ou sensores de temperatura (DHT). Pinos 2, 4, 5, 12, 13, 14, 15 são ótimos para LEDs.

- LEDs: É comum usar o 2 ou 4 para testar LEDs, e 12 ou 13 para botões.

- Aviso: Pinos GPIO 34, 35, 36 e 39 não possuem resistores pull-up ou pull-down internos, então use resistores externos se necessário.

- Projeto de referencia: https://wokwi.com/projects/460851158838958081
