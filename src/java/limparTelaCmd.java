package src.java;
import java.io.IOException;

public class limparTelaCmd {


    static void limparTela() throws IOException, InterruptedException {
    new ProcessBuilder("cmd", "/c", "cls")
    .inheritIO()
    .start()
    .waitFor();
    }

    
}
