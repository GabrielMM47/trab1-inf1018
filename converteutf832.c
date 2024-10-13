/* Leo Klinger Svartman 2310862 Turma 3WC*/
/* Gabriel Martins Mendes 2311271 Turma 3WC*/
#include "converteutf832.h"

int convUtf8p32(FILE arquivo_entrada, FILEarquivo_saida){
    unsigned char c;
    int s = 0;
    while(!feof(arquivo_entrada)){ 
        c = fgetc(c);
        if(!(c & 0x80)){

        } else if (c & 0b11100000 == 0xC0) {

        } else if (c & 0b11110000 == 0xE0) {

        } else if (c & 0b11111000 == 0xF0) {

        }
        return 0;
    }
}