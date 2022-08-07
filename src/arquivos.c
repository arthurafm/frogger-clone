#include "desenho.h"
#include "constantes.h"
#include "estruturas.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *const kFormatoRanking  = "%s,%d,%d,%d,%d\n";
static char *const kNomeRanking     = "ranking_frogger.txt";
static char *const kDelim           = ",";
static char *const kFormatoExibicao = "%10s - %04d";

// Muda uma ‘string’ para conter o nome do jogador com a extensão bin.
void NomeiaArquivo(char nome_jogador[], char nome_arquivo[])
{
	strcpy(nome_arquivo, nome_jogador);
	strcat(nome_arquivo, ".bin");
}

short SalvaJogo(ESTADO *estado) // Salva o ESTADO do jogo em arquivo binário.
{
	short retorno;
	FILE  *arq;
	char  nome_arquivo[TAM + 4];

	NomeiaArquivo(estado->jogador.nome, nome_arquivo);
	arq = fopen(nome_arquivo, "wb");

	// Se ocorrer um erro na abertura
	if (arq == NULL)
		return 1;
	else
	{
		if (fwrite(estado, sizeof(ESTADO), 1, arq) == 0) // Se não conseguiu escrever no arquivo.
			retorno = 1;
		else
			retorno = 0;
	}

	fclose(arq);
	return retorno;
}

void RemoveInvalidos(char *string) // Remove os caracteres inválidos de uma ‘string’
{
	char  invalidos[] = "\\<>:\"/|?*", texto[] = "Seus caracteres invalidos foram substituidos por '_'";
	short encontrou   = 0;

	// Termina a ‘string’ ao encontrar um caractere terminador de linha
	string[strcspn(string, "\r\n")] = '\0';

	// Substitui todos os caracteres inválidos por _
	while (strpbrk(string, invalidos) != NULL)
	{
		*strpbrk(string, invalidos) = '_';
		encontrou = 1;
	}

	if (encontrou)
	{
		CentralizaTexto(texto, Y_INI + 3, COR_TEXTO);
		Sleep(3000);
		CentralizaTexto(texto, Y_INI + 3, COR_FUNDO);
	}
}

void RecebeNome(char nome[]) // Recebe um nome do jogador e grava-o no arranjo de entrada.
{
	// Variáveis auxiliares
	char msg[] = "Informe seu nome:";
	int  y     = Y_INI + 1;

	CentralizaTexto(msg, y, COR_TEXTO); // Imprime na tela a mensagem
	gotoxy(CentralizaX(strlen(msg)), y + 1);
	fgets(nome, TAM, stdin); // Recebe o nome do jogador na linha abaixo da mensagem, gravando-o no arranjo
	RemoveInvalidos(nome);
	CentralizaTexto(msg, y, COR_FUNDO); // Apaga a mensagem
	textcolor(COR_FUNDO); // E o nome do jogador
	cputsxy(CentralizaX(strlen(msg)), y + 1, nome);
	textcolor(COR_TEXTO);
}

short LeJogoSalvo(ESTADO *estado, char *nome_jogador) // Lê um arquivo binário salvo
{
	char  nome_arquivo[TAM + 4];
	FILE  *arq;
	short retorno;

	NomeiaArquivo(nome_jogador, nome_arquivo);
	arq = fopen(nome_arquivo, "rb"); // Abre o arquivo

	if (fread(estado, sizeof(ESTADO), 1, arq) == 1) // Se não houver erros na leitura
		retorno = 0;
	else // Se houve erros na leitura
		retorno = 1;

	fclose(arq);
	return retorno;
}

short AbreArqJog(FILE **arq_txt) // Abre um arquivo texto
{
	*arq_txt = fopen(kNomeRanking, "r"); // Abre um arquivo texto

	if (*arq_txt == NULL) // Se houver um erro na abertura
		return 0;
	else
		return 1;
}

int LeArqTexto(FILE *arq_txt, JOGADOR lista_jog[]) // Lê um arquivo texto salvo
{
	char str_temp[MAX_STR];
	int  i = 0;

	while (!feof(arq_txt) && i < NUM_JOG)
	{
		if (fgets(str_temp, MAX_STR, arq_txt) != NULL) // Se não houver erros na leitura
		{
			strcpy(lista_jog[i].nome, strtok(str_temp, kDelim)); // Preenche nome
			lista_jog[i].sapos_espera =
				(short)atoi(strtok(NULL, kDelim)); // Preenche a quantidade de sapos em espera
			lista_jog[i].sapos_salvos = (short)atoi(strtok(NULL, kDelim)); // Preenche a quantidade de sapos salvos
			lista_jog[i].tempo_jogo   = atoi(strtok(NULL, kDelim)); // Preenche o tempo de jogo
			lista_jog[i].escore       = atoi(strtok(NULL, kDelim)); // Preenche o escore
			i++;
		}
	}

	fclose(arq_txt);
	return i;
}

