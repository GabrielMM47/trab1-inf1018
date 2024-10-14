/* Leo Klinger Svartman 2310862 Turma 3WC*/
/* Gabriel Martins Mendes 2311271 Turma 3WC*/
#include "converteutf832.h"

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida){
    
    if(arquivo_entrada == NULL || arquivo_saida == NULL) {
        fprintf(stderr, "Erro: Arquivo de entrada ou saída é nulo.\n");
        return -1;
    }
    
    unsigned char bytes[4];
    unsigned int s = 0;

    unsigned int bom = 0x0000FEFF;
    fwrite(&bom, sizeof(int), 1, arquivo_saida);

    while(fread(bytes, sizeof(char), 1, arquivo_entrada) == 1){ 
        //ERRO
        if(!(bytes[0] & 0x80)){
            s = s | bytes[0];
        }

        else if ((bytes[0] & 0xE0) == 0xC0) {
            if (fread(bytes + 1, sizeof(unsigned char), 1, arquivo_entrada) != 1)
            {
                fprintf(stderr, "Erro de leitura: não foi possível ler o segundo byte.\n");
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