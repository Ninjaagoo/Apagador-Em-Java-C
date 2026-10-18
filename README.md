<img src="assets/java.svg" alt="Ícone de Java" width="48"> <img src="assets/c.svg" alt="Ícone de C" width="48">

# Apagador em Java e C

**Desenvolvido por: Ninjaagoo**

Projeto de estudos criado para praticar Java, C e a integração entre as duas linguagens. O menu é feito em Java e executa programas em C que realizam a limpeza no Windows.

## O que cada opção faz

| Opção | Ação |
| --- | --- |
| `1` | Esvazia a Lixeira da unidade `C:`. |
| `2` | Tenta excluir arquivos e subpastas de `%TEMP%` do usuário e de `C:\Windows\Temp`. Arquivos em uso ou sem acesso podem ser ignorados. |
| `3` | Executa as opções `1` e `2`. |
| `4` | Mostra informações sobre o projeto. |
| `0` | Encerra o programa. |

**Antes de usar:** esvaziar a Lixeira remove seu conteúdo. A limpeza dos temporários também exclui arquivos e pode solicitar permissão de administrador no Windows.

## Como iniciar

1. No Windows, instale um **JDK** e abra o PowerShell na pasta do projeto.
2. Crie a pasta para as classes Java, compile e inicie o menu:

```powershell
New-Item -ItemType Directory -Force -Path output/classes | Out-Null
javac -d output/classes src/java/*.java
java -cp output/classes src.java.ApagadorAutomatico
```

Os executáveis em C usados pelo menu já estão em `output/bin`. Se eles não estiverem disponíveis ou se você alterar o código C, veja os comandos de compilação em [comoIniciar.md](comoIniciar.md). Para compilá-los, é necessário ter o GCC instalado.
