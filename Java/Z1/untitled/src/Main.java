import java.util.Scanner;

public class Main {
    public enum TErrors {
        FailData("Некорректные данные. Попробуйте еще раз."),
        FailLimitOfData("Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз."),
        FigureError("Фигура заданными координатами не образует многоугольника");

        public final String text;

        private TErrors(String text) {
            this.text = text;
        }
    };

    public static void main(String[] args) {
        final int
            MAX_LIMIT_COUNT = 1000,
            MIN_LIMIT_COUNT = 3,
            MAX_LIMIT = 1000,
            MIN_LIMIT = -1000;

        System.out.println("Программа для определения, является ли многоугольник выпуклым или нет?");

        int countOfPoints = 0;
        int i;
        boolean isFail;
        Scanner sc = new Scanner(System.in);

        do {
            isFail = false;

            System.out.printf("\nВведите количество вершин многоугольника от %s до %s: ", MIN_LIMIT_COUNT, MAX_LIMIT_COUNT);
            try {
                countOfPoints = Integer.parseInt(sc.nextLine());
            } catch (Exception e) {
                System.out.println(TErrors.FailData.text);
                isFail = true;
            }

            if (!isFail)
                if (countOfPoints < MIN_LIMIT_COUNT || countOfPoints > MAX_LIMIT_COUNT) {
                    System.out.println(TErrors.FailLimitOfData.text);
                    isFail = true;
                }
        } while (isFail);


        int[] pointX = new int[countOfPoints];
        int[] pointY = new int[countOfPoints];

        System.out.println("\nВведите точки вершин многоугольника последовательно.");
        for (i = 0; i < countOfPoints; i++) {
            do {
                isFail = false;

                System.out.printf("Введите координаты вершины #%s (x, y) от %s до %s: ", i+1, MIN_LIMIT, MAX_LIMIT);
                try {
                    String values = sc.nextLine();
                    String[] valuesArray = values.split(" ");

                    pointX[i] = Integer.parseInt(valuesArray[0]);
                    pointY[i] = Integer.parseInt(valuesArray[1]);
                } catch (Exception e) {
                    System.out.println(TErrors.FailData.text);
                    isFail = true;
                }

                if (!isFail)
                    if (pointX[i] < MIN_LIMIT || pointX[i] > MAX_LIMIT ||
                            pointY[i] < MIN_LIMIT || pointY[i] > MAX_LIMIT) {
                        System.out.println(TErrors.FailLimitOfData.text);
                        isFail = true;
                    }
            } while (isFail);

            pointX[i] += MAX_LIMIT;
            pointY[i] += MAX_LIMIT;
        }

        sc.close();

        int sign = -2;
        boolean isEndOfFor = false;

        for (i = 0; i < countOfPoints; i++) {
            int secondIndex = (i + 1) % countOfPoints;
            int thirdIndex = (i + 2) % countOfPoints;

            int firstLineX = pointX[secondIndex] - pointX[i];
            int firstLineY = pointY[secondIndex] - pointY[i];
            int secondLineX = pointX[thirdIndex] - pointX[secondIndex];
            int secondLineY = pointY[thirdIndex] - pointY[secondIndex];

            int vectorSum = (firstLineX * secondLineY - secondLineX * firstLineY);

            int signOfVectorSum;
            if (vectorSum != 0)
                signOfVectorSum = vectorSum / (vectorSum < 0 ? -vectorSum : vectorSum);
            else
                signOfVectorSum = 0;

            if (signOfVectorSum != 0)
                if (sign == -2)
                    sign = signOfVectorSum;
                else if (sign != signOfVectorSum)
                    isEndOfFor = true;
        }

        if (sign == -2)
            System.out.println(TErrors.FigureError.text);
        else
            System.out.printf("Многоугольник %sявляется выпуклым.\n", (isEndOfFor ? "не " : ""));
    }
}










