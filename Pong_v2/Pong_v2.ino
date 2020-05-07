/*Juego de Pong, con funciones adicionales.
Autores: Daniel Cano, 17272; Peter Yau 17914
Componentes: 
  Pantalla de inicio
  Selección de # de jugadores.
  Tipo de juego.
    - Pong normal.
    - Pong acelerado.
    - Pong con bola que desaparece.*/

//----------------------------------------------------------- Includes de las bibliotecas -------------------------------------------------------------------

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "TFT_FastPin.h"
#include "TFT_ILI9341.h"
#include "User_Setup.h"
#include "pitches.h"
// Pines de DC y CS defaults.
#define TFT_RST 7 // Definir el pin 7 como el reset.
#define TFT_DC 8  // Definir pin 8 como DC.
#define TFT_CS 10 // Definir pin 10 como chip select.


#define bajarJ1 2 // Definir pin 2 como el input para bajar la barra del jugador 1.
#define subirJ1 3 // DEfinit pin 3 como el input para subir la barra del jugador 1.
#define bajarJ2 5 // Definir pin 5 como el input para bajar la barra del jugador 2.
#define subirJ2 6 // Definit pin 6 como el input para subir la barra del jugador 2.

//------------------------ Inicializar la comunicación SPI con la pantalla TFT usando las definiciones anteriores. --------------------------------------------

TFT_ILI9341 tft = TFT_ILI9341();

//--------------------------------------------- Creación de variables -----------------------------------------------------------------------------------------

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

char score1 = 0;    // Puntaje jugador 1.
char score2 = 0;    // Puntaje jugador 2.

int lastBallX = 0;  // Última posición X de la pelota.
int lastBallY = 0;  // Última posición Y de la pelota.
int ballX = 160;    // Posición inicial X de la pelota.
int ballY = 138;    // Posición inicial Y de la pelota.
char ballSize = 4;  // Radio de la pelota.
int xDir = 1;       // Valor que se le va a sumar a la posición X de la pelota para que esta se mueva.
int yDir = 1;       // Valor que se le va a sumar a la posición Y de la pelota para que esta se mueva.
int moverX = 0;     // Posición X resultante de la suma anterior.
int moverY = 0;     // Posición X resultante de la suma anterior.

extern uint8_t cofre1 [];
//extern uint8_t cofre2 [];
extern uint8_t cofre3 [];
//extern uint8_t cofre4 [];
extern uint8_t cofre5 [];
/*int melody[]={NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4}; //Melodía con el buzzer
int noteDurations[]={4, 8, 8, 4, 4, 4, 4, 4}; //Duración de las notas*/

int melody[]={ NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
   NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
   NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
   
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_D5, NOTE_E5, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
   NOTE_C5, NOTE_A4, NOTE_B4, 0,

   NOTE_A4, NOTE_A4}; //Melodía con el buzzer
   
int noteDurations[]={125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125}; //Duración de las notas
//------------------------------------------ Setup de los botones, inicialización de pantalla TFT, pantalla de inicio --------------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


      
  Serial.println("PONG v2");

  pinMode(bajarJ1, INPUT);  // Set el pin 2 como input.
  pinMode(subirJ1, INPUT);  // Set el pin 3 como input.
  pinMode(bajarJ2, INPUT);  // Set el pin 5 como input.
  pinMode(subirJ2, INPUT);  // Set el pin 6 como input.

  tft.init(); // Inicialización de la pantalla TFT.

  // Esta progra es para establecer la pantalla de inicio.
  tft.setRotation(1);   // Orientación landscape.
  tft.fillScreen(ILI9341_BLACK);  // Pantalla negra.
  tft.setCursor(115,80);  // Poner el cursor en esta posición.
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(4);  // Color y tamaño de letra.
  tft.println("PONG");   
  tft.setTextSize(2);
  tft.setCursor(60,160);
  tft.println("Presione un boton");
  tft.setCursor(90,180);
  tft.println("para empezar");
  tft.setCursor(90,200);
}


//---------------------------------------------------------- Loop principal, el juego se lleva a cabo aquí -----------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:
  j1b = digitalRead(bajarJ1);
  j1s = digitalRead(subirJ1);
  j2b = digitalRead(bajarJ2);
  j2s = digitalRead(subirJ2);
  if (j1b == HIGH || j1s == HIGH || j2b == HIGH || j2s == HIGH){
    tft.fillScreen(ILI9341_BLACK);
    set_scoreboard();
    while(1){
      mover_tabla1();
      mover_tabla2();
      drawBall();
      scoreChange();
    }
  }
}

//----------------------------------------------------------- Funciones utilizadas ----------------------------------------------------------------------------

