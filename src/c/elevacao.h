#ifndef ELEVACAO_H
#define ELEVACAO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <shellapi.h>

static HANDLE token;
static TOKEN_ELEVATION elevacao;
static DWORD tamanho;
static BOOL consultou;
static char pastaTemp[MAX_PATH];
static char arquivoLog[MAX_PATH];
static char executavel[MAX_PATH];
static char parametros[2 * MAX_PATH + 48];
static const char *tempUsuario;
static DWORD comprimento;
static DWORD comprimentoExe;
static SHELLEXECUTEINFOA inicio;
static DWORD codigo;
static FILE *log;
static char buffer[4096];
static size_t lidos;

static int solicitarElevacao(int argc, char **argv) {
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        printf("Nao foi possivel verificar as permissoes.\n");
        return 1;
    }
    consultou = GetTokenInformation(token, TokenElevation, &elevacao,
                                   sizeof(elevacao), &tamanho);
    CloseHandle(token);
    if (!consultou) {
        printf("Nao foi possivel verificar as permissoes.\n");
        return 1;
    }
    if (elevacao.TokenIsElevated) {
        if (argc == 4 && strcmp(argv[1], "--elevated-log") == 0 &&
            freopen(argv[2], "w", stdout) == NULL) {
            fprintf(stderr, "Nao foi possivel abrir o arquivo de resultados.\n");
            return 1;
        }
        return -1;
    }

    tempUsuario = getenv("TEMP");
    if (tempUsuario == NULL) tempUsuario = "";
    comprimento = GetTempPathA(sizeof(pastaTemp), pastaTemp);
    comprimentoExe = GetModuleFileNameA(NULL, executavel, sizeof(executavel));
    if (comprimento == 0 || comprimento >= sizeof(pastaTemp) ||
        GetTempFileNameA(pastaTemp, "apg", 0, arquivoLog) == 0 ||
        comprimentoExe == 0 || comprimentoExe >= sizeof(executavel) ||
        snprintf(parametros, sizeof(parametros), "--elevated-log \"%s\" \"%s\"",
                 arquivoLog, tempUsuario) >= (int)sizeof(parametros)) {
        printf("Nao foi possivel preparar a solicitacao de administrador.\n");
        return 1;
    }

    ZeroMemory(&inicio, sizeof(inicio));
    inicio.cbSize = sizeof(inicio);
    inicio.fMask = SEE_MASK_NOCLOSEPROCESS;
    inicio.lpVerb = "runas";
    inicio.lpFile = executavel;
    inicio.lpParameters = parametros;
    inicio.nShow = SW_HIDE;
    if (!ShellExecuteExA(&inicio)) {
        printf("Permissao de administrador nao concedida. Erro: %lu\n",
               (unsigned long)GetLastError());
        DeleteFileA(arquivoLog);
        return 1;
    }

    WaitForSingleObject(inicio.hProcess, INFINITE);
    codigo = 1;
    GetExitCodeProcess(inicio.hProcess, &codigo);
    CloseHandle(inicio.hProcess);

    log = fopen(arquivoLog, "r");
    if (log != NULL) {
        while ((lidos = fread(buffer, 1, sizeof(buffer), log)) > 0) {
            fwrite(buffer, 1, lidos, stdout);
        }
        fclose(log);
    }
    DeleteFileA(arquivoLog);
    return (int)codigo;
}

#endif
