import sys
from PyQt5.QtWidgets import (
    QApplication,
    QWidget,
    QPushButton,
    QLabel,
    QVBoxLayout,
    QGridLayout,
    QFrame,
)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont
from plot import handlePlot
from auxiliary import formatText


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.port = ""  # Porta selecionada

        # Configurações da janela principal
        self.setWindowTitle("Seleção de Porta COM")
        self.setGeometry(100, 100, 1280, 720)

        # Layout principal
        layout = QVBoxLayout()

        # Rótulo que exibe a porta selecionada
        self.label = QLabel("Nenhuma porta selecionada")
        self.label.setFont(QFont("Arial", 14))
        layout.addWidget(self.label, alignment=Qt.AlignCenter)

        # Frame para os botões
        screen = QFrame(self)
        screen_layout = QVBoxLayout(screen)
        layout.addWidget(screen)

        # Grid de botões para as portas COM
        grid_port = QGridLayout()
        screen_layout.addLayout(grid_port)

        for i in range(1, 13):
            button = QPushButton(f"COM{i}")
            button.setFixedSize(150, 60)
            button.setFont(QFont("Arial", 12))
            button.setStyleSheet(
                """
                QPushButton {
                    background-color: #e0f7fa;
                    border-radius: 15px;
                    border: 2px solid #4caf50;
                }
                QPushButton:hover {
                    background-color: #b2ebf2;
                }
            """
            )
            button.clicked.connect(lambda _, i=i: self.set_port(f"COM{i}"))
            grid_port.addWidget(button, (i - 1) // 4, (i - 1) % 4)

        # Botão principal para gerar o gráfico
        self.mainButton = QPushButton("Gerar Gráfico")
        self.mainButton.setFixedSize(300, 80)
        self.mainButton.setFont(QFont("Arial", 16, QFont.Bold))
        self.mainButton.setStyleSheet(
            """
            QPushButton {
                background-color: #64b5f6;
                color: white;
                border-radius: 25px;
            }
            QPushButton:hover {
                background-color: #42a5f5;
            }
        """
        )
        self.mainButton.clicked.connect(self.generate_plot)
        screen_layout.addWidget(self.mainButton, alignment=Qt.AlignCenter)

        self.setLayout(layout)

    def set_port(self, value):
        self.port = value
        self.label.setText(f"Porta selecionada: {self.port}")

    def generate_plot(self):
        if self.port:
            result = handlePlot(self.port)

        if result is not None and isinstance(result, dict) and "error" in result:
            self.label.setText(formatText(result["error"]))


# Executa o aplicativo
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
