#include <Arduino.h>

#define TRIG_PIN 5   // Pin TRIG del sensor conectado al pin 5 del ESP32
#define ECHO_PIN 18  // Pin ECHO del sensor conectado al pin 18 del ESP32

unsigned long startTime;
unsigned long endTime;
unsigned long latency;

unsigned long receivedDataCount = 0;  // Contador de datos recibidos
unsigned long lostDataCount = 0;      // Contador de datos perdidos

unsigned long lastTime = 0;
unsigned long interval = 5000;  // Intervalo de 5 segundos para imprimir resultados

void setup() {
  Serial.begin(115200); // Inicializa la comunicación serial
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  unsigned long currentMillis = millis(); // Tiempo actual

  // Mide la latencia de la lectura del sensor ultrasónico
  startTime = micros();
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  endTime = micros();

  if (duration > 0) {
    // Si se recibe una duración válida, se considera que un dato ha sido recibido
    receivedDataCount++;
  } else {
    // Si no se recibe una duración válida, se cuenta como un dato perdido
    lostDataCount++;
  }

  latency = endTime - startTime;  // Calcula la latencia en microsegundos

  // Imprime los datos cada 5 segundos
  if (currentMillis - lastTime >= interval) {
    lastTime = currentMillis; // Actualiza el tiempo de la última impresión
    Serial.print("Datos recibidos: ");
    Serial.println(receivedDataCount);
    Serial.print("Datos perdidos: ");
    Serial.println(lostDataCount);
    Serial.print("Latencia: ");
    Serial.print(latency);  // Muestra la latencia
    Serial.println(" µs");
  }

  // Pausa corta antes de la siguiente medición
  delay(100);
}


