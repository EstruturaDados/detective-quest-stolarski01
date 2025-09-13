/* detective_novato.c
   Nível Novato: Mapa da Mansão com Árvore Binária
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if(!s) return NULL;
    strncpy(s->nome, nome, 49);
    s->nome[49] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

/* Monta uma árvore fixa (simples, sem inserção dinâmica) */
Sala* montarMansao() {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *bibli = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sotao");
    Sala *escritorio = criarSala("Escritorio");
    Sala *jardim = criarSala("Jardim");

    // ligação (apenas exemplo)
    hall->esq = bibli;
    hall->dir = cozinha;
    bibli->esq = sotao;
    bibli->dir = escritorio;
    cozinha->dir = jardim;

    return hall;
}

void liberarSalas(Sala *r) {
    if(!r) return;
    liberarSalas(r->esq);
    liberarSalas(r->dir);
    free(r);
}

int main() {
    Sala *atual;
    char comando[10];
    Sala *raiz = montarMansao();
    if(!raiz) { printf("Erro ao criar mansao\n"); return 1; }

    atual = raiz;
    printf("=== Detective Quest - Nivel Novato ===\n");
    printf("Comandos: e (esquerda), d (direita), u (voltar para hall), s (sair)\n");

    while(1) {
        printf("\nVoce esta em: %s\n", atual->nome);
        printf("Escolha: ");
        if(!fgets(comando, sizeof(comando), stdin)) break;
        if(comando[0] == 's') break;
        else if(comando[0] == 'e') {
            if(atual->esq) atual = atual->esq;
            else printf("Nao ha sala a esquerda.\n");
        } else if(comando[0] == 'd') {
            if(atual->dir) atual = atual->dir;
            else printf("Nao ha sala a direita.\n");
        } else if(comando[0] == 'u') {
            atual = raiz;
            printf("Voltando para o Hall de Entrada.\n");
        } else {
            printf("Comando invalido.\n");
        }
    }

    liberarSalas(raiz);
    printf("Saindo. Boa investigacao!\n");
    return 0;
}
