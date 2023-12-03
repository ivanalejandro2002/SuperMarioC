#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <allegro.h>
#define YSCREEN 15
#define XSCREEN 20
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
#define FACTORMHOR  0.003
#define MAXVELH     0.1
#define FAUTOM      0.025
#define ESPERA      5000000
#define RETFRAMES   15
#define ACCY        0.015
#define AUMENTOVERT 0.2
#define MAXVELV     0.2
#define ESCPIXEL    2
#define FRAMESSALTO 20
#define TECLA_ARRIBA    0
#define TECLA_ABAJO     2
#define TECLA_DERECHA   1
#define TECLA_IZQUIERDA 3
#define SALTO           4
#define CORRER          5
#define TECLA_ESC       6
#define YRESOLUTION 480
#define XRESOLUTION 640
#define FONDO       9605375
#define FONDO2      16777215
struct coord{
    float x;
    float y;
    float vh;
    float vy;
    char estado;
}mario;
struct fotos{
    int **pixeles;
    int alto;
    int ancho;
};
char mariociclo,tiemposalto,anterior,faltanframes;
struct fotos spritesmario;
struct fotos spritesescenario;
struct fotos spritesenemigos;
struct fotos spritesitems;

int xmundo,ymundo;
int xcamara,ycamara;
short **mundo;
BITMAP *bufferprint;
BITMAP *background;
int buffer[YRESOLUTION+2][XRESOLUTION+2];
int minimundo[YRESOLUTION+2][XRESOLUTION+2];
char oprimidas[7];
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
    printf("%d, %d\n",ymundo,xmundo);
    int i,j;
    mundo = (short **)malloc((ymundo+2)*sizeof(short *));
    for(i =0;i<ymundo+2;i++)mundo[i] = (short *)malloc(xmundo*sizeof(short));
    for(i=0;i<ymundo;i++){
        for(j=0;j<xmundo;j++){
            fscanf(fp,"%d",&mundo[i][j]);
        }
    }
    printf("\n\n");
    xcamara = XSCREEN-1;
    ycamara = ymundo-YSCREEN;

    mario.x = 2;
    mario.y = YSCREEN-3;
    mario.vh = mario.vy = 0;
    mario.estado = 0;
    mariociclo = 0;
    tiemposalto = FRAMESSALTO;
}
void retardo(){
    int x = ESPERA;
    while(x>0){
        if(key[KEY_I])oprimidas[TECLA_ARRIBA]=1;
        if(key[KEY_K])oprimidas[TECLA_ABAJO]=1;
        if(key[KEY_L])oprimidas[TECLA_DERECHA]=1;
        if(key[KEY_J])oprimidas[TECLA_IZQUIERDA]=1;
        if(key[KEY_Z])oprimidas[SALTO]=1;
        if(key[KEY_X])oprimidas[CORRER]=1;
        if(key[KEY_ESC])oprimidas[TECLA_ESC]=1;
        x--;
    }
}
void render(){
    int i,j,zi,zj;
    for(i=0;i<YSCREEN;i++){
        for(j=0;j<XSCREEN;j++){
            minimundo[i][j]=mundo[ycamara+i][xcamara-XSCREEN+1+j];
        }
    }
    for(i=0;i<YSCREEN;i++){
        for(j=0;j<XSCREEN;j++){
            for(int zi=0;zi<2*ESCALA;zi++){
                for(int zj=0;zj<2*ESCALA;zj++){
                    buffer[i*2*ESCALA+zi][j*2*ESCALA+zj]=spritesescenario.pixeles[(minimundo[i][j]/16)*2*ESCALA+zi][(minimundo[i][j]%16)*2*ESCALA+zj];
                }
            }
        }
    }
    if(mario.vh>=0^(mario.estado%16==4)){
        for(int i=0;i<4*ESCALA;i++){
            for(int j=0;j<2*ESCALA;j++){
                if(spritesmario.pixeles[mario.estado/16*4*ESCALA+i][(mario.estado%16)*2*ESCALA+j] !=FONDO && spritesmario.pixeles[mario.estado/16*4*ESCALA+i][(mario.estado%16)*2*ESCALA+j]!=FONDO2)
                buffer[(int)(mario.y*2*ESCALA)+i-2*ESCALA][(int)(mario.x*2*ESCALA)+j]=spritesmario.pixeles[mario.estado/16*4*ESCALA+i][(mario.estado%16)*2*ESCALA+j];
            }
        }
    }else{
        for(int i=0;i<4*ESCALA;i++){
            for(int j=0;j<2*ESCALA;j++){
                if(spritesmario.pixeles[mario.estado/16*4*ESCALA+i][(mario.estado%16)*2*ESCALA+2*ESCALA-j-1] !=FONDO && spritesmario.pixeles[mario.estado/16*4*ESCALA+i][(mario.estado%16)*2*ESCALA+2*ESCALA-j-1]!=FONDO2)
                buffer[(int)(mario.y*2*ESCALA)+i-2*ESCALA][(int)(mario.x*2*ESCALA)+j]=spritesmario.pixeles[mario.estado/16*4*ESCALA+i][(mario.estado%16)*2*ESCALA+2*ESCALA-j-1];
            }
        }
    }

    for(int i=0;i<YRESOLUTION;i++){
        for(int j=0;j<XRESOLUTION;j++){
            if(buffer[i][j]!=FONDO && buffer[i][j]!=FONDO2)
            rectfill(bufferprint,j*ESCPIXEL,i*ESCPIXEL,(j+1)*ESCPIXEL-1,(i+1)*ESCPIXEL-1,buffer[i][j]);
        }
    }

    blit(bufferprint,screen,0,0,0,0,XRESOLUTION,YRESOLUTION);
    /*for(i=0;i<YSCREEN;i++){
        for(int j=0;j<XSCREEN;j++){
            printf("%c",buffer[i][j]);
        }
        printf("\n");
    }*/
}
void juega(){
    int cont;
    while(1){
        cont = 0;
        //gotoxy(0,0);
        retardo();
        for(int i =0;i<6;i++){
            if(oprimidas[i]){
                cont++;
            }
        }
        render();
        if(mario.vh!=0 && !oprimidas[SALTO])tiemposalto=0;
        mario.estado-=mario.estado%16;
        if(cont!=0){
            if(oprimidas[TECLA_ESC])return;
            if(oprimidas[TECLA_DERECHA]){
                if(mario.vh<0)mario.estado=mario.estado-mario.estado%16+4;
                else{
                    mario.estado=mario.estado-mario.estado%16+1;
                    if(!faltanframes){
                        faltanframes=RETFRAMES;
                        mariociclo++;
                        if(mariociclo==4)mariociclo=0;
                    }else faltanframes--;
                    mario.estado+=mariociclo;
                    if(mariociclo==3)mario.estado-=2;
                }
                if(mario.vh<MAXVELH)mario.vh+=FACTORMHOR;
            }else if(oprimidas[TECLA_IZQUIERDA]){
                if(mario.vh>0)mario.estado =mario.estado-mario.estado%16+4;
                else{
                    mario.estado=mario.estado-mario.estado%16+1;
                    if(!faltanframes){
                        faltanframes=RETFRAMES;
                        mariociclo++;
                        if(mariociclo==4)mariociclo=0;
                    }else faltanframes--;
                    mario.estado+=mariociclo;
                    if(mariociclo==3)mario.estado-=2;
                }
                if(mario.vh>-MAXVELH)mario.vh-=FACTORMHOR;
            }
            if(oprimidas[SALTO] && tiemposalto){
                tiemposalto--;
                //printf(":0");
                mario.vy = -AUMENTOVERT;
            }
        }else{
            //if(mario.vy==0){
                if(mario.vh<0){
                    mario.vh+=FAUTOM;
                    if(mario.vh>0)mario.vh=0;
                }else if(mario.vh>0){
                    mario.vh-=FAUTOM;
                    if(mario.vh<0)mario.vh=0;
                }else{
                    mario.estado=mario.estado-mario.estado%16+0;
                }
            //}
        }

        if(mario.vh<0){
            if(minimundo[(int)mario.y][(int)(mario.x+mario.vh)]>0 || minimundo[(int)(mario.y-.9+1)][(int)(mario.x+mario.vh)]>0){
                mario.x = (int)(mario.x+mario.vh+.9)+.001;
                mario.vh=0;
            }
        }
        if(mario.vh>0){
            if(minimundo[(int)mario.y][(int)(mario.x+mario.vh+1)]>0 || minimundo[(int)(mario.y-.9+1)][(int)(mario.x+mario.vh+1)]>0){
                mario.x = (int)(mario.x+mario.vh)-.001;
                mario.vh=0;
            }
        }




        mario.x+=mario.vh;

        if(mario.x+mario.vh<0){
            mario.x=0;
            mario.vh=0;
        }

        if(mario.vy !=0)mario.estado=mario.estado-mario.estado%16+5;
        else tiemposalto=FRAMESSALTO;
        if(mario.vy<MAXVELV)mario.vy+=ACCY;


        if(mario.vy<0)
        if(minimundo[(int)(mario.y+mario.vy-.1)][(int)mario.x]>0 || minimundo[(int)(mario.y+mario.vy-.1)][(int)(mario.x+.9)]>0){
            mario.y = (int)(mario.y+mario.vy+.9);
            mario.vy=0;
        }
        if(mario.vy>0)
        if(minimundo[(int)(mario.y+1+mario.vy)][(int)mario.x]>0 || minimundo[(int)(mario.y+1+mario.vy)][(int)(mario.x+.9)]>0){
            mario.y = (int)(mario.y+.9);
            mario.vy=0;
        }
        mario.y+=mario.vy;


        //gotoxy(0,YSCREEN+1);
        /*printf("                     ");
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
        */
        for(int i =0;i<6;i++){
            if(oprimidas[i]){
                //printf("%d,",i);
                cont++;
            }
            oprimidas[i]=0;
        }
    }
}
void carga(int ***memoria,int *width, int *height,char *nombre){
    FILE* f = fopen(nombre, "rb");
    if(f == NULL){
        printf("Argument Exception");
        exit(1);
    }
    unsigned char *info = malloc(54*sizeof(unsigned char));
    for(int i=0;i<54;i++){
        fscanf(f,"%c",&info[i]);
    }
    //fread(info, sizeof(unsigned char), 54, f);//read the 54-byte header
    int w,h;
    w = *(int*)&info[18];
    h = *(int*)&info[22];
    *width = w;
    *height = h;

    *memoria = (int **)malloc(((int)*height+2)*sizeof(int *));
    for(int i =0;i<(int)*height+2;i++)(*memoria)[i] = (int *)malloc(((int)*width+2)*sizeof(int));

    int row_padded = ((int)*width*3 + 3) & (~3);
    unsigned char data[row_padded];

    for(int i = (int)*height-1; i >=0; i--){
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = ((int)*width-1)*3; j >=0; j -= 3){
           //Convert (B, G, R) to (R, G, B)
           (*memoria)[i][j/3]=(data[j]+data[j+1]*256+data[j+2]*256*256);
        }
    }


    fclose(f);
}
void archivos(){
    carga(&spritesmario.pixeles,&spritesmario.ancho,&spritesmario.alto,"mario.bmp");
    carga(&spritesescenario.pixeles,&spritesescenario.ancho,&spritesescenario.alto,"bloques.bmp");
}
int main()
{
    allegro_init();
    install_keyboard();
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480, 0, 0);
        if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }

    bufferprint = create_bitmap(640,480);
    background = create_bitmap(640,480);
    clear_to_color(bufferprint, 0);
    inicializa();
    archivos();
    juega();
    return 0;
}
END_OF_MAIN();
