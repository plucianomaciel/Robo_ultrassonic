// Definições dos pinos
#define IN1 5  // Motor A - Controle de direção
#define IN2 6  // Motor A - Controle de direção
#define IN3 9  // Motor B - Controle de direção
#define IN4 10 // Motor B - Controle de direção
#define TRIG_PIN 3  // Pino TRIG do sensor ultrassônico
#define ECHO_PIN 2  // Pino ECHO do sensor ultrassônico

int distancia;
int velocidade = 120;  // Velocidade dos motores (0-255)
int dist_dir = 0;
int dist_esq = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) return 100; // Evita distância inválida
  return (duracao * 0.034) / 2;
}

void frente() {
  analogWrite(IN1, velocidade);
  analogWrite(IN2, 0);
  analogWrite(IN3, velocidade);
  analogWrite(IN4, 0);
}

void re() {
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidade);
  analogWrite(IN3, 0);
  analogWrite(IN4, velocidade);
}

void parar() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  delay(250);
}

void direita() {
  analogWrite(IN1, velocidade);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, velocidade);
  delay(320); // Giro de 90°
  parar();
}

void esquerda() {
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidade);
  analogWrite(IN3, velocidade);
  analogWrite(IN4, 0);
  delay(300); // Giro de 90°
  parar();
}

void esquerda_180() {
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidade);
  analogWrite(IN3, velocidade);
  analogWrite(IN4, 0);
  delay(600); // Giro de 180°
  parar();
}

void loop() {
  distancia = getDistance();
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if(distancia <9){
    re();
    delay(200);
  }

  if (distancia > 10) {
    frente();
  } else {
    // Parar e verificar direções
    parar();
    
    // Girar 90° à direita e medir
    direita();
    dist_dir = getDistance();
    Serial.print("Distancia Direita: ");
    Serial.print(dist_dir);
    Serial.println(" cm");
    
    // Girar 180° à esquerda (partindo da direita)
    esquerda_180();
    dist_esq = getDistance();
    Serial.print("Distancia Esquerda: ");
    Serial.print(dist_esq);
    Serial.println(" cm");
    
    // Comparar e decidir
    if (dist_dir > dist_esq) {
      direita(); // Gira 90° à direita
    } else {
      esquerda(); // Gira 90° à esquerda
    }
    frente(); // Segue em frente
  }
  delay(10);
}
