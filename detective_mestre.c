/* detective_mestre.c
   Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
   - BST para pistas (opcional exibição)
   - Hash para suspeitos com lista de pistas associadas (encadeada)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 11   // tamanho simples
#define MAX_TXT 200

/* --- salas (igual aos anteriores) --- */
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *s = malloc(sizeof(Sala));
    if(!s) return NULL;
    strncpy(s->nome, nome, 49); s->nome[49] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

Sala* montarMansao() {
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

/* --- BST de pistas (igual ao aventureiro, usada apenas para listagem) --- */
typedef struct PistaNode {
    char texto[MAX_TXT];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

PistaNode* novoPista(const char *texto) {
    PistaNode *p = malloc(sizeof(PistaNode));
    if(!p) return NULL;
    strncpy(p->texto, texto, MAX_TXT-1);
    p->texto[MAX_TXT-1] = '\0';
    p->esq = p->dir = NULL;
    return p;
}

PistaNode* inserirPistaBST(PistaNode *r, const char *texto) {
    if(!r) return novoPista(texto);
    if(strcmp(texto, r->texto) < 0) r->esq = inserirPistaBST(r->esq, texto);
    else if(strcmp(texto, r->texto) > 0) r->dir = inserirPistaBST(r->dir, texto);
    return r;
}

void emOrdem(PistaNode *r) {
    if(!r) return;
    emOrdem(r->esq);
    printf("- %s\n", r->texto);
    emOrdem(r->dir);
}

void liberarPistas(PistaNode *r) {
    if(!r) return;
    liberarPistas(r->esq); liberarPistas(r->dir); free(r);
}

/* --- Hash de Suspeitos --- */
typedef struct PistaAssoc {
    char texto[MAX_TXT];
    struct PistaAssoc *prox;
} PistaAssoc;

typedef struct Suspeito {
    char nome[60];
    int contador; // quantas vezes citado
    PistaAssoc *pistas; // lista de pistas associadas
    struct Suspeito *prox; // para encadeamento na tabela (colisao)
} Suspeito;

Suspeito* tabela[HASH_SIZE];

void inicializarHash() {
    for(int i=0;i<HASH_SIZE;i++) tabela[i] = NULL;
}

unsigned int hash(const char *s) {
    unsigned int sum = 0;
    for(; *s; s++) sum += (unsigned char)(*s);
    return sum % HASH_SIZE;
}

Suspeito* buscarSuspeito(const char *nome) {
    unsigned int h = hash(nome);
    Suspeito *p = tabela[h];
    while(p) {
        if(strcmp(p->nome, nome) == 0) return p;
        p = p->prox;
    }
    return NULL;
}

Suspeito* criarSuspeito(const char *nome) {
    Suspeito *s = malloc(sizeof(Suspeito));
    if(!s) return NULL;
    strncpy(s->nome, nome, 59); s->nome[59] = '\0';
    s->contador = 0;
    s->pistas = NULL;
    s->prox = NULL;
    return s;
}

void inserirSuspeitoHash(const char *nome) {
    if(buscarSuspeito(nome)) return; // ja existe
    unsigned int h = hash(nome);
    Suspeito *s = criarSuspeito(nome);
    if(!s) return;
    s->prox = tabela[h];
    tabela[h] = s;
}

/* adiciona uma associacao pista -> suspeito (insere pista na lista do suspeito e incrementa contador) */
void inserirAssociacao(const char *pistaTxt, const char *suspeitoNome) {
    if(!pistaTxt || !suspeitoNome) return;
    Suspeito *s = buscarSuspeito(suspeitoNome);
    if(!s) {
        inserirSuspeitoHash(suspeitoNome);
        s = buscarSuspeito(suspeitoNome);
        if(!s) return;
    }
    // adicionar pista na lista do suspeito (no inicio)
    PistaAssoc *pa = malloc(sizeof(PistaAssoc));
    if(!pa) return;
    strncpy(pa->texto, pistaTxt, MAX_TXT-1); pa->texto[MAX_TXT-1] = '\0';
    pa->prox = s->pistas;
    s->pistas = pa;
    s->contador++;
}

/* imprime a tabela de suspeitos e suas pistas */
void listarAssociacoes() {
    printf("\n--- Suspeitos e pistas associadas ---\n");
    for(int i=0;i<HASH_SIZE;i++) {
        Suspeito *s = tabela[i];
        while(s) {
            printf("Suspeito: %s (citacoes: %d)\n", s->nome, s->contador);
            PistaAssoc *pa = s->pistas;
            while(pa) {
                printf("  * %s\n", pa->texto);
                pa = pa->prox;
            }
            s = s->prox;
        }
    }
}

/* encontra o suspeito mais citado (simples) */
Suspeito* suspeitoMaisProvavel() {
    Suspeito *melhor = NULL;
    for(int i=0;i<HASH_SIZE;i++) {
        Suspeito *s = tabela[i];
        while(s) {
            if(!melhor || s->contador > melhor->contador) melhor = s;
            s = s->prox;
        }
    }
    return melhor;
}

/* limpa toda a tabela (libera memoria) */
void liberarHash() {
    for(int i=0;i<HASH_SIZE;i++) {
        Suspeito *s = tabela[i];
        while(s) {
            PistaAssoc *pa = s->pistas;
            while(pa) {
                PistaAssoc *aux = pa->prox;
                free(pa);
                pa = aux;
            }
            Suspeito *auxs = s->prox;
            free(s);
            s = auxs;
        }
        tabela[i] = NULL;
    }
}

/* heuristica simples: decide qual suspeito associar com base em palavras na pista */
void heuristicaAssociacao(const char *pista, char *suspeitoOut) {
    // exemplos de suspeitos fixos (poderia ser dinamico)
    if(strstr(pista, "veneno") || strstr(pista, "frasc")) strcpy(suspeitoOut, "Dr. Moreau");
    else if(strstr(pista, "iniciais 'M'") || strstr(pista, "agenda")) strcpy(suspeitoOut, "Marcos Silva");
    else if(strstr(pista, "pegadas") || strstr(pista, "estante")) strcpy(suspeitoOut, "Sra. Almeida");
    else strcpy(suspeitoOut, "Desconhecido");
}

/* gera pista por sala, insere na BST e associa a suspeito via hash */
PistaNode* coletarPistaEAssociar(PistaNode *bst, const char *nomeSala) {
    char buffer[MAX_TXT];
    if(strcmp(nomeSala, "Biblioteca")==0) {
        snprintf(buffer, sizeof(buffer), "Pegadas molhadas perto da estante - (local: %s)", nomeSala);
    } else if(strcmp(nomeSala, "Cozinha")==0) {
        snprintf(buffer, sizeof(buffer), "Frasco de veneno na pia - (local: %s)", nomeSala);
    } else if(strcmp(nomeSala, "Sotao")==0) {
        snprintf(buffer, sizeof(buffer), "Bilhete rasgado com iniciais 'M.' - (local: %s)", nomeSala);
    } else if(strcmp(nomeSala, "Escritorio")==0) {
        snprintf(buffer, sizeof(buffer), "Agenda com encontro marcado - (local: %s)", nomeSala);
    } else {
        snprintf(buffer, sizeof(buffer), "Nada de especial no local: %s", nomeSala);
    }

    // inserir na BST (se for "Nada de especial", opcional inserir)
    if(strncmp(buffer, "Nada de especial", 16) != 0) {
        bst = inserirPistaBST(bst, buffer);
    }

    // heuristica para associar a um suspeito
    char suspeito[60];
    heuristicaAssociacao(buffer, suspeito);
    inserirAssociacao(buffer, suspeito);

    printf("Pista coletada: %s\n", buffer);
    printf("Associada a: %s\n", suspeito);
    return bst;
}

/* liberar salas e BST */
void liberarSalas(Sala *r) { if(!r) return; liberarSalas(r->esq); liberarSalas(r->dir); free(r); }

int main() {
    Sala *raiz = montarMansao();
    Sala *atual = raiz;
    PistaNode *pistas = NULL;
    inicializarHash();

    // registra suspeitos conhecidos (opcional)
    inserirSuspeitoHash("Dr. Moreau");
    inserirSuspeitoHash("Marcos Silva");
    inserirSuspeitoHash("Sra. Almeida");
    inserirSuspeitoHash("Desconhecido");

    char comando[30];

    printf("=== Detective Quest - Nivel Mestre ===\n");
    printf("Comandos: e (esq), d (dir), u (voltar hall), r (revisar pistas BST), h (listar suspeitos), m (mostrar suspeito mais provavel), s (sair)\n");

    while(1) {
        printf("\nVoce esta em: %s\n", atual->nome);
        printf("Escolha: ");
        if(!fgets(comando, sizeof(comando), stdin)) break;
        if(comando[0]=='s') break;
        else if(comando[0]=='e') {
            if(atual->esq) atual = atual->esq;
            else { printf("Nao ha sala a esquerda.\n"); continue; }
            pistas = coletarPistaEAssociar(pistas, atual->nome);
        } else if(comando[0]=='d') {
            if(atual->dir) atual = atual->dir;
            else { printf("Nao ha sala a direita.\n"); continue; }
            pistas = coletarPistaEAssociar(pistas, atual->nome);
        } else if(comando[0]=='u') {
            atual = raiz;
            printf("Voltando para o Hall.\n");
        } else if(comando[0]=='r') {
            printf("\n--- Pistas (BST, ordem alfabetica) ---\n");
            if(pistas) emOrdem(pistas);
            else printf("Nenhuma pista cadastrada.\n");
        } else if(comando[0]=='h') {
            listarAssociacoes();
        } else if(comando[0]=='m') {
            Suspeito *s = suspeitoMaisProvavel();
            if(s) {
                printf("\nSuspeito mais provavel: %s (citacoes: %d)\n", s->nome, s->contador);
            } else {
                printf("Nenhum suspeito encontrado.\n");
            }
        } else {
            printf("Comando invalido.\n");
        }
    }

    // liberar tudo
    liberarPistas(pistas);
    liberarSalas(raiz);
    liberarHash();

    printf("Saindo. Boa investigacao, detective!\n");
    return 0;
}
