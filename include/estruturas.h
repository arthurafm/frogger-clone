#include "constantes.h"
#include <conio2.h>

#ifndef FROGGER_ESTRUTURAS_H
#define FROGGER_ESTRUTURAS_H

typedef enum
{
  ESPERA = 1, ATIVO = 2, SALVO = 3, MORTO = 4
} STATUS_SAPO;

typedef enum
{
  ESQ = 0, DIR = 1
} DIRECAO;

typedef enum
{
  SEDAN = 1, SPORT = 2, ONIBUS = 3
} TIPO_VEICULO;

typedef struct Coordenada
{
  int x;
  int y;
} COORDENADA;

typedef struct Jogador
{
  char   nome[TAM];
  int    sapos_espera;
  int    sapos_salvos;
  time_t tempo_jogo;
  int    escore;
} JOGADOR;

typedef struct Sapo
{
  COORDENADA  env[2];
  STATUS_SAPO status;
  COLORS      cor;
} SAPO;

typedef struct Veiculo
{
  COORDENADA   env[2];
  TIPO_VEICULO tipo;
  int          distancia;
  DIRECAO      dir;
  short        valido;
  COLORS       cor;
} VEICULO;

typedef struct Pista
{
  double  t_0;
  DIRECAO dir;
  VEICULO lista[NUM_VEICULOS];
  float   velocidade;
  int     y_2;
} PISTA;

typedef struct
{
  JOGADOR jogador;
  SAPO    lista_sapos[NUM_SAPOS];
  PISTA   pistas[NUM_PISTAS];
} ESTADO;

#endif // FROGGER_ESTRUTURAS_H
