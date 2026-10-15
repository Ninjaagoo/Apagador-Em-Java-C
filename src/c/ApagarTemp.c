#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include "elevacao.h"

    static const char *pastaUsuario;
    static const char *pastaSistema;
    static const char *pastaOriginal;
    static int codigoElevacao;
    static unsigned long long arquivosExcluidos;

    int ApagarPastas(const char *pasta, unsigned long long *arquivosExcluidos) {
    char padrao[MAX_PATH];
    char caminhoArquivo[MAX_PATH];
    WIN32_FIND_DATAA item;
    HANDLE busca;
    DWORD erro;
    DWORD atributos;
    int tamanho;
    int sucesso;

    tamanho = snprintf(padrao, sizeof(padrao), "%s\\*", pasta);
    if (tamanho < 0 || tamanho >= MAX_PATH) {
        printf("Caminho muito longo: %s\n", pasta);
        return 0;
    }

    busca = FindFirstFileA(padrao, &item);

    if (busca == INVALID_HANDLE_VALUE) {
        erro = GetLastError();
        atributos = GetFileAttributesA(pasta);

        if (erro == ERROR_FILE_NOT_FOUND &&
            atributos != INVALID_FILE_ATTRIBUTES &&
            (atributos & FILE_ATTRIBUTE_DIRECTORY)) {
            return 1;
        }

        printf("Nao foi possivel acessar %s. Erro: %lu\n",
               pasta, (unsigned long)erro);
        return 0;
    }

    sucesso = 1;

    do {
        if (strcmp(item.cFileName, ".") == 0 ||
            strcmp(item.cFileName, "..") == 0) {
            continue;
        }

        tamanho = snprintf(caminhoArquivo, sizeof(caminhoArquivo),
                           "%s\\%s", pasta, item.cFileName);

        if (tamanho < 0 || tamanho >= MAX_PATH) {
            printf("Caminho muito longo: %s\n", item.cFileName);
            sucesso = 0;
            continue;
        }

        if (item.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Nao percorre atalhos ou juncoes do sistema de arquivos
            if (item.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                printf("Pasta especial ignorada: %s\n", caminhoArquivo);
                sucesso = 0;
                continue;
            }

            // Limpa a subpasta e depois tenta remove-la
            if (ApagarPastas(caminhoArquivo, arquivosExcluidos)) {
                if (RemoveDirectoryA(caminhoArquivo)) {
                    printf("Subpasta removida: %s\n", caminhoArquivo);
                } else {
                    erro = GetLastError();
                    printf("Falha ao remover %s. Erro: %lu\n",
                           caminhoArquivo, (unsigned long)erro);
                    sucesso = 0;
                }
            } else {
                sucesso = 0;
            }
        } else {
            // Apaga o arquivo
            if (DeleteFileA(caminhoArquivo)) {
                ++*arquivosExcluidos;
                printf("Apagado: %s\n", caminhoArquivo);
            } else {
                erro = GetLastError();

                if (erro == ERROR_SHARING_VIOLATION) {
                    printf("Em uso, ignorado: %s\n", caminhoArquivo);
                } else {
                    printf("Falha ao apagar %s. Erro: %lu\n",
                           caminhoArquivo, (unsigned long)erro);
                }

                sucesso = 0;
            }
        }
    } while (FindNextFileA(busca, &item));

    FindClose(busca);
    return sucesso;
}

void TEMP(unsigned long long *arquivosExcluidos, const char *pastaOriginal) {
    pastaUsuario = pastaOriginal != NULL ? pastaOriginal : getenv("TEMP");

    if (pastaUsuario == NULL || pastaUsuario[0] == '\0') {
        printf("Nao foi possivel encontrar a pasta TEMP do usuario.\n");
        return;
    }

    ApagarPastas(pastaUsuario, arquivosExcluidos);
}

void TEMP2(unsigned long long *arquivosExcluidos) {
    pastaSistema = "C:\\Windows\\Temp";
    ApagarPastas(pastaSistema, arquivosExcluidos);
}


int main(int argc, char **argv){
    codigoElevacao = solicitarElevacao(argc, argv);
    if (codigoElevacao >= 0) return codigoElevacao;
    arquivosExcluidos = 0;

    pastaOriginal = argc == 4 && strcmp(argv[1], "--elevated-log") == 0
        ? argv[3] : NULL;
    TEMP(&arquivosExcluidos, pastaOriginal);
    TEMP2(&arquivosExcluidos);
    printf("Arquivos excluidos: %llu\n", arquivosExcluidos);

    return 0;
}
