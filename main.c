//Includes das Libs
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <conio.h> //Para utilizar fun��es de movimenta��es do jogo
#include <windows.h> //Para detectar keystrokes
#include <ctype.h> //Para utilizar o tolower();

//Includes de headers locais
#include "labirinto.h"


//Structs
struct directions //Armazena eixo andado, e dire��o no eixo
{
    char coordinates; //'x', 'y' ou 's'
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
};

struct pacmanPosition //Armazena posi��o do pacman, e suas informa��es
{
    int x;
    int y;
    int lives;
};


//Inclus�o dos prot�pipos das fun��es da conio.c
void clrscr(void); //Fun��o que limpa a tela
void _setcursortype(int); //Fun��o para modificar o tipo de cursor
void textcolor(int); //Fun��o que controla cor do texto

//Prot�tipo das fun��es locais
void pacmanStart(void);
void pacmanPause(void);
void pacmanEnd(void);

void startGameMenu(void);
int startMessage(int);
void checkPacDots(void);

void setDirection(char, int*);
void movePacman(struct directions);
void testLimits(void);
void testColision(void);
void getPacmanPos(void);
void speedControl(struct directions); //PROVIS�RIO - SER� REMOVIDA

char detectKey(void);
void gotoXY(int, int);
void reconstructMaze(int, int, int, int);

//Variaveis Globais
struct pacmanPosition pacman; //Struct responsavel pelo pacman
char lab[30][100]; //Variavel responsavel por armazenar o labirinto
struct directions nextDirection; //Struct que armazena dados da posi��o a ser andada
struct directions lastDirection; //Struct que armazena dados da ultima posi��o andada
int contaPontos=0;


//Constantes
int const   TOP = 1, //Limite superior do mapa (Nunca menor que 0)
            LEFT = 1, //Limite esquerdo do mapa (Nunca menor que 0)
            HEIGHT = 30, //Limite inferior do mapa
            WIDTH = 100, //Limite direito do mapa
            SPEED = 100, //Velocidade padrao do jogo
            CURSOR = 0; // 0 para sem cursor; 1 para cursor de caixa; 2 para cursor normal



//Pac-man Main
int main()
{
    system("mode 100, 37"); //Defines screen size
    _setcursortype(CURSOR); //Sets the cursor according to a value declared in the constant 'CURSOR'

    startGameMenu(); //Start message

    pacman.lives=3; //Sets the initial number of lives of the pacman
    pacmanStart(); //The Game 'per se'

    return EXIT_SUCCESS; //End of the program
}



//Fun��es Locais

//Jogo em si
void pacmanStart(void)
{
    int continuaJogo=1, mostraStart=1, *point_continua=&continuaJogo;
    char key; //Tecla pressionada

    pacman.lives--; //Ao iniciar o jogo, diminui uma vida do pacman
    if(!pacman.lives)  //Se acabarem as vidas do pacman, finaliza o jogo
    {
        return;
    }

    if(showLab(lab)){ //Carrega o labirinto na memoria, carrega a posi��o do pacman, pastilhas, super-pastilhas, e fantasmas
        system("Pause");
        return; //Se der erro ao carregar, encera o programa
    }

    //Mensagem de saida
    gotoXY(36, 31);
    printf("Press 'space' or 'esc' to quit");
    gotoXY(46, 32);
    printf("Pontos: 0");

    //Carrega posi��es dos objetos
    getPacmanPos();

    //Roda o jogo
    while(continuaJogo)
    {
        if(mostraStart>-1)
        {
            mostraStart=startMessage(mostraStart); //Mostra mensagem de inicio enquanto necessario
        }

        if(kbhit()) //Ao pressionar uma tecla
        {
            //Flag para fazer n�o aparecer mais a mensagem inicial
            //Primeiro faz n�o imprimir mais a mensagem inicial, depois reconstroi o labirinto embaixo dela
            if(mostraStart>-1)
            {
                mostraStart--;
            }

            key=tolower(detectKey()); //Detecta tecla pressionada
            setDirection(key, point_continua); //Decodifica tecla pressionada
        }

        movePacman(nextDirection); //Realiza movimenta��o;
        checkPacDots();
        speedControl(nextDirection); //Controla a velocidade do jogo


    } //FIM DO WHILE

    pacmanEnd(); //Finaliza o jogo

    return;
}

//Pausa o jogo
void pacmanPause(void)
{
    char key='m'; //Inicializa com qualquer outro valor, para nao cair no While
    int count; //Contador para voltar o jogo ap�s 'count' segundos

    gotoXY(41,13);
    printf("                     ");
    gotoXY(41,14);
    printf("    Game paused!     ");
    gotoXY(41,15);
    printf(" Press 'R' to resume ");
    gotoXY(41,16);
    printf("                     ");

    while(key!='r')
    {
        if(kbhit())
        {
            if(GetAsyncKeyState(0x52))  //Ao clicar no R sair� desse loop
            {
                key='r';
            }
        }
    }

    for(count=2; count>0; count--)  //Contagem regressiva ao voltar para o jogo
    {
        gotoXY(45, 16);
        printf("%d seconds...", count);
        Sleep(1000);
    }

    reconstructMaze(12,17,40,62); //Reconstroi labirinto embaixo da mensagem

    return;
}

