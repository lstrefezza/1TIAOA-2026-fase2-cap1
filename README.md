# FarmTech Solutions – Análise de Dados Agrícolas
Projeto desenvolvido com o objetivo de simular um sistema que avalia o PH e umidade do solo e a probabilidade de chuva, executando python em um simulador de dispositivos eletrônicos ([Wokwi](https://wokwi.com/)), utilizando o modelo ESP32, afim de controlar a irrigação do solo conforme a necessidade para um tipo específico de plantação.

## Funcionalidades
O sistema permite:

* Pressionar 3 botões para simular os níveis dos elementos químicos: Nitrogênio(N), Fósforo(P), Potássio(K) no solo.
* Visualizar quais elementos, Nitrogênio(N), Fósforo(P), Potássio(K) estão presentes no solo (True ou False).
* Visualizar o nível de PH do solo, que será simulado pelo sensor de luz.
* Visualizar a porcentagem da umidade do solo, que será simulada pelo sensor de umidade do ar.
* Consultar via api e visualizar probabiliadde de precipitação (chuva).
* Controlar o relé azul representando uma bomba de àgua para irrigação do solo caso a umidade do solo seja menor que 50% ou pH seja menor que 5.5 e caso a probabilidade de chuva seja menor que 60%.

## Contexto dos valores
Para este projeto de automação, escolhemos a cultura do milho, uma das mais responsivas à nutrição e irrigação controlada. Com base em dados técnicos da  [Embrapa](https://www.embrapa.br/agencia-de-informacao-tecnologica/cultivos/milho/producao/manejo-do-solo-e-adubacao/adubacao-e-fertilidade-do-solo/exigencias-nutricionais-da-planta) e de guias de [agricultura de precisão](https://agroadvance.com.br/blog-adubacao-do-milho/), definimos os parâmetros ideais para o acionamento do relé.

Para que o sistema ligue a irrigação (relé) de forma inteligente, os níveis detectados pelos sensores devem estar abaixo das seguintes faixas ideais:
* Umidade: 	60% a 80% da Capacidade de Campo → Ligar se umidade < 50%
* pH:	6,0 a 6,5 →	Ligar (com corretivo) se pH < 5,5

Como o Nitrogênio (N) é o principal agente acidificante, o Fósforo (P) contribui de forma moderada e o Potássio (K) é praticamente neutro, podemos estabelecer uma lógica de soma de impactos.
Considerando um solo inicial neutro (pH), aqui está a tabela com todas as combinações possíveis para você seguir na simulação:

| N (Botão) | P (Botão) | K (Botão) | Estado Combinado | pH Esperado (Simulação) | Impacto no Solo |
| :---: | :---: | :---: | :--- | :---: | :--- |
| OFF | OFF | OFF | Nenhum Nutriente | **7.0** | Neutro |
| OFF | OFF | ON | Apenas Potássio (K) | **6.8 - 7.0** | Praticamente Neutro |
| OFF | ON | OFF | Apenas Fósforo (P) | **6.4 - 6.6** | Levemente Ácido |
| ON | OFF | OFF | Apenas Nitrogênio (N) | **5.8 - 6.0** | Ácido Moderado |
| OFF | ON | ON | P + K | **6.2 - 6.4** | Levemente Ácido |
| ON | OFF | ON | N + K | **5.6 - 5.8** | Ácido Moderado |
| ON | ON | OFF | N + P | **5.0 - 5.4** | Ácido Forte |
| ON | ON | ON | N + P + K (Completo) | **4.5 - 5.0** | Muito Ácido |

**Regra Lógica para Simulação:**
*   **Base:** pH 7.0
*   **Ação N:** -1.0 no pH
*   **Ação P:** -0.5 no pH
*   **Ação K:** -0.2 no pH

## Tecnologias utilizadas
* Python → sistema principal
* ESP32 → microcontrolador que executa o python para integração dos sensores.
* photoresistor-sensor (LDR) → sensor de luz, que irá ajudar na simulação do PH do solo.
* dht22 → sensor de umidade e temperatura do ar, que irá ajudar na simulação da umidade do solo.
* relay-module → relé que representa aa bomba de àgua.
* Weather API - API de previsão do Google.

## Como executar o projeto
1. Abra um projeto no wokwi: https://wokwi.com/projects/305568836183130690
2. Cole o código do main.py na aba main.py do wokwi.
3. Cole o código do diagram.py na aba diagram.py do wokwi.
4. Executar o projeto no wokwi clicando no botão verde (start the simulation)
5. Alterar a umidade do solo clicando no DHT22.
6. Simular os níveis dos elementos e pH do solo clicando nos botões e no LDR.

## Exemplo de saída
* Nitrogênio: True
* Fósforo: True
* Potássio: True
* Umidade do solo: 21.0%
* PH: 0.1%
* Consultando a API de clima...
* Status da resposta: 200
* Precipitação: 10%

##
  **Atenção:** Se a API retornar erro o valor de precipitação será zero. Caso o status seja 429, a conta usada chegou no limite diário de requisições (100 por dia) e será necessário alterar a API_KEY (no início do main.py) de uma conta que ainda não chegou no limite ou aguardar até o dia seguinte.

## Autor

Lucas Ribeiro Strefezza | Projeto acadêmico desenvolvido para a FIAP.
