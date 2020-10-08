// links para funcoes que vamos utilizar
// http://www.cplusplus.com/reference/cstring/strncpy/
// https://en.wikibooks.org/wiki/C_Programming/C_Reference/nonstandard/strcasecmp
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // isdigit, isalpha
#include <string.h>

char *buffer; // buffer lido
char *iniBuffer; // guarda o inicio do buffer para desalocar
int contaLinha=1; // faz a contagem de linha do programa

// constantes para os atomos do mini analisador l�xico
typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO_INTEIRO,
    ATRIBUICAO,
    WHILE,
    EOS,
    ABRE_PAR
}TAtomo;

// vetor de mensagems para o analisador lexico
char *strAtomo[] = {
    "ERRO LEXICO",
    "IDENTIFICADOR",
    "NUMERO_INTEIRO",
    "ATRIBUICAO",
    "WHILE",
    "END OF STRING",
    "ABRE_PAR",
};

// estrutura para retornar as informa��es de um atomo (tokens)
typedef struct{
    TAtomo atomo;
    int linha;
    int atributo_numero;
    char atributo_ID[15];
}TInfoAtomo;

TInfoAtomo obter_atomo();
void reconhece_num(TInfoAtomo *infoAtomo);
void reconhece_ID(TInfoAtomo *infoAtomo);


int main(void){
    FILE *fp_entrada;
    TInfoAtomo infoAtomo;

    // abre o arquivo
    fp_entrada = fopen("entrada.c", "r");
    if(!fp_entrada){ // testa se abriu corretamente
        printf("erro na abertura do arquivo de entrada.\n");
        return 1;
    }
    // abertura e leitura de arquivo
    fseek(fp_entrada, 0, SEEK_END); // move ponteiro do arquivo para o final
    long tamanho = ftell(fp_entrada); // conta a quantidade de bytes deslocados na movimentacao
    fseek(fp_entrada, 0, SEEK_SET); // move novamente para o inicio do arquivo
    buffer = (char*)calloc(tamanho+1,sizeof(char)); // aloca os memoria para guardar o arquivo lido
    fread(buffer, sizeof(char),tamanho,fp_entrada); // le de uma vez so o arquivo
    fclose(fp_entrada); // fecha arquivo

    printf("%s",buffer);
    printf("\n------------------------------\n");

    while(1){
        infoAtomo = obter_atomo();
        printf("\nlinha %d | %s ", infoAtomo.linha,strAtomo[infoAtomo.atomo]);
        if( infoAtomo.atomo == NUMERO_INTEIRO )
            printf("| %d ", infoAtomo.atributo_numero);
        if( infoAtomo.atomo == IDENTIFICADOR )
            printf("| %s ", infoAtomo.atributo_ID);
        else if( infoAtomo.atomo == EOS || infoAtomo.atomo == ERRO ){
            break;
        }
    }
    printf("\nfim de analise lexica.\n");
    free(iniBuffer);
    return 0;
}
/*
A rotina obter_atomo() do mini analisador l�xico retorna para cada �tomo reconhecido uma codifica��o inteira (constante) para representar o
valor do �tomo e o seu atributo, caso se fa�a necess�rio para o �tomo.
*/
TInfoAtomo obter_atomo(){
    TInfoAtomo infoAtomo;



    // descarta carateres delimitadores
    while( *buffer == '\n' || *buffer == '\r' || *buffer == '\t' || *buffer == ' '){
        if( *buffer == '\n' )
            contaLinha++;
        buffer++;
    }

    infoAtomo.atomo = ERRO;
    infoAtomo.linha = contaLinha;
    // if(buffer[0] ==':' && buffer[1] =='=')
    if(*buffer ==':' && *(buffer+1) =='='){ // reconhece atribuicao
        buffer+=2; // incrementa o buffer duas posicoes
        infoAtomo.atomo = ATRIBUICAO;
    }
    else if(*buffer =='('){ // reconhece numero inteiro
        infoAtomo.atomo = ABRE_PAR;
        buffer++;
    }
    else if(isdigit(*buffer)){ // reconhece numero inteiro
        reconhece_num(&infoAtomo);
    }
    else if( isalpha(*buffer)){ // reconhece identificador
        reconhece_ID(&infoAtomo);
    }
    else if(*buffer == 0) // reconhece fim de string
        infoAtomo.atomo = EOS;

    return infoAtomo;

}
void reconhece_num(TInfoAtomo *infoAtomo){
    char *iniNum = buffer;

    while( isdigit(*buffer))
        buffer++;

    if( isalpha(*buffer))
        return;

    strncpy(infoAtomo->atributo_ID,iniNum,buffer-iniNum);
    infoAtomo->atributo_ID[buffer-iniNum]=0; // finalizador de string
    infoAtomo->atributo_numero = atoi(infoAtomo->atributo_ID);
    infoAtomo->atomo = NUMERO_INTEIRO;

}

void reconhece_ID(TInfoAtomo *infoAtomo){
    char *iniID = buffer;

    while(isalpha(*buffer) || isdigit(*buffer))
        buffer++;

    strncpy(infoAtomo->atributo_ID,iniID,buffer-iniID);
    infoAtomo->atributo_ID[buffer-iniID]=0; // finalizador de string
    if( strcasecmp(infoAtomo->atributo_ID,"WHILE")==0 )
       infoAtomo->atomo = WHILE;
    else
       infoAtomo->atomo = IDENTIFICADOR;

}