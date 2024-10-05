import java.io.*;
import java.util.Scanner;

public class Main {
    enum TErrors {
        FailFileOpen ,
        FailFileCreateOrOpen,
        FailData,
        FailLimitOfData
    };

    static final String[] ERRORS = {
        "Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
        "Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.",
        "Некорректные данные данные. Попробуйте еще раз.",
        "Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз."
    };

    static int inWithChecking(final int MAX_LIMIT, final int MIN_LIMIT) {
        Scanner in = new Scanner(System.in);
        boolean isFail = false;
        int value;

        try {
            value = Integer.parseInt(in.nextLine());
            isFail = true;
        } catch (NumberFormatException e) {
            System.out.println(ERRORS[TErrors.FailData.ordinal()]);
            in.close();
            return inWithChecking(MAX_LIMIT, MIN_LIMIT);
        }

        if (!isFail && value > MAX_LIMIT || value < MIN_LIMIT) {
            System.out.println(ERRORS[TErrors.FailLimitOfData.ordinal()]);
            in.close();
            return inWithChecking(MAX_LIMIT, MIN_LIMIT);
        }

        in.close();
        return value;
    }

    static int openFileAndInput(int[][] matrix, int MAX_LIMIT, int MIN_LIMIT) throws IOException {
        Scanner in = new Scanner(System.in);
        String filename;

        System.out.print("\nВведите путь к файлу .txt для ввода данных: ");
        filename = in.nextLine();

        BufferedReader br = new BufferedReader(new FileReader(filename));
        System.out.println(br.readLine());

        int n = 0;
//        if (!file.is_open()) {
//            cout << Errors[FailFileOpen];
//            file.close();
//            openFileAndInput(matrix, MAX_LIMIT, MIN_LIMIT);
//        }
//
//        file.nextInt();
//
//        if (file.fail() || n > MAX_LIMIT || n < MIN_LIMIT) {
//            cout << Errors[FailData];
//            file.close();
//            openFileAndInput(matrix, MAX_LIMIT, MIN_LIMIT);
//        }
//
//        n *= 2;
//
//        matrix = new int* [n];
//
//        bool isLimitError = false;
//        for (int i = 0; i < n && !isLimitError; i++) {
//            matrix[i] = new int[n];
//            for (int j = 0; j < n && !isLimitError; j++) {
//                file >> matrix[i][j];
//                if (matrix[i][j] > MAX_LIMIT || matrix[i][j] < MIN_LIMIT)
//                    isLimitError = true;
//            }
//        }
//
//        if (isLimitError || file.fail()) {
//            cout << Errors[FailData];
//            file.close();
//            openFileAndInput(file, n, matrix, MAX_LIMIT, MIN_LIMIT);
//        }

        br.close();
        in.close();
        return n;
    }

//    void openOutFile(fstream& file) {
//        string filename;
//
//        cout << "\nВведите путь к файлу .txt для вывода данных:";
//        cin >> filename;
//
//        file.open(filename, ios::out);
//        if (!file.is_open()) {
//            cout << Errors[FailFileOpen];
//            file.close();
//            openOutFile(file);
//        }
//    }
//
//    void changeMatrix(int **&matrix, int n) {
//        int** newMatrix = new int*[n];
//        for (int i = 0; i < n; i++)
//            newMatrix[i] = new int[n] {0};
//
//        for (int i = 0; i < n; i++)
//            for (int j = 0; j < n; j++)
//                if (i < n / 2)
//                    if (j < n / 2)
//                        newMatrix[i + n / 2][j + n / 2] = matrix[i][j];
//                    else
//                        newMatrix[i + n / 2][j - n / 2] = matrix[i][j];
//                else
//                if (j < n / 2)
//                    newMatrix[i - n / 2][j] = matrix[i][j];
//                else
//                    newMatrix[i - n / 2][j] = matrix[i][j];
//
//        for (int i = 0; i < n; i++)
//            for (int j = 0; j < n; j++)
//                matrix[i][j] = newMatrix[i][j];
//
//        for (int i = 0; i < n; i++)
//            delete[] newMatrix[i];
//        delete[] newMatrix;
//    }


    static void exitProgram() {
        Scanner in = new Scanner(System.in);

        System.out.print("Для выхода из программы нажмите Enter...");
        in.nextLine();

        in.close();
    }

    public static void main(String[] args) throws IOException {
        System.out.println("Программа для изменения положения подматриц квадратной матрицы порядка 2n.");

        final int MIN_LIMIT = 0;
        final int MAX_LIMIT = 100;

        System.out.printf("Все значения матрицы должны быть от %s до %s.\n", -MAX_LIMIT, MAX_LIMIT);
        System.out.printf("Порядок матрицы n должен быть от %s до %s.\n", MIN_LIMIT , MAX_LIMIT);

        int inType = 0;
        int n = 0;
        int[][] matrix = new int[0][0];

        System.out.println("Введите предпочетаемый тип ввода данных: ");
        System.out.println("\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка n, дальше элементы в виде таблицы).");
        inType = inWithChecking(2, 1);

        if (inType == 1) {
            System.out.println("Введите порядок квадратной матрицы 2n.");
            n = inWithChecking(MAX_LIMIT, MIN_LIMIT);
            n *= 2;

            matrix = new int[n][n];

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    System.out.printf("Введите элемент матрицы [%s,'%s]: ", i + 1, j + 1);
                    matrix[i][j] = inWithChecking(MAX_LIMIT, -MAX_LIMIT);
                }
        }
        else {
            n = openFileAndInput(matrix, MAX_LIMIT, -MAX_LIMIT);
        }

        int outType;
        System.out.println("Введите предпочетаемый тип вывода данных: ");
        System.out.println("\t1 - только в консоль,\n\t2 - в консоль и в файл.");
        outType = inWithChecking(2, 1);
//
//        changeMatrix(matrix, n);
//
//        if (outType == 2)
//            openOutFile(fout);
//
//        System.out.println();
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                System.out.printf("%s ", matrix[i][j]);
//                if (outType == 2)
//                    fout << matrix[i][j] << ' ';
//            }
//
//            System.out.println();
//            if (outType == 2)
//                fout << endl;
//        }
//        System.out.println();

        exitProgram();
    }
}