#include "bdtimes.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

//Inicializa o BD de times com quantidade zero
void bdtimes_init(BDTimes *bd) {
    bd->quantidade = 0;
}

/*
 * Carrega times de um CSV no formato: ID,Nome
 * Ignora a primeira linha (cabeçalho).
 * Retorna 0 em sucesso, -1 se não conseguir abrir o arquivo.
 */
int bdtimes_carregar(BDTimes *bd, const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'\n", arquivo);
        return -1;
    }

    char linha[256];
    //Pula o cabeçalho
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f) && bd->quantidade < MAX_TIMES) {
        int id;
        char nome[MAX_NOME];
        //Lê id e nome separados por vírgula
        //Formato real: ID,Time — campo se chama "Time" no CSV
        if (sscanf(linha, "%d,%49[^\n\r]", &id, nome) == 2) {
            bd->times[bd->quantidade] = time_criar(id, nome);
            bd->quantidade++;
        }
    }

    fclose(f);
    return 0;
}

//Busca linear por ID
Time *bdtimes_buscar_por_id(BDTimes *bd, int id) {
    for (int i = 0; i < bd->quantidade; i++) {
        if (bd->times[i].id == id)
            return &bd->times[i];
    }
    return NULL;
}

/*
 * Busca por prefixo (case-insensitive).
 * Preenche o vetor 'resultados' e retorna quantos foram encontrados.
 */
int bdtimes_buscar_por_prefixo(BDTimes *bd, const char *prefixo,
                                Time **resultados, int max_resultados) {
    int encontrados = 0;
    int tam = strlen(prefixo);

    for (int i = 0; i < bd->quantidade && encontrados < max_resultados; i++) {
        if (strncasecmp(bd->times[i].nome, prefixo, tam) == 0) {
            resultados[encontrados++] = &bd->times[i];
        }
    }
    return encontrados;
}

//Imprime tabela completa ordenada por ID (Em ordem de inserção)
void bdtimes_imprimir_tabela(BDTimes *bd) {
    printf("\nImprimindo classificacao...\n\n");
    time_imprimir_cabecalho();
    for (int i = 0; i < bd->quantidade; i++) {
        time_imprimir(&bd->times[i]);
    }
    printf("\n");
}
