import serial
import time
import numpy as np
import matplotlib.pyplot as plt

PORT = 'COM5' 
BAUDRATE = 115200 

ser = serial.Serial(PORT, BAUDRATE)

distance_data = []
i = 0
try:
    while len(distance_data) < 100:
        i += 1
        if ser.in_waiting > 0: 
            line = ser.readline().decode('utf-8').strip()

            print(f"Línea cruda recibida: '{line}'")

            try:
                distance = float(line)
                
                distance_data.append(distance)
                
                print(f"Distancia leída {i}: {distance} cm")
            except ValueError:
                print(f"No se encontró valor válido en: {line}")
        
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Lectura interrumpida por el usuario.")

finally:
    ser.close()

mean = np.mean(distance_data)
std_dev = np.std(distance_data)

# Calcular la relación señal/ruido (SNR)
snr = mean / std_dev if std_dev != 0 else 0  # Evitar división por cero si el ruido es 0

# Crear un rango de valores centrado alrededor de la media
x_values = np.linspace(mean - 4*std_dev, mean + 4*std_dev, 1000)

# Función de la distribución normal (Gaussiana)
y_values = (1 / (std_dev * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((x_values - mean) / std_dev) ** 2)

# Graficar los datos
plt.figure(figsize=(10, 6))

# Histograma de los datos reales con normalización para la densidad
plt.hist(distance_data, bins=10, density=True, alpha=0.6, color='g', label='Datos reales')

# Curva de la distribución normal
plt.plot(x_values, y_values, color='b', label=f'Gaussiana\nMedia={mean:.2f} cm\nDesviación Estándar={std_dev:.2f}')

# Añadir una banda sombreada para el ruido (desviación estándar)
plt.fill_between(x_values, y_values, where=((x_values >= mean - std_dev) & (x_values <= mean + std_dev)), 
                 color='r', alpha=0.3, label='Ruido (±1 desviación estándar)')

# Añadir una línea vertical para marcar la media
plt.axvline(mean, color='black', linestyle='--', label=f'Media = {mean:.2f} cm')

# Etiquetas y título
plt.title('Distribución Normal (Campana de Gauss) de los Datos de Distancia')
plt.xlabel('Distancia (cm)')
plt.ylabel('Densidad de Probabilidad')
plt.legend()

# Ajustar límites en el eje Y para hacer la curva más visible
plt.ylim(0, max(y_values) * 1.1)  # Aumentar un poco la altura máxima de la curva para mayor visibilidad

# Mostrar la gráfica
plt.grid(True)
plt.show()

# Mostrar los resultados de ruido y SNR
print(f"\nEstadísticas de los datos:")
print(f"Media (señal): {mean:.2f} cm")
print(f"Desviación estándar (ruido): {std_dev:.2f} cm")
print(f"Relación señal/ruido (SNR): {snr:.2f}")