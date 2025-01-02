# Laboratorio de Reconocimiento de Gestos de Manos

## Descripción
Este proyecto implementa un sistema de reconocimiento de gestos de manos utilizando OpenCV en C++. El objetivo es identificar al menos cinco gestos diferentes (incluidos dígitos y gestos como "OK") y mostrarlos en pantalla en tiempo real.
![image](https://github.com/user-attachments/assets/b63408d4-75d8-498a-8417-a2e3b00b973a)


## Funcionalidades
1. **Preprocesamiento de la imagen**: 
   - Conversión al espacio de color HSV.
   - Segmentación de piel mediante umbrales.
   - Aplicación de operaciones morfológicas para limpiar la máscara.
   - Suavizado de la máscara con un filtro gaussiano.

2. **Detección de contornos y convexidad**:
   - Identificación de contornos de la mano.
   - Cálculo de defectos de convexidad para analizar la forma de la mano.

3. **Reconocimiento de gestos**:
   - Clasificación de gestos basados en el número de defectos de convexidad detectados.
   - Implementación de lógica específica para gestos como "OK" o números del 1 al 5.

4. **Visualización**:
   - Dibujado de puntos y líneas en la imagen para destacar las características detectadas.
   - Mostrar en pantalla el gesto reconocido junto con el video original y la máscara.

## Gestos Reconocidos
- Mano cerrada
- Número 1
- Número 2
- Número 3
- Mano abierta
- Gesto "OK"

## Requisitos
- OpenCV 4.x
- Compilador compatible con C++
- Cámara web

![image](https://github.com/user-attachments/assets/662d4c69-75ce-43a1-bf58-ce1e47edd819)
![image](https://github.com/user-attachments/assets/7a366025-bff5-4a1b-8b60-f3f978a85982)
![image](https://github.com/user-attachments/assets/59349e1c-5111-4f25-92c3-943533bfc4a1)
![image](https://github.com/user-attachments/assets/8e8138c9-14bb-4c70-8b4f-af317844fa0c)






