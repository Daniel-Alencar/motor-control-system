import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from serial import Serial, SerialException


def atualizeData(ser: float, y_val: list[float]):
    if ser.in_waiting > 0:
        print("Valor sendo lido")
        line = ser.readline().decode("utf-8").rstrip()
        new_data = float(line)
        print(f"Dado lido da serial: {new_data}")
        y_val.append(float(new_data))

    x_val = np.arange(0, len(y_val))
    return x_val, y_val


def animate(i: int, ser, y_val):
    x_val, y_val = atualizeData(ser, y_val)

    plt.cla()
    plt.title("Valores de RPM")
    plt.ylabel("RPM")
    plt.grid()
    plt.tight_layout()
    plt.plot(x_val, y_val)


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
            lambda i: animate(ser, y_val),
            interval=50,
            cache_frame_data=False,
        )

        plt.show()
        return None

    except SerialException as e:
        return {"error": f"Erro ao acessar a porta serial {port}: {e}"}
    except ValueError as ve:
        print(f"Erro ao processar os dados: {ve}")
        return {"error": f"Erro ao processar os dados: {ve}"}
    except Exception as e:
        return {"error": f"Ocorreu um erro inesperado: {e}"}
