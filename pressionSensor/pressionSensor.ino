#include <Arduino.h>

// Configuración del Sensor de Presión (Analógico)
const int pressureInput = 4;  // Pin analógico de presión
const float pressureZero = 446.4;  // Lectura ADC para 0 PSI (ajustar según tus mediciones)
const float pressureMax = 921.6;  // Lectura ADC para 100 PSI (ajustar según tus mediciones)
const float pressureTransducerMaxPSI = 100.0;  // Máxima presión del sensor en PSI

unsigned long pressureStartTime;
unsigned long pressureEndTime;
unsigned long pressureLatency;

void setup() {
  Serial.begin(115200);  
  pinMode(pressureInput, INPUT); 
}

void loop() {
  pressureStartTime = micros();
  float pressureReading = analogRead(pressureInput); 
  pressureEndTime = micros();
  pressureLatency = pressureEndTime - pressureStartTime;

  float pressurePSI = (pressureReading - pressureZero) * pressureTransducerMaxPSI / (pressureMax - pressureZero);
  if (pressurePSI < 0) pressurePSI = 0;

    Serial.print("Lectura: ");
    Serial.print(pressurePSI);
    Serial.println(" Psi");
    Serial.print("Latencia de lectura: ");
    Serial.print(pressureLatency);
    Serial.println(" µs");

  delay(500)
}
