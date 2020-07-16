#define PINO_ZC   2
#define PINO_DIM  9
#define PINO_LM35 A0
#define N         10

float temp,
      temp2;

int original,
    filtrado;
    
int numbers[N];

volatile float lum;

float setpoint = 60;
       
float lastProcess = 0,
                    e,
                    u,
                    e0,
                    u0,
                    deltaTime = 0;
                    
void setup() {
  pinMode(PINO_DIM, OUTPUT);
  pinMode(PINO_ZC, INPUT);
  pinMode(PINO_LM35, INPUT);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), ZeroCross, RISING);

}


void ZeroCross(){
  long t1 = 8200L * (100 - lum) / 100L;
  delayMicroseconds(t1);
  digitalWrite(PINO_DIM, HIGH);
  delayMicroseconds(6);
  digitalWrite(PINO_DIM, LOW);
}


void loop() {
  original = analogRead(PINO_LM35);                       //LE O VALOR DO SENSOR DE TEMPERATURA (0-1023)
  filtrado = MovingAverage(); 

  temp = (float(filtrado)*5/(1023))/0.01;                 //TRANSOFORMA VALOR PARA TEMPERATURA (0-100)
  temp2 = (float(original)*5/(1023))/0.01;

  //noInterrupts();
  e = setpoint - temp;
  u = (u0 + 0.6198*e - 0.6197*e0);
  //u = (u0 + 0.0023923*e - 0.0023923*e0);
  e0 = e;
  u0 = u;
  //u = e;
  lum = 0.970833*u;
  //lum = u;
  if (lum > 93) lum = 93;
  if (lum < 5) lum = 4;

 
  //interrupts();

  Serial.print(temp);
  Serial.print(" ");
  Serial.print(lum);
  Serial.print(" ");
  Serial.print(e);
  Serial.print(" ");
  Serial.println(u);
}


float MovingAverage(){

   for(uint8_t i = N-1; i > 0; i--) 
      numbers[i] = numbers[i-1];
    
   numbers[0] = original;
    
   float acc = 0;
    
   for(uint8_t i = 0; i < N; i++) 
      acc += numbers[i];
    
   return acc/N;
}
