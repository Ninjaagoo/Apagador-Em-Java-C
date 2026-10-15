package src.java;

import java.io.IOException;
import java.util.Scanner;

public class Menus {
    private static Scanner imput;
    private static int opcao;
    private static String entrada;

    private static void mostrarErro(String mensagem) {
        System.out.println();
        System.out.println("+---------------- ERRO ----------------+");
        System.out.println(mensagem);
        System.out.println("+--------------------------------------+");
    }

    private static void aguardarEnter() {
        System.out.print("Pressione Enter para prosseguir...");
        imput.nextLine();
    }

    static void MenuPrincipal() throws IOException, InterruptedException {
        imput = new Scanner(System.in);
        opcao = -1;

do{
    limparTelaCmd.limparTela();

    System.out.println("1. Limpar Lixeira");
    System.out.println("2. Limpar Arquivos Temp..");
    System.out.println("3. Executar ambos");
    System.out.println("4. Sobre");
    System.out.println("0. Sair");
    System.out.print("Digite sua opcao: ");
        entrada = imput.nextLine().trim();
        try {
            opcao = Integer.parseInt(entrada);
        } catch (NumberFormatException e) {
            mostrarErro("Opcao invalida. Digite um numero de 0 a 4.");
            aguardarEnter();
            continue;
        }

switch (opcao) {
    case 0:
    
    break;

    case 1:
        try {
        FuncoesExecutarExe.ExecutarLimparLixeira();

        }catch(Exception e){
                mostrarErro("Erro ao executar: " + e.getMessage());
        }
        break;

    case 2:
        try {
        FuncoesExecutarExe.ExecutarLimparTemp();

        }catch(Exception e){
                mostrarErro("Erro ao executar: " + e.getMessage());
        }
        break;
    case 3:
        try {
        FuncoesExecutarExe.ExecutarApagarTudo();

        }catch(Exception e){
                mostrarErro("Erro ao executar: " + e.getMessage());
        }    
    break;

    case 4:
    sobre.SobreSistema();
    break;

    default:
        mostrarErro("Opcao invalida. Digite um numero de 0 a 4.");
        break;
}
        if (opcao != 0) {
            aguardarEnter();
        }
    }while(opcao != 0);
    imput.close();
    }
}
