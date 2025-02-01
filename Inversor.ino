#define In1 25 // Pino GPIO27
#define In2 33 // Pino GPIO14
#define PWM_FREQ 16200 // Frequência do PWM em Hz
#define PWM_RESOLUTION 8 // Resolução do PWM em bits (máximo 16)

char Letra;
volatile float SENO[9]={0, 43,  84,  123, 157, 186, 208, 222, 229};
volatile byte conta=0;
volatile byte estado=0;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

unsigned int DutyCicle=125;

void timerEnd(hw_timer_t *timer);
void timerStart(hw_timer_t *timer);

void IRAM_ATTR onTimer() { // função de interrupção do timer
 //------------------------------------------------------------SENO-----------------------------------------------------------------
if(estado==0){
ledcWrite(0, SENO[conta]);  ledcWrite(1, 0);}else if(estado==1){
ledcWrite(0, SENO[17-conta]);  ledcWrite(1, 0);} else if(estado==2){
ledcWrite(0, 0);ledcWrite(1, SENO[conta-18]);}else{
ledcWrite(0, 0);ledcWrite(1, SENO[35-conta]);}
conta++;
if(conta<9){estado=0;}else if (conta<18){estado=1;} 
else if(conta<27){estado=2;}else if(conta<36){estado=3;}else{conta=0;estado=0;}
//timerEnd(timer);
//timer = NULL;
}




void setup() {
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  ledcWrite(1, DutyCicle); // Canal 1 no pino In2, ciclo de trabalho de 0%
  ledcWrite(0, DutyCicle); // Canal 1 no pino In2, ciclo de trabalho de 0%

timer = timerBegin(0, 80, true); // inicia o timer com a resolução e divisão desejadas
  timerAttachInterrupt(timer, &onTimer, true); // associa a função de interrupção ao timer
  timerAlarmWrite(timer, 500, true); // define o valor do alarme (250ms) e modo repetitivo

    // Configuração do timer para gerar PWM
  ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);
  // Anexa o pino ao canal PWM
  ledcAttachPin(In1, 0);
  ledcWrite(0, 0); // Canal 0 no pino In1, ciclo de trabalho de 0%
  ledcAttachPin(In2, 1);
  ledcWrite(1, 0); // Canal 1 no pino In2, ciclo de trabalho de 0%
  Serial.begin(115200);
}

void loop() {
    if (Serial.available() == 0) {
  } else {
    Letra = Serial.read();
    switch (Letra) {
    delay(100);
  case 'A': //Motor 1 anti-horario 
    Serial.println("A");
    timerAlarmWrite(timer, 500, true); // define o valor do alarme (10ms) e modo repetitivo
    timerAlarmEnable(timer); // habilita o alarme do timer
    break;
  case 'B': //Motor 1 anti-horario 
  ledcWrite(0, 0);ledcWrite(1, 0);
    Serial.println("B");
timerEnd(timer);
timer = NULL;
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
}
  Letra = 'X';
  }

}
