
setup(){        //Função de configuração do PIC
  
 /*             PERIODO TIMER 2
    período = (PR2 + 1) * ciclo de máquina * prescaler do timer 2
    T = 256 * 0,2 us * 16 = 819,2 us
    f = 1/ T = 1 / 819,2 = 1220,70 Hz
 */
  PR2 = 0xFF;                       //PR2 carregado com 255
  T2CON = 0b00000111;               //Timer 2 ligado, prescaler 16
  CCP2CON = 0b00001100;             //CCP modo PWM

  //Configuração conversor AD
  ADCON0 = 0x00110001;              //canal 12 selecionado e conversor ligado
  ADCON1 = 0x00;                    //Todos os pinos possíveis analógicos
  
  //Configuração dos pinos
  TRISC.f1 = 0;                     //RC1 saída do PWM
  TRISB.f0 = 1;                     //RB0 entrada analógica para potenciometro



}

/*
=>função que passa o valor de 10 bits da conversão AD para os
registradores de duty cicle do PWM
*/
void pwm(){

    CCPR2L.F7 = ADRESH.F1;     //bit MSB do PWM recebe bit MSB da conversão AD
    CCPR2L.F6 = ADRESH.F0;     //7º bit do PWM recebe 7º bit da conversão AD
    CCPR2L.F5= ADRESL.F7;      //
    CCPR2L.F4= ADRESL.F6;      //             .
    CCPR2L.F3= ADRESL.F5;      //             .
    CCPR2L.F2= ADRESL.F4;      //             .
    CCPR2L.F1= ADRESL.F3;      //
    CCPR2L.F0= ADRESL.F2;      //
    CCP2CON.F5 = ADRESL.F1;    //2º bit do PWM recebe 2º bit da conversão AD
    CCP2CON.F4 = ADRESL.F0;    //bit LSB do PWM recebe bit LSB da conversão AD
}

void main() {

   setup();                  //configura o PIC
   
   while(1){
      ADC_Read(12);          //leitura da conversão
      pwm();                 //Passa 10 bits da conversão para duty cycle
   }

}
