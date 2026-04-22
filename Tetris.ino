
#include <MD_MAX72xx.h>
 
#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 1
 
#define CLK_PIN   7
#define DATA_PIN  5
#define CS_PIN    6
 
#define DELAY_ANIMATION 200
 
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);

byte A[8][8];  //matriz en la que guardamos todo lo que está fijo
byte B[8][8];   //matriz en la que guardamos las figuras que aparecen antes de que se queden fijas
byte D[8];   //array en el que guardamos la matriz de control en decimal para representarlo en la pantalla

//mostrar pantalla es lo que estamos llamando todo el rato como D
 
void setup() {
Serial.begin(9600);

  //inicializamos la matriz dnd vamos a jugar (matrizControl) a todo ceros
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }


  //inicializamos la matriz de la figura a todo ceros
   for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      B[i][j]=0;
    }
  }

  //inicializamos la matriz de mostrar por pantalla a 0
  for(int i=0;i<8;i++){
    D[i]=0;
  }

  // inicializar el objeto mx
  mx.begin();
 
  // poniendo la matriz en modo de prueba
  mx.control(MD_MAX72XX::TEST, true);
  delay(2000);
 
  // desactivando el modo de prueba
  mx.control(MD_MAX72XX::TEST, false);
 
  // Establecer intencidad a un valor de 5
  mx.control(MD_MAX72XX::INTENSITY, 5);
 
}
 
/**
 * Recibe un arreglo con la configuración de 
 * las filas y las dibuja en la matriz
**/
void drawRows( const byte fig[] ){
  for( int i = 0; i < 8; i++ ){
    mx.setRow(0, i, fig[i]);
  }
}

//cambiar de binario a decimal
 
 //A matriz de entrada
 //D matriz que devuelve
void pasoAdecimal(byte A[][8],byte *D){
  
  for(int i=0;i<8;i++){
   D[i]=A[i][0]+2*A[i][1]+4*A[i][2]+8*A[i][3]+16*A[i][4]+32*A[i][5]+64*A[i][6]+128*A[i][7];
  }

}

///FUNCIONES DE CADA FIGURA

//Primero resetea la matriz, después le coloca la figura arriba, después lo pasa a decimal y lo muestra por pantalla.

void apareceFigura1(byte A[][8], byte *D){  //CUADRADO
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }

  A[1][3]=1;
  A[1][4]=1;
  A[0][3]=1;
  A[0][4]=1;

  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
}

void apareceFigura2(byte A[][8], byte *D){ //LÍNEA RECTA
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }

  A[0][2]=1;
  A[0][3]=1;
  A[0][4]=1;
  A[0][5]=1;

  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
}

void apareceFigura3(byte A[][8], byte *D){ //ELE
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }

  A[0][2]=1;
  A[1][2]=1;
  A[1][3]=1;
  A[1][4]=1;

  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
}

void apareceFigura4(byte A[][8], byte *D){  //TRIPODE
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }

  A[0][3]=1;
  A[1][2]=1;
  A[1][3]=1;
  A[1][4]=1;

  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
}

void apareceFigura5(byte A[][8], byte *D){  //L CHIQUITA
   for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }

  A[0][3]=1;
  A[1][4]=1;
  A[1][3]=1;  

  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
}

void apareceFigura6(byte A[][8], byte *D){  //ESE
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      A[i][j]=0;
    }
  }

  A[0][3]=1;
  A[0][4]=1;
  A[1][3]=1;
  A[1][2]=1;

  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
}


//elegir la figura que se genera de manera aleatoria
//elige un número de manera aleatoria y con eso selecciona una de las funciones de aparece figura

void elegirFigura(byte A[][8], byte *D){
  int numeroAleatorio = random(1, 7);
  if(numeroAleatorio==1){
    apareceFigura1(A,D);
  }
  if(numeroAleatorio==2){
    apareceFigura2(A,D);
  }
  if(numeroAleatorio==3){
    apareceFigura3(A,D);
  }
  if(numeroAleatorio==4){
    apareceFigura4(A,D);
  }
  if(numeroAleatorio==5){
    apareceFigura5(A,D);
  }
  if(numeroAleatorio==6){
    apareceFigura6(A,D);
  }
}

//función para comprobar si tiene que dejar de bajar
//Comprueba si la figura coincide con lo de debajo y si coincide devuelve un x=0. Si no coincide x=1
//A=matrizControl ; B=matrizFigura








 
 void pantalla(byte A[][8],byte *D){
  pasoAdecimal(A,D);
  drawRows(D);
  delay(10*DELAY_ANIMATION);
 }


    
 

 
void loop() {
  elegirFigura(A,D);
}
