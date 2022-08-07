/* Trabalho final de Arthur Melo e Eduardo Mattje.
 * Jogue com um tamanho de tela de no mínimo 50 (configurar pelo console).
 * Não foram atendidos os requisitos da fase 2, ela não foi implementada.*/

#include "constantes.h"
#include "estruturas.h"
#include "arquivos.h"
#include "desenho.h"
#include "veiculos.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>

char LeTecla() // Lê teclas do usuário.
{
	char t_saida = '\0'; // Variável de entrada
	fflush(stdin);

	if (kbhit())
	{
		t_saida = (char)getch();

		if (t_saida == '\0') // Se a tecla digitada por uma seta, e registrar primeiro um '\0'.
		{
			fflush(stdin); // Limpa a memória temporária no caso de o jogador ter pressionado e segurado uma tecla.
			t_saida = (char)getch();
		}
	}
	return t_saida;
}

void MensagensMenu(int y_ini, COLORS cor) // Exibe todas as mensagens iniciais centralizadas na tela
{
	char mensagens_ini[L_M_INI][T_MENSAGENS] =
			 {"Bem-vindo ao Frogger!", "Instrucoes:", "Use as setas para se mover.",
			  "Desvie dos carros e atravesse a rua.", "[ENTER] comeca o jogo",
			  "[c] carrega o jogo", "[p] pausa o jogo"};

	for (int mensagem = 0; mensagem < L_M_INI; mensagem++)
	{
		CentralizaTexto(mensagens_ini[mensagem], y_ini, cor);
		y_ini++;
	}
}

void MensagensCarrega(COLORS cor)
{
	char mensagens[T_CARREGA][T_MENSAGENS] = {"Nao foi possivel ler este arquivo",
											  "[p] voltar",
											  "[qualquer] carregar outro arquivo"};
	int  y_central                         = CentralizaY(0) - 1;

	for (int mensagem = 0; mensagem < T_CARREGA; ++mensagem)
	{
		CentralizaTexto(mensagens[mensagem], y_central + mensagem, cor);
	}
}

short CarregaJogo(ESTADO *estado)
{
	char  nome[TAM], entrada;
	short teste = 0;

	do
	{
		RecebeNome(nome);
		if (LeJogoSalvo(estado, nome) == 0) // Caso a leitura de jogo salvo deu certo
			teste = 1; // O loop pode ser quebrado
		else // Do contrário, mostra uma mensagem de erro, e solicita ou uma nova leitura, ou a volta à parte anterior
		{
			MensagensCarrega(COR_TEXTO);
			do
			{
				entrada = LeTecla();
			} while (entrada == '\0');
			MensagensCarrega(COR_FUNDO);
			if (entrada == CH_PAUSA)
				return 0;
		}
	} while (!teste);

	return teste;

}

short Menu(ESTADO *estado) // Exibe o menu do jogo.
{
	char  entrada;
	int   y_ini = 10;
	short teste; // Testa para ver se houve carregamento do jogo.

	do
	{
		MensagensMenu(y_ini, COR_TEXTO); // Mostra as mensagens do menu.

		do {entrada = LeTecla();}
		while ((entrada != CH_ENTER) && (entrada != CH_CARREGA)); // Espera o usuário digitar alguma das opções.

		MensagensMenu(y_ini, COR_FUNDO);
		if (entrada == CH_CARREGA)
			teste = CarregaJogo(estado);
		else
			return 0;
	} while (!teste);

	return teste;
}

// Transfere os valores do jogador, lista_sapos e pistas para o estado.
void InstanciaJogo(ESTADO estado, JOGADOR *jogador, SAPO *lista_sapos, PISTA pistas[])
{
	*jogador = estado.jogador; // Preenche o campo "jogador".

	for (int i = 0; i < NUM_SAPOS; i++)
		lista_sapos[i] = estado.lista_sapos[i]; // Preenche o campo "lista_sapos".

	for (int i = 0; i < NUM_PISTAS; ++i)
		pistas[i] = estado.pistas[i];
}

