#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#define TAMANHO 15 // Tamanho máximo das palavras

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

    // Inicia pegando o primeiro átomo e vai buscando todos os átomos do arquivo
    while(1) {
        TInfoAtomo atomo = obter_atomo();
        if(atomo.atomo == ERRO) {
            printf("\nLinha: %d - ERRO", atomo.linha);
            break;
        } else if(atomo.atomo == IDENTIFICADOR) {
            printf("\nLinha: %d - Identificador - %s", atomo.linha, atomo.atributo_ID);
        } else if(atomo.atomo == NUMERO_INTEIRO) {
            printf("\nLinha: %d - Número Inteiro - %d", atomo.linha, atomo.atributo_numero);
        } else if(atomo.atomo == ATRIBUICAO) {
            printf("\nLinha: %d - Operador Atribuição", atomo.linha);
        } else if(atomo.atomo == WHILE) {
            printf("\nLinha: %d - While", atomo.linha);
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

    // Contagem de linhas
    while(*buffer == '\n' || *buffer == '\r') {
        globalLinha++;
        buffer++;
    }
    // Contagem de espaços
    while(*buffer == ' ' || *buffer == '\t') {
        buffer++;
    }

    // Verificar as definições regulares
    if(*buffer == ':') {
        atomo = atomoAtribuicao();
    } else if(*buffer == 0) {
        atomo.atomo = EOS;
    } else if(isalpha(*buffer)) {
        atomo = atomoIdentificador();
    } else if(isdigit(*buffer)) {
        atomo = atomoInteiro();
    } else {
        atomo.atomo = ERRO;
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
            buffer++;
            atomo.atomo = ATRIBUICAO;
        }
    }

    return atomo;
}

TInfoAtomo atomoIdentificador() {
    TInfoAtomo atomo;
    atomo.atomo = ERRO;
    atomo.linha = globalLinha;
    int contagem = 0;

    for(int i = 0; i < TAMANHO; i++) {
        atomo.atributo_ID[i] = "";
    }

    id0:
        if(isalpha(*buffer)) { // Verifica se começa com uma letra
            atomo.atributo_ID[contagem] = *buffer;
            buffer++;
            contagem++;
            goto id1;
        } else {
            return atomo;
        }

    id1:
        if(isalpha(*buffer) || isdigit(*buffer)) { // Pode vir letra ou dígito depois da primeira letra
            atomo.atributo_ID[contagem] = *buffer;
            buffer++;
            contagem++;
            
            goto id1; // Permite mais letra ou números no identificador
        } else {
            goto id2; // Vai pro estado final
        }
    
    id2:
        atomo.atomo = IDENTIFICADOR; // Seta sempre como identificador válido

        // Verifica se é um While
        if(contagem == 5) {
            int result = strcasecmp("while", atomo.atributo_ID);

            if(result == 0) {
                atomo.atomo = WHILE;
            }
        }
        
        return atomo;

}

TInfoAtomo atomoInteiro() {
    TInfoAtomo atomo;
    atomo.atomo = ERRO;
    atomo.linha = globalLinha;
    char numero[TAMANHO];
    int contagem = 0;

    int0:
        if(isdigit(*buffer)) {
            numero[contagem] = *buffer;
            contagem++;
            buffer++;

            if(*buffer == 0) { // Caso de ser apenas um número simples
                goto int2;
            } else {    // Caso de ser um número com dois dígitos ou mais
                goto int1;
            }
        } else {
            return atomo; // Não é um número inteiro, dá erro
        }

    int1:
        if(isalpha(*buffer)) {
            buffer++;
            return atomo; // Veio uma letra, vai dar erro

        } else if (isdigit(*buffer)) { // Veio um dígito e o consome
            numero[contagem] = *buffer;
            contagem++;
            buffer++;
            goto int1;
        } else {
            goto int2;
        }
    
    int2: // Estado final que aceita o número inteiro, converte e retorna
        atomo.atomo = NUMERO_INTEIRO;
        atomo.atributo_numero = atoi(numero);
        return atomo;
}

/*
BUGS: Limpar cache do identificador"
*/