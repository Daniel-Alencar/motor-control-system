// Configurações do Motor DC
#define MOTOR_PWM_PIN 26 // Pino PWM conectado ao motor (GPIO26)
#define MOTOR_PWM_CHANNEL 0
#define MOTOR_PWM_FREQ 5000
#define MOTOR_PWM_RESOLUTION 8 // Resolução do PWM (8 bits = 0-255)

// Configurações do Encoder
#define ENCODER_PIN 14 // Pino conectado ao leitor óptico (GPIO14)
#define PPR 2          // Pulsos por rotação do motor (ajustar conforme o encoder)1/#inbox

// Variáveis para controle do motor
int pwmValue = 80; // Valor inicial do PWM (50%)
int rpmValue = 0;

// Variáveis para leitura de pulsos e cálculo de RPM
volatile unsigned long pulseCount = 0; // Contagem de pulsos
unsigned long previousMillis = 0;      // Tempo anterior
const unsigned long interval = 1000;   // Intervalo de 1 segundo para cálculo do RPM

// Função chamada em cada pulso do encoder
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup()
{
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

  ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (Serial.available() > 0)
  {
    String receivedData = Serial.readString();
    rpmValue = receivedData.toInt();
    if(rpmValue < 0) {
      Serial.println("Valor de RPM inválido, setando valor padrão para 0");
      rpmValue = 0;
    }

    pwmValue = (1.2 * rpmValue + 160) / 2.0;
    if (pwmValue < 0 || pwmValue >= 256)
    {
      Serial.println("Valor da frequência PWM inválido, setando valor padrão de 80");
      pwmValue = 80;
    }

    ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);
    Serial.printf("Novo valor para o PWM setado com sucesso %d\n", pwmValue);
  }

  // Calcular RPM a cada intervalo de 1 segundo
  if (currentMillis - previousMillis >= interval)
  {
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
    // pwmValue = (pwmValue + 10) % 256;
    // Aplica o novo valor de PWM
    // ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);
  }
}