#include "constantes.h"
#include "estruturas.h"
#include <string.h>
#include <conio2.h>
#include <windows.h>

int CentralizaX(int tamanho) // Dado um tamanho, retorna um x que centralizaria um objeto daquele tamanho
{
	return (X_FIM - X_INI - tamanho) / 2 + X_INI;
}

int CentralizaY(int tamanho) // Dado um tamanho, retorna um y que centralizaria um objeto daquele tamanho
{
	return (Y_FIM - Y_INI - tamanho) / 2 + Y_INI;
}

void CentralizaTexto(char texto[37], int y_1, COLORS cor) // Coloca um texto centralizado na tela, na posição y dada.
{
	int comprimento, x_central;
	comprimento = strlen(texto);
	x_central   = CentralizaX(comprimento);

	textcolor(cor);
	cputsxy(x_central, y_1, texto);
	textcolor(COR_TEXTO);
}

void DesenhaBorda(void) // Desenha a borda do jogo.
{
	int coluna, linha; // Variáveis contadoras

	textcolor(COR_BORDA);
	textbackground(COR_BORDA);

	for (coluna = X_INI; coluna <= X_FIM; ++coluna) // Preenche a borda com o caractere
	{
		for (linha = Y_INI; linha <= Y_FIM; ++linha)
		{
			if (coluna == X_INI || coluna == X_FIM || linha == Y_INI || linha == Y_FIM)
				putchxy(coluna, linha, CH_BORDA);
		}
	}

	textcolor(COR_FUNDO);
	textbackground(COR_FUNDO);
}

void PreparaTela(void)
{
	clrscr();
	DesenhaBorda();
}

void DesenhaTela(void)
{
	int y_azul[T_AZUL] = {Y_INI - 1, Y_FIM + 1};

	textbackground(BLUE);
	textcolor(BLUE);

	for (int l = 0; l < T_AZUL; l++)
	{
		for (int coluna = X_INI; coluna <= X_FIM; ++coluna)
		{
			putchxy(coluna, y_azul[l], '_');
		}
	}

	textcolor(COR_TEXTO);
	cputsxy(X_INI + 1, y_azul[0], "(c)arregar o jogo\t (p)ausa\t (ESC) Sair\t (r)anking");

	textbackground(COR_FUNDO);
}

// Desenha uma figura, coluna por coluna. Note que a figura é desenha mesmo se estiver parcialmente na área do jogo.
void DesenhaFigura(int x_1, int y_1, int tamanho_x, int tamanho_y, char figura[tamanho_y][tamanho_x], int cor)
{
	int linha, coluna; // Variáveis contadoras
	textcolor(cor); // Altera a cor para a fornecida
	for (coluna = 0; coluna < tamanho_x - 1; ++coluna)
	{
		for (linha = 0; linha < tamanho_y; ++linha)
		{
			if ((x_1 + coluna) < X_FIM && (x_1 + coluna) > X_INI
				&& (y_1 + linha) > Y_INI && (y_1 + linha) < Y_FIM) // Se estiver na área do jogo:
				putchxy((x_1 + coluna), (y_1 + linha),
						figura[linha][coluna]); // Desenha coluna por coluna, [se o caractere atual não for um nulo]
		}
	}
	textcolor(COR_TEXTO);
}

void DesenhaSport(VEICULO sport) // Desenha o esporte
{
	char sport_direita[L_SPORT][T_SPORT]  = {".--._.",
											 "`o--o´"};
	char sport_esquerda[L_SPORT][T_SPORT] = {"._.--.",
											 "`o--o´"};
	switch (sport.dir)
	{
		case DIR:
		{
			DesenhaFigura(sport.env[0].x, sport.env[0].y, T_SPORT, L_SPORT, sport_direita, sport.cor);
			break;
		}
		case ESQ:
		{
			DesenhaFigura(sport.env[0].x, sport.env[0].y, T_SPORT, L_SPORT, sport_esquerda, sport.cor);
			break;
		}
		default: break;
	}
}

