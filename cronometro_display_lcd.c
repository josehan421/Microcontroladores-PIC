
int seg = 0;                     //Variável que guarda o número de segundos
                                 //passados

//Configura Pinos do LCD.
sbit LCD_RS at RD3_bit;
sbit LCD_EN at RD2_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D4 at RD4_bit;
//Configura a direção dos pinos do LCD.
sbit LCD_RS_Direction at TRISD3_bit;
sbit LCD_EN_Direction at TRISD2_bit;
sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;


void setup();                     //configuações do PIC

void time();                      //função que conta os segundos

void lcd();                       //função que mostra a contagem dos
                                  //segundos

void main() {                     //função principal

    setup();                      //chama função setup
    
    Lcd_Init();                   //inicializa módulo LCD
    Lcd_Cmd(_LCD_CLEAR);          //limpa LCD
    Lcd_Cmd(_LCD_CURSOR_OFF);     //desliga o cursor
    Lcd_Out(1, 6, "time!");       //escreve "time" na linha 1, a partir da coluna 6
    delay_ms(1000);               //espera 1s

    while(1){
       time();                    // chama time (incrementa seg)
       lcd();                     // chama lcd (escreve no display o tempo atual)
       delay_ms(1000);            // espera 1s
    }

}

void setup(){
  ADCON1 = 15;                    //todas os pinos como digitais
  TRISD = 0x00;                   //PORTD como saída
  PORTD = 0X00;                   //Carrega 0 no PORTD
}

void time(){


  seg++;                          //incrementa seg a cada 1s
  if(seg == 3600){                //se seg é igual a 3600...
  
    seg = 0;                      //zera seg
  }

}

void lcd(){

  char charseg[10];               //cria um vetor de caracteres
  Inttostr(seg, charseg);         //converte os segundo de inteiros para char

  Lcd_Out(2, 4, charseg);         //mostra o tempo decorrido no display em segundos

}
