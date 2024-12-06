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
// Pulsos por rotação do motor (ajustar conforme o encoder)
#define PPR 2          

// Variáveis para controle do motor
int pwmValue = 80;
int rpmValue = 0;

// Variáveis para leitura de pulsos e cálculo de RPM
// Contagem de pulsos
volatile unsigned long pulseCount = 0; 
// Tempo anterior
unsigned long previousMillis = 0;      
// Intervalo de 1 segundo para cálculo do RPM
const unsigned long interval = 1000; 

// Constantes do PID
float Kp = 1.0;  // Ganho proporcional
float Ki = 0.5;  // Ganho integral
float Kd = 0.2;  // Ganho derivativo

// Variáveis do PID
float integral = 0;
float previousError = 0;


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
  // Configura PWM
  ledcSetup(MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION); 
  // Vincula ao pino
  ledcAttachPin(MOTOR_PWM_PIN, MOTOR_PWM_CHANNEL);                    
  // Define PWM inicial
  ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);                             

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

  // Receber novo RPM desejado pela Serial
  if (Serial.available() > 0)
  {
    String receivedData = Serial.readString();
    rpmValue = receivedData.toInt();
    if (rpmValue < 0)
    {
      Serial.println("Valor de RPM inválido, setando valor padrão para 0");
      rpmValue = 0;
    }
  }

  // Calcular RPM real a cada intervalo
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    // Cálculo do RPM real
    unsigned long rpmMeasured = (pulseCount * 60) / PPR;

    // Calcular o erro
    float error = rpmValue - rpmMeasured;

    // Componente proporcional
    float proportional = Kp * error;

    // Componente integral
    integral += error * (interval / 1000.0);
    float integralTerm = Ki * integral;

    // Componente derivativo
    float derivative = (error - previousError) / (interval / 1000.0);
    float derivativeTerm = Kd * derivative;

    // Calcular o valor do PWM
    float pwmAdjustment = proportional + integralTerm + derivativeTerm;

    // Atualizar o PWM
    pwmValue += pwmAdjustment;
    if (pwmValue < 0)
      pwmValue = 0;
    if (pwmValue > 255)
      pwmValue = 255;

    ledcWrite(MOTOR_PWM_CHANNEL, pwmValue);

    // Exibir informações no monitor serial
    Serial.print("RPM desejado: ");
    Serial.print(rpmValue);
    Serial.print(" | RPM medido: ");
    Serial.print(rpmMeasured);
    Serial.print(" | Erro: ");
    Serial.print(error);
    Serial.print(" | PWM: ");
    Serial.println(pwmValue);

    // Atualizar o erro anterior
    previousError = error;

    // Resetar contagem de pulsos
    pulseCount = 0;
  }
}
