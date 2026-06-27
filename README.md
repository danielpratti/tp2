# Campeonato Computacional de Futebol — Parte II

Sistema em C para gerenciamento de partidas e classificação de um campeonato de futebol fictício, utilizando **listas simplesmente encadeadas** como estrutura de dados principal.

Este projeto é a continuação do [TP1](https://github.com/danielpratti/tp1), migrando a persistência em memória de vetores estáticos para listas encadeadas e implementando as operações de manutenção de dados (inserção, atualização e remoção de partidas) que estavam desabilitadas na primeira entrega.

---

## Compilação e Execução

Requisitos: GCC 11+ (ou compatível com C11), Linux (Ubuntu 22.04 recomendado) ou WSL.

```bash
make          # compila o projeto (gera os objetos em obj/ e o executável campeonato)
./campeonato  # executa o sistema
```

Para rodar com `make` diretamente:

```bash
make run
```

Os arquivos `times.csv` e `partidas.csv` devem estar no **mesmo diretório** do executável — é deles que o sistema carrega o estado inicial do campeonato.

Para limpar os objetos e o executável compilados:

```bash
make clean
```

### Trocando o cenário de teste

O enunciado disponibiliza três cenários de teste para o arquivo de partidas:

| Arquivo                     | Cenário                                                   |
| --------------------------- | ---------------------------------------------------------- |
| `partidas_vazio.csv`        | Campeonato vazio (nenhum jogo disputado)                    |
| `partidas_parcial.csv`      | Campeonato em andamento (20 das 45 partidas)                |
| `partidas_completo.csv`     | Campeonato finalizado (todas as 45 partidas do turno único) |

Para testar um cenário específico, basta substituir o conteúdo de `partidas.csv` pelo do arquivo desejado antes de executar o programa, por exemplo:

```bash
cp partidas_vazio.csv partidas.csv
./campeonato
```

Por padrão, o repositório já é entregue com `partidas.csv` equivalente ao cenário completo (`partidas_completo.csv`), permitindo testar imediatamente a ordenação final da classificação.

---

## Estrutura do repositório

```
.
├── include/                    # Cabecalhos (.h) de todos os TADs e modulos
│   ├── time.h                  # TAD Time
│   ├── bdtimes.h               # TAD BDTimes (lista encadeada de Times)
│   ├── partida.h               # TAD Partida
│   ├── bdpartidas.h            # TAD BDPartidas (lista encadeada de Partidas)
│   ├── partidas_servico.h      # Camada de orquestracao Partidas <-> Times
│   ├── ordenacao.h              # Ordenacao da classificacao (Merge Sort)
│   ├── entrada.h                # Utilitarios de leitura segura de stdin
│   └── ui.h                     # Funcionalidades do menu (camada de interface)
├── src/                         # Implementacoes (.c) correspondentes
│   ├── time.c
│   ├── bdtimes.c
│   ├── partida.c
│   ├── bdpartidas.c
│   ├── partidas_servico.c
│   ├── ordenacao.c
│   ├── entrada.c
│   ├── ui.c
│   └── main.c                   # Programa principal (laco do menu)
├── Makefile
├── times.csv                    # Banco de dados dos times (entrada)
├── partidas.csv                 # Banco de dados das partidas (entrada)
├── partidas_vazio.csv           # Cenario de teste 1
├── partidas_parcial.csv         # Cenario de teste 2
├── partidas_completo.csv        # Cenario de teste 3
├── bd_partida.csv               # Snapshot das partidas apos a sessao (saida)
├── bd_classificacao.csv         # Tabela de classificacao final (saida)
└── README.md
```

---

## Principais TADs

### Time (`time.h` / `time.c`)

Representa uma única equipe de futebol. Armazena `id`, `nome` e as estatísticas acumuladas (vitórias, empates, derrotas, gols marcados e gols sofridos). Os campos derivados — **saldo de gols** e **pontos ganhos** — não são armazenados na estrutura: são calculados sob demanda pelas funções `time_saldo()` e `time_pontos()`, garantindo que nunca fiquem desatualizados em relação aos demais campos.

A função `time_aplicar_resultado()` centraliza a lógica de atualizar (ou desfazer, via flag `desfazer`) o impacto de uma partida nas estatísticas de um time — é reaproveitada tanto para aplicar um novo resultado quanto para "reverter" um resultado antigo nas operações de atualização e remoção de partidas.

### BDTimes (`bdtimes.h` / `bdtimes.c`)

Gerencia a coleção de todos os times por meio de uma **lista simplesmente encadeada** (`NoTime`). Principais responsabilidades:

- Carregar os times a partir de `times.csv` (`bdtimes_carregar_csv`);
- Persistir o estado atual em CSV (`bdtimes_salvar_csv`);
- Buscar um time por ID (`bdtimes_buscar_por_id`) — usado internamente para resolver os IDs armazenados nas partidas;
- Buscar times por prefixo do nome, case-insensitive (`bdtimes_buscar_por_prefixo`) — usado na funcionalidade "Consultar time";
- Converter a lista em um vetor de ponteiros (`bdtimes_para_vetor`) — usado para viabilizar a ordenação da classificação (ver seção sobre ordenação).

### Partida (`partida.h` / `partida.c`)

TAD simples que representa uma única partida: `id`, `id_time1`, `id_time2`, `gols_time1` e `gols_time2`. Note que a partida armazena os **IDs** dos times (e não seus nomes), evitando duplicação de dados — o nome é sempre resolvido em tempo de exibição, consultando o `BDTimes`.

### BDPartidas (`bdpartidas.h` / `bdpartidas.c`)

Gerencia a **lista simplesmente encadeada** (`NoPartida`) de todas as partidas cadastradas. É o TAD com a maior responsabilidade desta entrega, pois concentra as operações de manutenção exigidas pela Parte II:

- `bdpartidas_carregar_csv` / `bdpartidas_salvar_csv`: carga e persistência em CSV;
- `bdpartidas_inserir`: insere uma nova partida ao final da lista, **gerando automaticamente um novo ID** (autoincremento, baseado no maior ID já visto na carga inicial);
- `bdpartidas_buscar_por_id`: busca uma partida específica por ID;
- `bdpartidas_buscar_por_time`: busca partidas por prefixo do nome do time mandante, visitante, ou ambos (enum `ModoConsultaPartida`);
- `bdpartidas_atualizar`: atualiza o placar de uma partida existente, com suporte ao sentinela `-1` para "manter o valor atual" de um campo;
- `bdpartidas_remover`: remove uma partida da lista, ajustando os ponteiros dos nós vizinhos e liberando a memória do nó removido.

Importante: o TAD **BDPartidas não conhece o BDTimes** em suas operações de escrita (inserir/atualizar/remover) — ele apenas manipula sua própria coleção. Quem orquestra o impacto dessas operações nas estatísticas dos times é o módulo `partidas_servico`, descrito a seguir.

### Módulo `partidas_servico` (`partidas_servico.h` / `partidas_servico.c`)

Camada de **orquestração** entre `BDPartidas` e `BDTimes`, responsável por manter as duas coleções consistentes:

- `servico_recalcular_todas_estatisticas`: percorre todas as partidas carregadas na inicialização e aplica seus efeitos nas estatísticas dos times (usado uma única vez, ao iniciar o sistema);
- `servico_inserir_partida`: insere a partida no `BDPartidas` e aplica seu efeito nos dois times envolvidos;
- `servico_atualizar_partida`: **desfaz** o efeito do placar antigo, atualiza o registro, e **aplica** o efeito do novo placar — garantindo que a tabela de classificação reflita corretamente a mudança (por exemplo, uma vitória que se torna empate);
- `servico_remover_partida`: desfaz o efeito da partida antes de removê-la da lista, evitando que as estatísticas fiquem "fantasmas" após a exclusão.

Essa separação de responsabilidades (TADs de dados "ingênuos" + uma camada de serviço que conhece ambos) evita que `BDPartidas` precise depender de `BDTimes` para realizar operações simples de CRUD, mantendo os módulos mais coesos e testáveis isoladamente.

### Módulo `ordenacao` (`ordenacao.h` / `ordenacao.c`)

Implementa a ordenação da tabela de classificação usando **Merge Sort**. A escolha do algoritmo não foi arbitrária: como o `BDTimes` é uma lista encadeada (sem acesso indexado eficiente), um algoritmo como QuickSort ou HeapSort seria menos natural de implementar diretamente sobre a lista. A solução adotada:

1. Converte a lista encadeada em um **vetor de ponteiros** para `Time` (`bdtimes_para_vetor`) — operação O(n) que não duplica os dados dos times, apenas referencia os nós já existentes;
2. Ordena esse vetor de ponteiros com Merge Sort, que é estável e tem complexidade O(n log n) garantida (mesmo no pior caso, diferente do QuickSort);
3. Os ponteiros no vetor passam a apontar para os mesmos `Time`s da lista original, então a ordenação não duplica nem perde nenhuma informação.

O critério de comparação (`time_tem_prioridade`) segue exatamente a prioridade pedida no enunciado: **Pontos Ganhos (PG)** → **Vitórias (V)** → **Saldo de Gols (S)**, todos em ordem decrescente.

### Módulo `entrada` (`entrada.h` / `entrada.c`)

Utilitários para leitura robusta de dados via `stdin`:

- `entrada_ler_linha`: lê uma linha removendo o `\n`/`\r` final, retornando 0 caso o EOF seja atingido (evitando comportamentos indefinidos quando a entrada é redirecionada de um arquivo, por exemplo);
- `entrada_ler_inteiro`: insiste até receber um inteiro válido, com suporte ao sentinela `-1` (dígito `-`, usado nas atualizações de placar para "manter o valor atual") e tratamento de EOF (retorna `-2`, sinalizando ao chamador para abortar a operação com segurança em vez de entrar em loop infinito);
- `entrada_ler_confirmacao`: lê respostas do tipo S/N, repetindo a pergunta em caso de entrada inválida.

### Módulo `ui` (`ui.h` / `ui.c`)

Concentra as 6 funcionalidades do menu principal, isolando a lógica de interação com o usuário das estruturas de dados. Reaproveita uma função auxiliar comum (`fluxo_consulta_partidas`) tanto para a funcionalidade de consulta isolada quanto como primeiro passo das funcionalidades de atualização e remoção — conforme pedido no enunciado ("a rotina de atualização deve primeiro invocar a rotina de consulta").

A listagem da classificação é **paginada** (5 times por página, configurável via `LINHAS_POR_PAGINA`), permitindo navegar com ENTER ou interromper digitando `Q`.

---

## Funcionalidades implementadas

| Opção | Descrição                                                                |
| ----- | -------------------------------------------------------------------------- |
| 1     | Consultar time por nome ou prefixo (case-insensitive)                       |
| 2     | Consultar partidas por time mandante, visitante, ou ambos (prefixo)         |
| 3     | Atualizar o placar de uma partida existente (localizada via consulta)      |
| 4     | Remover uma partida existente (localizada via consulta), com confirmação   |
| 5     | Inserir uma nova partida, com ID gerado automaticamente e validação de IDs |
| 6     | Imprimir a tabela de classificação ordenada (PG > V > Saldo), paginada    |
| Q     | Encerrar o sistema, persistindo `bd_partida.csv` e `bd_classificacao.csv` |

Todas as seis funcionalidades estão habilitadas nesta entrega, conforme exigido pelo enunciado da Parte II.

---

## Principais decisões de implementação

- **Lista simplesmente encadeada** para `BDTimes` e `BDPartidas`, com alocação dinâmica de cada nó via `malloc`, em substituição aos vetores estáticos da Parte I.
- **IDs de partida com autoincremento**: o `BDPartidas` mantém um contador interno (`proximo_id`), inicializado a partir do maior ID encontrado na carga do CSV, garantindo unicidade mesmo após inserções, atualizações e remoções.
- **Separação entre dados e orquestração**: os TADs `BDTimes` e `BDPartidas` não se conhecem mutuamente; o módulo `partidas_servico` é o único responsável por sincronizar as estatísticas dos times sempre que uma partida é inserida, atualizada ou removida — desfazendo o efeito do estado antigo antes de aplicar o novo, sempre que aplicável.
- **Estatísticas recalculadas via desfazer/refazer**: em vez de recalcular a tabela inteira do zero a cada operação, a atualização e a remoção de partidas usam a flag `desfazer` de `time_aplicar_resultado` para reverter exatamente o efeito anterior, depois (se for o caso) aplicar o novo — operação O(1) por time, em vez de O(n) sobre todo o histórico.
- **Merge Sort sobre vetor de ponteiros** para ordenar a classificação: evita reescrever a lista encadeada original e garante O(n log n) estável, mesmo no pior caso.
- **Busca por prefixo case-insensitive** usando `tolower` (portável, em vez de `strncasecmp`, que é específico de POSIX), tanto para times quanto para partidas.
- **Sentinela `-1` (`'-'` no terminal) para "manter valor atual"** na atualização de placares, conforme o fluxo de exemplo do enunciado.
- **Tratamento de EOF em todas as leituras de `stdin`**: evita loops infinitos quando a entrada é redirecionada de um arquivo (útil para testes automatizados) ou quando o terminal é encerrado abruptamente (Ctrl+D).
- **Validações de robustez na inserção**: verificação de existência dos dois times informados, rejeição de Time1 igual a Time2, e rejeição de placares negativos, sempre com mensagens de erro específicas.
- **Modularização**: cada TAD possui seu próprio par `.h`/`.c`, com `include/` e `src/` separados; a camada de interface (`ui`) e a camada de orquestração (`partidas_servico`) ficam isoladas dos TADs de dados puros, facilitando manutenção e testes independentes.

---

## Testes realizados

O sistema foi validado manualmente e via script automatizado (entrada redirecionada via `stdin`) para todas as seis funcionalidades, incluindo casos de robustez (prefixo inexistente, ID inexistente, time inexistente, placar negativo, Time1 igual a Time2) e os três cenários de teste fornecidos no enunciado (`partidas_vazio.csv`, `partidas_parcial.csv`, `partidas_completo.csv`).

Também foi utilizado o **AddressSanitizer/LeakSanitizer** (`gcc -fsanitize=address,leak`) para validar a ausência de vazamentos de memória e de acessos inválidos em todas as operações de inserção, atualização e remoção sobre as listas encadeadas.

---

Daniel e Rafael Pratti™