// Función para establecer el tablero de puntaje.
void set_scoreboard(){
  tft.setCursor(40,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score1, DEC);   // Puntaje del jugador 1.
  tft.setCursor(100,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println("PUNTAJE");
  tft.setCursor(265,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score2, DEC);   // Puntaje del jugador 2.
  tft.drawFastHLine(0,28,320,ILI9341_WHITE);  // Línea de separación del tablero de puntaje y el juego.
}

// Código para que el jugador 1 pueda mover su tabla con los botones.
void mover_tabla1(){
  j1b = digitalRead(bajarJ1);
  j1s = digitalRead(subirJ1);
  dibujarT1();
  
  // El código siguiente se asegura que cada vez que el jugador mueva su tabla, se borre la parte de la tabla que se encuentra en la posición anterior,
  // para evitar que la tabla actúe como un marcador (no deje rastros donde se mueva).
  
  if (j1b == HIGH && Y_tabla1 < 240-tablasAlto){
    lastY_tabla1 = Y_tabla1;  // La posición actual de la tabla se convierte en la pasada.
    Y_tabla1 += desfase;      // Establece 
    tft.fillRect(X_tabla1, lastY_tabla1, tablasAncho, desfase, ILI9341_BLACK);
    tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    //tone (9, 233, 250);
    delay(2);
  }
  if (j1s == HIGH && Y_tabla1 > 32){
    lastY_tabla1 = Y_tabla1;
    Y_tabla1 -= desfase;
    tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla1, lastY_tabla1+desfaseSubida, tablasAncho, desfase, ILI9341_BLACK);
    //tone (9, 233, 250);
    delay(2);
  }
}

// Código para que el jugador 2 pueda mover su tabla con los botones.
void mover_tabla2(){
  j2b = digitalRead(bajarJ2);
  j2s = digitalRead(subirJ2);
  dibujarT2();
  
  // El código siguiente se asegura que cada vez que el jugador mueva su tabla, se borre la parte de la tabla que se encuentra en la posición anterior,
  // para evitar que la tabla actúe como un marcador (no deje rastros donde se mueva).
  
  if (j2b == HIGH && Y_tabla2 < 240-tablasAlto){
    lastY_tabla2 = Y_tabla2;
    Y_tabla2 += desfase;
    tft.fillRect(X_tabla2, lastY_tabla2, tablasAncho, desfase, ILI9341_BLACK);
    tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
    //tone (9, 233, 250);
    delay(2);
  }
  if (j2s == HIGH && Y_tabla2 > 32){
    lastY_tabla2 = Y_tabla2;
    Y_tabla2 -= desfase;
    tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
    tft.fillRect(X_tabla2, lastY_tabla2+desfaseSubida, tablasAncho, desfase, ILI9341_BLACK);
    //tone (9, 233, 250);
    delay(2);
  }
}

void dibujarT1(){
  tft.fillRect(X_tabla1, Y_tabla1, tablasAncho, tablasAlto, ILI9341_WHITE);
}

void dibujarT2(){
  tft.fillRect(X_tabla2, Y_tabla2, tablasAncho, tablasAlto, ILI9341_WHITE);
}

// Este código se llama cada vez que algun jugador anote un punto. Borra la última posición de la pelota y las vuelve a colocar en el centro.
void resetBall(){
  tft.fillCircle(lastBallX, lastBallY, ballSize, ILI9341_BLACK);
  ballX = 160;
  ballY = 138;
  tft.fillCircle(ballX, ballY, ballSize, ILI9341_YELLOW);
  xDir *= -1;
  yDir *= -1;
  moverX = ballX;
  moverY = ballY;
  lastBallX = moverX;
  lastBallY = moverY;
  delay(500);
}

// Función para darle un punto al jugador 1.
void puntoJ1(){
  tft.fillRect(40,2,27,25,ILI9341_BLACK);
  tft.setCursor(40,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score1+=1, DEC);
}

// Función para darle un punto al jugador 2.
void puntoJ2(){
  tft.fillRect(265,2,27,25,ILI9341_BLACK);
  tft.setCursor(265,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score2+=1, DEC);
}

// Función para poder reiniciar el juego cuando alguien haya ganado.
void resetGame(){
  score1 = 0;
  score2 = 0;
  tft.fillRect(100,76,120,89,ILI9341_BLACK);
  tft.fillRect(40,2,27,25,ILI9341_BLACK);
  tft.setCursor(40,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score1, DEC);
  tft.fillRect(265,2,27,25,ILI9341_BLACK);
  tft.setCursor(265,2);
  tft.setTextSize(3); tft.setTextColor(ILI9341_WHITE);
  tft.println(score2,DEC);
}

