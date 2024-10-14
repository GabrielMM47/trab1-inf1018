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

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida){

    int littleEndian = 0, bigEndian = 0;
	unsigned int utf32, bom;

	fread(&bom, sizeof(unsigned int), 1, input);
	printf("BOM: %02x\n", bom);

	if (bom == 0x0000FEFF)
	{
		printf("BOM: LE\n");
		littleEndian = TRUE;
	}
	else if (bom == 0xFFFE0000)
	{
		printf("BOM: BE\n");
		bigEndian = TRUE;
	}
	else
	{
		printf("BOM invalido ou ausente.\n");
		return -1;
	}

	while (fread(&utf32, sizeof(unsigned int), 1, input) == 1)
	{
		unsigned char [4]; // byte1..4 (so pra ser mais facil de escrever)
		if (bigEndian)
		{
			utf32 = trocaOrdem(utf32);
		}

		int caso = classificaCasoUTF32(utf32);

		switch (caso)
		{
		case UM_BYTE:
			b1 = utf32;
			fwrite(&b1, sizeof(unsigned char), 1, output);
			printf("Escrevendo 1 byte: %02x\n", b1);//mensagem de debug
			break;

		case DOIS_BYTES:
			b1 = 0b11000000 | (utf32 >> 6); //
			b2 = 0b10000000 | (utf32 & 0x3F);
			fwrite(&b1, sizeof(unsigned char), 1, output);
			fwrite(&b2, sizeof(unsigned char), 1, output);

			printf("Escrevendo 2 bytes: %02x %02x\n", b1, b2); //mensagem de debug 
			break;

		case TRES_BYTES:
			b1 = 0xE0 | (utf32 >> 12);
			b2 = 0x80 | ((utf32 >> 6) & 0x3F);
			b3 = 0x80 | (utf32 & 0x3F);
			fwrite(&b1, sizeof(unsigned char), 1, output);
			fwrite(&b2, sizeof(unsigned char), 1, output);
			fwrite(&b3, sizeof(unsigned char), 1, output);

			printf("Escrevendo 3 bytes: %02x %02x %02x\n", b1, b2, b3); //mensagem de debug
			break;

		case QUATRO_BYTES:
			b1 = 0b11110000 | (utf32 >> 18);
			b2 = 0b10000000 | ((utf32 >> 12) & 0x3F);
			b3 = 0b10000000 | ((utf32 >> 6) & 0x3F);
			b4 = 0b10000000 | (utf32 & 0x3F);
			fwrite(&b1, sizeof(unsigned char), 1, output);
			fwrite(&b2, sizeof(unsigned char), 1, output);
			fwrite(&b3, sizeof(unsigned char), 1, output);
			fwrite(&b4, sizeof(unsigned char), 1, output);
			printf("Escrevendo 4 bytes: %02x %02x %02x %02x\n", b1, b2, b3, b4); //mensagem de debug
			break;
		}
	}

    return 0;
}