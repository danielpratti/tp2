Campeonato Computacional de Futebol — Parte II

Sistema desenvolvido em C para gerenciar partidas e a classificação de um campeonato de futebol fictício, utilizando listas simplesmente encadeadas como estrutura principal de dados. Esta versão dá continuidade ao TP1, substituindo vetores estáticos por listas dinâmicas e implementando as operações de inserção, atualização e remoção de partidas.

Compilação
make
./campeonato

Ou:

make run

Os arquivos times.csv e partidas.csv devem estar no mesmo diretório do executável. Para limpar os arquivos compilados:

make clean

O sistema pode ser testado com três cenários de partidas (partidas_vazio.csv, partidas_parcial.csv e partidas_completo.csv), bastando substituir o conteúdo de partidas.csv.

Estrutura do projeto

O código é organizado em módulos independentes, cada um com seus arquivos .h e .c:

Time: representa um time e suas estatísticas.
BDTimes: gerencia os times em lista encadeada.
Partida: representa uma partida entre dois times.
BDPartidas: gerencia as partidas e suas operações de CRUD.
Partidas Serviço: sincroniza as alterações nas partidas com as estatísticas dos times.
Ordenação: utiliza Merge Sort para gerar a classificação.
Entrada: leitura segura de dados do usuário.
UI: interface de menu e interação com o usuário.
Funcionalidades

O sistema permite:

Consultar times por nome ou prefixo;
Consultar partidas por time;
Atualizar placares;
Remover partidas;
Inserir novas partidas com ID automático;
Exibir a classificação ordenada por pontos, vitórias e saldo de gols;
Salvar automaticamente os dados ao encerrar.
Principais decisões de implementação
Uso de listas simplesmente encadeadas para armazenar times e partidas;
Separação entre os módulos de dados e a camada de serviço responsável por manter as estatísticas consistentes;
Atualização incremental das estatísticas, evitando recalcular toda a tabela a cada alteração;
Ordenação da classificação com Merge Sort sobre um vetor de ponteiros;
Busca por prefixo sem diferenciar letras maiúsculas e minúsculas;
Validação de entradas e tratamento de erros, incluindo EOF e dados inválidos.
Testes

O sistema foi validado com os três cenários fornecidos, além de testes para inserção, atualização, remoção, consultas e casos de erro. Também foi utilizado o AddressSanitizer/LeakSanitizer para verificar a ausência de vazamentos de memória e acessos inválidos.
