#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ERRO,
    IDENTIFICADOR, 
    NUMERO_INTEIRO, 
    ATRIBUICAO, 
    WHILE,
    EOS
} TAtomo;

typedef struct {
    TAtomo atomo;
    int linha;
    int atributo_numero;
    char atributo_ID[15];
} TInfoAtomo;

char *buffer;
TInfoAtomo obter_atomo();

int main(void){
    FILE *fp_entrada, *fp_saida;
    char *iniBuffer;
    
    fp_entrada = fopen("entrada.txt", "r"); // Abre o arquivo
    if(!fp_entrada){ // Testa se abriu corretamente
        printf("Não conseguiu abriu o arquivo de entrada\n");
        return 1;
    }

    fseek(fp_entrada, 0, SEEK_END); // Move ponteiro do arquivo para o final
    long tamanho = ftell(fp_entrada); // Conta a quantidade de bytes do arquivo
    fseek(fp_entrada, 0, SEEK_SET); // Move ponteiro do arquivo para o inicio
    buffer = (char*)calloc(tamanho+1,sizeof(char)); // Aloca a memoria para guardar o arquivo lido
    fread(buffer, sizeof(char),tamanho,fp_entrada); // Lê de uma vez só o arquivo
    
    // printf("%s",buffer); // Imprime o arquivo

    fp_saida = fopen("saida.txt", "w+");// Abre o arquivo de saida, w+ abre para escrita e se o arquivo não existir cria
    if(!fp_saida){ // Testa se abriu corretamente
        printf("Não conseguiu abriu o arquivo de saída\n");
        return 1;
    }

    iniBuffer = buffer;

    TInfoAtomo atomo = obter_atomo(); // Inicia pegando o primeiro átomo
    while(1) {
        if(atomo.atomo == ERRO) {
            // TODO: Fazer algo com o ERRO
            break;
        } else if(atomo.atomo == IDENTIFICADOR) {
            // TODO: Fazer algo com o IDENTIFICADOR
            break;
        } else if(atomo.atomo == NUMERO_INTEIRO) {
            // TODO: Fazer algo com o NUMERO_INTEIRO
            break;
        } else if(atomo.atomo == ATRIBUICAO) {
            // TODO: Fazer algo com o ATRIBUICAO
            break;
        } else if(atomo.atomo == WHILE) {
            // TODO: Fazer algo com o WHILE
            break;
        } else if(atomo.atomo == EOS) {
            // TODO: Fazer algo com o EOS
            printf("AAAA %s\n",atomo.atributo_ID);
            fprintf(fp_saida,"%s",atomo.atributo_ID);
            break;
        }
    }

    free(iniBuffer);
    fclose(fp_entrada);
    fclose(fp_saida);
    
    return 0;
}

TInfoAtomo obter_atomo() {
    TInfoAtomo atomo;
    atomo.atributo_ID[0]= *buffer;
    atomo.atomo = EOS;
    return atomo;
}