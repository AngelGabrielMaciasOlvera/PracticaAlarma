/*---------Alarma de puerta para detección de intrusos con la minima utilización de Energía------------
 * 
 * El siguiente proyecto en Arduino ejemplifica el trabajo realiazdo por una alarma que notifica por medio 
 * de emisiones de luz si una puerta de ingreso a un determinado lugar ha sido abierta. Dicha alarma  
 * puede ser activada o desactivada según los pulsos que se le den al boton controlador de estado 
 * (su estado inicial es "Activa"); la alarma planteada realiza una emisión de luz Azul si 
 * se encuentra cerrada la puerta y si esta misma se abre, la alarma emite una emisión de luz intermitente
 * color Rojo (Si el imán se encuentra pegado al reed switch la puerta se encuentra cerrada, caso contrario
 * la puerta se encuentra abierta). Cabe a destacar que la alarma cuenta con un indicador de activación en forma  
 * de display, si el display muestra un "0" indica que la alarma se encuentra en un estado de no activación 
 * y en un modo "AHORRO DE ENERGÍA" (o sleep) (lo cual hace que el Arduino utilice lo minimo de energía)
 * y en caso contrario el display muestra un "1" si la alarma se encuentra activada con el comportamiento 
 * anteriormente detallado.
 * 
 * 
 *  Archivo para consulta y modificación orientado al público en general
*/


//Inclusión de librería LowPower para ahorro de energía
#include <LowPower.h>

int contacto = 2; /*Pin asignado al reed switch, que funcionará como 
            detector de si la puerta se encuentra cerrada o abierta*/
int ledPuertaAbierta= 13; /*Pin asignado para el LED que notificará 
      si la puerta se encuentra abierta (con la alarma activada)  */
int ledPuertaCerrada = 12; /*Pin asignado para el LED que notificará 
      si la puerta se encuentra cerrada (con la alarma activada)  */      

//Pulsador
int boton=3;
//Estatus de la alarma "Activa o Innactiva"
String estado;


void setup(){
  //Entradas
  pinMode(boton,INPUT);  
  pinMode(contacto,INPUT); //El reed switch como una entrada
  
  //Salidas
  pinMode(ledPuertaAbierta, OUTPUT); //LED indicador si puerta esta abierta
  pinMode(ledPuertaCerrada, OUTPUT); //LED indicador si puerta esta cerrada

  //Estado de la alarma de manera inicial
  estado="Activo"; //Se establece la alarma por default como Activa

  //Definición de salidas para el display
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  //Decisión para determinar si el botón ha sido presionado y cambiar de estado
  if (digitalRead(boton)==HIGH){
      //Si el estado actual es "Activo" que cambie a "Inactivo"
      if (estado.equalsIgnoreCase("Activo"))
        estado="Innactivo";
      //Caso contrario el estado se encuentra en "Innactivo" y pasa a ser "Activo"  
      else 
        estado="Activo"; 
     //---Ahorro de energía
     LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); 
     delay(50);  
  }

  //Método para verificación de estado de activación de Alarma
  verificaEstado();
}


/*Metodo para despligue en Display de 7 segmentos, según las
señales que se le manden*/
void display(int a, int b, int c, int e, int d, int f, int g){
  digitalWrite(4, a);
  digitalWrite(5, b);
  digitalWrite(6, c);
  digitalWrite(7, d);
  digitalWrite(8, e);
  digitalWrite(9, f);
  digitalWrite(10, g);
}


/*Método que verifica el estado en que se encuentra la alarma
y decta si la puerta se enceuntra abierta o cerrada y realiza las
emisiones de luz correspondientes según el estado de la alarma*/
void verificaEstado(){
  //Detecta si la alarma está activa
  if (estado.equalsIgnoreCase("Activo")){
    /*Si el estado de la alarma es "Activo"
    despliga un 1 en el display*/
    display(0,1,1,0,0,0,0); 
    //Detecta si la puerta se encuentra abierta (iman lejos del reed switch) 
    if (digitalRead(contacto)==HIGH){
        //Apaga el LED (azul) que indica que la puerta esta cerrada
        digitalWrite(ledPuertaCerrada,LOW);
        //Prende el LED (rojo) que indica que la puerta es abierta
        digitalWrite(ledPuertaAbierta,HIGH); 
        delay(50); //Tiempo para parpadeo
        digitalWrite(ledPuertaAbierta,LOW);
        delay(50);   
    }
    //Detecta que la puerta esta cerrrada 
    else{
      //Paga el LED que indica que la puerta esta abierta
      digitalWrite(ledPuertaAbierta,LOW);
      //Prende el LED que inidca que la puerta esta cerrada
      digitalWrite(ledPuertaCerrada,HIGH);
    } 
  }

  //Detecta que la alarma esta desactivada
  else{
     /*Si el estado de la alarma es "Innactivo"
    despliga un 0 en el display*/
     display(1,1,1,1,1,1,0);
     //Apaga los LED de las puertas
     digitalWrite(ledPuertaAbierta,LOW);
     digitalWrite(ledPuertaCerrada,LOW);
     //--Modo ahorro de energía ya que la puerta se encuentra encerrada 
     sleep(1);
    }
}

//Implementación de metodo sleep parael ahorro de energía
void sleep(int sec) {
  while (sec >= 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    sec -= 8;
  }
  if (sec >= 4) {
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    sec -= 4;
  }
  if (sec >= 2) {
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
    sec -= 2;
  }
  if (sec >= 1) {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
    sec -= 1;
  }
}
