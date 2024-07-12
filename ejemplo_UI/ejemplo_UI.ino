#include <Wire.h>
#include <U8g2lib.h>
#include "OneButton.h"

// Inicializar la pantalla
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); // pantalla grande revisar de todas formas porque tiene un pequeño despalazamiento de 2 pixeles
// Configuración de la pantalla OLED
//U8G2_SH1107_SEEED_128X128_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); // pantalla cuadrada
// Configuración del botón
#define BUTTON_PIN 15  // Centro

OneButton btn = OneButton(BUTTON_PIN, true);

volatile bool buttonFlag = false;  // Flag para la ISR
void IRAM_ATTR checkTicks() {
  buttonFlag = true;  // Establece el flag para manejar en el loop
}

int indice = 0;      // Para menú
int lastIndex = -1;  // Inicializar con un valor que no sea un índice válido
bool menu = false;

// Definir los pines de los botones
int centerButtonPin = 15;
const int upButtonPin = 16;
const int downButtonPin = 5;
const int leftButtonPin = 17;
const int rightButtonPin = 4;

// Definir los pines táctiles y otros pines
const int touchPin1 = T4;  // Pin físico 13
const int touchPin2 = T5;  // Pin físico 12
const int touchPin3 = T6;  // Pin físico 14
const int touchPin4 = T7;  // Pin físico 27

// Variables de calibración
int maxTouchValue1, maxTouchValue2, maxTouchValue3, maxTouchValue4;
int minTouchValue1, minTouchValue2, minTouchValue3, minTouchValue4;

// Estado de los botones
bool centerButtonState = false;
bool upButtonState = false;
bool downButtonState = false;
bool leftButtonState = false;
bool rightButtonState = false;

// Definir la imagen (logo) a mostrar
#define logoc_width 60
#define logoc_height 50
static unsigned char logoc_bits[] = {
  0xff, 0x8f, 0xff, 0xff, 0xff, 0xf1, 0xff, 0x0f, 0xff, 0x03, 0xfc, 0xff,
  0x7f, 0xc0, 0xff, 0x0f, 0xff, 0x01, 0xf8, 0xff, 0x1f, 0x80, 0xff, 0x0f,
  0xff, 0x00, 0xe0, 0xff, 0x07, 0x00, 0xff, 0x0f, 0x7f, 0x00, 0x00, 0xff,
  0x00, 0x00, 0xfe, 0x0f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f,
  0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x7f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfe, 0x0f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f,
  0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xfe, 0x0f, 0xff, 0x00, 0xe0, 0xff,
  0x07, 0x00, 0xff, 0x0f, 0xff, 0x01, 0xf8, 0xff, 0x1f, 0x80, 0xff, 0x0f,
  0xff, 0x03, 0xfe, 0xff, 0x7f, 0xc0, 0xff, 0x0f, 0xff, 0x8f, 0xff, 0xff,
  0xff, 0xf1, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
  0x3f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xcf, 0x0f, 0x07, 0xf8, 0xff, 0x00,
  0xff, 0xff, 0x03, 0x0e, 0x03, 0xf0, 0x7f, 0x00, 0xfe, 0xff, 0x00, 0x0c,
  0x01, 0xe0, 0x3f, 0x00, 0xf8, 0x7f, 0x00, 0x08, 0x01, 0xe0, 0x3f, 0x00,
  0xc0, 0x0f, 0x30, 0x08, 0x01, 0xe0, 0x3f, 0x00, 0x00, 0x00, 0x30, 0x00,
  0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0xc0, 0x1f, 0x00,
  0x00, 0x00, 0xfc, 0x01, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x30, 0x00,
  0x01, 0xe0, 0x3f, 0x00, 0x80, 0x07, 0x30, 0x00, 0x01, 0xe0, 0x3f, 0x00,
  0xf8, 0x3f, 0x20, 0x08, 0x03, 0xf0, 0x7f, 0x00, 0xfe, 0xff, 0x00, 0x0c,
  0x07, 0xf8, 0xff, 0x00, 0xff, 0xff, 0x01, 0x0e, 0x1f, 0xfe, 0xff, 0xc3,
  0xff, 0xff, 0x87, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
  0xff, 0xdf, 0xff, 0xff, 0xff, 0xfb, 0xff, 0x0f, 0xff, 0x03, 0xfe, 0xff,
  0x7f, 0xc0, 0xff, 0x0f, 0xff, 0x01, 0xf8, 0xff, 0x1f, 0x80, 0xff, 0x0f,
  0xff, 0x00, 0xe0, 0xff, 0x07, 0x00, 0xff, 0x0f, 0xff, 0x00, 0x80, 0xff,
  0x01, 0x00, 0xfe, 0x0f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f,
  0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x7f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfe, 0x0f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f,
  0x7f, 0x00, 0x00, 0x3c, 0x00, 0x00, 0xfe, 0x0f, 0xff, 0x00, 0xc0, 0xff,
  0x07, 0x00, 0xff, 0x0f, 0xff, 0x01, 0xf8, 0xff, 0x1f, 0x00, 0xff, 0x0f,
  0xff, 0x03, 0xfc, 0xff, 0x3f, 0xc0, 0xff, 0x0f, 0xff, 0x0f, 0xff, 0xff,
  0xff, 0xe0, 0xff, 0x0f
};

