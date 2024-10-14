#include <stdio.h>
#include "converteutf832.h"

int main() {
    // Abrindo o arquivo de entrada (UTF-8)
    FILE *arquivo_entrada = fopen("entrada_utf8.txt", "rb");
    if (arquivo_entrada == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return -1;
    }

    // Abrindo o arquivo de saída (UTF-32)
    FILE *arquivo_saida = fopen("saida_utf32.txt", "wb");
    if (arquivo_saida == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        fclose(arquivo_entrada);
        return -1;
    }

    // Chamando a função de conversão
    int resultado = convUtf8p32(arquivo_entrada, arquivo_saida);

    // Verificar o resultado da conversão
    if (resultado == 0) {
        printf("Conversão concluída com sucesso!\n");
    } else {
        fprintf(stderr, "Erro na conversão de UTF-8 para UTF-32.\n");
    }

    // Fechando os arquivos
    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    return 0;
}