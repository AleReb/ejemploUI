# Proyecto de Control y Visualización con ESP32 y Pantalla OLED

Este proyecto utiliza un ESP32 y una pantalla OLED SH1106 para crear un sistema de control y visualización interactivo. El sistema cuenta con un menú navegable y funciones adicionales para calibración y visualización de datos de sensores táctiles.

## Características

- **Interfaz de Menú:** Navegación a través de múltiples opciones de menú utilizando botones físicos.
- **Calibración de Sensores Táctiles:** Proceso de calibración para ajustar los valores máximos y mínimos de los pines táctiles del ESP32.
- **Visualización de Gráficos:** Gráficos de barras en tiempo real que representan los valores de los sensores táctiles.
- **Texto Desplazable:** Demostración de texto desplazable en la pantalla OLED.
- **Reinicio del Sistema:** Opción para reiniciar el ESP32 desde el menú.
- **Pantalla Animada:** Animación de un logotipo deslizándose hacia abajo al inicio.

## Hardware Necesario

- ESP32
- Pantalla OLED SH1106 (128x64 píxeles)
- Botones físicos conectados a los pines 15, 16, 5, 17 y 4
- Pines táctiles T4, T5, T6 y T7 del ESP32

## Configuración

1. **Abrir el proyecto en el IDE Arduino.**

2. **Instalar las bibliotecas necesarias:**
   - [U8g2](https://github.com/olikraus/u8g2) (versión 2.34.22) para la pantalla OLED.
   - [OneButton](https://github.com/mathertel/OneButton) (versión 2.5.0) para la gestión de botones.
   - ESP32 (versión 2.0.11) en el gestor de placas del IDE Arduino.

3. **Configurar los pines de acuerdo a tu hardware:**
   - Modificar los pines de los botones y pines táctiles en el código si es necesario.

## Uso

1. **Cargar el código en el ESP32.**

2. **Interacción con el menú:**
   - Utiliza los botones para navegar por el menú.
   - Realiza una pulsación larga para activar la función seleccionada en el menú.

## Funciones del Menú

- **Menu 1: Cuenta regresiva:** Muestra una cuenta regresiva de 30 segundos con gráficos de barras para los valores táctiles.
- **Menu 2: Texto desplazable:** Muestra un texto desplazable durante 30 segundos.
- **Menu 3: Calibración:** Calibra los sensores táctiles para obtener los valores máximos y mínimos.
- **Menu 4:** Espacio para una futura función.
- **Menu 5:** Espacio para una futura función.
- **Menu 6: Reiniciar ESP:** Reinicia el ESP32.
- **Menu 7: Desactivar menú:** Desactiva el menú y vuelve a la pantalla de prueba de botones y valores táctiles.

## Créditos

- Librería [U8g2](https://github.com/olikraus/u8g2) por olikraus.
- Librería [OneButton](https://github.com/mathertel/OneButton) por Matthias Hertel.
- Desarrollado utilizando el IDE Arduino y un ESP32.
