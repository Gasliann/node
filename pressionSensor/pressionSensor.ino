#include <Arduino.h>

const int pressureInput = 4; 
const float pressureZero = 432.0; // Lectura ADC para 0 psi (ajustar según pruebas)
const float pressureMax = 921.6; // Lectura ADC para 100 psi (ajustar según pruebas)
const float pressureTransducerMaxPsi = 100; // Presión máxima en PSI

unsigned long startTime;
unsigned long endTime;
unsigned long latency;

int pressure = 0;

void setup() {
  Serial.begin(115200);  // Inicializa la comunicación serial
}

void loop() {
  startTime = micros();  // Captura el tiempo de inicio
  pressure = analogRead(pressureInput);  // Lee el valor analógico del sensor
  endTime = micros();  // Captura el tiempo al final de la lectura

  // Calcular presión en PSI (ajustar según el sensor)
  float pressurePsi = (pressure - pressureZero) * pressureTransducerMaxPsi / (pressureMax - pressureZero);

  latency = endTime - startTime;  // Calcular la latencia de la lectura
  
  // Imprimir los resultados
  Serial.print("Presión (PSI): ");
  Serial.print(pressurePsi);
  Serial.print(" | Latencia: ");
  Serial.print(latency);
  Serial.println(" µs");

  delay(500);  // Retardo para evitar lecturas continuas sin espacio
}