// Muda as coordenadas dadas, somente se elas não saírem da área do jogo. Para ser usada com o sapo.
void MudaCoordenada(SAPO *sapo, char tecla)
{
	int mod_x = 0, mod_y = 0; // Variáveis modificadoras das coordenadas

	switch (tecla)
	{
		case CH_CIMA:
		{
			if (sapo->env[0].y > Y_INI) // Se o y1 não ultrapassar o limite superior da tela, muda as coordenadas.
				mod_y = -1;
			break;
		}
		case CH_ESQUERDA:
		{
			if (sapo->env[0].x > X_INI + 1) // Se o x1 não ultrapassar o limite esquerdo da tela, muda as coordenadas.
				mod_x = -1;
			break;
		}
		case CH_BAIXO:
		{
			if (sapo->env[1].y < Y_FIM - 1) // Se o y2 não ultrapassar o limite inferior da tela, muda as coordenadas.
				mod_y = 1;
			break;
		}
		case CH_DIREITA:
		{
			if (sapo->env[1].x < X_FIM - 1) // Se o x2 não ultrapassar o limite direito da tela, muda as coordenadas.
				mod_x = 1;
			break;
		}
		default: break; // Para demais teclas, fazer nada.
	}

	if (mod_x != 0 || mod_y != 0) // Se houve modificação, muda as coordenadas do envelope do sapo
	{
		for (int i = 0; i < 2; ++i)
		{
			sapo->env[i].x = sapo->env[i].x + mod_x;
			sapo->env[i].y = sapo->env[i].y + mod_y;
		}
	}
}

void MoveSapo(SAPO *sapo, char entrada) // Move o sapo
{
	COLORS cor_temp = sapo->cor;

	sapo->cor = COR_FUNDO;
	DesenhaSapoSt(*sapo); // Apaga o sapo
	MudaCoordenada(sapo, entrada); // Move a coordenada
	sapo->cor = cor_temp;
	DesenhaSapoSt(*sapo); // Desenha o sapo
}

short MataSapo(SAPO lista_sapos[], PISTA pistas[], int indice) // Mata o sapo
{
	int   i = indice; // Variável contadora
	short colisao; // Variável de teste

	colisao = TestaColisoesPistas(lista_sapos[indice], pistas); // Testa a colisao para todas as pistas
	if (colisao) // Se houver alguma colisão
	{
		lista_sapos[indice].cor = LIGHTRED;
		AnimaSapo(lista_sapos[indice]); // Animação de morte do sapo
		lista_sapos[indice].status = MORTO; // Muda o estado do sapo atual para morto
		while (i < NUM_SAPOS)
		{
			if (lista_sapos[i].status == ESPERA) // Se houver um sapo em ESPERA.
			{
				lista_sapos[i].status = ATIVO; // Seu status muda para ATIVO.
				return 1;
			}
			i++;
		}
		return 2; // Retorna 2 se não há mais sapos em espera.
	}
	else
		return 0;
}

void InicializaSapos(SAPO lista[]) // Preenche o arranjo dos sapos com seus valores padrões.
{
	for (int i = 0; i < NUM_SAPOS; i++)
	{
		if (i > 0)
			lista[i].status = ESPERA; // Se não for o primeiro sapo, preenche seu status como ESPERA.
		else
			lista[i].status = ATIVO; // Se for o primeiro sapo, preenche seu status como ATIVO.
		lista[i].cor        = WHITE;
		lista[i].env[0].x   = CentralizaX(T_SAPO - 1); // Preenche x de env[0].
		lista[i].env[0].y   = Y_FIM - L_SAPO; // Preenche x de env[1]
		lista[i].env[1].x   = lista[i].env[0].x + T_SAPO - 2; // Preenche y de env[0]
		lista[i].env[1].y   = lista[i].env[0].y + L_SAPO - 1; // Preenche y de env[1]
	}
}

void InicializaJogador(JOGADOR *jog) // Inicializa um jogador com valores padrões.
{
	jog->sapos_salvos = 0; // Preenche sapos_salvos.
	jog->sapos_espera = (NUM_SAPOS - 1); // Preenche sapos_espera
	jog->tempo_jogo   = 0; // Tempo no começo do jogo
	jog->escore       = 0; // Preenche escore
	jog->nome[0] = '\0'; // Preenche nome
}

// Transfere os valores das demais estruturas ao ESTADO.
void InstanciaEstado(ESTADO *estado, JOGADOR jogador, SAPO lista_sapos[], PISTA pistas[])
{
	estado->jogador = jogador; // Preenche o jogador

	for (int i = 0; i < NUM_SAPOS; i++)
		estado->lista_sapos[i] = lista_sapos[i]; // Preenche a lista de sapos

	for (int i = 0; i < NUM_PISTAS; ++i)
	{
		estado->pistas[i] = pistas[i]; // Preenche as pistas
	}
}

