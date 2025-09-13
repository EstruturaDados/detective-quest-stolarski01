/* detective_aventureiro.c
   Nível Aventureiro: Armazenamento de Pistas com BST
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct PistaNode {
    char texto[200];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* funcoes de sala (igual ao novato) */
Sala* criarSala(const char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if(!s) return NULL;
    strncpy(s->nome, nome, 49); s->nome[49] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

Sala* montarMansaoSimples() {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *bibli = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sotao");
    Sala *escritorio = criarSala("Escritorio");
    Sala *jardim = criarSala("Jardim");

    hall->esq = bibli; hall->dir = cozinha;
    bibli->esq = sotao; bibli->dir = escritorio;
    cozinha->dir = jardim;
    return hall;
}

/* BST de pistas */
PistaNode* novoPista(const char *texto) {
    PistaNode *p = (PistaNode*) malloc(sizeof(PistaNode));
    if(!p) return NULL;
    strncpy(p->texto, texto, 199); p->texto[199] = '\0';
    p->esq = p->dir = NULL;
    return p;
}

PistaNode* inserirPista(PistaNode *raiz, const char *texto) {
    if(!raiz) return novoPista(texto);
    if(strcmp(texto, raiz->texto) < 0) raiz->esq = inserirPista(raiz->esq, texto);
    else if(strcmp(texto, raiz->texto) > 0) raiz->dir = inserirPista(raiz->dir, texto);
    else {
        // ja existe - ignorar duplicata (poderia contar)
    }
    return raiz;
}

void emOrdem(PistaNode *r) {
    if(!r) return;
    emOrdem(r->esq);
    printf("- %s\n", r->texto);
    emOrdem(r->dir);
}

void liberarPistas(PistaNode *r) {
    if(!r) return;
    liberarPistas(r->esq); liberarPistas(r->dir);
    free(r);
}

/* Simula coletar pista ao visitar sala: mapeamento simples */
PistaNode* coletarPistaPorSala(PistaNode *raiz, const char *nomeSala) {
    char buffer[200];
    if(strcmp(nomeSala, "Biblioteca") == 0) {
        snprintf(buffer, sizeof(buffer), "Pegadas molhadas perto da estante - (local: %s)", nomeSala);
        raiz = inserirPista(raiz, buffer);
        printf("Pista coletada: %s\n", buffer);
    } else if(strcmp(nomeSala, "Cozinha") == 0) {
        snprintf(buffer, sizeof(buffer), "Frasco de veneno na pia - (local: %s)", nomeSala);
        raiz = inserirPista(raiz, buffer);
        printf("Pista coletada: %s\n", buffer);
    } else if(strcmp(nomeSala, "Sotao") == 0) {
        snprintf(buffer, sizeof(buffer), "Bilhete rasgado com iniciais 'M.' - (local: %s)", nomeSala);
        raiz = inserirPista(raiz, buffer);
        printf("Pista coletada: %s\n", buffer);
    } else if(strcmp(nomeSala, "Escritorio") == 0) {
        snprintf(buffer, sizeof(buffer), "Agenda com encontro marcado - (local: %s)", nomeSala);
        raiz = inserirPista(raiz, buffer);
        printf("Pista coletada: %s\n", buffer);
    } else {
        // outras salas podem nao ter pista
        printf("Nenhuma pista evidente nesta sala.\n");
    }
    return raiz;
}

void liberarSalas(Sala *r) {
    if(!r) return;
    liberarSalas(r->esq); liberarSalas(r->dir); free(r);
}

/* Programa principal: explorar e armazenar pistas */
int main() {
    Sala *raiz = montarMansaoSimples();
    Sala *atual = raiz;
    PistaNode *pistas = NULL;
    char comando[20];

    printf("=== Detective Quest - Nivel Aventureiro ===\n");
    printf("Comandos: e (esq), d (dir), u (voltar hall), r (revisar pistas), s (sair)\n");

    while(1) {
        printf("\nVoce esta em: %s\n", atual->nome);
        printf("Escolha: ");
        if(!fgets(comando, sizeof(comando), stdin)) break;
        if(comando[0]=='s') break;
        else if(comando[0]=='e') {
            if(atual->esq) atual = atual->esq;
            else printf("Nao ha sala a esquerda.\n");
            pistas = coletarPistaPorSala(pistas, atual->nome);
        } else if(comando[0]=='d') {
            if(atual->dir) atual = atual->dir;
            else printf("Nao ha sala a direita.\n");
            pistas = coletarPistaPorSala(pistas, atual->nome);
        } else if(comando[0]=='u') {
            atual = raiz;
            printf("Voltando para o Hall.\n");
        } else if(comando[0]=='r') {
            printf("\n--- Pistas coletadas (ordem alfabetica) ---\n");
            if(pistas) emOrdem(pistas);
            else printf("Nenhuma pista coletada ainda.\n");
        } else {
            printf("Comando invalido.\n");
        }
    }

    liberarPistas(pistas);
    liberarSalas(raiz);
    printf("Saindo. Boa sorte, detective!\n");
    return 0;
}
