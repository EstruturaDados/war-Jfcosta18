#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ========================== CONSTANTES ==========================
#define MAX_NOME 30
#define MAX_COR 10
#define TOTAL_MISSOES 5

// ========================== STRUCT ==========================
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// ========================== PROTÓTIPOS ==========================
Territorio* alocarMapa(int quantidade);
void inicializarTerritorios(Territorio *mapa, int quantidade);
void liberarMemoria(Territorio *mapa, char *missao);

void exibirMapa(const Territorio *mapa, int quantidade);
void exibirMenu();

void atacar(Territorio *origem, Territorio *destino);
void faseAtaque(Territorio *mapa, int quantidade);

void atribuirMissao(char *destino, char *missoes[], int total);
int verificarMissao(const Territorio *mapa, int quantidade, const char *missao);

// ========================== MISSÕES ==========================
char *missoes[TOTAL_MISSOES] = {
    "Conquistar 3 territorios",
    "Eliminar inimigos",
    "Dominar a maioria do mapa",
    "Expandir territorio inicial",
    "Controle militar dominante"
};

// ========================== MAIN ==========================
int main() {

    srand(time(NULL));

    int quantidade, opcao;
    char *missaoJogador;

    printf("====================================\n");
    printf("       WAR ESTRUTURADO - FINAL\n");
    printf("====================================\n");

    printf("Digite a quantidade de territorios: ");
    scanf("%d", &quantidade);

    Territorio *mapa = alocarMapa(quantidade);

    if(mapa == NULL) {
        printf("Erro de memoria!\n");
        return 1;
    }

    inicializarTerritorios(mapa, quantidade);

    // ================= MISSÃO =================
    missaoJogador = (char*) malloc(100 * sizeof(char));

    if(missaoJogador == NULL) {
        printf("Erro de memoria!\n");
        free(mapa);
        return 1;
    }

    atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES);

    printf("\n========= SUA MISSAO =========\n");
    printf("%s\n", missaoJogador);

    // ================= LOOP =================
    do {
        printf("\n");
        exibirMapa(mapa, quantidade);
        exibirMenu();

        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {

            case 1:
                faseAtaque(mapa, quantidade);
                break;

            case 2:
                if(verificarMissao(mapa, quantidade, missaoJogador)) {
                    printf("\n================================\n");
                    printf("   VOCE CUMPRIU A MISSAO!\n");
                    printf("   VITORIA!\n");
                    printf("================================\n");
                    opcao = 0;
                } else {
                    printf("\nMissao ainda nao cumprida.\n");
                }
                break;

            case 0:
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// ========================== FUNÇÕES ==========================

// Alocação
Territorio* alocarMapa(int quantidade) {

    return (Territorio*) calloc(quantidade, sizeof(Territorio));
}

// Inicialização
void inicializarTerritorios(Territorio *mapa, int quantidade) {

    for(int i = 0; i < quantidade; i++) {

        printf("\nTerritorio %d\n", i + 1);

        printf("Nome: ");
        scanf("%s", mapa[i].nome);

        printf("Cor: ");
        scanf("%s", mapa[i].cor);

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Exibir mapa
void exibirMapa(const Territorio *mapa, int quantidade) {

    printf("\n========= MAPA =========\n");

    for(int i = 0; i < quantidade; i++) {

        printf("[%d] %s | %s | %d tropas\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
}

// Menu
void exibirMenu() {

    printf("\n========= MENU =========\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar missao\n");
    printf("0 - Sair\n");
}

// Ataque
void atacar(Territorio *origem, Territorio *destino) {

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\nATAQUE!\n");

    if(dadoA > dadoD) {

        printf("Atacante venceu!\n");

        strcpy(destino->cor, origem->cor);

        int tropas = origem->tropas / 2;
        if(tropas < 1) tropas = 1;

        destino->tropas = tropas;
        origem->tropas -= tropas;

    } else {

        printf("Defensor venceu!\n");

        origem->tropas--;

        if(origem->tropas < 1)
            origem->tropas = 1;
    }
}

// Fase ataque
void faseAtaque(Territorio *mapa, int quantidade) {

    int a, d;

    printf("Escolha atacante: ");
    scanf("%d", &a);

    printf("Escolha defensor: ");
    scanf("%d", &d);

    a--;
    d--;

    // validação
    if(a < 0 || d < 0 || a >= quantidade || d >= quantidade) {
        printf("Indice invalido!\n");
        return;
    }

    if(strcmp(mapa[a].cor, mapa[d].cor) == 0) {
        printf("Nao pode atacar aliado!\n");
        return;
    }

    atacar(&mapa[a], &mapa[d]);
}

// Missão
void atribuirMissao(char *destino, char *missoes[], int total) {

    strcpy(destino, missoes[rand() % total]);
}

// ================= MISSÃO CORRIGIDA =================
int verificarMissao(const Territorio *mapa,
                    int quantidade,
                    const char *missao) {

    int i, contador = 0;

    // MISSÃO 1
    if(strcmp(missao, "Conquistar 3 territorios") == 0) {

        char corBase[MAX_COR];
        strcpy(corBase, mapa[0].cor);

        for(i = 0; i < quantidade; i++) {
            if(strcmp(mapa[i].cor, corBase) == 0)
                contador++;
        }

        return (contador >= 3);
    }

    // MISSÃO 2
    if(strcmp(missao, "Eliminar inimigos") == 0) {

        for(i = 0; i < quantidade; i++) {
            if(mapa[i].tropas > 1)
                return 0;
        }

        return 1;
    }

    // MISSÃO 3
    if(strcmp(missao, "Dominar a maioria do mapa") == 0) {

        char corBase[MAX_COR];
        strcpy(corBase, mapa[0].cor);

        contador = 0;

        for(i = 0; i < quantidade; i++) {
            if(strcmp(mapa[i].cor, corBase) == 0)
                contador++;
        }

        return (contador > quantidade / 2);
    }

    // outras missões (simplificadas)
    return 0;
}

// Liberar memória
void liberarMemoria(Territorio *mapa, char *missao) {

    free(mapa);
    free(missao);

    printf("\nMemoria liberada!\n");
}