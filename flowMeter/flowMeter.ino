#include <Arduino.h>

#define FLOW_PIN 4  // Pin donde está conectado el sensor de flujo

volatile int pulseCount = 0;  // Contador de pulsos
unsigned long lastTime = 0;
unsigned long interval = 5000;  // Intervalo de 5 segundos para imprimir resultados
unsigned long latency;

unsigned long receivedDataCount = 0;  // Contador de datos recibidos
unsigned long lostDataCount = 0;      // Contador de datos perdidos

void setup() {
  Serial.begin(115200);  // Inicializa la comunicación serial
  pinMode(FLOW_PIN, INPUT);  // Configura el pin del sensor de flujo como entrada
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), countPulse, FALLING);  // Interrupción por caída de flanco
}

void loop() {
  delay(10000);
  unsigned long currentMillis = millis(); // Tiempo actual

  // Calcula la latencia entre la interrupción y la lectura de pulsos
  unsigned long startTime = micros();
  // El contador de pulsos se actualiza en la interrupción
  unsigned long pulseCopy = pulseCount;
  unsigned long endTime = micros();
  
  latency = endTime - startTime;  // Calcula la latencia en microsegundos
  
  if (pulseCopy > 0) {
    // Si se ha recibido un pulso, se considera que un dato fue recibido
    receivedDataCount++;
  } else {
    // Si no se ha recibido ningún pulso, se cuenta como un dato perdido
    lostDataCount++;
  }

  // Imprime los resultados cada 5 segundos
  if (currentMillis - lastTime >= interval) {
    lastTime = currentMillis;  // Actualiza el tiempo de la última impresión
    Serial.print("Datos recibidos: ");
    Serial.println(receivedDataCount);
    Serial.print("Datos perdidos: ");
    Serial.println(lostDataCount);
    Serial.print("Latencia: ");
    Serial.print(latency);  // Muestra la latencia
    Serial.println(" µs");
  }

  delay(100);  // Pausa corta antes de la siguiente medición
}

// Función de interrupción que cuenta los pulsos del sensor de flujo
void countPulse() {
  pulseCount++;
}

