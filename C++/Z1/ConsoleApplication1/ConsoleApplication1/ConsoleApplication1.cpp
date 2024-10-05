#include <iostream>
using namespace std;

enum TErrors {
	FailData,
	FailLimitOfData,
	FigureError
};
const string ERRORS[4]{
	"Некорректные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.",
	"Фигура заданными координатами не образует многоугольника"
};

void cinBufClean() {
	cin.clear();
	while (cin.get() != '\n');
}

int absNum(int a) {
	if (a < 0)
		return -a;
	if (a == 0)
		return 1;
	return a;
}

void cinWithChecking(int &value, const int &MAX_LIMIT, const int &MIN_LIMIT) {
	cin >> value;

	if (cin.fail() || cin.get() != '\n') {
		cout << ERRORS[FailData] << endl;
		cinBufClean();
		cinWithChecking(value, MAX_LIMIT, MIN_LIMIT);
	}
	else if (value > MAX_LIMIT || value < MIN_LIMIT) {
		cout << ERRORS[FailLimitOfData] << endl;
		cinWithChecking(value, MAX_LIMIT, MIN_LIMIT);
	}

}

void cinTwiceWithChecking(int& value1, int &value2, const int& MAX_LIMIT, const int& MIN_LIMIT) {
	cin >> value1 >> value2;

	if (cin.fail() || cin.get() != '\n') {
		cout << ERRORS[FailData] << endl;
		cinBufClean();
		cinTwiceWithChecking(value1, value2, MAX_LIMIT, MIN_LIMIT);
	}
	else if (value1 > MAX_LIMIT || value1 < MIN_LIMIT || value2 > MAX_LIMIT || value2 < MIN_LIMIT) {
		cout << ERRORS[FailLimitOfData] << endl;
		cinTwiceWithChecking(value1, value2, MAX_LIMIT, MIN_LIMIT);
	}

}

int checkPoints(int firstIndex, int *&x, int *&y, int countOfElements) {
	int secondIndex = (firstIndex + 1) % countOfElements;
	int thirdIndex = (firstIndex + 2) % countOfElements;

	int firstLineX = x[secondIndex] - x[firstIndex];
	int firstLineY = y[secondIndex] - y[firstIndex];
	int secondLineX = x[thirdIndex] - x[secondIndex];
	int secondLineY = y[thirdIndex] - y[secondIndex];

	int vectorSum = (firstLineX * secondLineY - secondLineX * firstLineY);
	int answer = vectorSum / absNum(vectorSum);

	return answer;
}

void exitProgram() {
	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для определения, является ли многоугольник выпуклым или нет?" << endl;

	const int MIN_LIMIT = -1000;
	const int MAX_LIMIT = 1000;

	bool isFirst = true;
	bool isFail = true;
	int countOfPoints = 0;

	cout << "\nВведите количество вершин многоугольника от " << 3 << " до " << MAX_LIMIT << ":";
	cinWithChecking(countOfPoints, MAX_LIMIT, 3);

	int* pointX = new int[countOfPoints];
	int* pointY = new int[countOfPoints];

	cout << "\nВведите точки вершин многоугольника последовательно." << endl;
	for (int i = 0; i < countOfPoints; i++) {
		cout << "Введите координаты вершины (x, y) от " << MIN_LIMIT << " до " << MAX_LIMIT << " #" << i + 1 << ':';
		cinTwiceWithChecking(pointX[i], pointY[i], MAX_LIMIT, MIN_LIMIT);
		pointX[i] += MAX_LIMIT;
		pointY[i] += MAX_LIMIT;
	}

	int sign = -2;
	bool isEndOfFor = false;

	for (int i = 0; i < countOfPoints; i++) {
		int signOfVectorSum = checkPoints(i, pointX, pointY, countOfPoints);

		if (signOfVectorSum != 0)
			if (sign == -2)
				sign = signOfVectorSum;
			else if (sign != signOfVectorSum)
				isEndOfFor = true;
	}

	if (sign == -2)
		cout << ERRORS[FigureError] << endl;
	else
		cout << "Многоугольник " << (isEndOfFor ? "не " : "") << "является выпуклым." << endl;
	
	delete[] pointX;
	delete[] pointY;

	exitProgram();
}