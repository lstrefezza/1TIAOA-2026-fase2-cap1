import dht
from machine import Pin
from time import sleep

#Configuração do sensor DHT22 (sensor de umidade do solo).
sensor_dht = dht.DHT22(Pin(14))
sensor_ldr = Pin(15, Pin.IN)
#push_button = Pin(16, Pin.IN)  

#Configuração dos atuadores (relés).
rele1 = Pin(23, Pin.OUT)

#Loop principal.
while True:
    try:
        #Leitura dos sensor de umidade.
        sensor_dht.measure()
        h1 = sensor_dht.humidity()


#        logic_state = push_button.value()
#       if logic_state == True:     
#           led.value(1)            
#        else:                      
#            led.value(0)


        #Leitura do sensores de luz.
        l1 = sensor_ldr.value() # Lê 0 ou 1

        print("Umidade do solo: {:.1f}%".format(h1))
        print("Luz: {:.1f}%".format(l1))

        #Relé: Irrigação do Solo (Bomba d'água).
        if h1 < 25: 
            rele1.value(1)
        else:
            rele1.value(0)

    except OSError as e:
        print("Falha na leitura dos sensores. Verifique as conexões.")

    sleep(1) #Intervalo para a leitura dos sensores.