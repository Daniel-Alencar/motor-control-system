import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from serial import Serial, SerialException

AMOUNT_GRAPH = 100
target = []


def printOnSerial():
    ser = Serial("COM3", 115200)

    if ser.is_open:
        print("Conexão estabelecida na porta COM3!")

    try:
        # Envia um comando pela serial (como string, convertida para bytes)
        comando = input()  # Substitua pelo comando desejado
        ser.write(comando.encode("utf-8"))  # Envia o comando para o ESP
        print(f"Comando enviado: {comando}")

    except KeyboardInterrupt:
        print("Leitura interrompida.")


def updateData(ser: float, y_val: list[float]):
    if ser.in_waiting > 0:
        print("Valor sendo lido")
        line = ser.readline().decode("utf-8").rstrip()
        new_data = float(str(line).split(",")[1])
        target_read = float(str(line).split(",")[0])
        print(f"Dado lido da serial: {new_data}")

        if len(y_val) > AMOUNT_GRAPH:
            y_val.pop(0)
            target.pop(0)

        y_val.append(float(new_data))
        target.append(float(target_read))

    x_val = np.arange(0, len(y_val))

    return target, x_val, y_val


def animate(i: int, ser, y_val):
    target, x_val, y_val = updateData(ser, y_val)

    plt.cla()
    plt.title("Valores de RPM")
    plt.ylabel("RPM")
    plt.grid()
    plt.tight_layout()
    plt.plot(x_val, y_val, label="Mesured")
    plt.plot(x_val, target, label="Target")


def handlePlot(port: str):
    print(f"Lendo a porta {port}")

    if port == "":
        print("Porta não selecionada")
        return {"error": "Porta não selecionada"}

    try:
        ser = Serial(port, 115200)
        print("Porta sendo lida")

        y_val = []
        anim = FuncAnimation(
            plt.gcf(),
            lambda i: animate(i, ser, y_val),
            interval=50,
            cache_frame_data=False,
        )

        plt.show()
        return anim

    except SerialException as e:
        return {"error": f"Erro ao acessar a porta serial {port}: {e}"}
    except ValueError as ve:
        print(f"Erro ao processar os dados: {ve}")
        return {"error": f"Erro ao processar os dados: {ve}"}
    except Exception as e:
        return {"error": f"Ocorreu um erro inesperado: {e}"}


printOnSerial()
handlePlot("COM3")
