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
    AND, 
    BEGIN, 
    BOOLEAN, 
    CHAR, 
    DO, 
    ELSE, 
    END, 
    FALSE, 
    IF, 
    INTEGER, 
    MOD,
    NOT,
    OR, 
    PROGRAM,
    READ,
    REAL,
    THEN, 
    TRUE, 
    WRITE,
    ABRE_PAR,
    FECHA_PAR,
    PONTO,
    PONTO_VIRGULA,
    SUBTRACAO,
    ADICAO,
    DIVISAO,
    MULTIPLICACAO,
    ME, 
    MEI,
    IG, 
    DI, 
    MA, 
    MAI,
    COMENTARIO_1,
    COMENTARIO_2,
    NUMERO_REAL,
    CARACTERE
}TAtomo;

// vetor de mensagems para o analisador lexico
char *strAtomo[] = {
    "ERRO LEXICO",
    "IDENTIFICADOR",
    "NUMERO_INTEIRO",
    "ATRIBUICAO",
    "WHILE",
    "END OF STRING",
    "AND", 
    "BEGIN", 
    "BOOLEAN", 
    "CHAR", 
    "DO", 
    "ELSE", 
    "END", 
    "FALSE", 
    "IF", 
    "INTEGER", 
    "MOD",
    "NOT",
    "OR", 
    "PROGRAM",
    "READ",
    "REAL",
    "THEN", 
    "TRUE", 
    "WRITE",
    "ABRE_PAR",
    "FECHA_PAR",
    "PONTO",
    "PONTO_VIRGULA",
    "SUBTRACAO",
    "ADICAO",
    "DIVISAO",
    "MULTIPLICACAO",
    "ME", 
    "MEI",
    "IG", 
    "DI", 
    "MA", 
    "MAI",
    "COMENTARIO_1",
    "COMENTARIO_2",
    "NUMERO_REAL",
    "CARACTERE" 
};

// estrutura para retornar as informa��es de um atomo (tokens)
typedef struct{
    TAtomo atomo;
    int linha;
    int atributo_numero;
    double atributo_real;
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
        if( infoAtomo.atomo == NUMERO_REAL )
            printf("| %f ", infoAtomo.atributo_real);
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

    if(*buffer ==':' && *(buffer+1) =='='){
        buffer+=2; 
        infoAtomo.atomo = ATRIBUICAO;
    }
    else if(*buffer =='('){ 
        infoAtomo.atomo = ABRE_PAR;
        buffer++;
    }
    else if(*buffer ==')'){ 
        infoAtomo.atomo = FECHA_PAR;
        buffer++;
    }
    else if(*buffer =='.'){ // reconhece numero inteiro
        infoAtomo.atomo = PONTO;
        buffer++;
    }
    else if(*buffer ==';'){ 
        infoAtomo.atomo = PONTO_VIRGULA;
        buffer++;
    }
    else if(*buffer =='-'){ 
        infoAtomo.atomo = SUBTRACAO;
        buffer++;
    }
    else if(*buffer =='+'){ 
        infoAtomo.atomo = ADICAO;
        buffer++;
    }
    else if(*buffer =='/'){ 
        infoAtomo.atomo = DIVISAO;
        buffer++;
    }
    else if(*buffer =='*'){ 
        infoAtomo.atomo = MULTIPLICACAO;
        buffer++;
    }
    else if(*buffer =='<' && *(buffer+1) =='='){ 
        buffer+=2; 
        infoAtomo.atomo = MEI;
    }
    else if(*buffer =='>' && *(buffer+1) =='='){ 
        buffer+=2; 
        infoAtomo.atomo = MAI;
    }
    else if(*buffer =='!' && *(buffer+1) =='='){ 
        buffer+=2; 
        infoAtomo.atomo = DI;
    }
    else if(*buffer =='<'){ 
        buffer++; 
        infoAtomo.atomo = ME;
    }
    else if(*buffer == '>'){ 
        buffer++; 
        infoAtomo.atomo = MA;
    }
    else if(*buffer =='='){ 
        buffer++; 
        infoAtomo.atomo = IG;
    }
    // As três funções abaixo não funcionam
    /*
    else if(*buffer =='{'){
        infoAtomo.atomo = COMENTARIO_1;
        while(*buffer != '}') {
            if( *buffer == '\n' )
                contaLinha++;
            if(*buffer == 0) 
                infoAtomo.atomo = ERRO;
            buffer++;
        }
    }
    else if(*buffer =='#'){
        infoAtomo.atomo = COMENTARIO_2;
        while(*buffer != '\n') {
            buffer++;
        }
    }
    else if(isascii(*buffer)){ // reconhece caractere
        infoAtomo.atomo = CARACTERE;
        infoAtomo.atributo_ID[0] = *buffer;
        buffer++; 
    }
    */
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
    int isInt = 1;

    while( isdigit(*buffer))
        buffer++;

    if( isalpha(*buffer))
        return;
    
    if(*buffer =='.' && (*(buffer+1) =='e' || *(buffer+1) =='E')) { 
        buffer+=2;

        if(*buffer =='+' || *buffer =='-')
            buffer++;

        if( isalpha(*buffer))
            return;

        while( isdigit(*buffer))
            buffer++;
        
        printf("\nEntrou no reconhecimento de número real");
        isInt = 0;
    }
    
    if(isInt) {
        strncpy(infoAtomo->atributo_ID,iniNum,buffer-iniNum);
        infoAtomo->atributo_ID[buffer-iniNum]=0; // finalizador de string
        infoAtomo->atributo_numero = atoi(infoAtomo->atributo_ID);
        infoAtomo->atomo = NUMERO_INTEIRO;
    } else {
        strncpy(infoAtomo->atributo_ID,iniNum,buffer-iniNum);
        infoAtomo->atributo_ID[buffer-iniNum]=0; 
        infoAtomo->atributo_real = atof(infoAtomo->atributo_ID); // Não sei converter para número real, já adicionei a propriedade double na struct
        infoAtomo->atomo = NUMERO_REAL;
    }
}

