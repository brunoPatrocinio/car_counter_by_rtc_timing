/*Programa contador para previsão de veículos para o almoço
Coder - Bruno Patrocinio - Fatec.  V. 4.0.0  - com display OLED 0.96 - 128x64" 
*/

//inclusão de bibliotecas
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_SSD1306.h>

//declaração do objeto da classe RTC
RTC_DS3231 rtc;

//declaração do objeto display
Adafruit_SSD1306 display = Adafruit_SSD1306();

//Declaração do array (ou vetor) que armazena os dias da semana
char dSemana [7] [12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};

//variaveis
long timer = 0; // var. p/ armazenar o tempo para a função millis()
long timerPrint = 0; // var. p/ armazenar o tempo para a função millis()
int total = 3; // var. qtd de posições para compor o vetor 'tempo'
float tempoLinha = 0; //var. que armazena o tempo da linha atual
int limiteAlmoco = 0; //var. que armazena o tempo parada p/ o almoço
int limiteKiukey1 = 0; //var. que armazena o tempo parada p/ o kiukey 1
int limiteKiukey2 = 0; //var. que armazena o tempo parada p/ o kiukey 2
int resultado;
int num = 0;
int carrosRestantes = 0;

DateTime agora;;

//protótipo de função
void printaSerial();
void verificaHorario();
void printDisplay();

void setup() {
  Serial.begin(9600); // inicia a comunicação serial a 9600 bauds
  //config iniciais do display
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();

  //inicializa a var. tempoLinha com o tempo atual
  tempoLinha = 2.50;
  //inicializa as var. p/ armazenar o tempo de cada parada programada
  limiteKiukey1 = 35;
  limiteAlmoco = 20;  
  limiteKiukey2 = 30;

  if(!rtc.begin()){// se o objeto não iniciar
    Serial.println("Não inicializado!");
    while(1); // sempre entra no laço para verificação
  }

  if(rtc.lostPower()){ //se perder energia ou primeira vez iniciado
    Serial.println("Iniciado e OK!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // pega data e hora no momento da compilação 
  }                                                 //do programa
  delay(100); //aguarda 100 ms
}

void loop() {
 //chamada de funções modularizadas
 printaSerial();
 verificaHorario();
 printDisplay();
}

void printaSerial(){
  if(millis() - timer > 1000){ // imprime os dados a cada 1 seg. sem parar o processamento
                              //do microcontrolador
    agora = rtc.now(); // var. agora recebe os dados obtidos da função now()
    Serial.print("Data: "); 
    Serial.print(agora.day(), DEC); 
    Serial.print('/'); 
    Serial.print(agora.month(), DEC); 
    Serial.print('/'); 
    Serial.print(agora.year(), DEC); 
    Serial.print(" / Dia: "); 
    Serial.print(dSemana[agora.dayOfTheWeek()]); 
    Serial.print(" / Horas: "); 
    Serial.print(agora.hour(), DEC); 
    Serial.print(':'); 
    Serial.print(agora.minute(), DEC); 
    Serial.print(':'); 
    Serial.print(agora.second(), DEC); 
    Serial.println(); 
    
    timer = millis();
  }
}//fim de printaSerial

void verificaHorario(){
  //verifica se as condições de dia da semana e horario de refeição são satisfeitas
  //para a execução do laço for e ifs internos
    int tempo[total];
    tempo[0] = agora.hour();
    tempo[1] = agora.minute();
    tempo[2] = agora.second();
    if(agora.dayOfTheWeek() != 6 || agora.dayOfTheWeek() != 7){
       for(int i = 0; i <= tempo[1]; i++){
        if(tempo[0] == 8){
          if(tempo[1] >= 25 && tempo[1] < 35){
            resultado = limiteKiukey1 - agora.minute();
            carrosRestantes = (int)resultado / tempoLinha;
              if(millis() - timerPrint > 1000){
                Serial.print("Worked! - Qtd Carros rest: ");
                Serial.println(carrosRestantes);
                
                timerPrint = millis();
             }
          }
        }
        else if(tempo[0] == 11){
           if(tempo[1] >= 10 && tempo[1] < 20){
            resultado = limiteAlmoco - agora.minute();
            carrosRestantes = (int)resultado / tempoLinha;
              if(millis() - timerPrint > 1000){
                Serial.print("Worked! - Qtd Carros rest: ");
                Serial.println(carrosRestantes);                                        
                timerPrint = millis();
             }    
          }
        }
        else if(tempo[0] == 13){
          if(tempo[1] >= 20 && tempo[1] < 30){
            resultado = limiteKiukey2 - agora.minute();
            carrosRestantes = (int)resultado / tempoLinha;
              if(millis() - timerPrint > 1000){
                Serial.print("Worked! - Qtd Carros rest: ");
                Serial.println(carrosRestantes);
                
                timerPrint = millis();
             }
          }
        }        
     }          
  }                
}//fim de verificaHorario

void printDisplay(){
  int tempo[total];
  tempo[0] = agora.hour();
  tempo[1] = agora.minute();
  tempo[2] = agora.second();
  
  char buf2[] = "DD/MM/YY - hh:mm:ss"; //vetor carac. p/ armazenar a string para conversão.
  display.setCursor(1,2);
  display.print(agora.toString(buf2));//converte e printa o DD/MM/AA e Hora em tempo real
  display.print("\n");
  if(tempo[0] == 8){
    if(tempo[1] >= 25 && tempo[1] < 35){
    //display.setCursor(1,12);
    display.print("Carros Rest: ");
    display.print(carrosRestantes);
    display.print("\n");
    display.print("--Kiukey 1--");
    display.display();  
  }
}
  else if(tempo[0] == 11){
    if(tempo[1] >= 10 && tempo[1] < 20){
      display.print("Carros Rest: ");
      display.print(carrosRestantes);
      display.print("\n");
      display.print("--Almoco--");
      display.display();
    }
  }
  else if(tempo[0] == 13){
    if(tempo[1] >= 20 && tempo[1] < 30){
      display.print("Carros Rest: ");
      display.print(carrosRestantes);
      display.print("\n");
      display.print("--Kiukey 2--");
      display.display();
    }
  } 
  display.display();
  display.clearDisplay();
  
}//fim de printDisplay
