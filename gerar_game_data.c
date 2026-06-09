#include <stdio.h>   // Biblioteca usada para entrada e saída, como printf e fprintf
#include <stdlib.h>  // Biblioteca usada para funções gerais, como rand()
#include <time.h>    // Biblioteca usada para pegar o tempo atual com time()

// Define o tamanho do tabuleiro
#define BOARD_SIZE 25

// Define a quantidade de bombas no jogo
#define BOMB_COUNT 5


// Função que verifica se um valor já existe dentro de um vetor
static int contains(int values[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (values[i] == value) {
            return 1; // Retorna 1 se encontrou o valor
        }
    }

    return 0; // Retorna 0 se não encontrou
}


// Função que ordena as posições das bombas em ordem crescente
static void sort_bombs(int bombs[]) {
    for (int i = 0; i < BOMB_COUNT - 1; i++) {
        for (int j = i + 1; j < BOMB_COUNT; j++) {
            if (bombs[j] < bombs[i]) {
                int temp = bombs[i];
                bombs[i] = bombs[j];
                bombs[j] = temp;
            }
        }
    }
}


// Função que gera posições aleatórias para as bombas
static void generate_bombs(int bombs[]) {
    int count = 0; // Conta quantas bombas já foram geradas

    while (count < BOMB_COUNT) {
        // Gera uma posição aleatória entre 0 e 24
        int position = rand() % BOARD_SIZE;

        // Verifica se essa posição ainda não foi usada
        if (!contains(bombs, count, position)) {
            bombs[count] = position; // Salva a posição da bomba
            count++; // Aumenta o contador de bombas
        }
    }

    // Ordena as bombas depois de gerar todas
    sort_bombs(bombs);
}


// Função que escreve as bombas no arquivo JSON
static void write_bombs(FILE *file, int bombs[]) {
    fprintf(file, "  \"bombas\": [");

    for (int i = 0; i < BOMB_COUNT; i++) {
        fprintf(file, "%d", bombs[i]);

        // Coloca vírgula entre os números, menos no último
        if (i < BOMB_COUNT - 1) {
            fprintf(file, ", ");
        }
    }

    fprintf(file, "],\n");
}


// Função que escreve todas as perguntas no arquivo JSON
static void write_questions(FILE *file) {
    fprintf(file,
        "  \"perguntas\": [\n"

        // Cada pergunta tem:
        // question = enunciado
        // options = alternativas
        // answer = índice da resposta correta

        "    { \"question\": \"Qual simbolo representa E?\", \"options\": [\"\\u2228\", \"\\u2227\", \"\\u00ac\", \"\\u2192\"], \"answer\": 1 },\n"
        "    { \"question\": \"Qual simbolo representa OU?\", \"options\": [\"\\u00ac\", \"\\u2192\", \"\\u2228\", \"\\u2227\"], \"answer\": 2 },\n"
        "    { \"question\": \"Qual simbolo representa NAO?\", \"options\": [\"\\u00ac\", \"\\u2227\", \"\\u2228\", \"\\u2192\"], \"answer\": 0 },\n"
        "    { \"question\": \"A frase 'Se acertar, entao continua vivo' representa:\", \"options\": [\"Conjuncao\", \"Disjuncao\", \"Negacao\", \"Implicacao\"], \"answer\": 3 },\n"
        "    { \"question\": \"'Peguei diamante E ganhei vida' representa:\", \"options\": [\"p \\u2227 q\", \"p \\u2228 q\", \"\\u00acp\", \"p \\u2192 q\"], \"answer\": 0 },\n"
        "    { \"question\": \"'Peguei diamante OU ganhei bonus' representa:\", \"options\": [\"p \\u2227 q\", \"p \\u2228 q\", \"\\u00acp\", \"p \\u2192 q\"], \"answer\": 1 },\n"
        "    { \"question\": \"'Nao encontrei bomba' pode ser representado por:\", \"options\": [\"p \\u2227 q\", \"p \\u2228 q\", \"\\u00acp\", \"p \\u2192 q\"], \"answer\": 2 },\n"
        "    { \"question\": \"A palavra 'E' na logica significa:\", \"options\": [\"Conjuncao\", \"Disjuncao\", \"Negacao\", \"Implicacao\"], \"answer\": 0 },\n"
        "    { \"question\": \"A palavra 'OU' na logica significa:\", \"options\": [\"Conjuncao\", \"Disjuncao\", \"Negacao\", \"Implicacao\"], \"answer\": 1 },\n"
        "    { \"question\": \"Se p = peguei diamante e q = ganhei pontos, 'se p entao q' e:\", \"options\": [\"p \\u2227 q\", \"p \\u2228 q\", \"p \\u2192 q\", \"\\u00acp\"], \"answer\": 2 },\n"
        "    { \"question\": \"Se p e verdadeiro e q e falso, p \\u2227 q e:\", \"options\": [\"Verdadeiro\", \"Falso\", \"Indefinido\", \"Uma implicacao\"], \"answer\": 1 },\n"
        "    { \"question\": \"Se p e falso e q e verdadeiro, p \\u2228 q e:\", \"options\": [\"Verdadeiro\", \"Falso\", \"Negacao\", \"Contradicao\"], \"answer\": 0 },\n"
        "    { \"question\": \"A negacao de 'peguei diamante' e:\", \"options\": [\"Peguei bomba\", \"Nao peguei diamante\", \"Peguei diamante e bomba\", \"Se peguei, pontuei\"], \"answer\": 1 },\n"
        "    { \"question\": \"Em p \\u2192 q, p e chamado de:\", \"options\": [\"Consequente\", \"Antecedente\", \"Negacao\", \"Disjuncao\"], \"answer\": 1 },\n"
        "    { \"question\": \"Em p \\u2192 q, q e chamado de:\", \"options\": [\"Antecedente\", \"Consequente\", \"Conjuncao\", \"Tabela verdade\"], \"answer\": 1 }\n"

        "  ]\n"
    );
}