// Ordena a lista de jogadores do de maior pontuação ao menor, através do algoritmo bubble sort.
void OrdenaListaJogadores(JOGADOR lista_jog[], int jogs_lidos, int inserido)
{
	JOGADOR aux;

	if (inserido == 3) // Se o jogador tiver entrado na pontuação
		jogs_lidos++;

	for (int i = 0; i < jogs_lidos; ++i) // Bubble sort
	{
		for (int j = 0; j < jogs_lidos - i - 1; ++j)
		{
			if (lista_jog[j].escore < lista_jog[j + 1].escore)
			{
				aux = lista_jog[j];
				lista_jog[j]     = lista_jog[j + 1];
				lista_jog[j + 1] = aux;
			}
		}
	}
}

void InsereJogador(JOGADOR jog, JOGADOR *lista, int jog_lidos, int *aux) // Insere o jogador na lista de jogadores
{
	for (int i = 0; i < jog_lidos; i++)
	{
		if (strcmp(jog.nome, lista[i].nome) == 0) // Se o jogador já tiver na pontuação
		{
			if (jog.escore > lista[i].escore) // Se o escore do jogador for maior que o da lista
				lista[i] = jog; // Substitui o jogador da lista
			*aux = 1;
		}
	}

	if (*aux != 1) // Se o jogador não estiver na lista
	{
		if (jog_lidos != NUM_JOG) // Se a lista tiver menos jogadores que NUM_JOG
		{
			lista[jog_lidos] = jog; // Coloca o jogador no final da lista
			*aux = 3;
		}
		else if (jog.escore > lista[(NUM_JOG - 1)]
			.escore) // Caso contrário, se o jogador tiver um escore maior que o menor da lista
		{
			lista[(jog_lidos - 1)] = jog; // Substitui o último da lista
			*aux = 2;
		}
	}
}

void MostraListaJogadores(JOGADOR *lista, JOGADOR jog, int jogs_lidos, int aux) // Mostra o High Score
{
	int i;

	textcolor(COR_TEXTO);
	cputsxy(X_FIM + 3, Y_INI + 1, "Pontuacao:");

	if (aux == 3) // Se o jogador tiver entrado na pontuação
		jogs_lidos++;

	for (i = 0; i < jogs_lidos + 1; i++)
	{
		gotoxy((X_FIM + 3), (Y_INI + 2 + i));
		clreol();
	}
	for (i = 0; i < jogs_lidos; i++) // Mostra o nome e o escore de todos da lista
	{
		gotoxy((X_FIM + 3), (Y_INI + 2 + i));
		printf(kFormatoExibicao, lista[i].nome, lista[i].escore);
	}

	if (aux == 0) // Caso o jogador não tenha entrado, nem substituído na pontuação
	{
		gotoxy((X_FIM + 3), (Y_INI + 2 + jogs_lidos));
		printf(kFormatoExibicao, jog.nome, jog.escore); // Mostra o nome e o escore do jogador
	}
}

// Salva a pontuação em arquivo de texto
int SalvaListaJogadores(FILE *arq_txt, JOGADOR *lista, int jogs_lidos, int aux)
{
	int i = 0;

	if (aux == 3) // Se o jogador entrar na pontuação
		jogs_lidos++;

	arq_txt = fopen(kNomeRanking, "w");

	if (arq_txt == NULL) // Se houver erro na abertura
	{
		return 0;
	}
	else
	{
		while ((!feof(arq_txt)) && i < NUM_JOG && i < jogs_lidos)
		{
			// Escreve no txt os dados do jogador
			fprintf(arq_txt, kFormatoRanking, lista[i].nome, lista[i].sapos_espera, lista[i].sapos_salvos,
					(int)lista[i].tempo_jogo, lista[i].escore);
			i++;
		}
		fclose(arq_txt);

		return 1;
	}
}

void CriaArquivoTxt(FILE *arq_txt, JOGADOR jog, JOGADOR *lista) // Cria um arquivo em txt
{
	arq_txt = fopen(kNomeRanking, "w"); // Abre o arquivo texto

	if (arq_txt != NULL) // Se não houver erros na abertura
	{
		// Salva os dados do jogador
		fprintf(arq_txt, kFormatoRanking, jog.nome, jog.sapos_espera, jog.sapos_salvos, (int)jog.tempo_jogo,
				jog.escore);
		fclose(arq_txt);
		lista[0] = jog; // Coloca o jogador na primeira posição da lista
	}
}

void OrganizaPontuacao(FILE *arq_txt, JOGADOR jog) // Faz todos os processos com arquivos de texto
{
	int     j_lidos = 0, inserido = 0;
	JOGADOR lista_aux[NUM_JOG];

	if (AbreArqJog(&arq_txt) == 1) // Se já houver uma pontuação salva
	{
		j_lidos = LeArqTexto(arq_txt, lista_aux);
		OrdenaListaJogadores(lista_aux, j_lidos, inserido);
		InsereJogador(jog, lista_aux, j_lidos, &inserido);
		OrdenaListaJogadores(lista_aux, j_lidos, inserido);
		SalvaListaJogadores(arq_txt, lista_aux, j_lidos, inserido);
	}
	else
		CriaArquivoTxt(arq_txt, jog, lista_aux);

	MostraListaJogadores(lista_aux, jog, j_lidos, inserido);
}
