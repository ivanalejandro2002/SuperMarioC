#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <allegro.h>
#define YSCREEN 16
#define XSCREEN 22
#define ESCALA 8
#define ESC         27
#define ARRIBA      72
#define ABAJO       80
#define IZQUIERDA   75
#define DERECHA     77
#define SALTOMAY    62
#define SALTOMIN    122
#define CORROMAY    61
#define CORROMIN    121
#define FACTORMHOR  0.04
#define MAXVELH     0.5
#define FAUTOM      0.1
#define ESPERA      700
#define ACCY        0.1
#define AUMENTOVERT 0.8
#define MAXVELV     0.9
#define TECLA_ARRIBA    0
#define TECLA_ABAJO     2
#define TECLA_DERECHA   1
#define TECLA_IZQUIERDA 3
#define SALTO           4
#define CORRER          5
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
char oprimidas[6];
char oprimo;
/*void color(int X){
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),X);      ///cambia el color del texto
}*/
/*void gotoxy(int x, int y){
    HANDLE hcon;
    COORD dwpos;
    dwpos.X=x;
    dwpos.Y=y;
    hcon=GetStdHandle(STD_OUTPUT_HANDLE);               ///cambia la poscición del cursor
    SetConsoleCursorPosition(hcon,dwpos);
}*/
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
    while(x>0){
        if(kbhit())
        oprimo = teclear();
        else oprimo = 0;
        if(oprimo==ARRIBA)oprimidas[TECLA_ARRIBA]=1;
        if(oprimo==DERECHA)oprimidas[TECLA_DERECHA]=1;
        if(oprimo==IZQUIERDA)oprimidas[TECLA_IZQUIERDA]=1;
        if(oprimo==ABAJO)oprimidas[TECLA_ABAJO]=1;
        if(oprimo==SALTOMAY || oprimo == SALTOMIN)oprimidas[SALTO]=1;
        if(oprimo==CORROMAY || oprimo==CORROMIN)oprimidas[CORRER]=1;
        x--;
    }
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
    int cont;
    while(1){
        cont = 0;
        //gotoxy(0,0);
        system("cls");
        retardo();
        for(int i =0;i<6;i++){
            if(oprimidas[i])cont++;
            oprimidas[i]=0;
        }
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
            if(mario.vy==0){
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
        //gotoxy(0,YSCREEN+1);
        printf("                     ");
        printf("%f",mario.vh);
        //gotoxy(0,YSCREEN+2);
        printf("                     ");
        printf("%f",mario.x);
        //gotoxy(0,YSCREEN+3);
        printf("                     ");
        //gotoxy(0,YSCREEN+3);
        printf("%f",mario.vy);
        //gotoxy(0,YSCREEN+4);
        printf("                     ");
        //gotoxy(0,YSCREEN+4);
        printf("%f",mario.y);
        //gotoxy(0,YSCREEN+5);
        printf("                     ");
        //gotoxy(0,YSCREEN+5);
        printf("%d",cont);
    }
}
int main()
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480, 0, 0);
        if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }
    BITMAP *buffer = create_bitmap(640,480);
    clear_to_color(buffer, 0x33FF5B);

    inicializa();
    juega();
    return 0;
}
END_OF_MAIN();