void MensagensPausa(COLORS cor) // Exibe as mensagens de pausa na tela.
{
	char mensagem[T_MENSAGENS] = {"Pressione [p] para voltar ao jogo",};
	textcolor(cor);
	for (int i = 0; i < 1; ++i)
	{
		CentralizaTexto(mensagem, Y_INI + i + 1, cor); // Impressão das mensagens
	}
	textcolor(COR_TEXTO);
}

void NomeiaJogador(JOGADOR *jogador) // Preenche o campo "nome" do JOGADOR.
{
	if (jogador->nome[0] == '\0') // Se não existe nome, solicita ao usuário para criar um
	{
		RecebeNome(jogador->nome); // Recebe o nome
	}
}

void Pausa(ESTADO *estado, SAPO sapos[], PISTA pistas[], JOGADOR *jogador) // Pausa o jogo
{
	char entrada; // Variáveis de entrada

	NomeiaJogador(jogador);
	MensagensPausa(COR_TEXTO); // Exibe as mensagens de pausa.

	do // Espera um comando
	{
		entrada = LeTecla();
		InstanciaEstado(estado, *jogador, sapos, pistas); // Instancia o ESTADO
		SalvaJogo(estado); // E salva o jogo em arquivo binário
	} while (entrada != CH_PAUSA); // Até que a tecla "p" seja pressionada
	MensagensPausa(COR_FUNDO); // Apaga as mensagens de pausa.
}

void CalculaEscore(JOGADOR *jog) // Calcula a pontuação do jogador
{
	int escore; // Variável auxiliar

	escore = (1200 * jog->sapos_salvos) - (jog->tempo_jogo * 5);

	if (escore < 0) // Caso o escore seja inválido (negativo), zera-o.
		escore = 0;

	jog->escore = escore; // Preenche escore do JOGADOR.
}

void MostraEscore(JOGADOR *jog) // Mostra o escore na tela
{
	int y_central = CentralizaY(0); // Variável auxiliar
	PreparaTela(); // Apaga a tela
	NomeiaJogador(jog); // Nomeia o jogador
	CalculaEscore(jog); // Calcula escore
	gotoxy((CentralizaX(20 + strlen(jog->nome))), y_central);
	textcolor(COR_TEXTO);
	printf("O escore de %s e: %04d", jog->nome, jog->escore); // Informa na tela
}

void MostraInfo(JOGADOR jog) // Mostra o tempo e pontuação do jogo na tela
{
	char      tempo_f[T_TEMPO];
	struct tm *info_tempo;

	info_tempo = localtime(&(jog.tempo_jogo));
	strftime(tempo_f, T_TEMPO, "%M:%S", info_tempo); // Formata o tempo em MM:SS

	textcolor(COR_TEXTO);
	textbackground(BLUE);
	gotoxy((X_INI + 1), (Y_FIM + 1));
	printf("TEMPO: %s\t PONTUACAO: %d\t NOME: %s", tempo_f, jog.escore, jog.nome);
	textbackground(COR_FUNDO);
}

