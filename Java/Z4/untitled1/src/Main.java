import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Main {
    enum TErrors {
        FailData("Некорректные данные данные. Попробуйте еще раз."),
        FailLimitOfData("Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз."),
        FailFileOpen("Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла."),
        FailFileCreateOrOpen("Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла."),
        NotTXTFile("Расширение файла должно быть TXT. Попробуйте еще раз.");

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

    static boolean checkFileNotTxt(String pathToFile) {
        String newStr = pathToFile.toLowerCase();
        int stringLength = newStr.length();

        if (stringLength < 4)
            return true;
        if ((newStr.charAt(stringLength - 1) == 't') &&
                (newStr.charAt(stringLength - 2) == 'x') &&
                (newStr.charAt(stringLength - 3) == 't') &&
                (newStr.charAt(stringLength - 4) == '.'))
            return false;
        return true;
    }

    static int cinWithChecking(Scanner input, final int MAX_LIMIT, final int MIN_LIMIT) {
        int value = 0;
        boolean isFail;

        do {
            isFail = false;
            try {
                value = Integer.parseInt(input.nextLine());
            } catch (NumberFormatException e) {
                System.out.println(TErrors.FailData.text);
                isFail = true;
            }

            if (value > MAX_LIMIT || value < MIN_LIMIT) {
                System.out.println(TErrors.FailLimitOfData.text);
                isFail = true;
            }
            return value;
        } while (isFail);
    }

    static int finWithChecking(Scanner scannerFileInput, final int MAX_LIMIT, final int MIN_LIMIT, int[] status) {
        int value = 0;

        try {
            value = scannerFileInput.nextInt();
        } catch (NumberFormatException e) {
            System.out.println(TErrors.FailData.text);
            status[0] = 1;
        } catch (NoSuchElementException e) {
            System.out.println(TErrors.FailData.text);
            status[0] = 1;
        }

        if ((status[0] == 0) && (value > MAX_LIMIT || value < MIN_LIMIT)) {
            System.out.println(TErrors.FailLimitOfData.text);
            status[0] = 1;
        }

        return value;
    }

    static int[][] inMatrixWithConsole(Scanner input, int[] m, int[] n) {
        int i, j;

        System.out.println("Введите размерность 2M квадратной матрицы: ");
        m[0] = cinWithChecking(input, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

        m[0] *= 2;
        n[0] = m[0];

        int[][] matrix = new int[m[0]][n[0]];

        for (i = 0; i < m[0]; i++)
            for (j = 0; j < n[0]; j++) {
                System.out.printf("Введите элемент матрицы [%s, %s]:", i + 1, j + 1);
                matrix[i][j] = cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);
            }

        return matrix;
    }

    static String getFileName(Scanner sc, TFile fileType) {
        String pathAns = "";
        boolean isFail;

        do {
            isFail = false;

            System.out.printf("\nВведите путь к файлу .txt для %s данных: ", (fileType == TFile.FileIn ? "ввода" : "вывода"));
            String pathToFile;

            pathToFile = sc.nextLine();

            if (checkFileNotTxt(pathToFile)) {
                System.out.println(TErrors.NotTXTFile.text);
                isFail = true;
            }

            File file = new File(pathToFile);
            if (!file.exists())
                if (fileType == TFile.FileOut) {
                    try {
                        file.createNewFile();
                    } catch (IOException ex) {
                        System.out.println(TErrors.FailFileOpen.text);
                        isFail = true;
                    }
                } else {
                    System.out.println(TErrors.FailFileOpen.text);
                    isFail = true;
                }
            pathAns = file.getAbsolutePath();;
        } while (isFail);
        return pathAns;
    }

    static int[][] inMatrixWithFile(Scanner input, int[] m, int[] n, int[] isFileError) {
        int i, j;
        String pathToFile = getFileName(input, TFile.FileIn);

        Scanner scannerFileInput = null;
        try {
            scannerFileInput = new Scanner(new File(pathToFile));
        } catch (FileNotFoundException e) {
            isFileError[0] = 1;
            System.out.println(TErrors.FailFileOpen.text);
        }

        m[0] = finWithChecking(scannerFileInput, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE, isFileError);
        m[0] *= 2;
        n[0] = m[0];

        int[][] matrix = new int[m[0]][n[0]];

        for (i = 0; i < m[0]; i++)
            for (j = 0; j < n[0]; j++) {
                if (isFileError[0] == 0)
                    matrix[i][j] = finWithChecking(scannerFileInput, MAX_LIMIT, MIN_LIMIT, isFileError);
            }

        scannerFileInput.close();

        return matrix;
    }

    static int[][] inMatrixWithSize(Scanner input, int[] m, int[] n, int inType) {
        int i, j;
        int[] isFileError = {0};
        int[][] matrix;

        if (inType == 1)
            matrix = inMatrixWithConsole(input, m, n);
        else
            do {
                matrix = inMatrixWithFile(input, m, n, isFileError);
            } while (isFileError[0] == 1);

        return matrix;
    }

    static void outMatrixToConsole(int[][] matrix, int[] n, int[] m) {
        int i, j;
        System.out.println();
        for (i = 0; i < m[0]; i++) {
            for (j = 0; j < n[0]; j++)
                System.out.print(matrix[i][j] + " ");
            System.out.println();
        }
    }

    static void outMatrixToFile(Scanner input, int[][] matrix, int[] n, int[] m) {
        int i, j;
        String pathToFile = getFileName(input, TFile.FileOut);
        try (FileWriter writer = new FileWriter(pathToFile, false)) {
            for (i = 0; i < m[0]; i++) {
                for (j = 0; j < n[0]; j++)
                    writer.append(matrix[i][j] + " ");
                writer.append('\n');
            }
            writer.flush();
        } catch (IOException ex) {
            System.out.println(TErrors.FailFileCreateOrOpen.text);
        }
    }

    static void changeMatrix(int[][] matrix, int[] m, int[] n) {
        int i, j;
        int[][] newMatrix = new int[m[0]][n[0]];

        for (i = 0; i < m[0]; i++)
            for (j = 0; j < n[0]; j++)
                if (i < n[0] / 2)
                    if (j < n[0] / 2)
                        newMatrix[i + n[0] / 2][j + n[0] / 2] = matrix[i][j];
                    else
                        newMatrix[i + n[0] / 2][j - n[0] / 2] = matrix[i][j];
                else
                if (j < n[0] / 2)
                    newMatrix[i - n[0] / 2][j] = matrix[i][j];
                else
                    newMatrix[i - n[0] / 2][j] = matrix[i][j];

        for (i = 0; i < m[0]; i++)
            for (j = 0; j < n[0]; j++)
                matrix[i][j] = newMatrix[i][j];
    }

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int[] n = {0};
        int[] m = {0};

        System.out.println("Программа для изменения положения подматриц квадратной матрицы порядка 2n.\n");

        System.out.printf("Все значения матрицы должны быть от %s до %s.\n", MIN_LIMIT, MAX_LIMIT);
        System.out.printf("Размеры квадратной матрицы 2M быть от %s.\n\n", MIN_LIMIT_SIZE, MAX_LIMIT_SIZE);

        System.out.println("Введите предпочитаемый тип ввода данных: ");
        System.out.println("\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы).");
        int inType = cinWithChecking(input, 2, 1);

        int[][] matrix = inMatrixWithSize(input, m, n, inType);

        System.out.println("Введите предпочитаемый тип вывода данных: ");
        System.out.println("\t1 - только в консоли,\n\t2 - в консоль и в файл.");
        int outType = cinWithChecking(input, 2, 1);

        outMatrixToConsole(matrix, m, n);

        changeMatrix(matrix, n, m);

        if (outType == 2)
            outMatrixToFile(input, matrix, n, m);
        outMatrixToConsole(matrix, m, n);

        input.close();
    }
}
