#include "constantes.h"
#include "estruturas.h"

#ifndef FROGGER_DESENHO_H
#define FROGGER_DESENHO_H

int CentralizaX(int tamanho);

int CentralizaY(int tamanho);

void CentralizaTexto(char texto[T_MENSAGENS], int y_1, COLORS cor);

void DesenhaBorda(void);

void PreparaTela(void);

void DesenhaTela(void);

void DesenhaFigura(int x_1, int y_1, int tamanho_x, int tamanho_y, char figura[tamanho_y][tamanho_x], int cor);

void DesenhaSport(VEICULO sport);

void DesenhaSedan(VEICULO sedan);

void DesenhaOnibus(VEICULO onibus);

short DesenhaVeiculo(VEICULO veiculo);

void DesenhaSapoSt(SAPO sapo);

void MostraSapos(SAPO lista_sapos[]);

void AnimaSapo(SAPO sapo);

#endif // FROGGER_DESENHO_H
