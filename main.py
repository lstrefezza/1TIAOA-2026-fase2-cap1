import dht
from machine import Pin, ADC
from time import sleep

#Variáveis dos elementos Nitrogênio(n), Fósforo(p) e Potássio(k).
n = True
p = True
k = True

#Configuração do botões que simulam os elementos NPK.
push_button_n = Pin(32, Pin.IN, Pin.PULL_UP)
push_button_p = Pin(33, Pin.IN, Pin.PULL_UP)
push_button_k = Pin(21, Pin.IN, Pin.PULL_UP)

#Configuração do sensor DHT22 (sensor de umidade do solo).
sensor_dht = dht.DHT22(Pin(14))

#Configura o pino 34 como entrada analógica (ADC) para receber o valor do LDR.
sensor_ldr = ADC(Pin(34))

# Define a resolução para 12 bits (0-4095)
sensor_ldr.width(ADC.WIDTH_12BIT)

#Configuração do relé.
rele1 = Pin(23, Pin.OUT)

#Função para converter o valor análógico (32-4095) em uma proporção de PH (0-14)
def analogToPH(x):
    # Definindo os limites
    min_origem, max_origem = 32, 4095
    min_destino, max_destino = 0, 14.1
    # Garante que o valor esteja dentro do intervalo de entrada antes do cálculo
    x = max(min_origem, min(x, max_origem))
    resultado = (x - min_origem) * (max_destino - min_destino) / (max_origem - min_origem) + min_destino
    return resultado

#Loop principal.
while True:
    try:
        #Leitura do sensor de umidade.
        sensor_dht.measure()
        h = sensor_dht.humidity()

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

        #Lê o valor analógico (0-4095)
        ph = sensor_ldr.read();
        ph =  analogToPH(ph);

        print("Umidade do solo: {:.1f}%".format(h))
        print("PH: {:.1f}%\n".format(ph))

        #Relé: Irrigação do Solo (Bomba d'água).
        if h < 25: 
            rele1.value(1)
        else:
            rele1.value(0)

    except OSError as e:
        print("Falha na leitura dos sensores. Verifique as conexões.")

    sleep(0.1) #Intervalo para a leitura dos sensores.