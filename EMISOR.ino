#include<SPI.h>
#include"nRF24L01.h"
#include"RF24.h"
#include"printf.h"


const int FLEX_PIN = A0; 
const float VCC = 3.27; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
const float STRAIGHT_RESISTANCE = 10300.0; // resistance when straight
const float BEND_RESISTANCE = 8000.0; // resistance at 90 deg

// Radio Vars
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

int botIzq=5;
int botDer=4;
int botNoche=3;
int nocheEncendido; 
int nocheApagado;




void setup() {
  // Serial Setup
  Serial.begin(57600);
  pinMode(FLEX_PIN, INPUT);
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
  // Other Setups
  for (int i=2;i<=5;i++)
    {
      pinMode(i, INPUT_PULLUP);
    }
  for (int i=6;i<=8;i++)
    {
      pinMode(i, OUTPUT);
    }


pinMode(botIzq, INPUT);  
    

    
}
void loop() {


  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  //Serial.print("Resistencia: ");
  //Serial.println(flexR);
  //Serial.println("Resistance: " + String(flexR) + " ohms");


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
  
 
  int valorIzq = digitalRead(botIzq);
  int valorDer= digitalRead(botDer);
  int valorNoche= digitalRead(botNoche);
  
  
  
  
    if(valorIzq==1){
      bool ok = radio.write("IZQ", 32);
      int contador=1;
      while((ok != 1) && (contador<5)){
        ok = radio.write("IZQ", 32);
        contador=contador+1;
        if (ok==1){
        printf("CONTROL : %s\n", "IZQ");
        }
        else printf("could not write IZQ....\n");      
      }            
    }

     if(valorDer==1){
      bool ok = radio.write("DER", 32);
      int contador=1;
      while((ok != 1) && (contador<5)){
        ok = radio.write("DER", 32);
        contador=contador+1;
        if (ok==1){
        printf("CONTROL : %s\n", "DER");
        }
        else printf("could not write DER....\n");      
      }            
    }


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

  /*
  if (Serial.available())
  { 
    int len = Serial.readBytesUntil('\n', input, 31);
    input[len] = '\0';
    go_to_tx_mode = true;
  }*/
  // Buttons
  //estadoBotonAnterior=true;
 // estadoBoton =digitalRead (i+1);
  /*
  if (estadoBoton != estadoBotonAnterior) 
  {
    if (antirebote (i+1))
        {
          if(i==4)
          {
            if(FOURp)
            {
              FOURp=false;
              msg=i;
            }
            else
            {
              FOURp=true;
              msg=0;
            }
            String str = String(msg);
            str.toCharArray(input,32);
            go_to_tx_mode = true;
          }

          if(i!=iprev && i<4)
          {
              String str = String(i);
              str.toCharArray(input,32);
              go_to_tx_mode = true;
              iprev=i;
          }
          else
          {
            String str = String(msg);
            str.toCharArray(input,32);
            go_to_tx_mode = true;
            iprev=msg;
          }
        }
  }*/
 
}


// Anti-bounce Function
boolean antirebote  (int pin ) 
{
    int  contador =0;
    boolean estado;            // guarda el estado del boton 
    boolean estadoAnterior;    // guarda el ultimo estado del boton 

  do {
    estado = digitalRead (pin);
    if (estado != estadoAnterior ){  // comparamos el estado actual 
      contador = 0;                   // reiniciamos el contador 
      estadoAnterior = estado;
    }
    else{
      contador = contador +1;       // aumentamos el contador en 1
    }
    delay (1);
  }
  while (contador < ABT);
  return estado;
}
