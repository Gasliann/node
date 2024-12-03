#include <Arduino.h>

// Pines para el sensor de presión
const int pressureInput = 4;  // Pin donde está conectado el sensor de presión

// Variables de calibración
const float pressureZero = 436.0; // Promedio de los valores ADC cuando no hay presión (ajustado)
const float pressureMax = 921.6;  // Lectura ADC para 100 psi (ajustado)
const float pressureTransducerMaxPSI = 100;  // Presión máxima del sensor en psi

// Variables para medir el tiempo y la latencia
unsigned long startTime;
unsigned long endTime;
unsigned long latency;

// Variables para los contadores
int pressureADC = 0;  // Variable para la lectura ADC
int pressure = 0;     // Variable para la presión en PSI

// Variables para el conteo de pérdida de datos
unsigned long lastReadingTime = 0;
unsigned long missedReadings = 0;
unsigned long readingInterval = 1000;  // Intervalo esperado entre lecturas (1 segundo)

void setup(){
  Serial.begin(115200);  // Inicializa la comunicación serial
}

void loop(){
  // Medir latencia
  startTime = micros();
  
  // Leer el valor del sensor de presión
  pressureADC = analogRead(pressureInput);
  
  // Calcular la presión en PSI usando la fórmula de calibración
  pressure = (pressureADC - pressureZero) * pressureTransducerMaxPSI / (pressureMax - pressureZero);
  
  // Evitar valores negativos
  if (pressure < 0) 
    pressure = 0;

  // Medir la latencia
  endTime = micros();
  latency = endTime - startTime;

  // Verificar si ha pasado más tiempo de lo esperado entre lecturas
  if (millis() - lastReadingTime > readingInterval) {
    missedReadings++;  // Contar la lectura perdida si no se ha llegado al intervalo esperado
    lastReadingTime = millis();
  }

  // Imprimir los resultados
  Serial.print("Latencia: ");
  Serial.print(latency);
  Serial.print(" µs | ");
  Serial.print("Valor ADC: ");
  Serial.print(pressureADC);
  Serial.print(" | ");
  Serial.print("Presión: ");
  Serial.print(pressure, 1);
  Serial.println(" PSI");

  // Imprimir estadísticas de pérdida de datos cada 5 segundos
  if (millis() % 5000 == 0) {
    Serial.print("Datos de presión perdidos: ");
    Serial.println(missedReadings);
  }

  // Retraso entre lecturas
  delay(250);  // Pequeña pausa antes de la siguiente medición
}


