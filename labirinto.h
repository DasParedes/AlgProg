#ifndef COMMANDS_H_
#define COMMANDS_H_

// cabe�alho das fun��es
// fun��o que l� o labirinto do endereco e coloca na matriz lab
int readLab(char endereco[], char lab[30][100]);
// fun��o que imprime o lab original
void showLab(char lab[30][100]);
// carrega o labirinto e carrega as structs
int iniciaLabirinto(char lab[30][100], char endereco[]);
// apenas para testar as fun��es aqui criadas
int main();
// fun��o para testar apenas leitura do arquivo
void testaLabirinto();

#endif
