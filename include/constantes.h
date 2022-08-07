#include <conio2.h>
#include <time.h>

#ifndef FROGGER_CONSTANTES_H
#define FROGGER_CONSTANTES_H

// Constantes da borda
#define X_INI 1
#define Y_INI 2
#define X_FIM 90
#define Y_FIM 39
#define COR_BORDA 12 // Cor vermelha
#define CH_BORDA '_'

// Constantes de cores
#define COR_FUNDO 0  // Cor preta
#define COR_TEXTO 15 // Cor branca

// Constantes de teclas
#define CH_ENTER '\15'
#define CH_ESC '\33'
#define CH_CIMA '\110'
#define CH_ESQUERDA '\113'
#define CH_BAIXO '\120'
#define CH_DIREITA '\115'
#define CH_CARREGA 'c'
#define CH_PAUSA 'p'
#define CH_RANKING 'r'

// Constantes de tamanho
#define L_M_INI 7
#define T_MENSAGENS 50
#define T_CARREGA 3
#define T_TEMPO 6
#define TAM 20
#define T_AZUL 2
#define C_INCR 1

// Constantes dos sapos
#define L_SAPO 2
#define T_SAPO 9
#define NUM_SAPOS 6      // Quantidade de sapos

// Constantes dos veículos
#define L_SPORT 2
#define T_SPORT 7
#define L_SEDAN 3
#define T_SEDAN 12
#define L_ONIBUS 4
#define T_ONIBUS 14
#define TIPOS 3            // Tipos de veículos
#define NUM_VEICULOS 20    // Quantidade de veículos por fileira
#define NUM_PISTAS 4
#define VEL_EXPLOSAO 1000    // Velocidade da animação da explosão.

// Constantes dos arquivos
#define NUM_JOG 10 // Número de jogadores gravados no arquivo de ranking
#define MAX_STR 255

#endif // FROGGER_CONSTANTES_H
