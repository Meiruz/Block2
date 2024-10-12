import java.util.Scanner;
import static java.lang.Math.floor;

public class Main {
    enum TErrors {
        FailData("Некорректные данные данные. Попробуйте еще раз."),
        FailLimitOfData("Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.");

        private String text;
        private TErrors(String text) {
            this.text = text;
        }
    };

    static void splitNumToArray(int num, int[] arr, int numLength) {
        while (num > 0) {
            arr[--numLength] = num % 10;
            num = num / 10;
        }
    }

    static int cinWithChecking(Scanner sc, final int MAX_LIMIT, final int MIN_LIMIT) {
        int value = 0;
        boolean isFail;

        do {
            isFail = false;

            try {
                value = Integer.parseInt(sc.nextLine());
            } catch (NumberFormatException e) {
                System.out.println(TErrors.FailData.text);
                isFail = true;
            }

            if (value > MAX_LIMIT || value < MIN_LIMIT) {
                System.out.println(TErrors.FailLimitOfData.text);
                isFail = true;
            }
        } while (isFail);

        return value;
    }

    static void outElement(char element, int count, boolean isStartEndl, boolean isEndEndl) {
        int i;
        
        if (isStartEndl)
            System.out.print("\n");

        for (i = 0; i < count; i++)
            System.out.print(element);

        if (isEndEndl)
            System.out.print("\n");
    }

    static int getNumLength(int num) {
        if (num == 0)
            return 1;

        int length = 0;
        while (num > 0) {
            length++;
            num = num / 10;
        }
        return length;
    }

    static void divisionOfElements(int[] arrFirstNum, int firstNum, int secondNum, int firstNumLength) {
        System.out.printf("\n%s | %s\n", firstNum, secondNum);

        int pos = 0;
        int underlinedNum = 0;
        int addUnderlinedRange = 1;
        int underlinedNumLength = 0;
        int checkPos = 0;
        boolean isFirst = true;

        while (checkPos < firstNumLength) {
            // Get underlineNum
            while (checkPos < firstNumLength && underlinedNum < secondNum) {
                underlinedNum = underlinedNum * 10 + arrFirstNum[checkPos];
                addUnderlinedRange++;
                checkPos++;
            }
            addUnderlinedRange--;

            // Minus round
            if (!isFirst) {
                outElement(' ', pos, false, false);
                outElement('-', addUnderlinedRange, false, true);
                outElement(' ', pos, false, false);
                System.out.println(underlinedNum);
            }

            int divisionAns = underlinedNum / secondNum;
            int roundDivision = divisionAns * secondNum;
            int divisionPos = pos + (addUnderlinedRange - getNumLength(roundDivision));

            outElement(' ', divisionPos, false, false);
            System.out.print(roundDivision);

            if (isFirst) {
                outElement(' ', firstNumLength - addUnderlinedRange, false, false);
                System.out.printf(" | %s\n", (firstNum / secondNum));
            }
            else
                System.out.println();

            underlinedNum -= roundDivision;

            int newUnderlinedRange = getNumLength(underlinedNum);
            pos += addUnderlinedRange - newUnderlinedRange;
            addUnderlinedRange = newUnderlinedRange + 1;

            if (underlinedNum == 0) {
                pos++;
                addUnderlinedRange--;
            }

            checkPos = pos + addUnderlinedRange - 1;

            isFirst = false;
        }

        outElement(' ', pos, false, false);
        outElement('-', addUnderlinedRange - 1, false, true);
        outElement(' ', pos, false, false);

        System.out.println(underlinedNum + "\n");
    }

    public static void main(String[] args) {
        final int MIN_LIMIT = 1;
        final int MAX_LIMIT = 200000000;

        System.out.println("Программа для деления чисел в столбик.\n");

        boolean isFail = true;
        Scanner input = new Scanner(System.in);

        System.out.printf("Введите значение делимого от %s до %s: ", MIN_LIMIT, MAX_LIMIT);
        int firstNum = cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);

        System.out.printf("Введите значение делителя от %s до %s: ", MIN_LIMIT, MAX_LIMIT);
        int secondNum = cinWithChecking(input, MAX_LIMIT, MIN_LIMIT);
        input.close();

        int firstNumLength = getNumLength(firstNum);

        int[] arrNum = new int[firstNumLength];
        splitNumToArray(firstNum, arrNum, firstNumLength);

        divisionOfElements(arrNum, firstNum, secondNum, firstNumLength);
    }
}