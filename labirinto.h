#ifndef COMMANDS_H_
#define COMMANDS_H_

// Cabe�alho das fun��es

// Fun��o que l� o labirinto do endereco e coloca na matriz lab
int readLab(char lab[30][100]);

// Fun��o que imprime o lab original
int showLab(char lab[30][100]);

// Carrega o labirinto e carrega as structs
void iniciaLabirinto(char lab[30][100]);

//Fun��es que retornam posi��o do pacman para as fun��es do arquivo main.c (Bugadas no momento)
int retornaXPacman(void);
int retornaYPacman(void);

#endif
