# FarmTech Solutions – Análise de Dados Agrícolas
Projeto desenvolvido com o objetivo de simular um sistema que avalia o PH e umidade do solo executando python em um simulador de dispositivos eletrônicos([Wokwi](https://wokwi.com/)), utilizando o modelo ESP32, afim de controlar a irrigação do solo conforme a necessidade. 

## Funcionalidades
O sistema permite:

* Pressionar 3 botões para simular os níveis dos elementos químicos: Nitrogênio(N), Fósforo(P), Potássio(K) no solo.
* Visualizar quais elementos, Nitrogênio(N), Fósforo(P), Potássio(K) estão presentes no solo.
* Visualizar o nível de PH do solo, que será simulado pelo sensor de luz.
* Visualizar a porcentagem da umidade do solo, que será simulada pelo sensor de umidade do ar.
* Consultar via api e visualizar probabiliadde de precipitação (chuva).
* Controlar o relé azul representando uma bomba de àgua para irrigação do solo caso a umidade ou PH estejam abaixo do esperado e caso a probabilidade de chuva seja menor que 60%.

## Tecnologias utilizadas
* Python → sistema principal
* ESP32 → microcontrolador que executa o python para integração dos sensores.
* photoresistor-sensor (LDR) → sensor de luz, que irá ajudar na simulação do PH do solo.
* dht22 → sensor de umidade e temperatura do ar, que irá ajudar na simulação da umidade do solo.
* relay-module → relé que representa aa bomba de àgua.

## Como executar o projeto
1. Abra um projeto no wokwi: https://wokwi.com/projects/305568836183130690
2. Cole o código do main.py na aba main.py do wokwi.
3. Cole o código do diagram.py na aba diagram.py do wokwi.

## Exemplo de saída
* Nitrogênio: True
* Fósforo: True
* Potássio: True
* Umidade do solo: 21.0%
* PH: 0.1%
* Consultando a API de clima...
* Status da resposta: 200
* Precipitação: 10%

## Autor

Lucas Ribeiro Strefezza | Projeto acadêmico desenvolvido para a FIAP.
