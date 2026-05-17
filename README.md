# FarmTech Solutions – Análise de Dados Agrícolas
Projeto desenvolvido com o objetivo de simular um sistema que avalia o PH e umidade do solo e a probabilidade de chuva, executando código em C++ (PlatformIO/Arduino) em um simulador de dispositivos eletrônicos ([Wokwi](https://wokwi.com/)), utilizando o modelo ESP32, afim de controlar a irrigação do solo conforme a necessidade para um tipo específico de plantação.

## Funcionalidades
O sistema permite:

* Alterar dinamicamente os níveis dos elementos químicos: Nitrogênio(N), Fósforo(P), Potássio(K) no solo utilizando os botões físicos na simulação e o menu interativo no Terminal Serial.
* Visualizar quais elementos estão presentes no solo de forma intuitiva com indicadores (Ativo 🟢 ou Inativo 🔴).
* Visualizar o nível de PH do solo, que será simulado pelo sensor de luz.
* Visualizar a porcentagem da umidade do solo, que será simulada pelo sensor de umidade do ar (DHT22).
* Consultar via API (Google Weather com Proxy) a probabilidade de precipitação (chuva) ou inserir o valor manualmente pelo terminal.
* Controlar o relé azul representando uma bomba de água para irrigação do solo caso a umidade do solo seja menor que 50% ou pH seja menor que 5.5 e caso a probabilidade de chuva seja menor que 60%.
* Armazenamento automático do histórico de leituras (Umidade, PH, N, P, K, Precipitação e estado da Irrigação) em uma planilha do google utilizando um webhook [make](https://hook.us2.make.com/6ap7p29fuyrktts5t2ik3w767p5gfky5).

## Contexto dos valores
Para este projeto de automação, escolhemos a cultura do milho, uma das mais responsivas à nutrição e irrigação controlada. Com base em dados técnicos da  [Embrapa](https://www.embrapa.br/agencia-de-informacao-tecnologica/cultivos/milho/producao/manejo-do-solo-e-adubacao/adubacao-e-fertilidade-do-solo/exigencias-nutricionais-da-planta) e de guias de [agricultura de precisão](https://agroadvance.com.br/blog-adubacao-do-milho/), definimos os parâmetros ideais para o acionamento do relé.

Para que o sistema ligue a irrigação (relé) de forma inteligente, os níveis detectados pelos sensores devem estar abaixo das seguintes faixas ideais:
* Umidade: 	60% a 80% → Ligar se umidade menor que 50% e precipitação menor que 60%
* PH:	6,0 a 6,5 →	Ligar se pH menor que 5,5 e precipitação menor que 60%
* Elementos K, P e N -> Ligar se N e P Ativos (Ácido Forte) ou N, P e K Ativos (Muito Ácido)

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
* C++ (Framework Arduino) → código principal rodando no microcontrolador.
* PlatformIO → gerenciador de dependências e ambiente de compilação.
* ESP32 → microcontrolador simulado que executa o código e integra os sensores.
* photoresistor-sensor (LDR) → sensor de luz, que ajuda na simulação do PH do solo.
* dht22 → sensor de umidade e temperatura do ar, que ajuda na simulação da umidade do solo.
* relay-module → relé que representa a bomba de água.
* Weather API → API de clima do Google (através de um proxy HTTP configurado no código).
* Webhook Make → sistema de webhook que permite automatizar fluxos integrando com outros sistemas.

## Como executar o projeto
1. Abra a pasta do projeto no VS Code com a extensão do **PlatformIO** e do **Wokwi** instaladas.
2. Deixe o PlatformIO instalar as dependências e compilar o projeto (`PlatformIO: Build`).
3. Abra o arquivo `diagram.json`.
4. Inicie a simulação do Wokwi.
5. Utilize o **Terminal Serial** integrado para interagir com o sistema:
   - Responda aos menus para alterar os estados dos botões N, P e K (clicando neles quando solicitado no terminal).
   - Escolha se deseja buscar o clima via API ou digitá-lo manualmente.
6. Altere a umidade do solo e o LDR (pH) clicando diretamente nos componentes durante a simulação.

## Exemplo de saída no Terminal Serial
```text
================= STATUS =================

🌡️  Umidade do Solo: 25.5%

🧪 PH do Solo:      10.7

🌱 Nutrientes NPK:
   [N] Nitrogênio: Inativo 🔴
   [P] Fósforo:    Ativo 🟢
   [K] Potássio:   Inativo 🔴

Deseja alterar o valor dos elementos NPK?
1 - Sim
2 - Nao
Sua escolha: 2

Escolha como obter a previsao de precipitacao:
1 - Atraves da API de Clima (Proxy HTTP para Google)
2 - Digitar o valor diretamente
Sua escolha: 1

🌍 Consultando API de Clima (Proxy HTTPS)...
Status: 200

☁️  Precipitação:    10%

💧 Bomba de Água: LIGADA (Baixa Umidade)

Enviando dados para o Make (Webhook)...
Status: 200
Dados enviados com sucesso!
```

## Imagem do projeto em execução
<img width="916" height="515" alt="image" src="https://github.com/user-attachments/assets/2d7b0b0f-a6ca-46c2-b83c-10007ac7b479" />
<img width="920" height="516" alt="image" src="https://github.com/user-attachments/assets/67fedadd-cd16-468f-9fbf-ab196d790a15" />


## Video explicativo sobre o funcionamento do projeto
https://www.youtube.com/watch?v=xdoqt5Xyvbc

##
  **Atenção:** Como o projeto utiliza C++ para o microcontrolador no Wokwi, todas as interações e leituras síncronas agora são feitas através do Terminal Serial interativo, garantindo uma simulação mais realista e robusta das lógicas de acionamento.

## Autor

Lucas Ribeiro Strefezza | Projeto acadêmico desenvolvido para a FIAP.
