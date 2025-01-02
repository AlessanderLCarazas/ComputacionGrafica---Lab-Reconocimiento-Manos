#include <opencv2/opencv.hpp>
#include <iostream>

// Función para preprocesar la imagen, detectando la piel en la imagen
cv::Mat preprocessImage(const cv::Mat& frame) {
    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); // Convertir la imagen de BGR a HSV
    cv::Scalar lower_skin(0, 20, 70); // Umbral inferior para detectar piel
    cv::Scalar upper_skin(20, 255, 255); // Umbral superior para detectar piel
    cv::inRange(hsv, lower_skin, upper_skin, mask); // Crear una máscara donde se detecta la piel
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2); // Erosionar la máscara para eliminar ruido
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2); // Dilatar la máscara para restaurar el tamaño original
    cv::GaussianBlur(mask, mask, cv::Size(3, 3), 0); // Aplicar un desenfoque gaussiano para suavizar la imagen
    return mask; // Devolver la máscara procesada
}

// Función para reconocer gestos de la mano basándose en la máscara procesada
std::string recognizeHandGesture(const cv::Mat& mask, cv::Mat& output) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE); // Encontrar contornos en la máscara

    if (contours.empty()) { // Si no se encuentran contornos
        return "No seña detectada"; // Devolver mensaje de no detección
    }

    std::vector<cv::Point> largest_contour = contours[0]; // Suponemos que el primer contorno es el mayor
    for (size_t i = 1; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) > cv::contourArea(largest_contour)) { // Comparar áreas de contornos
            largest_contour = contours[i]; // Actualizar el contorno más grande
        }
    }

    std::vector<int> hull; // Contendrá los índices de los puntos que forman el convex hull
    cv::convexHull(largest_contour, hull, true); // Calcular el convex hull del contorno más grande

    std::vector<cv::Vec4i> defects; // Contendrá los defectos de convexidad
    cv::convexityDefects(largest_contour, hull, defects); // Calcular los defectos de convexidad

    // Dibujar puntos y líneas
    /*
    for (size_t i = 0; i < hull.size(); i++) {
        cv::circle(output, largest_contour[hull[i]], 5, cv::Scalar(0, 0, 255), -1); // Puntos rojos
        if (i > 0) {
            cv::line(output, largest_contour[hull[i - 1]], largest_contour[hull[i]], cv::Scalar(0, 255, 0), 2); // Líneas verdes
        }
    }

    if (!hull.empty()) {
        cv::line(output, largest_contour[hull[hull.size() - 1]], largest_contour[hull[0]], cv::Scalar(0, 255, 0), 2); // Cerrar la mano
    }
    */

    // Dibujar contorno de la mano
    cv::drawContours(output, std::vector<std::vector<cv::Point>>{largest_contour}, -1, cv::Scalar(0, 255, 0), 2);

    int num_defects = 0; // Contador para el número de defectos de convexidad
    std::vector<cv::Point> fingerTips; // Para almacenar las puntas de los dedos
    for (const auto& defect : defects) {
        float depth = defect[3] / 256.0; // Calcular la profundidad del defecto
        if (depth > 20) { // Si la profundidad es mayor que un umbral
            num_defects++; // Incrementar el contador de defectos
            cv::Point start = largest_contour[defect[0]]; // Punto de inicio del defecto
            cv::Point end = largest_contour[defect[1]]; // Punto final del defecto
            cv::Point far = largest_contour[defect[2]]; // Punto más lejano del defecto

            fingerTips.push_back(start); // Agregar punto de inicio a las puntas de los dedos
            fingerTips.push_back(end); // Agregar punto final a las puntas de los dedos

            // Dibujar las líneas verdes entre los puntos de inicio y fin del defecto
            cv::line(output, start, far, cv::Scalar(0, 255, 0), 2); // Línea verde
            cv::line(output, far, end, cv::Scalar(0, 255, 0), 2); // Línea verde

            // Dibujar un punto rojo en la articulación
            cv::circle(output, far, 5, cv::Scalar(0, 0, 255), -1); // Punto rojo
        }
    }

    // Basado en el número de defectos de convexidad y posición de los dedos, decidir la seña
    if (num_defects == 0) {
        return "Mano cerrada"; // Si no hay defectos, la mano está cerrada
    }
    else if (num_defects == 1) {
        return "Numero 1"; // Si hay un defecto, es el número 1
    }
    else if (num_defects == 2) {
        // Añadiendo lógica para detectar la seña "OK"
        if (fingerTips.size() == 4) { // Comprobar si hay cuatro puntas de dedos
            cv::Point a = fingerTips[0];
            cv::Point b = fingerTips[1];
            cv::Point c = fingerTips[2];
            cv::Point d = fingerTips[3];
            double ab = cv::norm(a - b);
            double cd = cv::norm(c - d);
            if (ab > 20 && cd > 20) { // Si las distancias entre los puntos son significativas
                return "OK"; // Es el gesto "OK"
            }
        }
        return "Numero 2"; // Si no, es el número 2
    }
    else if (num_defects == 3) {
        return "Numero 3"; // Si hay tres defectos, es el número 3
    }
    else if (num_defects == 4) {
        return "Mano abierta"; // Si hay cuatro defectos, la mano está abierta
    }
    else {
        return "No reconocida"; // Si no se reconoce la seña
    }
}

int main() {
    cv::VideoCapture cap(0); // Abrir la cámara predeterminada
    if (!cap.isOpened()) { // Verificar si la cámara se abrió correctamente
        std::cerr << "Error al abrir la camara" << std::endl; // Mostrar error si no se abrió
        return -1; // Salir del programa
    }

    cv::Mat frame;
    while (true) {
        cap >> frame; // Capturar un frame de la cámara
        if (frame.empty()) { // Verificar si el frame está vacío
            break; // Salir del bucle si el frame está vacío
        }

        cv::Mat preprocessed = preprocessImage(frame); // Preprocesar la imagen
        cv::Mat output = frame.clone(); // Copiar el frame original para dibujar

        std::string gesture = recognizeHandGesture(preprocessed, output); // Reconocer el gesto de la mano

        cv::putText(output, gesture, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); // Mostrar el gesto reconocido en la imagen
        cv::imshow("Frame", output); // Mostrar la imagen con el gesto reconocido
        cv::imshow("Mask", preprocessed); // Mostrar la máscara preprocesada

        if (cv::waitKey(30) >= 0) { // Esperar 30ms y salir del bucle si se presiona una tecla
            break;
        }
    }

    cap.release(); // Liberar la cámara
    cv::destroyAllWindows(); // Cerrar todas las ventanas
    return 0;
}