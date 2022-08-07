#ifndef FROGGER__VEICULOS_H_
#define FROGGER__VEICULOS_H_

double TransformaTempo(struct timespec tempo);

short TestaColisao(SAPO sapo, VEICULO veiculo);

short TestaColisoes(SAPO sapo, VEICULO lista[]);

short TestaColisoesPistas(SAPO sapo, PISTA pistas[]);

void InformaTamanho(VEICULO veiculo, int *tam_x, int *tam_y);

void InicializaVeiculos(VEICULO lista[], DIRECAO direcao, int y_2);

void InicializaPistas(PISTA pistas[], const float velocidades[], const int y_pistas[]);

void MoveListaVeiculos(VEICULO lista[]);

void MovePistas(PISTA pistas[]);

#endif //FROGGER__VEICULOS_H_
