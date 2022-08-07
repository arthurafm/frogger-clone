#include "arquivos.h"
#include "constantes.h"
#include "estruturas.h"
#include "desenho.h"
#include <stdlib.h>

double TransformaTempo(struct timespec tempo) // Converte uma estrutura do tipo timespec em um tempo medido em segundos
{
	return tempo.tv_sec + (double)tempo.tv_nsec / 1000000000;
}

short TestaColisao(SAPO sapo, VEICULO veiculo) // Testa se houve colis�o com algum ve�culo.
{
	if (veiculo.valido) // Se o ve�culo � v�lido
	{
		// Se houve colis�o vertical,
		if ((sapo.env[0].y <= veiculo.env[1].y) && (sapo.env[1].y >= veiculo.env[0].y))
		{
			// Se houve colis�o horizontal
			if ((sapo.env[0].x <= veiculo.env[1].x) && (sapo.env[1].x >= veiculo.env[0].x))
				return 1;
		}
	}
	return 0;
}

short TestaColisoes(SAPO sapo, VEICULO lista[]) // Testa se houve colis�o com qualquer ve�culo de uma lista.
{
	short teste = 0; // Vari�vel de retorno
	int   i     = 0; // Vari�vel contadora

	// verifica se tamb�m houve colis�o vertical com algum ve�culo.
	while ((teste == 0) && (i < NUM_VEICULOS))
	{
		teste = TestaColisao(sapo, lista[i]);
		i++;
	}

	return teste;
}

short TestaColisoesPistas(SAPO sapo, PISTA pistas[]) // Testa colis�es para todas as pistas
{
	short teste = 0;
	int   i     = 0;

	while (!teste && (i < NUM_PISTAS))
	{
		teste = TestaColisoes(sapo, pistas[i].lista);
		i++;
	}

	return teste;
}

void InformaTamanho(VEICULO veiculo, int *tam_x,
					int *tam_y) // Retorna o tamanho nas dimens�es x e y de um dado ve�culo.
{
	switch (veiculo.tipo)
	{
		case SPORT:
		{
			*tam_x = T_SPORT;
			*tam_y = L_SPORT;
			break;
		}
		case SEDAN:
		{
			*tam_x = T_SEDAN;
			*tam_y = L_SEDAN;
			break;
		}
		case ONIBUS:
		{
			*tam_x = T_ONIBUS;
			*tam_y = L_ONIBUS;
			break;
		}
		default: break;
	}
	*tam_x = *tam_x - 1;
}

void AlteraCor(VEICULO *veic)
{
	COLORS cor_temp;

	switch (veic->cor)
	{
		case 1:
		{
			cor_temp = LIGHTGRAY;
			break;
		}
		case 2:
		{
			cor_temp = LIGHTMAGENTA;
			break;
		}
		case 3:
		{
			cor_temp = YELLOW;
			break;
		}
		case 4:
		{
			cor_temp = LIGHTCYAN;
			break;
		}
		default: cor_temp = WHITE;
	}
	veic->cor = cor_temp;
}

void AleatorizaDistancia(VEICULO *veic) {veic->distancia = (int)((double)rand() / RAND_MAX * 15) + 11;}

// Preenche um arranjo de ve�culos com seus valores padr�es.
void InicializaVeiculos(VEICULO lista[], DIRECAO direcao, int y_2)
{
	int i; // Vari�vel contadora
	int tam_x, tam_y, x_central = CentralizaX(0); // Vari�veis auxiliares

	for (i = 0; i < NUM_VEICULOS; i++)
	{
		lista[i].tipo = (int)((double)rand() / RAND_MAX * TIPOS) + 1;
		AleatorizaDistancia(&lista[i]);
		lista[i].dir      = direcao;
		lista[i].cor      = (int)((double)rand() / RAND_MAX * 6);
		AlteraCor(&lista[i]);
		InformaTamanho(lista[i], &tam_x, &tam_y);

		switch (direcao)
		{
			case ESQ: // Se for esquerdo, o x1 � o x2 do carro anterior + a dist�ncia do carro atual
			{
				if (i > 0)
					lista[i].env[0].x = lista[i - 1].env[1].x + lista[i].distancia;
				else
					lista[i].env[0].x = x_central;
				lista[i].env[1].x     = lista[i].env[0].x + tam_x - 1;
				break;
			}
			case DIR: // Se for direito, o x2 � o x1 do carro anterior - a dist�ncia do carro atual
			{
				if (i > 0)
					lista[i].env[1].x = lista[i - 1].env[0].x - lista[i].distancia;
				else
					lista[i].env[1].x = x_central;
				lista[i].env[0].x     = lista[i].env[1].x - tam_x + 1;
				break;
			}
			default:break;
		}
		lista[i].env[1].y = y_2;
		lista[i].env[0].y = lista[i].env[1].y - tam_y + 1;
		lista[i].valido   = 1;
	}
}

