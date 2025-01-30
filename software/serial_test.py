import serial

ser = serial.Serial("COM3", 115200)

if ser.is_open:
    print("Conexão estabelecida na porta COM13!")
try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode("utf-8").strip()
            print(f"Dados lidos: {data}")
except KeyboardInterrupt:
    print("Leitura interrompida.")
finally:
    ser.close()