void setup() {
  Serial.begin(115200);
  // Inicializar la pantalla
  u8g2.begin();

  // Rotar la pantalla (ejemplo de 180 grados, puedes ajustar según lo necesario)
  u8g2.setDisplayRotation(U8G2_R2);  // R0 = 0 grados, R1 = 90 grados, R2 = 180 grados, R3 = 270 grados

  // Configurar los pines de los botones como INPUT_PULLUP
  pinMode(centerButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  // Mostrar animación de imagen deslizándose hacia abajo
  u8g2.setDrawColor(0);  // Establece el color de dibujo a negro
  for (int y = -logoc_height; y <= 64; y++) {
    u8g2.clearBuffer();
    u8g2.drawXBMP((128 - logoc_width) / 2, y, logoc_width, logoc_height, logoc_bits);
    u8g2.sendBuffer();
    delay(20);
  }
  u8g2.setDrawColor(1);  // Establece el color de dibujo a blanco

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), checkTicks, CHANGE);
  btn.setPressTicks(3000);  // Tiempo en milisegundos para considerar una pulsación larga (3000 ms)
  btn.attachClick(clickFunction);
  btn.attachLongPressStart(longPressStartFunction);
}

void loop() {
  btn.tick();        // Maneja el estado del botón
  if (buttonFlag) {  // Uso de flags para el interruptor así evitar reseteo del esp32
    btn.tick();      // Acción del botón
    Serial.println("Entre en el flag");
    if (menu == false) {
      clickFunction();
    }
    delay(50);
    buttonFlag = false;  // Reinicia el flag
  }
  // Leer el estado de los botones
  centerButtonState = !digitalRead(centerButtonPin);  // Leer estado del botón (invertido por INPUT_PULLUP)
  upButtonState = !digitalRead(upButtonPin);
  downButtonState = !digitalRead(downButtonPin);
  leftButtonState = !digitalRead(leftButtonPin);
  rightButtonState = !digitalRead(rightButtonPin);

  // Leer los valores táctiles
  int touchValue1 = touchRead(touchPin1);
  int touchValue2 = touchRead(touchPin2);
  int touchValue3 = touchRead(touchPin3);
  int touchValue4 = touchRead(touchPin4);

  if (lastIndex != indice) {
    Serial.println("Current Index after increment: " + String(indice));
    lastIndex = indice;  // Actualizar el último índice
  }
  if (upButtonState == true && menu) {
    indice = indice - 1;
    if (indice <= 0) {
      indice = 0;
    }
    delay(100);
  }
  if (downButtonState == true && menu) {
    indice = indice + 1;
    if (indice >= 6) {
      indice = 6;
    }
    delay(100);
  }
  if (menu) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x7_tr);  // Fuente más pequeña
    const char* menuItems[] = {
      "Menu 1: Cuenta regresiva",
      "Menu 2: Texto desplazable",
      "Menu 3: Calibracion",
      "Menu 4: funcion4",
      "Menu 5: funcion5",
      "Menu 6: Reiniciar ESP",
      "Menu 7: Desactivar menu"
    };
    for (int i = 0; i < 7; i++) {
      if (i == indice) {
        u8g2.setCursor(1, 8 * (i + 1));
        u8g2.print(">");
      }
      u8g2.setCursor(8, 8 * (i + 1));
      u8g2.print(menuItems[i]);
    }
    u8g2.sendBuffer();
  } else {
    // Mostrar el estado de los botones en la pantalla
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);  // Elegir una fuente adecuada
    u8g2.setCursor(4, 8);
    u8g2.print("Test");
    u8g2.setFont(u8g2_font_unifont_t_symbols);  // Unicode
    u8g2.drawUTF8(30, 12, "☀ ☁");
    u8g2.setFont(u8g2_font_5x7_tr);  // Fuente más pequeña
    // Al usar print, se mantiene correlativo el orden hasta un set cursor nuevo
    u8g2.setCursor(4, 20);
    u8g2.print("T4 ");
    u8g2.print(touchValue1);
    u8g2.print(" T5 ");
    u8g2.print(touchValue2);
    u8g2.print(" T6 ");
    u8g2.print(touchValue3);
    u8g2.print(" T7 ");
    u8g2.print(touchValue4);

    u8g2.setCursor(4, 30);
    u8g2.print("Center Button: ");
    u8g2.print(centerButtonState ? "Pressed" : "Released");

    u8g2.setCursor(4, 38);
    u8g2.print("Up Button: ");
    u8g2.print(upButtonState ? "Pressed" : "Released");

    u8g2.setCursor(4, 46);
    u8g2.print("Down Button: ");
    u8g2.print(downButtonState ? "Pressed" : "Released");

    u8g2.setCursor(4, 54);
    u8g2.print("Left Button: ");
    u8g2.print(leftButtonState ? "Pressed" : "Released");

    u8g2.setCursor(4, 62);
    u8g2.print("Right Button: ");
    u8g2.print(rightButtonState ? "Pressed" : "Released");

    u8g2.sendBuffer();
  }
}

