#include <LiquidCrystal_I2C.h>  //BIBLIOTECA DO LCD
#include "RTClib.h"             //BIBLIOTECA DO "RELOGIO" PARA PEGAR DATA

#define botao_prioridade 4  //BOTAO PRIORIDADE
#define botao_geral 5       //BOTAO GERAL
#define botao_exame 6       //BOTAO EXAME
#define botao_atendente 7   //BOTAO ATENDENTE

RTC_DS1307 rtc;                     //NOMEAÇÃO DO "RELOGIO/CALENDARIO"
LiquidCrystal_I2C lcd(0x27, 16, 2); //NOMEAÇÃO DO LCD

String fila[100];                //FILA DE ESPERA
int contador_fila = 0;          //CONTADOR FILA
int contador_prioridade = 0;    //CONTADOR PRIORIDADE
int contador_geral = 0;         //CONTADOR GERAL
int contador_exame = 0;         //CONTADOR EXAME
int senha_atendente = 0;        //CONTADOR DAS SENHAS CHAMADAS PELA ATENDENTE

String data;                //ARMAZENA A DATA
char formato[] = "YYMMDD";  //FORMATO DA DATA
String senha = "";          //ARMAZENA A SENHA COMPLETA
String senha_chamada;       //SENHA CHAMADA PELA ATENDENTE

void setup() {
  rtc.begin();        //INICIA O RELOGIO
  lcd.begin(16, 2);   //INICIA O LCD
  lcd.backlight();    //LIGA AS LUZES DO LCD

  /*--- SETANDO AS PORTAS COMO INPUT ---*/
  pinMode(botao_prioridade, INPUT_PULLUP); 
  pinMode(botao_geral, INPUT_PULLUP);
  pinMode(botao_exame, INPUT_PULLUP);
  pinMode(botao_atendente, INPUT_PULLUP);  
}

void loop() {
  DateTime now = rtc.now();       //PEGA DATA DO MOMENTO
  data = now.toString(formato);   //ARMAZENA A DATA FORMATADA

  /*---FAZ A CHAMADA DA FUNCAO PARA TIRAR SENHA DE ACORDO COM O SELECIONADO---*/
  if(digitalRead(botao_prioridade) == LOW)senhas(1); 
  if(digitalRead(botao_geral) == LOW)senhas(2);
  if(digitalRead(botao_exame) == LOW)senhas(3);
  if(digitalRead(botao_atendente) == LOW)atendente();

  /*---MOSTRA A ULTIMA SENHA CHAMADA---*/
  lcd.setCursor(0, 0);
  lcd.print("Ultima Senha:");
  lcd.setCursor(0, 1);
  lcd.print(senha_chamada);
}

/*---FUNÇÃO RESPONSAVEL PELA GERAÇÃO DAS SENHAS---*/
void senhas(int a) {
  switch (a) {
    case 1:
      contador_prioridade++;
      senha = data + "-SP" + contador_prioridade;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha Prioridade");
      lcd.setCursor(0, 1);
      lcd.print(senha);
      delay(2000);
      break;

    case 2:
      contador_geral++;
      senha = data + "-SG" + contador_geral;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha Geral");
      lcd.setCursor(0, 1);
      lcd.print(senha);
      delay(2000);
      break;

    case 3:
      contador_exame++;
      senha = data + "-SE" + contador_exame;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha Exame");
      lcd.setCursor(0, 1);
      lcd.print(senha);
      delay(2000);
      break;
  }
  fila[contador_fila] = senha; //SALVA A SENHA RETIRADA NA FILA DE SENHAS
  contador_fila++;
  lcd.clear();
}

/*---FUNÇÃO QUE REPESENTA A ATENDENTE CHAMANDO AS SENHAS---*/
void atendente() {
  senha_chamada = fila[senha_atendente];
  senha_atendente++;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(senha_chamada);
  lcd.setCursor(0, 1);
  lcd.print("GUICHE: X");
  delay(2000);
  lcd.clear();
}