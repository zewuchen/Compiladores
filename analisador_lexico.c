/*
    Entrega de trabalho - Mini Analisador Léxico

    Nós,

    Zewu Chen 31808751

    declaramos que

    todas as respostas são fruto de nosso próprio trabalho,
    não copiamos respostas de colegas externos à equipe,
    não disponibilizamos nossas respostas para colegas externos à equipe e
    não realizamos quaisquer outras atividades desonestas para nos beneficiar ou prejudicar outros.

    Matéria: Compiladores
    Curso: Ciência da Computação
    TIA: 31808751
    Professor: Fabio Lubacheski
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define TAMANHO 16 // Tamanho máximo das palavras

char *buffer; // Buffer lido
char *iniBuffer; // Guarda o inicio do buffer para desalocar
int contaLinha=1; // Faz a contagem de linha do programa

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
    CARACTERE,
    VIRGULA
}TAtomo;

// Vetor de mensagems para o analisador lexico
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
    "CARACTERE",
    "VIRGULA" 
};

// Estrutura para retornar as informações de um atomo (tokens)
typedef struct{
    TAtomo atomo;
    int linha;
    int atributo_numero;
    double atributo_real;
    char atributo_ID[TAMANHO];
} TInfoAtomo;

TInfoAtomo obter_atomo();
TInfoAtomo infoAtomo;
TInfoAtomo lookahead; // Simbolo no inicio da cadeia
void reconhece_num(TInfoAtomo *infoAtomo);
void reconhece_ID(TInfoAtomo *infoAtomo);
void consome(TAtomo atomo);
void programa();

int main(void){
    FILE *fp_entrada;

    // Abre o arquivo
    fp_entrada = fopen("entrada2.c", "r");
    if(!fp_entrada){ // Testa se abriu corretamente
        printf("Erro na abertura do arquivo de entrada.\n");
        return 1;
    }
    // Abertura e leitura de arquivo
    fseek(fp_entrada, 0, SEEK_END); // Move ponteiro do arquivo para o final
    long tamanho = ftell(fp_entrada); // Conta a quantidade de bytes deslocados na movimentacao
    fseek(fp_entrada, 0, SEEK_SET); // Move novamente para o inicio do arquivo
    buffer = (char*)calloc(tamanho+1,sizeof(char)); // Aloca os memoria para guardar o arquivo lido
    fread(buffer, sizeof(char),tamanho,fp_entrada); // Le de uma vez so o arquivo
    fclose(fp_entrada); // Fecha arquivo

    // printf("%s",buffer);
    // printf("\n------------------------------\n");

    // while(1) {
    //     infoAtomo = obter_atomo();
    //     printf("\nLinha %d | %s ", infoAtomo.linha, strAtomo[infoAtomo.atomo]);
    //     if( infoAtomo.atomo == NUMERO_INTEIRO )
    //         printf("| %d ", infoAtomo.atributo_numero);
    //     if( infoAtomo.atomo == NUMERO_REAL )
    //         printf("| %f ", infoAtomo.atributo_real);
    //     if( infoAtomo.atomo == IDENTIFICADOR )
    //         printf("| %s ", infoAtomo.atributo_ID);
    //     if( infoAtomo.atomo == CARACTERE )
    //         printf("| %s ", infoAtomo.atributo_ID);
    //     else if( infoAtomo.atomo == EOS || infoAtomo.atomo == ERRO ){
    //         break;
    //     }
    // }
    infoAtomo = obter_atomo();
    lookahead = infoAtomo;
    programa();
    free(iniBuffer);
    return 0;
}

void consome(TAtomo atomo) {

    if(infoAtomo.atomo == atomo) {
        infoAtomo = obter_atomo();
        lookahead = infoAtomo;
    } else {
        if(infoAtomo.atomo == ERRO) {
            printf("Erro léxico: esperado [%s] encontrado [%s] na linha [%d]\n", strAtomo[atomo], strAtomo[infoAtomo.atomo],infoAtomo.linha);
        } else {
            printf("Erro sintático: esperado [%s] encontrado [%s] na linha [%d]\n", strAtomo[atomo], strAtomo[infoAtomo.atomo],infoAtomo.linha);
        }
        exit(1);
    }
}

// <programa>::= program identificador “;” <bloco> “.”
void programa() {
    consome(PROGRAM);
    consome(IDENTIFICADOR);
    consome(PONTO_VIRGULA);
    // bloco();
    consome(PONTO);
    printf("\nFim de programa\n");
}

TInfoAtomo obter_atomo(){
    TInfoAtomo infoAtomo;

    // Descarta carateres delimitadores
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
    else if(*buffer =='.'){
        infoAtomo.atomo = PONTO;
        buffer++;
    }
    else if(*buffer ==','){ 
        infoAtomo.atomo = VIRGULA;
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
    else if(*buffer =='{'){
        infoAtomo.atomo = ERRO;
        while(*buffer != '}') {
            if( *buffer == '\n' )
                contaLinha++;
            if(*buffer == 0)
                break;
            buffer++;
        }
        // Consome o fecha comentario e valida se fechou o comentário
        if(*buffer != 0 && *buffer == '}') {
            infoAtomo.atomo = COMENTARIO_1;
            buffer++;
        }
    }
    else if(*buffer =='#'){
        infoAtomo.atomo = COMENTARIO_2;
        while(*buffer != '\n') {
            if(*buffer == 0)
                break;

            buffer++;
        }
    }
    else if(*buffer=='\''){ // Reconhece caractere
        // Consome o abre caractere '
        buffer++;
        infoAtomo.atomo = CARACTERE;
        infoAtomo.atributo_ID[0] = *buffer;
        buffer++;// Caractere
        buffer++;// Fecha caractere '
    }
    else if(isdigit(*buffer)){ // Reconhece numero inteiro
        reconhece_num(&infoAtomo);
    }
    else if(isalpha(*buffer)){ // Reconhece identificador
        reconhece_ID(&infoAtomo);
    }
    else if(*buffer == 0){ // Reconhece fim de string
        infoAtomo.atomo = EOS;
    }
    // TODO: ERRO Léxico

    return infoAtomo;
}

void reconhece_num(TInfoAtomo *infoAtomo){
    char *iniNum = buffer;
    int isInt = 1;

    while(isdigit(*buffer))
        buffer++;

    if(isalpha(*buffer))
        return;
    
    if(*buffer =='.' && (*(buffer+1) =='e' || *(buffer+1) =='E')) { 
        buffer+=2;

        if(*buffer =='+' || *buffer =='-')
            buffer++;

        if( isalpha(*buffer))
            return;

        while( isdigit(*buffer))
            buffer++;

        isInt = 0;
    }

    if(*buffer =='.') {
        return;
    }
    
    if(isInt) {
        strncpy(infoAtomo->atributo_ID,iniNum,buffer-iniNum);
        infoAtomo->atributo_ID[buffer-iniNum]=0; // Finalizador de string
        infoAtomo->atributo_numero = atoi(infoAtomo->atributo_ID);
        infoAtomo->atomo = NUMERO_INTEIRO;
    } else {
        strncpy(infoAtomo->atributo_ID,iniNum,buffer-iniNum);
        infoAtomo->atributo_ID[buffer-iniNum]=0; 
        infoAtomo->atributo_real = atof(infoAtomo->atributo_ID);
        infoAtomo->atomo = NUMERO_REAL;
    }
}

void reconhece_ID(TInfoAtomo *infoAtomo){
    char *iniID = buffer;
    
    if(isalpha(*buffer)) {
        buffer++;
    }

    while(isalpha(*buffer) || isdigit(*buffer) || *buffer == '_')
        buffer++;
    strncpy(infoAtomo->atributo_ID,iniID,buffer-iniID);

    infoAtomo->atributo_ID[buffer-iniID]=0; // Finalizador de string
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
    else {
        int tamanho_palavra = strlen(infoAtomo->atributo_ID);

        if(tamanho_palavra > TAMANHO) {
            infoAtomo->atomo = ERRO;
        } else {
            infoAtomo->atomo = IDENTIFICADOR;
        }
    }   
}