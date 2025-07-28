#include <stdio.h>
#include <stdlib.h>
#include "usuarios.h"
#include "cartas.h"

void menuPrincipal(CartaNode **lista) {
    int opc;
    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Cadastrar Carta\n");
        printf("2. Listar Cartas\n");
        printf("3. Editar Carta\n");
        printf("4. Excluir Carta\n");
        printf("5. Ordenar Cartas por ID\n");
        printf("6. Buscar Carta por ID (Binária)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch(opc) {
            case 1:
                cadastrarCarta(lista);
                salvarCartas(*lista);
                break;
            case 2:
                listarCartas(*lista);
                break;
            case 3:
                editarCarta(*lista);
                salvarCartas(*lista);
                break;
            case 4:
                excluirCarta(lista);
                salvarCartas(*lista);
                break;
            case 5:
                ordenarCartasPorID(lista);
                salvarCartas(*lista);
                break;
            case 6: {
                int n = 0;
                CartaNode *temp = *lista;
                while (temp) { n++; temp = temp->prox; }

                CartaPokemon *array = malloc(n * sizeof(CartaPokemon));
                temp = *lista;
                for (int i = 0; i < n; i++) {
                    array[i] = temp->carta;
                    temp = temp->prox;
                }
                ordenarCartasPorID(lista);

                int id;
                printf("Digite o ID a buscar: ");
                scanf("%d", &id);
                CartaPokemon *res = buscaBinaria(array, 0, n - 1, id);
                if (res)
                    printf("Encontrado: %s | Tipo: %s | ATK: %d | DEF: %d\n",
                           res->nome, res->tipo, res->ataque, res->defesa);
                else
                    printf("Carta não encontrada.\n");
                free(array);
                break;
            }
        }
    } while (opc != 0);
}

int main() {
    char email[80];
    int escolha;
    CartaNode *listaCartas = carregarCartas();

    printf("\nBem-vindo ao sistema Pokémon TCG Manager!\n");
    printf("1. Login\n2. Cadastrar\nEscolha: ");
    scanf("%d", &escolha);

    if (escolha == 2)
        cadastrarUsuario();

    if (login(email)) {
        mensagemBoasVindas(email);
        menuPrincipal(&listaCartas);
    }

    salvarCartas(listaCartas);
    printf("Encerrando programa.\n");
    return 0;
}
