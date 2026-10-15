# Como iniciar

No Windows, instale o JDK e abra o PowerShell na pasta do projeto. O GCC so e necessario se os executaveis C ainda nao existirem.

1. Compile os programas C apenas se os arquivos `.exe` nao existirem em `output/bin`:

```powershell
New-Item -ItemType Directory -Force -Path output/bin, output/classes | Out-Null
if (-not (Test-Path output/bin/ApagarLixeira.exe)) { gcc src/c/ApagarLixeira.c -o output/bin/ApagarLixeira.exe -lshell32 }
if (-not (Test-Path output/bin/ApagarTemp.exe)) { gcc src/c/ApagarTemp.c -o output/bin/ApagarTemp.exe -lshell32 }
if (-not (Test-Path output/bin/ApagarTudo.exe)) { gcc src/c/ApagarTudo.c -o output/bin/ApagarTudo.exe -lshell32 }
```

Se alterar um arquivo C, compile o `.exe` correspondente novamente.

2. Compile o Java:

```powershell
javac -d output/classes src/java/*.java
```

3. Inicie o programa:

```powershell
java -cp output/classes src.java.ApagadorAutomatico
```