# Campeonato Computacional de Futebol --- Parte II

Sistema desenvolvido em C para gerenciamento de partidas e classificação
de um campeonato de futebol fictício, utilizando **listas simplesmente
encadeadas** como estrutura de dados principal. Este projeto é a
continuação do TP1, substituindo vetores estáticos por listas dinâmicas
e implementando as operações de inserção, atualização e remoção de
partidas.

## Compilação e execução

``` bash
make
./campeonato
```

Ou:

``` bash
make run
```

Os arquivos `times.csv` e `partidas.csv` devem estar no mesmo diretório
do executável.

Para limpar os arquivos compilados:

``` bash
make clean
```

O sistema pode ser testado utilizando os cenários `partidas_vazio.csv`,
`partidas_parcial.csv` e `partidas_completo.csv`, bastando substituir o
conteúdo de `partidas.csv` antes da execução.

## Estrutura do projeto

O projeto está organizado em módulos independentes:

-   **Time:** representa um time e suas estatísticas.
-   **BDTimes:** gerencia a lista encadeada de times.
-   **Partida:** representa uma partida entre dois times.
-   **BDPartidas:** realiza o gerenciamento das partidas e das operações
    de inserção, atualização e remoção.
-   **Partidas Serviço:** mantém as estatísticas dos times sincronizadas
    com as alterações nas partidas.
-   **Ordenação:** implementa o Merge Sort para gerar a classificação.
-   **Entrada:** realiza a leitura segura dos dados informados pelo
    usuário.
-   **UI:** concentra as funcionalidades da interface e do menu
    principal.

## Funcionalidades

-   Consulta de times por nome ou prefixo;
-   Consulta de partidas por time;
-   Inserção de novas partidas com geração automática de ID;
-   Atualização de placares;
-   Remoção de partidas;
-   Exibição da tabela de classificação ordenada por pontos, vitórias e
    saldo de gols;
-   Persistência dos dados ao encerrar a execução.

## Principais decisões de implementação

-   Utilização de listas simplesmente encadeadas para armazenar times e
    partidas;
-   Separação entre os módulos responsáveis pelos dados e a camada de
    serviço;
-   Atualização incremental das estatísticas dos times;
-   Ordenação utilizando Merge Sort sobre um vetor de ponteiros;
-   Busca por prefixo sem diferenciação entre letras maiúsculas e
    minúsculas;
-   Validação das entradas do usuário e tratamento de erros.

## Testes

Foram realizados testes utilizando os três cenários disponibilizados
(`partidas_vazio.csv`, `partidas_parcial.csv` e
`partidas_completo.csv`), além de testes para todas as funcionalidades
do sistema. Também foi utilizado o **AddressSanitizer/LeakSanitizer**
para verificar a ausência de vazamentos de memória e acessos inválidos.
