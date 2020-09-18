#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

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
TInfoAtomo atomoIdentificador();
TInfoAtomo atomoInteiro();

int main(void) {
    FILE *fp_entrada;
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
    
    // printf("Arquivo \n%s", buffer); // Imprime o arquivo

    iniBuffer = buffer;

    // Inicia pegando o primeiro átomo
    while(1) {
        TInfoAtomo atomo = obter_atomo();
        if(atomo.atomo == ERRO) {
            printf("\nLinha: %d - ERRO", atomo.linha);
            break;
        } else if(atomo.atomo == IDENTIFICADOR) {
            printf("\nLinha: %d - Identificador - %s", atomo.linha, atomo.atributo_ID);
        } else if(atomo.atomo == NUMERO_INTEIRO) {
            char number[TAMANHO];
            printf("\nLinha: %d - Número Inteiro - %d", atomo.linha, atomo.atributo_numero);
        } else if(atomo.atomo == ATRIBUICAO) {
            printf("\nLinha: %d - Operador Atribuição", atomo.linha);
        } else if(atomo.atomo == WHILE) {
            printf("\nLinha: %d - While\n", atomo.linha);
        } else if(atomo.atomo == EOS) {
            printf("\nLinha: %d - Fim de String(EOS)", atomo.linha);
            break;
        }
    }

    free(iniBuffer);
    fclose(fp_entrada);
    
    return 0;
}

TInfoAtomo obter_atomo() {
    TInfoAtomo atomo;
    atomo.atomo = EOS;

    // Acrescentar linhas
    if(*buffer == '\n' || *buffer == '\r') {
        globalLinha++;
        buffer++;
    }

    if(*buffer == ' ' || *buffer == '\t') {
        buffer++;
    }

    atomo.linha = globalLinha;

    // Verificar as definições regulares
    if(*buffer == ':') {
        atomo = atomoAtribuicao();
        buffer++;
    } else if(*buffer == 'w') {
        atomo = atomoWhile();
        buffer++;
    } else if(*buffer == 0) {
        atomo.atomo = EOS;
    } else if(isalpha(*buffer)) {
        atomo = atomoIdentificador();
    } else if(isdigit(*buffer)) {
        atomo = atomoInteiro();
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
                    }
                }
            }
        }
    }

    return atomo;
}

TInfoAtomo atomoIdentificador() {
    TInfoAtomo atomo;
    atomo.atomo = ERRO;
    atomo.linha = globalLinha;
    int contagem = 0;

    id0:
        if(isalpha(*buffer)) {
            atomo.atributo_ID[contagem] = *buffer;
            buffer++;
            contagem++;
            goto id1;
        } else {
            return atomo;
        }

    id1:
        if(isalpha(*buffer) || isdigit(*buffer)) {
            atomo.atributo_ID[contagem] = *buffer;
            buffer++;
            contagem++;
            
            goto id1;
        } else {
            goto id2;
        }
    
    id2:
        atomo.atomo = IDENTIFICADOR;
        while(contagem <= TAMANHO) {
            atomo.atributo_ID[contagem] = ' ';
            contagem++;
        }
        return atomo;

}

TInfoAtomo atomoInteiro() {
    TInfoAtomo atomo;
    atomo.atomo = ERRO;
    atomo.linha = globalLinha;
    char numero[TAMANHO];
    int contagem = 0;

    i0:
        if(isdigit(*buffer)) {
            numero[contagem] = *buffer;
            contagem++;
            buffer++;
            goto i1;
        } else {
            return atomo;
        }

    i1:
        if(isalpha(*buffer)) {
            buffer++;
            return atomo;
            
        } else if (isdigit(*buffer)) {
            numero[contagem] = *buffer;
            contagem++;
            atomo.atomo = NUMERO_INTEIRO;
            buffer++;
            goto i1;
        } else {
            atomo.atributo_numero = atoi(numero);
            goto i2;
        }
    
    i2:
        return atomo;

}

/* Bugs:

- Espaçamento
- Pular linha


- Colocar o while dentro do identificador
strcasecmp, converter tudo para maiúsculo e comparar

while( *buffer == '\n' || *buffer=='\r' || *buffer==' ' || *buffer == '\t')
*/