void reconhece_ID(TInfoAtomo *infoAtomo){
    char *iniID = buffer;

    while(isalpha(*buffer) || isdigit(*buffer))
        buffer++;

    strncpy(infoAtomo->atributo_ID,iniID,buffer-iniID);
    infoAtomo->atributo_ID[buffer-iniID]=0; // finalizador de string
    if( strcasecmp(infoAtomo->atributo_ID,"WHILE")==0 )
       infoAtomo->atomo = WHILE;
    else if( strcasecmp(infoAtomo->atributo_ID,"AND")==0 )
       infoAtomo->atomo = AND;
    else if( strcasecmp(infoAtomo->atributo_ID,"BEGIN")==0 )
       infoAtomo->atomo = BEGIN;
    else if( strcasecmp(infoAtomo->atributo_ID,"BOOLEAN")==0 )
       infoAtomo->atomo = BOOLEAN;
    else if( strcasecmp(infoAtomo->atributo_ID,"CHAR")==0 )
       infoAtomo->atomo = CHAR;
    else if( strcasecmp(infoAtomo->atributo_ID,"DO")==0 )
       infoAtomo->atomo = DO;
    else if( strcasecmp(infoAtomo->atributo_ID,"ELSE")==0 )
       infoAtomo->atomo = ELSE;
    else if( strcasecmp(infoAtomo->atributo_ID,"END")==0 )
       infoAtomo->atomo = END;
    else if( strcasecmp(infoAtomo->atributo_ID,"FALSE")==0 )
       infoAtomo->atomo = FALSE;
    else if( strcasecmp(infoAtomo->atributo_ID,"IF")==0 )
       infoAtomo->atomo = IF;
    else if( strcasecmp(infoAtomo->atributo_ID,"INTEGER")==0 )
       infoAtomo->atomo = INTEGER;
    else if( strcasecmp(infoAtomo->atributo_ID,"MOD")==0 )
       infoAtomo->atomo = MOD;
    else if( strcasecmp(infoAtomo->atributo_ID,"NOT")==0 )
       infoAtomo->atomo = NOT;
    else if( strcasecmp(infoAtomo->atributo_ID,"OR")==0 )
       infoAtomo->atomo = OR;
    else if( strcasecmp(infoAtomo->atributo_ID,"PROGRAM")==0 )
       infoAtomo->atomo = PROGRAM;
    else if( strcasecmp(infoAtomo->atributo_ID,"READ")==0 )
       infoAtomo->atomo = READ;
    else if( strcasecmp(infoAtomo->atributo_ID,"REAL")==0 )
       infoAtomo->atomo = REAL;
    else if( strcasecmp(infoAtomo->atributo_ID,"THEN")==0 )
       infoAtomo->atomo = THEN;
    else if( strcasecmp(infoAtomo->atributo_ID,"TRUE")==0 )
       infoAtomo->atomo = TRUE;
    else if( strcasecmp(infoAtomo->atributo_ID,"WRITE")==0 )
       infoAtomo->atomo = WRITE;
    else
       infoAtomo->atomo = IDENTIFICADOR;
}