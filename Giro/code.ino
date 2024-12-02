// Definição dos pinos
#define MOTOR_ENABLE_PIN  25 // Pino de habilitação (EN)
#define MOTOR_PWM_PIN     26 // Pino PWM para controle de velocidade
#define MOTOR_DIR_PIN     27 // Pino para controle de direção

// Configuração do PWM
#define PWM_CHANNEL       0   // Canal PWM do ESP32
#define PWM_FREQUENCY  5000   // Frequência do PWM (5 kHz)
#define PWM_RESOLUTION    8   // Resolução do PWM (8 bits: 0-255)

void setup() {
  // Configurar os pinos como saída
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);

  // Configurar o canal PWM
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_PWM_PIN, PWM_CHANNEL);

  // Habilitar o motor
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
}

void loop() {
  // Rotação no sentido horário
  digitalWrite(MOTOR_DIR_PIN, HIGH);
  // Velocidade média (50%)
  setMotorSpeed(50);  
  delay(3000);

  // Rotação no sentido anti-horário
  digitalWrite(MOTOR_DIR_PIN, LOW);
  // Velocidade alta (~78%)
  setMotorSpeed(200);
  delay(3000);

  // Parar o motor
  // Duty cycle = 0 (parado)
  setMotorSpeed(0);
  delay(2000);
}

// Função para ajustar a velocidade do motor
void setMotorSpeed(int dutyCycle) {
  ledcWrite(PWM_CHANNEL, dutyCycle);
}
