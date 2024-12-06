#include <Arduino.h>

const int flowPin = 2;  // Pin donde se conecta la señal de flujo

volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned long previousMillis = 0;
unsigned long interval = 1000;  // Medir cada segundo
unsigned long startTime;
unsigned long endTime;
unsigned long latency;

const float calibrationFactor = 4.5;  // Pulsos por segundo para un flujo de 1 L/min

void setup() {
  Serial.begin(115200);  // Inicializa la comunicación serial
  pinMode(flowPin, INPUT);  // Configura el pin de flujo como entrada
  attachInterrupt(digitalPinToInterrupt(flowPin), countPulse, FALLING);  // Interrupción para contar los pulsos
}

void loop() {
  startTime = micros();  // Captura el tiempo de inicio
  unsigned long currentMillis = millis();

  // Cada segundo, calcula el flujo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Calcular el flujo en L/min
    flowRate = pulseCount / calibrationFactor;  // Calcular el flujo en L/min
    pulseCount = 0;  // Resetear el contador de pulsos

    latency = micros() - startTime;  // Calcular la latencia de la lectura

    // Imprimir los resultados
    Serial.print("Flujo (L/min): ");
    Serial.print(flowRate);
    Serial.print(" | Latencia: ");
    Serial.print(latency);
    Serial.println(" µs");
  }
  delay(100);  // Retardo para evitar lecturas continuas sin espacio
}

void countPulse() {
  pulseCount++;
}

