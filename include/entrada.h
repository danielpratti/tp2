#ifndef ENTRADA_H
#define ENTRADA_H

/*
 * Modulo entrada
 * ---------------
 * Funcoes auxiliares para leitura robusta de dados digitados pelo
 * usuario via stdin, evitando problemas comuns como buffers nao
 * limpos (lixo de '\n'), entradas invalidas/nao numericas e travamento
 * em loop infinito quando o fluxo de entrada e encerrado (EOF).
 */

/*
 * Le uma linha de stdin (sem o '\n' final) para o buffer informado.
 * Retorna 1 em sucesso, ou 0 se o fim do arquivo (EOF) foi atingido
 * antes de qualquer leitura (nesse caso, buffer recebe uma string
 * vazia).
 */
int entrada_ler_linha(char *buffer, int tamanho);

/*
 * Le um numero inteiro de stdin. Insiste ate receber uma entrada
 * numerica valida ou o fim do arquivo (EOF) ser atingido. Se
 * `aceitar_hifen` for diferente de 0, o caractere '-' isolado e
 * aceito como sentinela e a funcao retorna -1. Em caso de EOF,
 * retorna -2 (valor que o chamador deve tratar como "encerrar").
 */
int entrada_ler_inteiro(const char *mensagem, int aceitar_hifen);

/*
 * Le uma confirmacao do tipo S/N. Retorna 1 para sim, 0 para nao, ou
 * -1 se o fim do arquivo (EOF) foi atingido (tratado pelo chamador
 * como uma negativa/cancelamento, para nao travar o programa).
 */
int entrada_ler_confirmacao(const char *mensagem);

#endif
