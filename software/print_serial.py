import serial

# Abre a conexão serial
ser = serial.Serial("COM3", 115200)

if ser.is_open:
    print("Conexão estabelecida na porta COM3!")

try:
    # Envia um comando pela serial (como string, convertida para bytes)
    comando = input()  # Substitua pelo comando desejado
    ser.write(comando.encode("utf-8"))  # Envia o comando para o ESP
    print(f"Comando enviado: {comando}")

    # Leitura da resposta do ESP (se houver)
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode("utf-8").strip()
            print(f"Dados lidos: {data}")

except KeyboardInterrupt:
    print("Leitura interrompida.")
finally:
    ser.close()  # Fecha a porta serial
