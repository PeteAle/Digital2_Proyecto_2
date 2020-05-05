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
#define TFT_RST 7 // Definir el pin 7 como el reset.
#define TFT_DC 8  // Definir pin 8 como DC.
#define TFT_CS 10 // Definir pin 10 como chip select.

#define bajarJ1 2 // Definir pin 2 como el input para bajar la barra del jugador 1.
#define subirJ1 3 // DEfinit pin 3 como el input para subir la barra del jugador 1.
#define bajarJ2 5 // Definir pin 5 como el input para bajar la barra del jugador 2.
#define subirJ2 6 // Definit pin 6 como el input para subir la barra del jugador 2.

// Pin 4 del Arduino está dañado.

// Inicializar la comunicación SPI con la pantalla TFT usando las definiciones anteriores.
TFT_ILI9341 tft = TFT_ILI9341();

int anchoPantalla = 320; //tft.width();  // Ancho de la pantalla.
int altoPantalla = 240; //tft.height();  // Alto de la pantalla.

char j1b = 0;         // Variable donde se lee el input del pin 2.
char j1s = 0;         // Variable donde se lee el input del pin 3.
char j2b = 0;         // Variable donde se lee el input del pin 5.
char j2s = 0;         // Variable donde se lee el input del pin 6.
int lastY_tabla1 = 0; // Última posición Y de la tabla del jugador 1.
int lastY_tabla2 = 0; // Última posición Y de la tabla del jugador 2.
int X_tabla1 = 5;     // Posición X de la tabla 1. 
int Y_tabla1 = 116;   // Posición Y de la tabla 1.
char tablasAncho = 10;// Ancho de las tablas.
char tablasAlto = 40; // Alto de las tablas.
int X_tabla2 = 305;   // Posición X de la tabla 2.
int Y_tabla2 = 116;   // Posición Y de la tabla 2.
char desfase = 5;
char desfaseSubida = tablasAlto - desfase;

char score1 = 0;
char score2 = 0;
char resetBit = 0;

int lastBallX = 0;
int lastBallY = 0;
int ballX = 160;
int ballY = 138;
char ballSize = 4;
int xDir = 1;
int yDir = 1;
int moverX = 0;
int moverY = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("PONG v2");

  pinMode(bajarJ1, INPUT);
  pinMode(subirJ1, INPUT);
  pinMode(bajarJ2, INPUT);
  pinMode(subirJ2, INPUT);

  tft.init();

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
  j1b = digitalRead(bajarJ1);
  j1s = digitalRead(subirJ1);
  j2b = digitalRead(bajarJ2);
  j2s = digitalRead(subirJ2);
  if (j1b == HIGH || j1s == HIGH || j2b == HIGH || j2s == HIGH){
    tft.fillScreen(ILI9341_BLACK);
    set_scoreboard();
    //delay(500); 
    while(1){
      mover_tabla1();
      mover_tabla2();
      drawBall();
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
  dibujarT1();
  //tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
  if (j1b == HIGH && Y_tabla1 < 240-tablasAlto){
    lastY_tabla1 = Y_tabla1;
    Y_tabla1 += desfase;
    tft.fillRect(X_tabla1, lastY_tabla1, tablasAncho, desfase, ILI9341_BLACK);
    tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    delay(5);
  }
  if (j1s == HIGH && Y_tabla1 > 32){
    lastY_tabla1 = Y_tabla1;
    Y_tabla1 -= desfase;
    tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla1, lastY_tabla1+desfaseSubida, tablasAncho, desfase, ILI9341_BLACK);
    delay(5);
  }
}

