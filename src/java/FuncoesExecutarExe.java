package src.java;
import java.nio.file.*;

public class FuncoesExecutarExe {
    private static Path local;
    private static Path pastaClasses;
    private static Path exe;
    private static int codigo;

    private static void executar(String nomeExe) throws Exception {
        local = Paths.get(
            ApagadorAutomatico.class.getProtectionDomain()
                .getCodeSource().getLocation().toURI());
        pastaClasses = Files.isDirectory(local) ? local : local.getParent();
        exe = pastaClasses.getParent().resolve("bin").resolve(nomeExe);

        codigo = new ProcessBuilder(exe.toString())
            .inheritIO()
            .start()
            .waitFor();
        if (codigo != 0) {
            throw new IllegalStateException(nomeExe + " terminou com codigo " + codigo);
        }
    }

    static void ExecutarLimparLixeira() throws Exception {
        executar("ApagarLixeira.exe");
    }

    static void ExecutarLimparTemp() throws Exception {
        executar("ApagarTemp.exe");
    }

    static void ExecutarApagarTudo() throws Exception {
        executar("ApagarTudo.exe");
    }
}
