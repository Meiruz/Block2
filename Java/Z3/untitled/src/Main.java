import java.util.Scanner;

public class Main {
    enum TErrors {
        FailData("Некорректные данные данные. Попробуйте еще раз."),
        FailLimitOfData("Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз."),
        FailFileOpen("Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла."),
        FailFileCreateOrOpen("Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.");

        private String text;
        private TErrors(String text) {
            this.text = text;
        }
    };
    enum TFile {
        FileIn,
        FileOut
    };

    static final int MIN_LIMIT_SIZE = 0;
    static final int MAX_LIMIT_SIZE = 100;
    static final int MIN_LIMIT = -100;
    static final int MAX_LIMIT = 100;

    static int cinWithChecking(Scanner input, final int MAX_LIMIT, final int MIN_LIMIT) {
        int value;
        try {
            value = Integer.parseInt(input.nextLine());
        } catch (NumberFormatException e) {
            System.out.println(TErrors.FailData.text);
            return cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);
        }

        if (value > MAX_LIMIT || value < MIN_LIMIT) {
            System.out.println(TErrors.FailLimitOfData.text);
            return cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);
        }
        return value;
    }

    static void inDataWithConsole(Scanner input, IntNum m, IntNum n, int[][] matrix) {
        System.out.println("Введите колчество строк M матрицы: ");
        m.value = cinWithChecking(input, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

        System.out.println("Введите колчество столбцов N матрицы: ");
        n.value = cinWithChecking(input, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

        matrix = new int[n.value][m.value];

        for (int i = 0; i < m.value; i++)
            for (int j = 0; j < n.value; j++) {
                System.out.printf("Введите элемент матрицы [%s, %s]:", i + 1, j + 1);
                matrix[i][j] = cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);
            }
    }

    static void inDataWithFile(IntNum n, IntNum m, int[][] matrix) {
        
    }

    static void inData(Scanner input, IntNum m, IntNum n, int[][] matrix, int inType) {
        boolean isDataOk;

        if (inType == 1)
            inDataWithConsole(input, m, n, matrix);
        else {
            do {
                isDataOk = inDataWithFile(m, n, matrix);
            } while (isDataOk);
        }
    }


    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        IntNum n = new IntNum(0);
        IntNum m = new IntNum(0);
        int[][] matrix = new int[][]{new int[0]};

        System.out.println("Программа для определения \"седловой\" точки матрицы.\n");

        System.out.printf("Все значения матрицы должны быть от %s до %s.\n", MIN_LIMIT, MAX_LIMIT);
        System.out.printf("Размеры матрицы MxN быть от %s до %s.\n\n", MIN_LIMIT_SIZE, MAX_LIMIT_SIZE);

        System.out.println("Введите предпочитаемый тип ввода данных: ");
        System.out.println("\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы).");
        int inType = cinWithChecking(input, 2, 1);

        inData(input, n, m, matrix, inType);

        System.out.println("Введите предпочитаемый тип ввода данных: ");
        System.out.println("\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы).");
        int outType = cinWithChecking(input, 2, 1);

        // points

        // out

        input.close();
    }

    static class IntNum {
        int value;

        IntNum(int value) {
            this.value = value;
        }
    }
}