#include <stdio.h>
#include "converteutf832.h"  // Assume que ambas as funções estão neste arquivo

int main() {
    // Abrindo o arquivo de entrada (UTF-8)
    FILE *arquivo_entrada_utf8 = fopen("entrada_utf8.txt", "rb");
    if (arquivo_entrada_utf8 == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada UTF-8.\n");
        return -1;
    }

    // Abrindo o arquivo de saída (UTF-32) para conversão de UTF-8 -> UTF-32
    FILE *arquivo_saida_utf32 = fopen("saida_utf32.txt", "wb");
    if (arquivo_saida_utf32 == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída UTF-32.\n");
        fclose(arquivo_entrada_utf8);
        return -1;
    }

    // Chamando a função de conversão de UTF-8 para UTF-32
    int resultado = convUtf8p32(arquivo_entrada_utf8, arquivo_saida_utf32);

    // Verificar o resultado da conversão UTF-8 -> UTF-32
    if (resultado == 0) {
        printf("Conversão de UTF-8 para UTF-32 concluída com sucesso!\n");
    } else {
        fprintf(stderr, "Erro na conversão de UTF-8 para UTF-32.\n");
        fclose(arquivo_entrada_utf8);
        fclose(arquivo_saida_utf32);
        return -1;
    }

    // Fechar os arquivos após a conversão de UTF-8 para UTF-32
    fclose(arquivo_entrada_utf8);
    fclose(arquivo_saida_utf32);

    // Abrir o arquivo UTF-32 de saída para leitura
    FILE *arquivo_entrada_utf32 = fopen("saida_utf32.txt", "rb");
    if (arquivo_entrada_utf32 == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo UTF-32 para conversão reversa.\n");
        return -1;
    }

    // Abrir o arquivo de saída final (UTF-8) para conversão de UTF-32 -> UTF-8
    FILE *arquivo_saida_utf8 = fopen("saida_utf8.txt", "wb");
    if (arquivo_saida_utf8 == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída UTF-8.\n");
        fclose(arquivo_entrada_utf32);
        return -1;
    }

    // Chamando a função de conversão de UTF-32 para UTF-8
    resultado = convUtf32p8(arquivo_entrada_utf32, arquivo_saida_utf8);

    // Verificar o resultado da conversão UTF-32 -> UTF-8
    if (resultado == 0) {
        printf("Conversão de UTF-32 para UTF-8 concluída com sucesso!\n");
    } else {
        fprintf(stderr, "Erro na conversão de UTF-32 para UTF-8.\n");
        fclose(arquivo_entrada_utf32);
        fclose(arquivo_saida_utf8);
        return -1;
    }

    // Fechando os arquivos após a conversão de UTF-32 para UTF-8
    fclose(arquivo_entrada_utf32);
    fclose(arquivo_saida_utf8);

    printf("Processo completo!\n");

    return 0;
}