void mover_tabla2(){
  j2b = digitalRead(bajarJ2);
  j2s = digitalRead(subirJ2);
  dibujarT2();
  //tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
  if (j2b == HIGH && Y_tabla2 < 240-tablasAlto){
    lastY_tabla2 = Y_tabla2;
    Y_tabla2 += desfase;
    tft.fillRect(X_tabla2, lastY_tabla2, tablasAncho, desfase, ILI9341_BLACK);
    tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
    delay(2);
  }
  if (j2s == HIGH && Y_tabla2 > 32){
    lastY_tabla2 = Y_tabla2;
    Y_tabla2 -= desfase;
    tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla2, lastY_tabla2+desfaseSubida, tablasAncho, desfase, ILI9341_BLACK);
    delay(2);
  }
}

void dibujarT1(){
  tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
}

void dibujarT2(){
  tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
}

void resetBall(){
  tft.fillCircle(lastBallX, lastBallY, ballSize, ILI9341_BLACK);
  ballX = 160;
  ballY = 138;
  tft.fillCircle(ballX, ballY, ballSize, ILI9341_YELLOW);
  resetBit = 1;
  xDir *= -1;
  yDir *= -1;
  moverX = ballX;
  moverY = ballY;
  lastBallX = moverX;
  lastBallY = moverY;
  delay(500);
}

void puntoJ1(){
  tft.fillRect(40,2,27,25,ILI9341_BLACK);
  tft.setCursor(40,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score1+=1, DEC);
}

void puntoJ2(){
  tft.fillRect(265,2,27,25,ILI9341_BLACK);
  tft.setCursor(265,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score2+=1, DEC);
}

void drawBall(){
  tft.fillCircle(lastBallX, lastBallY, ballSize, ILI9341_BLACK);
  tft.fillCircle(ballX, ballY, ballSize, ILI9341_YELLOW);
  lastBallX = ballX;
  lastBallY = ballY;
  moverX = ballX + xDir;
  moverY = ballY + yDir;
  if (moverX + ballSize == anchoPantalla){
    if (score1 < 10 && score2 < 10){
      puntoJ1();
      resetBall();
      while (score1 == 10 && score2 < 10){
      // hacer print que jugador 1 es el ganador.
      // código de reset juego.
      } 
    }
  }
  else if (moverX == 0){
    if (score1 < 10 && score2 < 10){
      puntoJ2();
      resetBall();
      while (score1 < 10 && score2 == 10){
        // hacer print que jugador 2 es el ganador.
        // código de reset juego.
      }
    }
  }
  // El siguiente código será para limitar a la pelota para que rebote solamente cuando entre en contacto con la tabla del J2 (derecha).
  if (moverX > 0 && (moverX+ballSize) < anchoPantalla){
    //if (resetBit == 1){
    //  ballX = 160;
    //  ballY = 138;
    //  lastBallX = ballX;
    //  lastBallY = ballY;
    //  moverX = ballX + xDir;
    //  moverY = ballY + yDir;
    //  tft.fillCircle(lastBallX, lastBallY, ballSize, ILI9341_BLACK);
    //  resetBit = 0;
    //}
    ballX = moverX;
    delay(2);
    if ((moverX+ballSize) == X_tabla2 && moverY >= Y_tabla2 && (moverY+ballSize) <= (Y_tabla2+tablasAlto) || moverX == (X_tabla1+tablasAncho) && moverY >= Y_tabla1 && (moverY+ballSize) <= (Y_tabla1+tablasAlto)){
      xDir *= -1;
      //yDir *= -1;
      ballX += xDir;
      //ballY += yDir;
    }
    else if (moverY == 32 || (moverY+ballSize) == altoPantalla){
      yDir *= -1;
      ballY += yDir;
    }
  }
  if (moverY > 32 && (moverY+ballSize) < altoPantalla){
    delay(2);
    ballY = moverY;
    if (moverY == 32 || (moverY+ballSize) == altoPantalla){
      xDir *= -1;
      yDir *= -1;
      ballX += xDir;
      ballY += yDir;
    }
  } // Aquí termina la progra para limitar la bola al entrar en contacto con la tabla del J2 (derecha).
}