void LoopDoJogo(JOGADOR *jog, SAPO lista_sapos[], PISTA pistas[], ESTADO *estado) // ‘Loop’ principal do jogo
{
	char  entrada  = '\0';
	short teste_colisao; // Variável do teste de colisão, 0 = não colidiu, 1 = colidiu
	int   i_sapo;
	FILE  *arq_txt = NULL;

	time_t tempo_inicial = time(NULL), tempo_jogo, tempo_pausa_1, tempo_pausa_2 = 0;
	tempo_jogo = jog->tempo_jogo; // Salva o tempo de jogo para caso tenha sido aberto um arquivo.
	i_sapo     = NUM_SAPOS - jog->sapos_espera - 1; // Dá valor ao índice do sapo

	DesenhaSapoSt(lista_sapos[i_sapo]); // Faz o desenho inicial do sapo.

	while (entrada != CH_ESC) // Enquanto não for pressionado ESC.
	{
		jog->tempo_jogo = time(NULL) - tempo_inicial + tempo_jogo - tempo_pausa_2; // Altera o tempo de jogo
		CalculaEscore(jog);
		MoveSapo(&lista_sapos[i_sapo], entrada);
		MovePistas(pistas); // Desenha as pistas E move os veículos
		MostraSapos(lista_sapos);
		teste_colisao = MataSapo(lista_sapos, pistas, i_sapo);

		if (lista_sapos[i_sapo].env[0].y == (Y_INI + 1))  // Caso o sapo atravesse a rua.
		{
			lista_sapos[i_sapo].status = SALVO;
			jog->sapos_salvos++;
			jog->sapos_espera--;
			lista_sapos[i_sapo].cor = LIGHTGREEN;
			AnimaSapo(lista_sapos[i_sapo]); // Desenha o sapo em vermelho
			++i_sapo;

			if (i_sapo == NUM_SAPOS) // Se o índice de sapos for igual ao número de sapos
				entrada = CH_ESC; // Encerra o loop
			else
				lista_sapos[i_sapo].status = ATIVO; // Coloca o próximo sapo como ATIVO.
		}
		else if (teste_colisao == 1)  // Caso haja colisão, mas ainda hajam sapos
		{
			jog->sapos_espera--;
			++i_sapo;
			lista_sapos[i_sapo].status = ATIVO; // Coloca o próximo sapo como ATIVO.
		}
		else if (teste_colisao == 2 || lista_sapos[(NUM_SAPOS - 1)].status == SALVO
				 || lista_sapos[(NUM_SAPOS - 1)].status == MORTO)   // Caso não hajam mais sapos
			entrada = CH_ESC; // Encerra o loop
		else // Caso nada tenha ocorrido
			entrada = LeTecla(); // Lê tecla

		if (entrada == CH_PAUSA) // Se 'p' for pressionado
		{
			tempo_pausa_1 = time(NULL);
			Pausa(estado, lista_sapos, pistas, jog); // Pausa o jogo
			tempo_pausa_2 = tempo_pausa_2 + time(NULL) - tempo_pausa_1;
			textcolor(COR_TEXTO);
		}
		else if (entrada == CH_CARREGA)
		{
			InstanciaEstado(estado, *jog, lista_sapos, pistas);
			CarregaJogo(estado);
			PreparaTela();
			DesenhaTela();
			InstanciaJogo(*estado, jog, lista_sapos, pistas);
			i_sapo        = NUM_SAPOS - jog->sapos_espera - 1;
			tempo_inicial = time(NULL);
			tempo_jogo    = jog->tempo_jogo;
		}
		else if (entrada == CH_RANKING)
		{
			NomeiaJogador(jog);
			OrganizaPontuacao(arq_txt, *jog);
		}
		MostraInfo(*jog); // Mostra o tempo de jogo e pontuação
	}
	MostraSapos(lista_sapos); // Mostra os estatutos dos sapos
	Sleep(1000); // Aguarda 1s
}

int main(void) // Aplicação das funções e ‘interface’
{
	PISTA   pistas[NUM_PISTAS];
	SAPO    lista_sapos[NUM_SAPOS];
	JOGADOR jog;
	ESTADO  estado;
	FILE    *arq_txt                = NULL;
	float   velocidades[NUM_PISTAS] = {0.10, 0.11, 0.13, 0.14};
	char    msg_fim[]               = "Fim do jogo!", entrada;
	int     y_pistas[NUM_PISTAS];

	y_pistas[0] = Y_INI + L_SAPO + L_ONIBUS + 2;
	y_pistas[1] = y_pistas[0] + 5;
	y_pistas[2] = CentralizaY(0) + 2;
	y_pistas[3] = y_pistas[2] + 5;

	_setcursortype(_NOCURSOR); // Muda o cursor
	PreparaTela();

	if (Menu(&estado) == 1) // Limpa a tela e faz os desenhos iniciais. Se for carregado um arquivo:
	{
		// Preenche as estruturas do jogo com os valores do ESTADO carregado.
		InstanciaJogo(estado, &jog, lista_sapos, pistas);
	}
	else
	{
		// Inicializa as configurações padrões do jogo
		InicializaSapos(lista_sapos);
		InicializaPistas(pistas, velocidades, y_pistas);
		InicializaJogador(&jog);
		InstanciaEstado(&estado, jog, lista_sapos, pistas);
	}

	DesenhaTela();
	LoopDoJogo(&jog, lista_sapos, pistas, &estado);// "Loop" do jogo
	fflush(stdin);
	MostraEscore(&jog); // Mostra o escore final do jogador
	gotoxy(CentralizaX(strlen(msg_fim)), CentralizaY(0) + 1);
	puts(msg_fim);

	// Mostra as pontuações maiores e salva os 10 maiores.
	OrganizaPontuacao(arq_txt, jog);
	gotoxy(X_INI, Y_FIM + 2);

	printf("Pressione [ENTER] para encerrar");
	do {entrada = LeTecla();}
	while ((entrada != CH_ENTER) && (entrada != CH_CARREGA)); // Não encerrar o jogo direto quando aberto em exe
	return 0;
}
