import dht
import urequests
import network
from machine import Pin, ADC
from time import sleep

precipitation_limit = 60 #Não liga a bomba de água se houver previsão de chuva igual ou maior que 60%
h_limit = 50 #Liga a bomba de água se houver umidade menor que 50%
ph_limit = 5.5 #Liga a bomba de água se o PH for menor que 5.5

#Chave da api para consultar a previsão do tempo
API_KEY = "AIzaSyCaa5maiC35idGJ6xYuH9QKTKFo5IK5CN8"

#Variáveis dos elementos Nitrogênio(N), Fósforo(P) e Potássio(K).
n = True
p = True
k = True

#Configuração dos botões que simulam os elementos NPK.
push_button_n = Pin(32, Pin.IN, Pin.PULL_UP)
push_button_p = Pin(33, Pin.IN, Pin.PULL_UP)
push_button_k = Pin(21, Pin.IN, Pin.PULL_UP)

#Configuração do sensor DHT22 (sensor de umidade do solo).
dht_sensor = dht.DHT22(Pin(14))

#Configura o pino 34 como entrada analógica (ADC) para receber o valor do LDR.
ldr_value = ADC(Pin(34))

#Configuração do relé.
rele = Pin(23, Pin.OUT)

#Função para converter o valor analógico do LDR em uma proporção de PH (0-14)
def analog_to_ph(x):
    #Definindo os limites
    min_origem, max_origem = 32, 4095
    min_destino, max_destino = 14, 0
    #Garante que o valor esteja dentro do intervalo de entrada antes do cálculo
    x = max(min_origem, min(x, max_origem))
    resultado = (x - min_origem) * (max_destino - min_destino) / (max_origem - min_origem) + min_destino
    return resultado

#Configuração do Wi-Fi (Wokwi-GUEST é a rede padrão do simulador Wokwi)
print("Conectando ao Wi-Fi", end="")
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect('Wokwi-GUEST', '')
while not sta_if.isconnected():
    print(".", end="")
    sleep(0.1)
print(" Conectado com sucesso!\n")

#Função para buscar dados de clima na API do Google
def fetch_weather():
    url = f"https://weather.googleapis.com/v1/forecast/days:lookup?key={API_KEY}&location.latitude=23.5614&location.longitude=46.6564"
    try:
        print("Consultando a API de clima...")
        response = urequests.get(url)
        print(f"Status da resposta: {response.status_code}")
        # print(f"Dados:\n{response.text}\n") # Descomente para ver os dados retornados
        precipitation = response.json()['forecastDays'][0]['daytimeForecast']['precipitation']['probability']['percent']
        response.close()
    except Exception as e:
        print(f"Erro na requisição: {e}\n")
        precipitation = 0 #Caso de erro, assume 0%
    return precipitation

#Loop principal.
while True:
    try:
        #Leitura do sensor de umidade.
        dht_sensor.measure()
        h = dht_sensor.humidity()

        #Leitura dos botões.
        if push_button_n.value() == 0:
            n = not n
        if push_button_p.value() == 0:
            p = not p
        if push_button_k.value() == 0:
            k = not k

        print(f"Nitrogênio: {n}")
        print(f"Fósforo: {p}")
        print(f"Potássio: {k}")

        #Lê o valor analógico do LDR.
        ph_value = ldr_value.read()
        ph =  analog_to_ph(ph_value);

        print(f"Umidade do solo: {h:.1f}%")
        print(f"PH: {ph:.1f}%")
        
        #Busca dados de clima.
        precipitation = fetch_weather()
        print(f"Precipitação: {precipitation}%\n")

        #Relé: Irrigação do Solo (Bomba d'água).
        if precipitation < precipitation_limit: #Se houver previsão de chuva menor que 60%
            if h < h_limit: #Se houver umidade menor que 50%
                rele.value(1) #Liga a bomba
            elif ph < ph_limit: #Se o PH for menor que 5.5
                rele.value(1) #Liga a bomba
            else:
                rele.value(0) #Desliga a bomba
        else: #Se houver previsão de chuva maior ou igual a 60%
            rele.value(0) #Desliga a bomba

    except OSError as e:
        print("Falha na leitura dos sensores. Verifique as conexões.")

    sleep(3) #Intervalo para a leitura dos sensores e da API de clima em segundos.