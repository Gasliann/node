import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import re  # Para manejar la expresión regular

# Configuración del puerto y la tasa de baudios (ajustados a tus necesidades)
PORT = 'COM4'  # Cambiado al puerto COM4
BAUDRATE = 115200  # Cambiado a 9600 baudios

# Inicializar el puerto serial
ser = serial.Serial(PORT, BAUDRATE)

# Lista para almacenar los últimos 100 datos de presión
pressure_data = []

# Expresión regular para extraer un número flotante antes de "Psi"
# Asegurándonos de que se manejen correctamente los valores con más de un decimal o espacios extra
pattern = re.compile(r"([0-9]+(?:\.[0-9]+)?)\s*Psi")

# Captura los datos durante 100 lecturas
try:
    while len(pressure_data) < 100:
        if ser.in_waiting > 0:  # Verifica si hay datos disponibles para leer
            # Lee una línea de datos (suponiendo que cada valor de presión se imprima en una línea separada)
            line = ser.readline().decode('utf-8').strip()
            
            # Imprimir la línea cruda recibida para depurar
            print(f"Línea cruda recibida: '{line}'")
            
            # Buscar la coincidencia de la expresión regular
            match = pattern.search(line)
            
            if match:
                # Si se encuentra un valor de presión, extraemos el número flotante
                pressure = float(match.group(1))
                
                # Añadir el valor a la lista
                pressure_data.append(pressure)

                # Mostrar el valor en consola (opcional)
                print(f"Presión leída: {pressure} Psi")
            else:
                print(f"No se encontró valor válido en: {line}")
        
        # Esperar un poco antes de volver a leer
        time.sleep(0.1)

except KeyboardInterrupt:
    # Salir del programa con Ctrl+C
    print("Lectura interrumpida por el usuario.")

finally:
    # Cerrar el puerto serial cuando termine
    ser.close()

# Calcular la media y la desviación estándar
mean = np.mean(pressure_data)
std_dev = np.std(pressure_data)

# Calcular la relación señal/ruido (SNR)
snr = mean / std_dev if std_dev != 0 else 0  # Evitar división por cero si el ruido es 0

# Crear un rango de valores centrado alrededor de la media
x_values = np.linspace(mean - 4*std_dev, mean + 4*std_dev, 1000)

# Función de la distribución normal (Gaussiana)
y_values = (1 / (std_dev * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((x_values - mean) / std_dev) ** 2)

# Graficar los datos
plt.figure(figsize=(10, 6))

# Histograma de los datos reales
plt.hist(pressure_data, bins=10, density=True, alpha=0.6, color='g', label='Datos reales')

# Curva de la distribución normal
plt.plot(x_values, y_values, color='b', label=f'Gaussiana\nMedia={mean:.2f} Psi\nDesviación Estándar={std_dev:.2f}')

# Añadir una banda sombreada para el ruido (desviación estándar)
plt.fill_between(x_values, y_values, where=((x_values >= mean - std_dev) & (x_values <= mean + std_dev)), 
                 color='r', alpha=0.3, label='Ruido (±1 desviación estándar)')

# Añadir una línea vertical para marcar la media
plt.axvline(mean, color='black', linestyle='--', label=f'Media = {mean:.2f} Psi')

# Etiquetas y título
plt.title('Distribución Normal (Campana de Gauss) de los Datos de Presión')
plt.xlabel('Presión (Psi)')
plt.ylabel('Densidad de Probabilidad')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()

# Mostrar los resultados de ruido y SNR
print(f"\nEstadísticas de los datos:")
print(f"Media (señal): {mean:.2f} Psi")
print(f"Desviación estándar (ruido): {std_dev:.2f} Psi")
print(f"Relación señal/ruido (SNR): {snr:.2f}")
