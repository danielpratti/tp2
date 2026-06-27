#include "entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int entrada_ler_linha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        return 1;
    }
    buffer[0] = '\0';
    return 0;
}

int entrada_ler_inteiro(const char *mensagem, int aceitar_hifen) {
    char linha[64];

    while (1) {
        if (mensagem != NULL) {
            printf("%s", mensagem);
        }

        if (!entrada_ler_linha(linha, sizeof(linha))) {
            return -2; /* EOF: sinaliza ao chamador para encerrar com seguranca. */
        }

        /* Remove espacos em branco nas extremidades. */
        char *inicio = linha;
        while (isspace((unsigned char) *inicio)) {
            inicio++;
        }

        if (aceitar_hifen && strcmp(inicio, "-") == 0) {
            return -1;
        }

        if (strlen(inicio) == 0) {
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        /* Valida se todos os caracteres formam um numero inteiro (opcionalmente negativo). */
        int valido = 1;
        int comeco = 0;
        if (inicio[0] == '-' || inicio[0] == '+') {
            comeco = 1;
            if (strlen(inicio) == 1) {
                valido = 0;
            }
        }
        for (size_t i = (size_t) comeco; i < strlen(inicio); i++) {
            if (!isdigit((unsigned char) inicio[i])) {
                valido = 0;
                break;
            }
        }

        if (!valido) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }

        return atoi(inicio);
    }
}

int entrada_ler_confirmacao(const char *mensagem) {
    char linha[16];

    while (1) {
        if (mensagem != NULL) {
            printf("%s", mensagem);
        }

        if (!entrada_ler_linha(linha, sizeof(linha))) {
            return -1; /* EOF: trata como cancelamento, evitando loop infinito. */
        }

        if (strlen(linha) >= 1) {
            char c = (char) toupper((unsigned char) linha[0]);
            if (c == 'S') {
                return 1;
            }
            if (c == 'N') {
                return 0;
            }
        }
        printf("Resposta invalida. Digite S ou N.\n");
    }
}
