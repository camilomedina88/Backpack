#include<SPI.h>
#include"nRF24L01.h"
#include"RF24.h"
#include"printf.h"
#include <Adafruit_NeoPixel.h>
#include <Battery.h>
#include <SoftReset.h>

// Radio Vars
RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
bool go_to_tx_mode = false;
char input[32] = "";
char msg[32] = "";
char payload[32];
int ledIzq=8;
int ledDer=5;
int ledNoche=7;
int ledFreno=6;
int cantidadLeds=7;
int appTestigoNoche=0;
int appTestigoIzq=0;
int appTestigoDer=0;

int voltageBateriaMaletin; 

float pilotosValue[5] = {0,0,0,0,0};

const int FLEX_PIN = A0;

//Program Vars
boolean flag=false;

  Adafruit_NeoPixel stripIzq = Adafruit_NeoPixel(10, ledIzq, NEO_RGBW + NEO_KHZ800);
  Adafruit_NeoPixel stripDer = Adafruit_NeoPixel(10, ledDer, NEO_RGBW + NEO_KHZ800);
  Adafruit_NeoPixel stripFreno = Adafruit_NeoPixel(10, ledFreno, NEO_RGBW + NEO_KHZ800);
  Adafruit_NeoPixel stripNoche = Adafruit_NeoPixel(10, ledNoche, NEO_RGBW + NEO_KHZ800);

    Battery battery(3400, 4600, A1, 3);
//Battery::Battery(uint16_t minVoltage, uint16_t maxVoltage, uint8_t sensePin, uint8_t activationPin) 


void setup() {
  // Serial Setup
  Serial.begin(9600);
  // Printf Setup
  printf_begin();
  // Radio Setup
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  // Print Radio Details
  radio.printDetails();

  stripIzq.begin();
  stripIzq.show(); 

  stripDer.begin();
  stripDer.show(); 

  stripFreno.begin();
  stripFreno.show(); 

  stripNoche.begin();
  stripNoche.show(); 

  battery.begin();

  
}
void loop() {


  voltageBateriaMaletin= battery.voltage();
  //Serial.println(voltageBateriaMaletin);

// -------------- TX Mode ----------------
if (go_to_tx_mode)
  {
    radio.stopListening();
    bool ok = radio.write(input, 32);
    if (ok)
    {
      printf("MALETA : %s\n", input);
      go_to_tx_mode = false;
      radio.startListening();
    }
    else
      printf("could not write....\n");
  }
  
  // -------------- RX Mode -----------------
  if (radio.available())
  {
    payload[32] = "";
    bool done = false;
    //Serial.println("AQUI VOY");
    while (!done)
    {
      done = radio.read( payload , 32 );
      //Serial.println(payload);
      //sendAndroidValues();
      //printf("CONTROL : %s\n", payload);
    }
    flag=true;
    
  }



 
  // Leds
  if(flag)
  {
    String str(payload);

 if(str=="FRENO"){
          
      for(int i=0;i<cantidadLeds;i++){
          stripFreno.setPixelColor(i, 0,255, 0);   
      } 
      stripFreno.show(); 
      delay(100);

      for(int i=0;i<cantidadLeds;i++){
          stripFreno.setPixelColor(i, 0,0, 0);   
      }
     stripFreno.show();
               
   }

    if(str=="IZQIN"){
       for(int i=0;i<cantidadLeds;i++){
          stripIzq.setPixelColor(i, 255,255, 0);   
      } 
      stripIzq.show();
      appTestigoIzq=1; 
   }

   if(str=="IZQOUT"){
       for(int i=0;i<cantidadLeds;i++){
          stripIzq.setPixelColor(i, 0,0, 0);   
      } 
      stripIzq.show();
      appTestigoIzq=0;  
   }

    if(str=="DERIN"){
      for(int i=0;i<cantidadLeds;i++){
          stripDer.setPixelColor(i, 255,255, 0);   
      }
      stripDer.show();
      appTestigoDer=1;   
     }


    if(str=="DEROUT"){
      for(int i=0;i<cantidadLeds;i++){
          stripDer.setPixelColor(i, 0,0, 0);   
      }
      stripDer.show();  
      appTestigoDer=0;
     }

  if(str=="NOCHEIN"){     
     for(int i=0;i<cantidadLeds;i++){
          stripNoche.setPixelColor(i, 0,255, 0);   
      } 
      stripNoche.show(); 
      appTestigoNoche=1;       
    }


   if(str=="NOCHEOUT"){ 
      for(int i=0;i<cantidadLeds;i++){
          stripNoche.setPixelColor(i, 0,0, 0);   
      } 
      stripNoche.show();          
      appTestigoNoche=0;
    }   
    actualizarTestigos();
    flag=false;
    
    sendAndroidValues(); //Enviar info a app 
    delay(500); 
  } 
  
}



void actualizarTestigos()
{
  pilotosValue[0] = voltageBateriaMaletin; //led
  pilotosValue[1] = appTestigoIzq;
  pilotosValue[2] = appTestigoNoche;
  pilotosValue[3] = appTestigoDer;
  pilotosValue[4] = 10;
  
}



void sendAndroidValues()
 {

 Serial.print('#'); //hay que poner # para el comienzo de los datos, así Android sabe que empieza el String de datos
  for(int k=0; k<5; k++)
  {
    Serial.print(pilotosValue[k]);
    Serial.print('+'); //separamos los datos con el +, así no es más fácil debuggear la información que enviamos
  }
 Serial.print('~'); //con esto damos a conocer la finalización del String de datos
 Serial.println();
 delay(10);        //agregamos este delay para eliminar tramisiones faltantes
 
  /*
  
  Serial.print('#'); //hay que poner # para el comienzo de los datos, así Android sabe que empieza el String de datos
  Serial.print('1'); //primer Dato... Se envia algo
  Serial.print('+'); //separamos los datos con el +, así no es más fácil debuggear la información que enviamos
  Serial.print(appTestigoIzq); //Testigo boton Izquierda
  Serial.print('+'); //separamos los datos con el +, así no es más fácil debuggear la información que enviamos
  Serial.print(appTestigoNoche); //Testigo boton Noche
  Serial.print('+'); //separamos los datos con el +, así no es más fácil debuggear la información que enviamos
  Serial.print(appTestigoDer); //Testigo boton Derecha
  Serial.print('~'); //con esto damos a conocer la finalización del String de datos
  Serial.println();
  delay(10);        //agregamos este delay para eliminar tramisiones faltantes*/
}