//Mensagem de t�rmino de jogo
void pacmanEnd(void)
{
    gotoXY(36,31);
    printf("The program will be finished!  ");

    gotoXY(35,32);
    printf("Press any key to close the game");

    textcolor(0); //Deixa texto em preto, tornando-o invisivel
    system("pause");
    textcolor(15);

    return;
}


//Apresenta menu de inicio do jogo
void startGameMenu(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacman.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    gotoXY(20, contador+1);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impress�o do PacMan
    {
        if(contador>19)
        {
            textcolor(15);
        }
        else
        {
            textcolor(14);
        }

        printf("%c", ch);

        if(ch=='\n' && contador<25)
        {
            contador++;
            gotoXY(20, contador+1);
            Sleep((contador-25)*-8); //Acelera a impress�o de cada linha aos poucos
        }
    }

    getch();
    clrscr();
    return;
}

//Mensagem que aparece no inicio do jogo
int startMessage(int flag)
{

    if (flag==1)  //Mensagem de inicio
    {
        gotoXY(39, 14);
        printf("                         ");
        gotoXY(39,15);
        printf(" Press any key to start  ");
        gotoXY(39, 16);
        printf("                         ");
    }
    else if(flag==0)  //Reconstru��o do labirinto embaixo ap�s a mensagem de in�cio
    {
        reconstructMaze(13,15,38,65);
        flag--;
    }

    return flag;
}

void checkPacDots(void){

    if(lab[pacman.y-1][pacman.x-1]=='o')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        contaPontos+=10;
        //printf("\a");
        //Beep(500, 30); //- Comentado at� sabermos se utilizaremos isso ou n�o

        gotoXY(46, 32);
        printf("Pontos: %d", contaPontos);
    }

}


void setDirection(char key, int* pointer){

     switch(key) //Verifica para onde ser� a nova dire��o
            {
            case 'w':
                nextDirection.coordinates='y'; //Seta dire��o no eixo y
                nextDirection.aumenta_diminui=-1; //Seta dire��o negativa
                break;
            case 'x':
                nextDirection.coordinates='y';
                nextDirection.aumenta_diminui=1; //Seta dire��o positiva
                break;
            case 'a':
                nextDirection.coordinates='x'; //Seta dire��o no eixo x
                nextDirection.aumenta_diminui=-1;
                break;
            case 'd':
                nextDirection.coordinates='x';
                nextDirection.aumenta_diminui=1;
                break;
            case 's':
                nextDirection.coordinates='s';
                nextDirection.aumenta_diminui=0;
                break;
            case 'p': //Pausa o jogo ao pressionar 'P'
                pacmanPause();
                break;
            case ' ':
                *pointer=0; //Ir� fazer o programa terminar sua execu��o
                nextDirection.coordinates='s'; //Faz pacman parar sua movimenta��o
                nextDirection.aumenta_diminui=0;
                break;
            }

}

//Movimenta��o e impress�o do PacMan na posi��o correta
void movePacman(struct directions next)
{

    gotoXY(pacman.x, pacman.y); //Apaga a posi��o atual do pacman
    printf(" ");

    switch(next.coordinates) //Calcula a proxima posi��o do pacman
    {
    case 'y':
        pacman.y+=next.aumenta_diminui;
        break;
    case 'x':
        pacman.x+=next.aumenta_diminui;
        break;
    }

    testLimits(); //Caso tenha chegado nos limites do mapa, coloca pacman no outro lado
    testColision(); //Caso comando coloque o pacman dentro de uma parede, tira ele de l�

    gotoXY(pacman.x,pacman.y);
    printf("C"); //Imprime a nova posi��o do pacman

    return;
}

//Testa se pacman chegou no limite do mapa, assim podendo colocar na posi��o correta
void testLimits(void)
{

    if(pacman.y < TOP) //Ao chegar nos limites do mapa, posiciona pacman no outro lado
    {
        pacman.y= HEIGHT;
    }
    else if(pacman.y > HEIGHT)
    {
        pacman.y = TOP;
    }
    else if(pacman.x < LEFT)
    {
        pacman.x=WIDTH;
    }
    else if(pacman.x > WIDTH)
    {
        pacman.x = LEFT;
    }

    return;
}

