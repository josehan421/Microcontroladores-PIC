/*
  cont -> variável para contar estouros do timer 0
  cont1 -> variável para contar estouros do timer 1
  seg -> guarda o número de segundos passados
  min -> guarda o número de min passados
*/

unsigned char cont, cont1 = 0, seg = 22, min = 56;

void interrupt();               //função de interrupções do PIC

void setup();                   //função de configurações

void multi();                   //função de multiplexação

void time();                    //função que incrementa o tempo nos displays

void pause();                   //função que pausa o cronômetro (extra)

void main() {
                                //chama função setup
     setup();

     while(1){
        if(RD0_bit){            //se botão de pause for pressionado...
          pause();              //chama função pause
          delay_ms(300);        //espera 300ms
        }
     }
}

void interrupt(){

    if(TMR0IF_bit == 1){        //se Timer 0 estourou...

      TMR0IF_bit = 0;           //limpa flag do Timer 0
      cont++;                   //incrementa cont
      TMR0L = 6;                //carraga timer 1 com 6
      
      if(cont ==25){            //se cont for 25 (passou 25* 400us = 10ms)
        multi();                //chama a função multi
        cont = 0;               //reinicia cont
      }

    }
    
    if(TMR1IF_bit){             //se Timer 1 estourou...
    
       TMR1IF_bit = 0;          //limpa flag do Timer 1
       //carrega timer 1 com 50000
       TMR1L = 0xB0;
       TMR1H = 0x3C;
       cont1++;                //cont1 incrementa
       
       if(cont1 == 100){       //se cont1 for 100(passou 100*10ms = 1s)
       
         cont1 = 0;            //reinicia cont1
         time();               //chama função time
       
       }
    
    }
}

 void setup(){

// ======comparadores=======
   ADCON1 = 15;                  //todos os pinos como digitais
   
// =========Timer 0==========
   T0CON = 0b11000010;           //Timer 0 ativo, 8 bits, clock interno, PRE 1:8
   TMR0L = 6;                    //estoro Timer 0 -> TMR0 * ciclo de maquina * prescale
                                 //                  (256 - 6) * 0,2us * 8 = 400us ou 0,4ms
                                 
// =========Timer 1 ==========
   T1CON = 0b01000101;           /* RD16 -> 0 para mostrar TMR1H recebendo overflow de TMRL
                                    T1RUN -> 0, 18F4550 não usa timer 1 como fonte de clock
                                    prescale - > 1:1
                                 */
   TMR1L = 0xB0;                 //estouro: 10ms -> TMR1 * 0,2us * PRE
   TMR1H = 0x3C;                 //         10000/0,2 = TMR1 * 1
                                 //         TMR1 = 50000 =3CB0
                                 
// ======interrupções=======

   GIE_bit =1;                   // habilitação geral
   TMR0IE_bit = 1;               // habilita interrupção timer 0
   TMR0IP_bit =1;                // timer 0 com prioridade alta
   
   PEIE_bit = 1;                 //habilitação interrupção de periféricos(timer 1)
   TMR1IE_bit = 1;               // habilita interrupção timer 1
   TMR1IP_bit = 1;               // timer 1 com prioridade alta
   
//=========PORT==========
   TRISD.F0 = 1;                //RD0 botão de pausa
   TRISB = 0;                   //Porta para 7 bits dos displays
   TRISA = 0b110000;            //bits particulares dos displays como saídas

 }

void multi(){
     static short ctrl = 0;     //variável estática(permanente)
                                //seleciona qual display está ativo
     
     //vetor com códigos para números de 0 a 9 em um display de 7 segmentos
     unsigned char display[] = {0x3f,0x06, 0x5B, 0x4F, 0x66,
     0x6D, 0x7D, 0x07, 0x7f, 0x67 } ;
     
     switch(ctrl){

        case 0:                           //caso ctrl for 0
           ctrl = 1;                      //ctrl passa a ser 1
           PORTA = 1;                     //RA0 é 1 e ativa display 0
           PORTB = display[seg % 10];     //display 0 recebe a unidade da variável seg
           break;
        case 1:                           //caso ctrl for 1
           ctrl = 2;                      //ctrl passa a ser 2
           PORTA = 2;                     //RA1 é 1 e ativa display 1
           PORTB = display[seg/10 - (seg % 10)/10];     //display 1 recebe a dezena da variável seg
           break;
        case 2:                           //caso ctrl for 2
           ctrl = 3;                      //ctrl passa a ser 3
           PORTA = 4;                     //RA2 é 1 e ativa display 2
           PORTB = display[min % 10];     //display 2 recebe a unidade da variável min
           break;
        case 3:                           //caso ctrl for 3
           ctrl = 0;                      //ctrl passa a ser 0
           PORTB = display[min/10 - (min % 10)/10]; //display 3 recebe a dezena da variável min
           PORTA = 8;                     //RA3 é 1 e ativa display 3
           break;
     }

}

void time(){

  seg++;                        //incrementa a variável seg
  if(seg == 60){                //se seg for 60...
    min++;                      //incrementa min
    seg = 0;                    //reinicia seg
  }
  
  if(min == 60){                //se min for 60
    min = 0;                    //reinicia min
  }

}

void pause(){

  TMR1ON_bit = ~TMR1ON_bit;    //desliga timer 1 e conseguentemente pausa o cronômetro

}
