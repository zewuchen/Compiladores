#include <stdio.h>
#include <stdlib.h>

#define TAMANHO 15

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
int globalLinha = 1;
TInfoAtomo obter_atomo();
TInfoAtomo atomoAtribuicao();
TInfoAtomo atomoWhile();

int main(void) {
    FILE *fp_entrada, *fp_saida;
    char *iniBuffer;
    
    fp_entrada = fopen("entrada.c", "r"); // Abre o arquivo
    if(!fp_entrada) { // Testa se abriu corretamente
        printf("Não conseguiu abriu o arquivo de entrada\n");
        return 1;
    }

    fseek(fp_entrada, 0, SEEK_END); // Move ponteiro do arquivo para o final
    long tamanho = ftell(fp_entrada); // Conta a quantidade de bytes do arquivo
    fseek(fp_entrada, 0, SEEK_SET); // Move ponteiro do arquivo para o inicio
    buffer = (char*)calloc(tamanho+1,sizeof(char)); // Aloca a memoria para guardar o arquivo lido
    fread(buffer, sizeof(char),tamanho,fp_entrada); // Lê de uma vez só o arquivo
    
    printf("Arquivo \n%s", buffer); // Imprime o arquivo

    fp_saida = fopen("saida.c", "w+");// Abre o arquivo de saida, w+ abre para escrita e se o arquivo não existir cria
    if(!fp_saida){ // Testa se abriu corretamente
        printf("Não conseguiu abriu o arquivo de saída\n");
        return 1;
    }

    iniBuffer = buffer;

    // Inicia pegando o primeiro átomo
    while(1) {
        TInfoAtomo atomo = obter_atomo();
        printf("\n-----ATOMO TIPO %d", atomo.atomo);
        if(atomo.atomo == ERRO) {
            fprintf(fp_saida,"Linha: %d - ERRO\n", atomo.linha);
            break;
        } else if(atomo.atomo == IDENTIFICADOR) {
            // fprintf(fp_saida,"Linha: %d - Identificador - %s\n", atomo.linha, atomo.atributo_ID);
            break;
        } else if(atomo.atomo == NUMERO_INTEIRO) {
            break;
        } else if(atomo.atomo == ATRIBUICAO) {
            fprintf(fp_saida,"Linha: %d - Operador Atribuição\n", atomo.linha);
        } else if(atomo.atomo == WHILE) {
            fprintf(fp_saida,"Linha: %d - While\n", atomo.linha);
        } else if(atomo.atomo == EOS) {
            fprintf(fp_saida,"Linha: %d - Fim de String(EOS)\n", atomo.linha);
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
    atomo.atomo = EOS;
    atomo.linha = globalLinha;

    // Acrescentar linhas
    if(*buffer == '\n') {
        globalLinha++;
        printf("Linhas %d", globalLinha);
    }

    // Verificar as definições regulares
    if(*buffer == ':') {
        atomo = atomoAtribuicao();
        buffer++;
    } else if(*buffer == 'w') {
        atomo = atomoWhile();
        buffer++;
    } else if(*buffer == 0) {
        atomo.atomo = EOS;
        atomo.linha = globalLinha;
        printf("\nRetornou Fim");
    }

    return atomo;
}

TInfoAtomo atomoAtribuicao() {
    TInfoAtomo atomo;
    atomo.atomo = ERRO;
    atomo.linha = globalLinha;

    if(*buffer == ':') {
        buffer++;
        
        if(*buffer == '=') {
            atomo.atomo = ATRIBUICAO;
            printf("\nRetornou Atribuicao");
        }
    }

    return atomo;
}

TInfoAtomo atomoWhile() {
    TInfoAtomo atomo;
    atomo.atomo = ERRO;
    atomo.linha = globalLinha;

    if(*buffer == 'w' || *buffer == 'W') {
        buffer++;
        if(*buffer == 'h' || *buffer == 'H') {
            buffer++;
            if(*buffer == 'i' || *buffer == 'I') {
                buffer++;
                if(*buffer == 'l' || *buffer == 'L') {
                    buffer++;
                    if(*buffer == 'e' || *buffer == 'E') {
                        atomo.atomo = WHILE;
                        printf("\nRetornou While");
                    }
                }
            }
        }
    }

    return atomo;
}