#include "estruturas.h"
#include <stdio.h>

#ifndef FROGGER_ARQUIVOS_H
#define FROGGER_ARQUIVOS_H

void RemoveInvalidos(char *string);

void RecebeNome(char nome[]);

short SalvaJogo(ESTADO *estado);

short LeJogoSalvo(ESTADO *estado, char *nome_jogador);

short AbreArqJog(FILE **arq_txt);

int LeArqTexto(FILE *arq_txt, JOGADOR lista_jog[]);

void OrdenaListaJogadores(JOGADOR lista_jog[], int jogs_lidos, int inserido);

void InsereJogador(JOGADOR jog, JOGADOR *lista, int jog_lidos, int *aux);

void MostraListaJogadores(JOGADOR *lista, JOGADOR jog, int jogs_lidos, int aux);

int SalvaListaJogadores(FILE *arq_txt, JOGADOR *lista, int jogs_lidos, int aux);

void CriaArquivoTxt(FILE *arq_txt, JOGADOR jog, JOGADOR *lista);

void OrganizaPontuacao(FILE *arq_txt, JOGADOR jog);

#endif // FROGGER_ARQUIVOS_H