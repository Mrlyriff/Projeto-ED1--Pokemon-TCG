
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int verificarEmail(char *email) {
    if (strchr(email, '@') == NULL || strlen(email) >= 80) return 0;
    FILE *f = fopen("usuarios.txt", "r");
    if (!f) return 1;
    char linha[100];
    while (fgets(linha, 100, f)) {
        char emailSalvo[80];
        sscanf(linha, "%[^;];", emailSalvo);
        if (strcmp(email, emailSalvo) == 0) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

void cadastrarUsuario() {
    char email[80], senha[20];
    printf("\n--- Cadastro de Usuário ---\n");
    do {
        printf("Digite seu e-mail: ");
        scanf("%s", email);
        if (!verificarEmail(email)) printf("E-mail inválido ou já cadastrado.\n");
    } while (!verificarEmail(email));

    printf("Digite uma senha: ");
    scanf("%s", senha);

    FILE *f = fopen("usuarios.txt", "a");
    fprintf(f, "%s;%s\n", email, senha);
    fclose(f);
    printf("Usuário cadastrado com sucesso!\n");
}

int login(char *email) {
    char senha[20], senhaLida[20];
    int tentativas = 0;
    FILE *f;

    do {
        printf("\n--- Login ---\n");
        printf("E-mail: ");
        scanf("%s", email);
        printf("Senha: ");
        scanf("%s", senha);

        f = fopen("usuarios.txt", "r");
        if (!f) {
            printf("Nenhum usuário cadastrado. Cadastre-se primeiro.\n");
            return 0;
        }

        char linha[100];
        while (fgets(linha, 100, f)) {
            char emailLido[80];
            sscanf(linha, "%[^;];%s", emailLido, senhaLida);
            if (strcmp(email, emailLido) == 0 && strcmp(senha, senhaLida) == 0) {
                fclose(f);
                return 1;
            }
        }
        fclose(f);
        printf("Login ou senha incorretos.\n");
        tentativas++;
    } while (tentativas < 3);

    printf("Muitas tentativas. Tente novamente mais tarde.\n");
    return 0;
}

void mensagemBoasVindas(char *usuario) {
    printf("\nBem-vindo(a), %s, ao mundo Pokémon TCG! Que comece a batalha!\n", usuario);
}


CartaNode* novoNo(CartaPokemon carta) {
    CartaNode* no = (CartaNode*)malloc(sizeof(CartaNode));
    no->carta = carta;
    no->prox = NULL;
    return no;
}

CartaNode* carregarCartas() {
    FILE *f = fopen("cartas.dat", "rb");
    if (!f) return NULL;

    CartaNode *inicio = NULL, *fim = NULL;
    CartaPokemon temp;

    while (fread(&temp, sizeof(CartaPokemon), 1, f)) {
        CartaNode* no = novoNo(temp);
        if (!inicio) inicio = fim = no;
        else {
            fim->prox = no;
            fim = no;
        }
    }
    fclose(f);
    return inicio;
}

void salvarCartas(CartaNode* lista) {
    FILE *f = fopen("cartas.dat", "wb");
    while (lista) {
        fwrite(&lista->carta, sizeof(CartaPokemon), 1, f);
        lista = lista->prox;
    }
    fclose(f);
}

void listarCartas(CartaNode* lista) {
    printf("\n--- Lista de Cartas ---\n");
    while (lista) {
        CartaPokemon c = lista->carta;
        printf("ID: %d | Nome: %s | Tipo: %s | ATK: %d | DEF: %d | %s\n",
               c.id, c.nome, c.tipo, c.ataque, c.defesa, c.favorito ? "Favorito" : "");
        lista = lista->prox;
    }
}

void cadastrarCarta(CartaNode **lista) {
    CartaPokemon c;
    printf("\n--- Cadastro de Carta ---\n");
    printf("ID: ");
    scanf("%d", &c.id);
    limparBuffer();
    printf("Nome: ");
    fgets(c.nome, 50, stdin);
    strtok(c.nome, "\n");
    printf("Tipo: ");
    fgets(c.tipo, 30, stdin);
    strtok(c.tipo, "\n");
    printf("Ataque: ");
    scanf("%d", &c.ataque);
    printf("Defesa: ");
    scanf("%d", &c.defesa);
    c.favorito = 0;

    CartaNode *novo = novoNo(c);
    novo->prox = *lista;
    *lista = novo;
    printf("Carta cadastrada!\n");
}

void editarCarta(CartaNode *lista) {
    int id;
    printf("\nID da carta a editar: ");
    scanf("%d", &id);
    limparBuffer();

    while (lista) {
        if (lista->carta.id == id) {
            printf("Novo nome: ");
            fgets(lista->carta.nome, 50, stdin);
            strtok(lista->carta.nome, "\n");
            printf("Novo tipo: ");
            fgets(lista->carta.tipo, 30, stdin);
            strtok(lista->carta.tipo, "\n");
            printf("Novo ataque: ");
            scanf("%d", &lista->carta.ataque);
            printf("Nova defesa: ");
            scanf("%d", &lista->carta.defesa);
            printf("Carta atualizada!\n");
            return;
        }
        lista = lista->prox;
    }
    printf("Carta não encontrada.\n");
}

void excluirCarta(CartaNode **lista) {
    int id;
    printf("\nID da carta a excluir: ");
    scanf("%d", &id);

    CartaNode *ant = NULL, *atual = *lista;

    while (atual) {
        if (atual->carta.id == id) {
            if (ant) ant->prox = atual->prox;
            else *lista = atual->prox;
            free(atual);
            printf("Carta excluída!\n");
            return;
        }
        ant = atual;
        atual = atual->prox;
    }
    printf("Carta não encontrada.\n");
}

void merge(CartaPokemon arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    CartaPokemon *L = malloc(n1 * sizeof(CartaPokemon));
    CartaPokemon *R = malloc(n2 * sizeof(CartaPokemon));

    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i].id <= R[j].id) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

void mergeSort(CartaPokemon arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void ordenarCartasPorID(CartaNode **lista) {
    int n = 0;
    CartaNode *temp = *lista;
    while (temp) { n++; temp = temp->prox; }

    if (n == 0) return;

    CartaPokemon *array = malloc(n * sizeof(CartaPokemon));
    temp = *lista;
    for (int i = 0; i < n; i++) {
        array[i] = temp->carta;
        temp = temp->prox;
    }

    mergeSort(array, 0, n - 1);

    temp = *lista;
    for (int i = 0; i < n; i++) {
        temp->carta = array[i];
        temp = temp->prox;
    }

    free(array);
    printf("Cartas ordenadas por ID!\n");
}

CartaPokemon* buscaBinaria(CartaPokemon* arr, int ini, int fim, int id) {
    if (ini > fim) return NULL;
    int meio = (ini + fim) / 2;
    if (arr[meio].id == id) return &arr[meio];
    else if (arr[meio].id > id)
        return buscaBinaria(arr, ini, meio - 1, id);
    else
        return buscaBinaria(arr, meio + 1, fim, id);
}


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
            case 1: cadastrarCarta(lista); salvarCartas(*lista); break;
            case 2: listarCartas(*lista); break;
            case 3: editarCarta(*lista); salvarCartas(*lista); break;
            case 4: excluirCarta(lista); salvarCartas(*lista); break;
            case 5: ordenarCartasPorID(lista); salvarCartas(*lista); break;
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
