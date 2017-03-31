#include<SPI.h>
#include"nRF24L01.h"
#include"RF24.h"
#include"printf.h"
#include <Adafruit_NeoPixel.h>

const float VCC = 3.27; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
const float STRAIGHT_RESISTANCE = 10300.0; // resistance when straight
const float BEND_RESISTANCE = 8000.0; // resistance at 90 deg+
int LedsPin=6;

// Radio Vars
Adafruit_NeoPixel cintaLeds = Adafruit_NeoPixel(10, LedsPin, NEO_RGBW + NEO_KHZ800);
RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
bool go_to_tx_mode = false;
char input[32] = "";

//Program Vars
int i=1;
int estadoBoton;
int estadoBotonAnterior;
int ABT=10;
boolean FOURp=true;
int msg=0;
int iprev=0;

const int FLEX_PIN = A0; 
//const int pilotoIzq = A1; 
//const int pilotoDer = A2; 
//const int pilotoNoch = A3; 

int botIzq=5;
int botDer=4;
int botNoche=3;
int nocheEncendido; 
int izqEncendido;
int derEncendido;

void setup() {
  // Serial Setup
  //Serial.begin(57600);
  Serial.begin(9600);  
  printf_begin();
  // Radio Setup
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  radio.printDetails();

  cintaLeds.begin();
  cintaLeds.show();
  
  // Other Setups
  for (int i=2;i<=5;i++)
    {
      pinMode(i, INPUT_PULLUP);
    }
  for (int i=6;i<=8;i++)
    {
      pinMode(i, OUTPUT);
    }

  pinMode(FLEX_PIN, INPUT);
  pinMode(botIzq, INPUT);  
  pinMode(botDer,INPUT);
  pinMode(botNoche,INPUT);

//  pinMode(pilotoIzq, OUTPUT);
 // pinMode(pilotoDer, OUTPUT);
  //pinMode(pilotoNoch, OUTPUT);
        
}
void loop() {
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);

// TRANSMISION DATOS FRENO
   if(flexR>200000.00){
    //Serial.println("Entro");
     bool ok = radio.write("FRENO", 32);
      int contador=1;
      while((ok != 1) && (contador<5)){
        ok = radio.write("FRENO", 32);
        contador=contador+1;
        if (ok==1){
        printf("CONTROL : %s\n", "FRENO");
        }
        else printf("could not write FRENO ....\n");      
      }
  }
  
 //LECTURA DE BOTONES
  int valorIzq = digitalRead(botIzq);
  int valorDer= digitalRead(botDer);
  int valorNoche= digitalRead(botNoche);
  

//--------------- ESCRITURA VARIABLES TX----------------------
//APAGAR BOMBILLO IZQUIERDO
    if((valorIzq==1)&&(izqEncendido==1)){
        bool ok = radio.write("IZQOUT", 32);
        int contador=1;
       while((ok != 1) && (contador<5)){
          ok = radio.write("IZQOUT", 32);
          contador=contador+1;
          if (ok==1){
            printf("CONTROL : %s\n", "IZQOUT");
            izqEncendido=0;
            valorIzq=0;
          }
          else printf("could not write IZQOUT....\n");      
        }   
    }
//PRENDER BOMBILLO IZQUIERDO
    if((valorIzq==1)&&(izqEncendido==0)){
      bool ok = radio.write("IZQIN", 32);
      int contador=1;
       while((ok != 1) && (contador<5)){
          ok = radio.write("IZQIN", 32);
          contador=contador+1;
          if (ok==1){
            printf("CONTROL : %s\n", "IZQIN");
            izqEncendido=1;
          }
          else printf("could not write IZQIN....\n");      
        }                         
    }
//APAGAR BOMBILLO DERECHO
    if((valorDer==1)&&(derEncendido==1)){
        bool ok = radio.write("DEROUT", 32);
        int contador=1;
       while((ok != 1) && (contador<5)){
          ok = radio.write("DEROUT", 32);
          contador=contador+1;
          if (ok==1){
            printf("CONTROL : %s\n", "DEROUT");
            derEncendido=0;
            valorDer=0;
          }
          else printf("could not write DEROUT....\n");      
        }   
    }
//PRENDER BOMBILLO IZQUIERDO
    if((valorDer==1)&&(derEncendido==0)){
      bool ok = radio.write("DERIN", 32);
      int contador=1;
       while((ok != 1) && (contador<5)){
          ok = radio.write("DERIN", 32);
          contador=contador+1;
          if (ok==1){
            printf("CONTROL : %s\n", "DERIN");
            derEncendido=1;
          }
          else printf("could not write DERIN....\n");      
        }                         
    }
    //APAGAR BOMBILLO NOCHE
    if((valorNoche==1)&&(nocheEncendido==1)){
        bool ok = radio.write("NOCHEOUT", 32);
        int contador=1;
       while((ok != 1) && (contador<5)){
          ok = radio.write("NOCHEOUT", 32);
          contador=contador+1;
          if (ok==1){
            printf("CONTROL : %s\n", "NOCHEOUT");
            nocheEncendido=0;
            valorNoche=0;
          }
          else printf("could not write NOCHEOUT....\n");      
        }     
    }
    //PRENDER BOMBILLO NOCHE
       if((valorNoche==1)&&(nocheEncendido==0)){
        bool ok = radio.write("NOCHEIN", 32);
        int contador=1;
       while((ok != 1) && (contador<5)){
          ok = radio.write("NOCHEIN", 32);
          contador=contador+1;
          if (ok==1){
            printf("CONTROL : %s\n", "NOCHEIN");
            nocheEncendido=1;
          }
          else printf("could not write NOCHEIN....\n");      
        }       
      }
// PRENDER TESTIGOS PILOTOS




      if(nocheEncendido==1){

        printf("NOCHE ENCENDIO .... \n ");
        cintaLeds.setPixelColor(2, 0,255, 0);   
        cintaLeds.show();
      }else{
        cintaLeds.setPixelColor(2, 0, 0 , 0);   
        cintaLeds.show();
      }
      if(derEncendido==1){
        
        cintaLeds.setPixelColor(0, 0,255, 0);   
        cintaLeds.show();
        
      }else{
        cintaLeds.setPixelColor(0, 0, 0 , 0);   
        cintaLeds.show();
      }
      if(izqEncendido==1){
        cintaLeds.setPixelColor(1, 0,255, 0);   
        cintaLeds.show();
        
      }else{
        cintaLeds.setPixelColor(1, 0, 0 , 0);   
        cintaLeds.show();
        
      }  
//------------------- RX Mode ------------------
  if (radio.available())
  {
    char payload[32] = "";
    bool done = false;
    while (!done)
    {
      done = radio.read( payload , 32 );
      printf("MALETA : %s\n", payload);
    }
  }  
}