// Função principal do programa
int main(void) {
    int bombs[BOMB_COUNT]; // Vetor que guarda as posições das bombas
    FILE *file;            // Ponteiro para o arquivo JSON

    // Inicializa o gerador de números aleatórios usando o tempo atual
    srand((unsigned int) time(NULL));

    // Gera as bombas aleatórias
    generate_bombs(bombs);

    // Cria ou sobrescreve o arquivo gameData.json
    file = fopen("gameData.json", "w");

    // Verifica se houve erro ao criar o arquivo
    if (file == NULL) {
        printf("Erro ao criar gameData.json\n");
        return 1;
    }

    // Começa a escrever o JSON
    fprintf(file, "{\n");

    // Escreve configurações básicas do jogo
    fprintf(file, "  \"tamanhoTabuleiro\": %d,\n", BOARD_SIZE);
    fprintf(file, "  \"quantidadeBombas\": %d,\n", BOMB_COUNT);

    // Escreve as posições das bombas
    write_bombs(file, bombs);

    // Escreve outras configurações do jogo
    fprintf(file, "  \"vidasIniciais\": 3,\n");
    fprintf(file, "  \"tempoPergunta\": 10,\n");
    fprintf(file, "  \"pontosPorDiamante\": 10,\n");
    fprintf(file, "  \"pontosPorAcerto\": 15,\n");

    // Escreve os bônus por sequência de diamantes
    fprintf(file,
        "  \"bonusSequencia\": [\n"
        "    { \"sequencia\": 3, \"pontos\": 20, \"tempoExtra\": 5, \"escudo\": 0, \"vidas\": 0, \"mensagem\": \"Sequencia de 3 diamantes! +5s nas perguntas.\" },\n"
        "    { \"sequencia\": 5, \"pontos\": 30, \"tempoExtra\": 0, \"escudo\": 1, \"vidas\": 0, \"mensagem\": \"Sequencia de 5 diamantes! Voce ganhou 1 escudo.\" },\n"
        "    { \"sequencia\": 8, \"pontos\": 50, \"tempoExtra\": 0, \"escudo\": 0, \"vidas\": 1, \"mensagem\": \"Sequencia de 8 diamantes! Voce ganhou 1 vida extra.\" }\n"
        "  ],\n"
    );

    // Escreve as perguntas no JSON
    write_questions(file);

    // Fecha o JSON
    fprintf(file, "}\n");

    // Fecha o arquivo
    fclose(file);

    // Mostra no terminal onde as bombas foram colocadas
    printf("gameData.json criado com bombas em: ");

    for (int i = 0; i < BOMB_COUNT; i++) {
        printf("%d", bombs[i]);

        if (i < BOMB_COUNT - 1) {
            printf(", ");
        }
    }

    printf("\n");

    // Finaliza o programa com sucesso
    return 0;
}