// Funciones para el menú y clics

// Función para manejar el inicio de una pulsación larga para entrar en funciones
void longPressStartFunction() {
  Serial.println("Botón pulsado largo");
  if (menu) {
    switch (indice) {
      case 0:
        Serial.println("Menu 1: Cuenta regresiva");
        countDownAndGraph();
        break;
      case 1:
        Serial.println("Menu 2: Texto desplazable");
        scrollText();
        break;
      case 2:
        Serial.println("Menu 3: Calibracion");
        calibrateTouch();
        break;
      case 3:
        Serial.println("Menu 4");
        break;
      case 4:
        Serial.println("Menu 5");
        break;
      case 5:
        Serial.println("Reiniciar ESP");
        delay(500);
        ESP.restart();
        break;
      case 6:
        Serial.println("Desactivando menu");
        indice = -1;
        menu = false;
        break;
    }
  } else {
    Serial.println("Activando menu");
    menu = true;
  }

  if (lastIndex != indice) {
    Serial.println(menu ? "Menu Activo" : "Menu Inactivo");
    Serial.println("Índice Actual: " + String(indice));
    lastIndex = indice;  // Actualizar el último índice
  }
}

// Funciones clic
void clickFunction() {
  Serial.println("Clicked!");
  Serial.println("Current Index before increment: " + String(indice));
  indice = indice + 1;
  if (indice > 6) {
    indice = 0;
  }
  if (lastIndex != indice) {
    Serial.println("Current Index after increment: " + String(indice));
    lastIndex = indice;  // Actualizar el último índice
  }
}

// Función de cuenta regresiva y gráfico de barras
void countDownAndGraph() {

  unsigned long startTime = millis();
  while (millis() - startTime < 30000) {
    int touchValue1 = touchRead(touchPin1);
    int touchValue2 = touchRead(touchPin2);
    int touchValue3 = touchRead(touchPin3);
    int touchValue4 = touchRead(touchPin4);

    int barHeight1 = map(touchValue1, minTouchValue1, maxTouchValue1, 60, 0);
    int barHeight2 = map(touchValue2, minTouchValue2, maxTouchValue2, 60, 0);
    int barHeight3 = map(touchValue3, minTouchValue3, maxTouchValue3, 60, 0);
    int barHeight4 = map(touchValue4, minTouchValue4, maxTouchValue4, 60, 0);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(4, 8);
    u8g2.print("Cuenta regresiva: ");
    u8g2.print(30 - (millis() - startTime) / 1000);

    // Dibujar las barras
    u8g2.drawBox(20, 64 - barHeight1, 10, barHeight1);
    u8g2.drawBox(40, 64 - barHeight2, 10, barHeight2);
    u8g2.drawBox(60, 64 - barHeight3, 10, barHeight3);
    u8g2.drawBox(80, 64 - barHeight4, 10, barHeight4);

    u8g2.sendBuffer();
    delay(100);
  }
  menu = true;  // Volver al menú
}

// Función de texto desplazable
void scrollText() {
  int x = 128;
  unsigned long startTime = millis();
  while (millis() - startTime < 30000) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(x + 3, 32, "Esta es una prueba");
    u8g2.sendBuffer();
    x -= 2;
    if (x < -120) {
      x = 128;
    }
    delay(50);
  }
  menu = true;  // Volver al menú
}

// Función de calibración de los pines táctiles
void calibrateTouch() {
  maxTouchValue1 = touchRead(touchPin1);
  maxTouchValue2 = touchRead(touchPin2);
  maxTouchValue3 = touchRead(touchPin3);
  maxTouchValue4 = touchRead(touchPin4);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.setCursor(4, 20);
  u8g2.print("Calibracion: No tocar");
  u8g2.sendBuffer();
  delay(5000);

  maxTouchValue1 = touchRead(touchPin1);
  maxTouchValue2 = touchRead(touchPin2);
  maxTouchValue3 = touchRead(touchPin3);
  maxTouchValue4 = touchRead(touchPin4);

  u8g2.clearBuffer();
  u8g2.setCursor(4, 20);
  u8g2.print("Calibracion: Tocar");
  u8g2.sendBuffer();
  delay(5000);

  minTouchValue1 = touchRead(touchPin1);
  minTouchValue2 = touchRead(touchPin2);
  minTouchValue3 = touchRead(touchPin3);
  minTouchValue4 = touchRead(touchPin4);

  u8g2.clearBuffer();
  u8g2.setCursor(4, 20);
  u8g2.print("Calibracion Completa");
  u8g2.sendBuffer();
  delay(2000);

  menu = true;  // Volver al menú
}

