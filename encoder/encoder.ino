// Configurações do Motor DC

// Pino PWM conectado ao motor (GPIO26)
#define MOTOR_PWM_PIN 26
#define MOTOR_PWM_CHANNEL 0
#define MOTOR_PWM_FREQ 5000
// Resolução do PWM (8 bits = 0-255)
#define MOTOR_PWM_RESOLUTION 8

// Configurações do Encoder
// Pino conectado ao leitor óptico (GPIO14)
#define ENCODER_PIN 14
// Pulsos por rotação do motor (ajustar conforme o encoder).
// O PPR (Pulses Per Revolution) representa a quantidade de pulsos 
// que um sensor (como um encoder óptico) gera para cada rotação completa de um eixo ou motor.
#define PPR 2      

// Variáveis para controle do motor
int pwmValue = 80; 

// Variáveis para leitura de pulsos e cálculo de RPM
// Contagem de pulsos
volatile unsigned long pulseCount = 0;  
// Tempo anterior
unsigned long previousMillis = 0;       
// Intervalo de 1 segundo para cálculo do RPM
const unsigned long interval = 1000;    

// Função chamada em cada pulso do encoder
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);

  // Configuração do pino do motor para PWM
  ledcSetup(MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION); // Configura PWM
  ledcAttachPin(MOTOR_PWM_PIN, MOTOR_PWM_CHANNEL);                    // Vincula ao pino
  ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);                             // Define PWM inicial

  // Configuração do pino do encoder como entrada
  pinMode(ENCODER_PIN, INPUT_PULLUP);
  
  // Configurar interrupção no pino do encoder
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), pulseCounter, RISING);

  Serial.println("Sistema iniciado!");
}

void loop() {
  unsigned long currentMillis = millis();

  // Calcular RPM a cada intervalo de 1 segundo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Cálculo do RPM
    unsigned long rpm = (pulseCount * 60) / (PPR);

    // Exibir o valor no monitor serial
    Serial.print("PWM: ");
    Serial.print(pwmValue);
    Serial.print(" | Pulsos: ");
    Serial.print(pulseCount);
    Serial.print(" | RPM: ");
    Serial.println(rpm);

    // Resetar a contagem de pulsos
    pulseCount = 0;

    // Atualizar o valor do PWM (opcional: para testes)
    // Incrementa PWM para observar a mudança no RPM
    pwmValue = (pwmValue + 10) % 256; 
    // Aplica o novo valor de PWM
    ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);
  }
}
