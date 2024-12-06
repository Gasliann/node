#include <Arduino.h>

#define TRIG_PIN 5  // Pin TRIG del sensor conectado al pin 5 del ESP32
#define ECHO_PIN 18 // Pin ECHO del sensor conectado al pin 18 del ESP32

unsigned long startTime;
unsigned long endTime;
unsigned long latency;
long duracion;
int distancia;

void setup() {
  Serial.begin(115200);  // Inicializa la comunicación serial
  pinMode(TRIG_PIN, OUTPUT);  // Define el pin TRIG como salida
  pinMode(ECHO_PIN, INPUT);  // Define el pin ECHO como entrada
}

void loop() {
  // Envía un pulso de 10 microsegundos al pin TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  startTime = micros();  // Captura el tiempo de inicio
  duracion = pulseIn(ECHO_PIN, HIGH);  // Mide la duración del pulso en el pin ECHO
  endTime = micros();  // Captura el tiempo al final de la medición

  // Calcula la distancia en centímetros
  distancia = duracion * 0.034 / 2;

  latency = endTime - startTime;  // Calcula la latencia de la lectura

  // Imprimir los resultados
  Serial.print("Distancia (cm): ");
  Serial.print(distancia);
  Serial.print(" | Latencia: ");
  Serial.print(latency);
  Serial.println(" µs");

  delay(500);  // Retardo para evitar lecturas continuas sin espacio
}

