/*Juego de Pong, con funciones adicionales.
Autores: Daniel Cano, 17272; Peter Yau 17914
Componentes: 
  Pantalla de inicio
  Selección de # de jugadores.
  Tipo de juego.
    - Pong normal.
    - Pong acelerado.
    - Pong con bola que desaparece.*/

#include "SPI.h"
#include "Adafruit_GFX.h"
//#include "Adafruit_ILI9341.h"
#include "TFT_FastPin.h"
#include "TFT_ILI9341.h"
#include "User_Setup.h"

// Pines de DC y CS defaults.
#define TFT_RST 7
#define TFT_DC 8
#define TFT_CS 10

#define bajarJ1 2
#define subirJ1 3
#define bajarJ2 5
#define subirJ2 6

TFT_ILI9341 tft = TFT_ILI9341();

int ancho = tft.width();
int alto = tft.height();

char j1b = 0;
char j1s = 0;
char j2b = 0;
char j2s = 0;
int lastY_tabla1 = 0;
int lastY_tabla2 = 0;
int X_tabla1 = 5;
int Y_tabla1 = 90;
char tablasAncho = 10;
char tablasAlto = 40;
int X_tabla2 = 305;
int Y_tabla2 = 90;
char score1 = 0;
char score2 = 0;
char desfase = 5;
char desfaseSubida = tablasAlto - desfase;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("PONG v2");

  pinMode(bajarJ1, INPUT);
  pinMode(subirJ1, INPUT);
  pinMode(bajarJ2, INPUT);
  pinMode(subirJ2, INPUT);

  tft.init();

  ancho = tft.width();
  alto = tft.height();

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(115,80);
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(4);
  tft.println("PONG");
  tft.setTextSize(2);
  tft.setCursor(60,160);
  tft.println("Presione un boton");
  tft.setCursor(90,180);
  tft.println("para empezar");
  tft.setCursor(90,200);
}

void loop() {
  // put your main code here, to run repeatedly:
  j1b = digitalRead(2);
  j1s = digitalRead(3);
  if (j1b == HIGH || j1s == HIGH || j2b == HIGH || j2s == HIGH){
    tft.fillScreen(ILI9341_BLACK);
    set_scoreboard();
    delay(500); 
    while(1){
      mover_tabla1();
      mover_tabla2();
    }
  }
}

void set_scoreboard(){
  tft.setCursor(40,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score1, DEC);
  tft.setCursor(100,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println("PUNTAJE");
  tft.setCursor(265,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score2, DEC);
  tft.drawFastHLine(0,28,320,ILI9341_WHITE);
}

void mover_tabla1(){
  j1b = digitalRead(bajarJ1);
  j1s = digitalRead(subirJ1);
  tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
  if (j1b == HIGH && Y_tabla1 < 199){
    lastY_tabla1 = Y_tabla1;
    Y_tabla1 += desfase;
    tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla1, lastY_tabla1, tablasAncho, desfase, ILI9341_BLACK);
    delay(100);
  }
  if (j1s == HIGH && Y_tabla1 > 32){
    lastY_tabla1 = Y_tabla1;
    Y_tabla1 -= desfase;
    tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla1, lastY_tabla1+desfaseSubida, tablasAncho, desfase, ILI9341_BLACK);
    delay(100);
  }
}

void mover_tabla2(){
  j2b = digitalRead(bajarJ2);
  j2s = digitalRead(subirJ2);
  tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
  if (j2b == HIGH && Y_tabla2 < 199){
    lastY_tabla2 = Y_tabla2;
    Y_tabla2 += desfase;
    tft.fillRect(X_tabla2, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla2, lastY_tabla2, tablasAncho, desfase, ILI9341_BLACK);
    delay(100);
  }
  if (j2s == HIGH && Y_tabla2 > 32){
    lastY_tabla2 = Y_tabla2;
    Y_tabla2 -= desfase;
    tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla2, lastY_tabla2+desfaseSubida, tablasAncho, desfase, ILI9341_BLACK);
    delay(100);
  }
}

void dibujarT1(){
  tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
}
