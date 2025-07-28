#ifndef CARTAS_H
#define CARTAS_H

typedef struct CartaPokemon {
    int id;
    char nome[50];
    char tipo[30];
    int ataque;
    int defesa;
    int favorito;
} CartaPokemon;

typedef struct CartaNode {
    CartaPokemon carta;
    struct CartaNode *prox;
} CartaNode;

CartaNode* carregarCartas();
void salvarCartas(CartaNode* lista);
void listarCartas(CartaNode* lista);
void cadastrarCarta(CartaNode **lista);
void editarCarta(CartaNode *lista);
void excluirCarta(CartaNode **lista);
void ordenarCartasPorID(CartaNode **lista);
CartaPokemon* buscaBinaria(CartaPokemon* array, int inicio, int fim, int id);

#endif
