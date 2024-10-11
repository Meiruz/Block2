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

    static int finWithChecking(Scanner scannerFileInput, final int MAX_LIMIT, final int MIN_LIMIT, IntNum status) {
        int value = 0;

        try {
            value = scannerFileInput.nextInt();
        } catch (NumberFormatException e) {
            System.out.println(TErrors.FailData.text);
            status.value = 1;
        } catch (NoSuchElementException e) {
            System.out.println(TErrors.FailData.text);
            status.value = 1;
        }

        if ((status.value == 0) && (value > MAX_LIMIT || value < MIN_LIMIT)) {
            System.out.println(TErrors.FailLimitOfData.text);
            status.value = 1;
        }

        return value;
    }

    static int[][] inMatrixWithConsole(Scanner input, IntNum m, IntNum n) {
        System.out.println("Введите колчество строк M матрицы: ");
        m.value = cinWithChecking(input, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

        System.out.println("Введите колчество столбцов N матрицы: ");
        n.value = cinWithChecking(input, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

        int[][] matrix = new int[m.value][n.value];

        for (int i = 0; i < m.value; i++)
            for (int j = 0; j < n.value; j++) {
                System.out.printf("Введите элемент матрицы [%s, %s]:", i + 1, j + 1);
                matrix[i][j] = cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);
            }

        return matrix;
    }

    static String getFileName(Scanner sc, TFile fileType) {
        System.out.printf("\nВведите путь к файлу .txt для %s данных: ", (fileType == TFile.FileIn ? "ввода" : "вывода"));
        String pathToFile;

        pathToFile = sc.nextLine();

        if (checkFileNotTxt(pathToFile)) {
            System.out.println(TErrors.NotTXTFile.text);
            return getFileName(sc, fileType);
        }

        File file = new File(pathToFile);
        if (!file.exists())
            if (fileType == TFile.FileOut) {
                try {
                    file.createNewFile();
                }
                catch(IOException ex){
                    System.out.println(TErrors.FailFileOpen.text);
                    return getFileName(sc, fileType);
                }
            } else {
                System.out.println(TErrors.FailFileOpen.text);
                return getFileName(sc, fileType);
            }
        return file.getAbsolutePath();
    }

    static int[][] inMatrixWithFile(Scanner input, IntNum m, IntNum n, IntNum isFileError) {
        String pathToFile = getFileName(input, TFile.FileIn);

        Scanner scannerFileInput = null;
        try {
            scannerFileInput = new Scanner(new File(pathToFile));
        } catch (FileNotFoundException e) {
            isFileError.value = 1;
            System.out.println(TErrors.FailFileOpen.text);
        }

        m.value = finWithChecking(scannerFileInput, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE, isFileError);
        if (isFileError.value == 0)
            n.value = finWithChecking(scannerFileInput, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE, isFileError);
        int[][] matrix = new int[m.value][n.value];

        for (int i = 0; i < m.value; i++)
            for (int j = 0; j < n.value; j++) {
                if (isFileError.value == 0)
                    matrix[i][j] = finWithChecking(scannerFileInput, MAX_LIMIT, MIN_LIMIT, isFileError);
            }

        scannerFileInput.close();

        return matrix;
    }

    static int[][] inMatrixWithSize(Scanner input, IntNum m, IntNum n, int inType) {
        IntNum isFileError = new IntNum(0);
        int[][] matrix;

        if (inType == 1)
            matrix = inMatrixWithConsole(input, m, n);
        else
            do {
                matrix = inMatrixWithFile(input, m, n, isFileError);
            } while (isFileError.value == 1);

        return matrix;
    }

    static void outToFile(String pathToFile, String data) {
        try (FileWriter writer = new FileWriter(pathToFile, false)) {
            writer.append(data);
            writer.flush();
        } catch (IOException ex) {
            System.out.println(TErrors.FailFileCreateOrOpen.text);
        }
    }

    static void findSeddlePoints(Scanner input, IntNum m, IntNum n, int[][] matrix, int outType) {
        int[] minRows = new int[m.value];
        int[] maxColumns = new int[n.value];

        for (int i = 0; i < m.value; i++)
            minRows[i] = 101;
        for (int i = 0; i < n.value; i++)
            maxColumns[i] = 0;

        for (int i = 0; i < m.value; i++)
            for (int j = 0; j < n.value; j++) {
                if (matrix[i][j] < minRows[i])
                    minRows[i] = matrix[i][j];
                if (matrix[i][j] > maxColumns[j])
                    maxColumns[j] = matrix[i][j];
            }

        String pathToFile = getFileName(input, TFile.FileOut);

        int countOfAnswers = 0;
        System.out.println();
        for (int i = 0; i < m.value; i++)
            for (int j = 0; j < n.value; j++)
                if (minRows[i] == maxColumns[j]) {
                    countOfAnswers++;
                    System.out.println(minRows[i] + " - [" + (i + 1) + ',' + (j + 1) + ']');
                    if (outType == 2)
                        outToFile(pathToFile, (minRows[i] + " - [" + (i + 1) + ',' + (j + 1) + ']'));
                }

        if (countOfAnswers == 0) {
            System.out.println("В данный матрице нет седловых точек.");
            if (outType == 2)
                outToFile(pathToFile, "No saddle point in this matrix.");
        }

        System.out.println();
    }


    static void outMatrixToFile(Scanner input, int[][] matrix, IntNum n, IntNum m) {
        String pathToFile = getFileName(input, TFile.FileOut);
        try (FileWriter writer = new FileWriter(pathToFile, false)) {
            for (int i = 0; i < m.value; i++) {
                for (int j = 0; j < n.value; j++)
                    writer.append(matrix[i][j] + " ");
                writer.append('\n');
            }
            writer.flush();
        } catch (IOException ex) {
            System.out.println(TErrors.FailFileCreateOrOpen.text);
        }
    }

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        IntNum n = new IntNum(0);
        IntNum m = new IntNum(0);

        System.out.println("Программа для определения \"седловой\" точки матрицы.\n");

        System.out.printf("Все значения матрицы должны быть от %s до %s.\n", MIN_LIMIT, MAX_LIMIT);
        System.out.printf("Размеры матрицы MxN быть от %s до %s.\n\n", MIN_LIMIT_SIZE, MAX_LIMIT_SIZE);

        System.out.println("Введите предпочитаемый тип ввода данных: ");
        System.out.println("\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы).");
        int inType = cinWithChecking(input, 2, 1);

        int[][] matrix = inMatrixWithSize(input, m, n, inType);

        System.out.println("Введите предпочитаемый тип вывода данных: ");
        System.out.println("\t1 - только в консоли,\n\t2 - в консоль и в файл.");
        int outType = cinWithChecking(input, 2, 1);

        findSeddlePoints(input, m, n, matrix, outType);
        input.close();
    }

    static class IntNum {
        int value;

        IntNum(int value) {
            this.value = value;
        }
    }
}