void InicializaPistas(PISTA pistas[], const float velocidades[], const int y_pistas[]) // Inicializa as pistas
{
	DIRECAO         dir = ESQ;
	struct timespec t_0;
	double          tempo_0;

	clock_gettime(CLOCK_MONOTONIC, &t_0);
	tempo_0 = TransformaTempo(t_0);

	srand(time(NULL));

	for (int i = 0; i < NUM_PISTAS; ++i)
	{
		if (i >= 2)
			dir = DIR;
		pistas[i].velocidade = velocidades[i];
		pistas[i].dir        = dir;
		pistas[i].t_0        = tempo_0;
		pistas[i].y_2        = y_pistas[i];
		InicializaVeiculos(pistas[i].lista, pistas[i].dir, pistas[i].y_2);
	}
}

void MoveVeiculo(VEICULO *veic) // Move um �nico ve�culo na tela.
{
	short  incremento = C_INCR;
	COLORS cor_temp   = veic->cor;

	// Apaga o ve�culo
	veic->cor    = COR_FUNDO;
	veic->valido = DesenhaVeiculo(*veic);

	if (veic->dir == ESQ) // Calcula o incremento
		incremento = -C_INCR;

	for (int j = 0; j < 2; ++j) // Aplica o incremento nos 2 pontos do envelope
		veic->env[j].x = veic->env[j].x + incremento;

	veic->cor    = cor_temp;
	veic->valido = DesenhaVeiculo(*veic);
}

void MoveListaVeiculos(VEICULO lista[]) // Move toda uma lista de ve�culos
{
	int     dist_aux; // Vari�vel auxiliar
	DIRECAO direcao;

	direcao = lista[0].dir;

	for (int i = 0; i < NUM_VEICULOS; i++)
	{
		MoveVeiculo(&lista[i]);
		/* Se o ve�culo for inv�lido, ou seja, estiver fora da tela, � preciso mudar sua coordenada para que ele
		 * fique atr�s do ve�culo anterior: */
		if (!lista[i].valido)
		{
			switch (direcao)
			{
				case ESQ:
				{
					if (lista[i].env[1].x <= X_INI) // Se o ve�culo estiver na borda esquerda
					{
						if (i > 0)
							// A dist�ncia auxiliar � inicializada como o x do ve�culo anterior
							dist_aux = lista[i - 1].env[1].x;
						else
							// A dist�ncia auxiliar � inicializada como o x do �ltimo ve�culo da lista
							dist_aux = lista[NUM_VEICULOS - 1].env[1].x;
						AleatorizaDistancia(&lista[i]);
						dist_aux = dist_aux + lista[i].distancia - lista[i].env[0].x;

						for (int j = 0; j < 2; ++j)
						{
							lista[i].env[j].x += dist_aux;
						}
						break;
					}
					break;
				}
				case DIR:
				{
					if (lista[i].env[0].x >= X_FIM) // Se o ve�culo estiver na borda direita
					{
						if (i > 0) // A dist�ncia auxiliar � inicializada como o x do ve�culo anterior
							dist_aux = lista[i - 1].env[0].x;
						else // A dist�ncia auxiliar � inicializada como o x do �ltimo ve�culo da lista
							dist_aux = lista[NUM_VEICULOS - 1].env[0].x;
						AleatorizaDistancia(&lista[i]);
						dist_aux = lista[i].env[1].x - dist_aux + lista[i].distancia;

						for (int j = 0; j < 2; ++j)
							lista[i].env[j].x -= dist_aux;
					}
					break;
				}
				default: break;
			}
		}
	}
}

void MovePistas(PISTA pistas[]) // Move todas as pistas
{
	struct timespec tempo_atual;
	double          tempo_atual_s;

	// Grava o tempo atual e converte-o para segundos
	clock_gettime(CLOCK_MONOTONIC, &tempo_atual);
	tempo_atual_s = TransformaTempo(tempo_atual);

	for (int i = 0; i < NUM_PISTAS; ++i)
	{
		// Se se passou um tempo maior do que a velocidade, desenha o ve�culo e atualiza o tempo
		if (tempo_atual_s - pistas[i].t_0 >= pistas[i].velocidade)
		{
			pistas[i].t_0 = tempo_atual_s;
			MoveListaVeiculos(pistas[i].lista);
		}
	}
}
