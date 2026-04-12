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
1. Clone o repositório: git clone https://github.com/
2. Execute o sistema: python main.py

## Exemplo de saída
O sistema exibe o PH e a umidde do solo em temreal como:

- PH do solo: 7.0
- Umidade do solo: 25,5%

## Autor

Lucas Ribeiro Strefezza | Projeto acadêmico desenvolvido para a FIAP.
