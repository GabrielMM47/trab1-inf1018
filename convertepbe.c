#include <stdio.h>

int convertLittleToBigEndian(FILE *arquivo_little, FILE *arquivo_big) {
    unsigned int s;

    // Escreve o BOM para indicar que o arquivo é Big Endian
    unsigned int bom = 0xFFFE0000;
    fwrite(&bom, sizeof(unsigned int), 1, arquivo_big);

    // Lê cada caractere UTF-32 do arquivo Little Endian
    while (fread(&s, sizeof(unsigned int), 1, arquivo_little) == 1) {
        unsigned int b1 = (s & 0x000000FF) << 24;
        unsigned int b2 = (s & 0x0000FF00) << 8;
        unsigned int b3 = (s & 0x00FF0000) >> 8;
        unsigned int b4 = (s & 0xFF000000) >> 24;
        unsigned int big_endian_value = b1 | b2 | b3 | b4;

        // Grava o valor convertido em Big Endian no arquivo de saída
        fwrite(&big_endian_value, sizeof(unsigned int), 1, arquivo_big);
    }

    // Verifica se houve erro de leitura
    if (ferror(arquivo_little)) {
        fprintf(stderr, "Erro ao ler o arquivo Little Endian.\n");
        return -1;
    }

    return 0;
}

int main() {
    // Abrir o arquivo Little Endian para leitura
    FILE *arquivo_little = fopen("saida_utf32.txt", "rb");
    if (arquivo_little == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo UTF-32 Little Endian.\n");
        return -1;
    }

    // Abrir o arquivo Big Endian para escrita
    FILE *arquivo_big = fopen("saida_utf32_be.txt", "wb");
    if (arquivo_big == NULL) {
        fprintf(stderr, "Erro ao criar o arquivo UTF-32 Big Endian.\n");
        fclose(arquivo_little);
        return -1;
    }

    // Converter o arquivo Little Endian para Big Endian
    if (convertLittleToBigEndian(arquivo_little, arquivo_big) == 0) {
        printf("Arquivo convertido para Big Endian com sucesso!\n");
    } else {
        fprintf(stderr, "Erro na conversão para Big Endian.\n");
    }

    // Fechar os arquivos
    fclose(arquivo_little);
    fclose(arquivo_big);

    return 0;
}