void testColision(void)
{

    if(lab[pacman.y-1][pacman.x-1]=='#') //Testa a colis�o caso esteja dentro de um campo de 'parede'
    {
        switch(nextDirection.coordinates) //Volta a ultima posi��o andada
        {
        case 'y':
            pacman.y-=nextDirection.aumenta_diminui;
            break;
        case 'x':
            pacman.x-=nextDirection.aumenta_diminui;
            break;
        }

        if(nextDirection.coordinates!=lastDirection.coordinates) //Faz continuar andando na ultima posi��o andada nessa itera��o
        {
            switch(lastDirection.coordinates)
            {
            case 'y':
                pacman.y+=lastDirection.aumenta_diminui;
                break;
            case 'x':
                pacman.x+=lastDirection.aumenta_diminui;
                break;
            }
        }
        //Faz programa continuar correndo na ultima dire��o andada na proxima itera��o
        nextDirection.aumenta_diminui=lastDirection.aumenta_diminui;
        nextDirection.coordinates=lastDirection.coordinates;

        if(lab[pacman.y-1][pacman.x-1]=='#') //Caso ja esteja em uma esquina, faz n�o entrar na parede, caso seja for�ado
    {
        switch(nextDirection.coordinates)
        {
        case 'y':
            pacman.y-=nextDirection.aumenta_diminui;
            break;
        case 'x':
            pacman.x-=nextDirection.aumenta_diminui;
            break;
        }
    }

    }
    else //Sen�o, confirma que ocorreu um movimento valido, e seta a ultima posi��o para ser igual a atual, para funcionar a proxima itera��o
    {
        lastDirection.coordinates=nextDirection.coordinates;
        lastDirection.aumenta_diminui=nextDirection.aumenta_diminui;
    }

    return;
}

void getPacmanPos(void){

    pacman.x=retornaXPacman();
    pacman.y=retornaYPacman();

    return;
}

//Controla velocidade do jogo - SER� ALTERADO PARA FUNCIONAR COM BASE NO TEMPO DO SISTEMA - PROVIS�RIO
void speedControl(struct directions last)
{

    //Caso o computador esteja com o CMD padrao, usar esse c�digo abaixo
    if (last.coordinates=='y')
    {
        Sleep(SPEED*1.5); //Corrige velocidade no eixo y
    }
    else
    {
        Sleep(SPEED);
    }

    //Caso o computador esteja com o CMD alterado para ter letras quadradas, usar o c�digo abaixo
    //Sleep(SPEED);
}



//Detecta a tecla pressionada
char detectKey(void)
{

    char key;

    if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) //Tecla para cima ou tecla 'W'
    {
        key = 'w';
    }
    else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x58)) //Tecla para baixo ou tecla 'X'
    {
        key = 'x';
    }
    else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) //Tecla para esquerda ou tecla 'A'
    {
        key = 'a';
    }
    else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) //Tecla para direita ou tecla 'D'
    {
        key = 'd';
    }
    else if (GetAsyncKeyState(0x53)) //Tecla 'S'
    {
        key = 's';
    }
    else if (GetAsyncKeyState(0x50)) //Tecla 'P'
    {
        key = 'p';
    }
    else if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) //Tecla 'ESC' ou tecla 'Espa�o'
    {
        key = ' ';
    }
    else //Sen�o retorna uma letra que n�o significa nenhuma dire��o, assim ser� utilizada a ultima dire��o andada
    {
        key='m';
    }

    return key;

}


// Fun��o gotoXY, tem limite igual a: system("mode 'x-1', 'y-1'");
// �ndice inicia em 0
// vai at� system(mode x-1, y-1)
void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x-1;
    coord.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Fun��o que reconstroi parte do labirinto conforme parametros passados
void reconstructMaze(int y_inicial, int y_final, int x_inicial, int x_final)
{

    int contador, count;
    for(contador=y_inicial; contador<=y_final; contador++)
    {
        for (count=x_inicial; count<=x_final; count++)
        {
            gotoXY(count+1,contador+1);
            if(lab[contador][count]=='#')
            {
                textcolor(15);
                printf("%c", lab[contador][count]);
            }
            else if(lab[contador][count]=='o')
            {
                textcolor(14);
                printf("%c", lab[contador][count]);
            }
            else
            {
                printf(" ");
            }

        }
    }

    gotoXY(pacman.x, pacman.y);
    printf("C");

    return;
}



//TODO LIST:

//CMD
//Mudar timer do jogo, para ser em fun��o do tempo do Sistema

//PACMAN
//Transformar dire��es do pacman em variaveis locais, e utilizar ponteiros
//Detec��o de colis�o com os fantasmas, pacman.lives--, Respawn;

//FANTASMAS
//Movimenta��o dos fantasmas (Movimenta��o com SuperPastilha OFF/ON - Movimenta��o 30% menor)
//Timer para ressuscitar
//Na struct, adicionar parametro 'Alive' (0 - Morto, 1 - Vivo)

//PASTILHAS
//Criar fun��o que detecta que comeu todas pastilhas -> EndGame - WIN

//SUPER-PASTILHAS
//Criar fun��o da super-pastilha
//Criar time - "contador" da super-pastilha

//EXTRAS:
//Dijkshtra
//Sistema de som
//Highscores
//Adicionar Cheat no F9 (Desativa detecc��o de colis�o com as paredes - Paredes valem 10000 pontos)

//FIM DO ARQUIVO
