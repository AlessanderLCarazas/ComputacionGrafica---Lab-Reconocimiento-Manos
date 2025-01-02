# Laboratorio de Reconocimiento de Gestos de Manos

## Descripción
Este proyecto implementa un sistema de reconocimiento de gestos de manos utilizando OpenCV en C++. El objetivo es identificar al menos cinco gestos diferentes (incluidos dígitos y gestos como "OK") y mostrarlos en pantalla en tiempo real.

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
