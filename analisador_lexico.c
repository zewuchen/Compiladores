#include <stdio.h>
#include <stdlib.h>

char *buffer;

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
    // rotina(fp_saida); // Implementar os requisitos para o analisador léxico

    free(iniBuffer);
    fclose(fp_entrada);
    fclose(fp_saida);
    
    return 0;
}