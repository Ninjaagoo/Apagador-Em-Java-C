#include <stdio.h>
#include <windows.h>
#include <shellapi.h>

static SHQUERYRBINFO info;
static HRESULT consulta;
static HRESULT limpeza;

int main(){
    info.cbSize = sizeof(info);
    consulta = SHQueryRecycleBinA("c:\\", &info);

    if (SUCCEEDED(consulta)) {
        if (info.i64NumItems > 0) {
            limpeza = SHEmptyRecycleBinA(NULL, "C:\\",
                SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
            if (SUCCEEDED(limpeza)) {
                printf("Lixeira esvaziada.\n");
                printf("Itens excluidos da Lixeira: %llu\n",
                       (unsigned long long)info.i64NumItems);
            } else {
                printf("Nao foi possivel esvaziar a Lixeira.\n");
                printf("Itens excluidos da Lixeira: 0\n");
            }
        } else {
            printf("A Lixeira esta vazia.\n");
            printf("Itens excluidos da Lixeira: 0\n");
        }
    } else {
        printf("Nao foi possivel consultar a Lixeira.\n");
        printf("Itens excluidos da Lixeira: 0\n");
    }

    return 0;
}