void DesenhaSedan(VEICULO sedan) // Desenha o sedã
{
	char sedan_direita[L_SEDAN][T_SEDAN]  = {"  _____",
											 "-´|_|_\\`_",
											 "`-(_)(_)-'"};
	char sedan_esquerda[L_SEDAN][T_SEDAN] = {"   _____",
											 " _´/_|_|`-",
											 "'-(_)(_)-´"};
	switch (sedan.dir)
	{
		case DIR:
		{
			DesenhaFigura(sedan.env[0].x, sedan.env[0].y, T_SEDAN, L_SEDAN, sedan_direita, sedan.cor);
			break;
		}
		case ESQ:
		{
			DesenhaFigura(sedan.env[0].x, sedan.env[0].y, T_SEDAN, L_SEDAN, sedan_esquerda, sedan.cor);
			break;
		}
		default: break;
	}
}

void DesenhaOnibus(VEICULO onibus) // Desenha o ônibus
{
	char onibus_direita[L_ONIBUS][T_ONIBUS]  = {" _________",
												"|[][][]||]\\",
												"|______||__|",
												" O    O    O"};
	char onibus_esquerda[L_ONIBUS][T_ONIBUS] = {"  _________",
												" /]||[][][]|",
												"|__||______|",
												" O   O    O"};
	switch (onibus.dir)
	{
		case DIR:
		{
			DesenhaFigura(onibus.env[0].x, onibus.env[0].y, T_ONIBUS, L_ONIBUS, onibus_direita, onibus.cor);
			break;
		}
		case ESQ:
		{
			DesenhaFigura(onibus.env[0].x, onibus.env[0].y, T_ONIBUS, L_ONIBUS, onibus_esquerda, onibus.cor);
			break;
		}
		default: break;
	}
}

short DesenhaVeiculo(VEICULO veiculo) // Desenha um veículo qualquer
{
	int xc_1, xc_2, yc_1, yc_2; // Variáveis auxiliares
	xc_1 = veiculo.env[0].x;
	xc_2 = veiculo.env[1].x;
	yc_1 = veiculo.env[0].y;
	yc_2 = veiculo.env[1].y;

	// Se o veículo está, mesmo que parcialmente na área do jogo
	if (xc_2 > X_INI && xc_1 < X_FIM && yc_2 > X_INI && yc_1 < X_FIM)
	{
		switch (veiculo.tipo)
		{
			case SPORT:
			{
				DesenhaSport(veiculo);
				break;
			}
			case SEDAN:
			{
				DesenhaSedan(veiculo);
				break;
			}
			case ONIBUS:
			{
				DesenhaOnibus(veiculo);
				break;
			}
		}
		return 1;
	}
	return 0;
}

void DesenhaSapo(COORDENADA env, COLORS cor)
{
	char sapo_s[L_SAPO][T_SAPO] = {"  @..@",
								   "\\(----)/"};

	textcolor(cor);
	for (int i = 0; i < L_SAPO; ++i)
	{
		cputsxy(env.x, env.y + i, sapo_s[i]);
	}
	textcolor(COR_FUNDO);
}

void DesenhaSapoSt(SAPO sapo) // Desenha o sapo
{
	DesenhaSapo(sapo.env[0], sapo.cor);
}

void MostraSapos(SAPO lista_sapos[]) // Mostra os sapos do jogador na parte inferior da tela.
{
	COORDENADA temp;
	COLORS     cor = COR_TEXTO;

	temp.y = Y_FIM + 2;

	for (int i = 0; i < NUM_SAPOS; i++)
	{
		switch (lista_sapos[i].status)
		{
			case ATIVO:
			{
				cor = COR_TEXTO;
				break;
			}
			case ESPERA:
			{
				cor = LIGHTBLUE;
				break;
			}
			case SALVO:
			{
				cor = LIGHTGREEN;
				break;
			}
			case MORTO:
			{
				cor = LIGHTRED;
				break;
			}
			default: break;
		}
		temp.x = i * (T_SAPO + 3) + X_INI + 4;

		DesenhaSapo(temp, cor);
	}
	textbackground(COR_FUNDO);
}

void AnimaSapo(SAPO sapo) // Animação de morte/socorro do sapo
{
	DesenhaSapoSt(sapo);
	Sleep(VEL_EXPLOSAO); // Aguarda
	// Apaga o sapo
	sapo.cor = COR_FUNDO;
	DesenhaSapoSt(sapo);
}
