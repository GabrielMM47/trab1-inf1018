/* Leo Klinger Svartman 2310862 Turma 3WC*/
/* Gabriel Martins Mendes 2311271 Turma 3WC*/
#include "converteutf832.h"

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
    
    if(arquivo_entrada == NULL || arquivo_saida == NULL) {
        fprintf(stderr, "Erro: Arquivo de entrada ou saída é nulo.\n");
        return -1;
    }
    
    unsigned char bytes[4];
    unsigned int s;

    unsigned int bom = 0x0000FEFF;
    fwrite(&bom, sizeof(int), 1, arquivo_saida);

    while(fread(bytes, sizeof(char), 1, arquivo_entrada) == 1){ 

        s = 0;
        //ERRO
        if(!(bytes[0] & 0x80)){
            s = s | bytes[0];
        }

        else if ((bytes[0] & 0xE0) == 0xC0) {
            if (fread(bytes + 1, sizeof(unsigned char), 1, arquivo_entrada) != 1)
            {
                fprintf(stderr, "Erro de leitura: não foi possível ler o bytes seguintes.\n");
                return -1;
            }
            s = s | (bytes[0]&0x1F) << 6; //
            s = s | (bytes[1]&0x3F);
        }

        else if ((bytes[0] & 0xF0) == 0xE0) {
            
            if (fread(bytes + 1, sizeof(unsigned char), 2, arquivo_entrada) != 2)
            {
                fprintf(stderr, "Erro de leitura: não foi possível ler os bytes seguintes.\n");
                return -1;
            }
            s = s | (bytes[0]&0x0F) << 12;
            s = s | (bytes[1]&0x3F) << 6;
            s = s | (bytes[2]&0x3F);
        }

        else if ((bytes[0] & 0xF8) == 0xF0) {

            if (fread(bytes + 1, sizeof(unsigned char), 3, arquivo_entrada) != 3)
            {
                fprintf(stderr, "Erro de leitura: não foi possível ler os bytes seguintes.\n");
                return -1;
            }
            s = s | (bytes[0] & 0x07) << 18;
            s = s | (bytes[1] & 0x3F) << 12;
            s = s | (bytes[2] & 0x3F) << 6;
            s = s | (bytes[3] & 0x3F);
        }

        fwrite(&s, sizeof(int), 1, arquivo_saida);

        //apenas exibindo o bytes para ver o que esta acontencendo... (mini dump)
		unsigned char* utf32Bytes = (unsigned char*)&s;
		printf("Bytes: %02X %02X %02X %02X\n", utf32Bytes[0], utf32Bytes[1], utf32Bytes[2], utf32Bytes[3]);
    }
    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){

    int littleEndian = 0;
	unsigned int s, bom;

	// Ler o BOM do arquivo de entrada
    if (fread(&bom, sizeof(int), 1, arquivo_entrada) != 1) {
        fprintf(stderr, "Erro de leitura: não foi possível ler o BOM.\n");
        return -1;
    }

    // Verificar o BOM (Little-endian ou Big-endian)
    printf("BOM: %02x\n", bom);
    if (bom == 0x0000FEFF) {
        littleEndian = 1; // Little Endian
    } else if (bom == 0xFFFE0000) {
        littleEndian = 0; // Big Endian
    } else {
        fprintf(stderr, "Erro: BOM inválido.\n");
        return -1;
    }

	while (fread(&s, sizeof(unsigned int), 1, arquivo_entrada) == 1){
		unsigned char bytes[4];
		if (!littleEndian){
			unsigned int b1, b2, b3, b4;

			b1 = 0x000000FF & s;
			b2 = 0x0000FF00 & s;
			b3 = 0x00FF0000 & s;
			b4 = 0xFF000000 & s;

			s = 0;
			s = s | (b1 << 24);
			s = s | (b2 << 8);
			s = s | (b3 >> 8);
			s = s | (b4 >> 24);
		}

		if(s <= 0x7F){
			// 1 byte (ASCII)
            bytes[0] = s;
            if (fwrite(bytes, sizeof(char), 1, arquivo_saida) != 1) {
                fprintf(stderr, "Erro de gravação: Não foi possível escrever 1 byte.\n");
                return -1;
            }
            printf("Escrevendo 1 byte: %02x\n", bytes[0]);
		}
		else if(s <= 0x7FF){
			// 2 bytes
            bytes[0] = 0xC0 | (s >> 6);
            bytes[1] = 0x80 | (s & 0x3F);
            if (fwrite(bytes, sizeof(char), 2, arquivo_saida) != 2) {
                fprintf(stderr, "Erro de gravação: Não foi possível escrever 2 bytes.\n");
                return -1;
            }
            printf("Escrevendo 2 bytes: %02x %02x\n", bytes[0], bytes[1]);
		}
		else if(s <= 0xFFFF){
			// 3 bytes
            bytes[0] = 0xE0 | (s >> 12);
            bytes[1] = 0x80 | ((s >> 6) & 0x3F);
            bytes[2] = 0x80 | (s & 0x3F);
            if (fwrite(bytes, sizeof(char), 3, arquivo_saida) != 3) {
                fprintf(stderr, "Erro de gravação: Não foi possível escrever 3 bytes.\n");
                return -1;
            }
            printf("Escrevendo 3 bytes: %02x %02x %02x\n", bytes[0], bytes[1], bytes[2]);
		}
		else if(s <= 0x10FFFF){
			 // 4 bytes
            bytes[0] = 0xF0 | (s >> 18);
            bytes[1] = 0x80 | ((s >> 12) & 0x3F);
            bytes[2] = 0x80 | ((s >> 6) & 0x3F);
            bytes[3] = 0x80 | (s & 0x3F);
            if (fwrite(bytes, sizeof(char), 4, arquivo_saida) != 4) {
                fprintf(stderr, "Erro de gravação: Não foi possível escrever 4 bytes.\n");
                return -1;
            }
            printf("Escrevendo 4 bytes: %02x %02x %02x %02x\n", bytes[0], bytes[1], bytes[2], bytes[3]);
		}
	}

     // Verificar se houve erro de leitura
    if (ferror(arquivo_entrada)) {
        fprintf(stderr, "Erro de leitura: Não foi possível ler o arquivo de entrada.\n");
        return -1;
    }

    return 0;
}