void scoreChange(){
  if (moverX + ballSize == anchoPantalla){
    if (score1 < 10 && score2 < 10){
      puntoJ1();
      
      tone (9,392,250);
      tone (9,311,250);
      tone (9,247,250); //Sonido de perder un punto
      
      resetBall();
      if (score1 == 5 && score2 < 5){
        tft.fillCircle(160,138,4,ILI9341_BLACK);
        int thisNote=0;
        for (thisNote == 0; thisNote < 78; thisNote++){
          int noteDuration = noteDurations [thisNote]*1.5;
          tone(9, melody[thisNote], noteDuration);
          
          delay (noteDuration);

          noTone(9);
        }
        Ganar();
        while (score1 == 5 && score2 < 5){
          j1b = digitalRead(bajarJ1);
          j1s = digitalRead(subirJ1);
          j2b = digitalRead(bajarJ2);
          j2s = digitalRead(subirJ2);
          if (j1b == HIGH || j1s == HIGH || j2b == HIGH || j2s == HIGH){
            delay(100);
            resetGame();
          }
          
          // código de reset juego.
        }       
      } 
    }
  }
  else if (moverX == 0){
    if (score1 < 5 && score2 < 5){
      puntoJ2();
      
      tone (9,392,250);
      tone (9,311,250);
      tone (9,247,250); //Sonido de perder un punto
      //------------------------------------------
      // COLOCAR AQUÍ EL SONIDO DE PERDER UN PUNTO
      //------------------------------------------
      
      resetBall();
      if (score1 < 5 && score2 == 5){
        tft.fillCircle(160,138,4,ILI9341_BLACK);
        int thisNote=0;
        for (thisNote == 0; thisNote < 78; thisNote++){
          int noteDuration = noteDurations [thisNote]*1.5;
          tone(9, melody[thisNote], noteDuration);

          delay (noteDuration);

          noTone(9);
         }
         Ganar();
        while (score1 < 5 && score2 == 5){
          j1b = digitalRead(bajarJ1);
          j1s = digitalRead(subirJ1);
          j2b = digitalRead(bajarJ2);
          j2s = digitalRead(subirJ2);
          if (j1b == HIGH || j1s == HIGH || j2b == HIGH || j2s == HIGH){
            delay(100);
          
            resetGame();
            
          // código de reset juego.
          }
        }
      }
    }
  }
}

// Función principal que se encarga del movimiento de la pelota, además de limitar su movimiento de manera que el juego funcione. Establece que cuando
// golpea una tabla, rebote y cuando toque un lugar donde no se encuentre la tabla, el jugador opuesto obtiene un punto.
void drawBall(){
  tft.fillCircle(lastBallX, lastBallY, ballSize, ILI9341_BLACK);
  tft.fillCircle(ballX, ballY, ballSize, ILI9341_YELLOW);
  lastBallX = ballX;    // Guarda la posición X de la bola como la posición pasada.
  lastBallY = ballY;    // Guarda la posición Y de la bola como la posición pasada.
  moverX = ballX + xDir;  // Le suma xDir a la posición X para que la pelota se mueva.
  moverY = ballY + yDir;  // Le suma yDir a la posoción Y para que la pelota se mueva.
  
  // El siguiente código será para limitar a la pelota para que rebote solamente cuando entre en contacto con la tabla del J2 (derecha).
  if (moverX > 0 && (moverX+ballSize) < anchoPantalla){
    ballX = moverX;
    delay(2);
    if ((moverX+ballSize) == X_tabla2 && (moverY) >= (Y_tabla2-2) && (moverY+(ballSize/2)) <= (Y_tabla2+tablasAlto) || moverX == (X_tabla1+tablasAncho) && moverY >= (Y_tabla1-2) && (moverY+(ballSize/2)) <= (Y_tabla1+tablasAlto)){
      xDir *= -1;
      //yDir *= -1;
      ballX += xDir;
      //ballY += yDir;

      tone (9,466,250);//Sonido de choque
      
    }
    else if (moverY == 32 || (moverY+ballSize) == altoPantalla){
      yDir *= -1;
      ballY += yDir;
      
      tone (9,466,250);//Sonido de choque
      
    }
  }
  if (moverY > 32 && (moverY+ballSize) < altoPantalla){
    delay(2);
    ballY = moverY;
    if (moverY == 32 || (moverY+ballSize) == altoPantalla){
      xDir *= -1;
      yDir *= -1;

      tone (9,466,250);//Sonido de choque
      
      ballX += xDir;
      ballY += yDir;
    }
  } // Aquí termina la progra para limitar la bola al entrar en contacto con la tabla del J2 (derecha).
}
void drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++) {
      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x80) tft.drawPixel(x+i, y+j, color);
    }
  }
}

void Ganar(){
  drawBitmap(100,76,cofre1,120,89,ILI9341_WHITE);
  delay(2);
  tft.fillRect(100,76,120,89,ILI9341_BLACK);
  //drawBitmap(100,66,cofre2,120,109,0xFFFF);
  //delay(200);
  drawBitmap(93,60,cofre3,135,120,ILI9341_WHITE);
  delay(2);
  tft.fillRect(100,76,120,89,ILI9341_BLACK);
  //drawBitmap(80,64,cofre4,160,113,0xFFFF);
  //delay(200);
  drawBitmap(95,62,cofre5,130,116,ILI9341_WHITE);
}
