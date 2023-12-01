#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#define YSCREEN 16
#define XSCREEN 22
#define ESCALA 1
#define ESC         27
#define ARRIBA      72
#define ABAJO       80
#define IZQUIERDA   75
#define DERECHA     77
#define SALTOMIN    62
#define SALTOMAY    122
#define FACTORMHOR  0.01
#define MAXVELH     0.5
#define FAUTOM      0.1
#define ESPERA      100000000
#define ACCY        0.1
#define AUMENTOVERT 0.8
#define MAXVELV     0.9
struct coord{
    float x;
    float y;
    float vh;
    float vy;
    char estado;
}mario;
int xmundo,ymundo;
int xcamara,ycamara;
char **mundo;
char buffer[YSCREEN+2][XSCREEN+2];
char minimundo[YSCREEN+2][XSCREEN+2];
void color(int X){
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),X);      ///cambia el color del texto
}
void gotoxy(int x, int y){
    HANDLE hcon;
    COORD dwpos;
    dwpos.X=x;
    dwpos.Y=y;
    hcon=GetStdHandle(STD_OUTPUT_HANDLE);               ///cambia la poscición del cursor
    SetConsoleCursorPosition(hcon,dwpos);
}
char teclear(){
    char tecla;
  if(kbhit()){

    //Posible eliminacion
    tecla=getch();
    if(tecla==-32){
        tecla=getch();
    }
    //hasta aquí

  }else return 0;
}
void inicializa(){
    FILE *fp;
    fp = fopen("nivel.txt","r");
    fscanf(fp,"%d %d\n",&ymundo,&xmundo);
    int i,j;
    mundo = (char **)malloc((ymundo+2)*sizeof(char *));
    for(i =0;i<ymundo+2;i++)mundo[i] = (char *)malloc(xmundo*sizeof(char));
    for(i=0;i<ymundo;i++){
        for(j=0;j<xmundo;j++){
            fscanf(fp,"%c",&mundo[i][j]);
        }
        fscanf(fp,"\n");
    }
    printf("\n\n");
    xcamara = XSCREEN-1;
    ycamara = ymundo-YSCREEN;

    mario.x = 2;
    mario.y = YSCREEN-3;
    mario.vh = mario.vy = 0;
    mario.estado = 'Q';
}
void retardo(){
    int x = ESPERA;
    while(x>0)x--;
}
void render(){
    int i,j;
    for(i=0;i<YSCREEN;i++){
        for(j=0;j<XSCREEN;j++){
            minimundo[i][j]=mundo[ycamara+i][xcamara-XSCREEN+1+j];
        }
    }
    for(i=0;i<YSCREEN;i++){
        for(j=0;j<XSCREEN;j++){
            buffer[i][j]=minimundo[i][j];
        }
    }
    buffer[(int)((mario.y*ESCALA)/ESCALA)][(int)((mario.x*ESCALA)/ESCALA)]=mario.estado;
    buffer[(int)(((mario.y-1)*ESCALA)/ESCALA)][(int)((mario.x*ESCALA)/ESCALA)]=mario.estado;
    for(i=0;i<YSCREEN;i++){
        for(int j=0;j<XSCREEN;j++){
            printf("%c",buffer[i][j]);
        }
        printf("\n");
    }
}
void juega(){
    while(1){
        gotoxy(0,0);
        retardo();
        render();
        char mov = teclear();
        if(mov){
            if(mov == ESC)return;
            if(mov == DERECHA){
                if(mario.vh<0)mario.estado='F';
                else(mario.estado)='R';
                if(mario.vh<MAXVELH)mario.vh+=FACTORMHOR;
            }else if(mov == IZQUIERDA){
                if(mario.vh>0)mario.estado = 'F';
                else(mario.estado)='R';
                if(mario.vh>-MAXVELH)mario.vh-=FACTORMHOR;
            }else if(mov == SALTOMAY || mov == SALTOMIN){
                mario.vy = -AUMENTOVERT;
            }
        }else{
            if(mario.vh<0){
                mario.vh+=FAUTOM;
                if(mario.vh>0)mario.vh=0;
            }else if(mario.vh>0){
                mario.vh-=FAUTOM;
                if(mario.vh<0)mario.vh=0;
            }else{
                mario.estado='Q';
            }
        }
        if(mario.x+mario.vh<=0){
            mario.vh=0;
            mario.x = 0;
        }
        if(minimundo[(int)mario.y][(int)(mario.x+mario.vh)]>'0')mario.vh=0;

        if(mario.vy !=0)mario.estado='S';
        if(mario.vy<MAXVELV)mario.vy+=ACCY;
        if(mario.vy>=0)
        if(minimundo[(int)(mario.y+1)][(int)(mario.y+mario.vy)]>'0'){
            if(mario.vy<0)
            mario.vy=(int)mario.vy;
            mario.vy=0;
        }
        mario.y+=mario.vy;

        mario.x+=mario.vh;
        gotoxy(0,YSCREEN+1);
        printf("                     ");
        printf("%f",mario.vh);
        gotoxy(0,YSCREEN+2);
        printf("                     ");
        printf("%f",mario.x);
        gotoxy(0,YSCREEN+3);
        printf("                     ");
        gotoxy(0,YSCREEN+3);
        printf("%f",mario.vy);
        gotoxy(0,YSCREEN+4);
        printf("                     ");
        gotoxy(0,YSCREEN+4);
        printf("%f",mario.y);


    }
}
int main()
{
    inicializa();
    juega();
    return 0;
}
