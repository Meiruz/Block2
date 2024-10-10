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

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для определения, является ли многоугольник выпуклым или нет?" << endl;

	const int MIN_LIMIT = -1000;
	const int MAX_LIMIT = 1000;

	bool isFail;
	int countOfPoints = 0;

	cout << "\nВведите количество вершин многоугольника от " << 3 << " до " << MAX_LIMIT << ":";
	do {
		isFail = false;
		cin >> countOfPoints;

		if (cin.fail() || cin.get() != '\n') {
			cout << ERRORS[FailData] << endl;

			cin.clear();
			while (cin.get() != '\n');

			isFail = true;
		}
		else if (countOfPoints > MAX_LIMIT || countOfPoints < 3) {
			cout << ERRORS[FailLimitOfData] << endl;
			isFail = true;
		}
	} while (isFail);
	

	int* pointX = new int[countOfPoints];
	int* pointY = new int[countOfPoints];

	cout << "\nВведите точки вершин многоугольника последовательно." << endl;
	for (int i = 0; i < countOfPoints; i++) {
		cout << "Введите координаты вершины (x, y) от " << MIN_LIMIT << " до " << MAX_LIMIT << " #" << i + 1 << ':';
		do {
			isFail = false;

			cin >> pointX[i] >> pointY[i];

			if (cin.fail() || cin.get() != '\n') {
				cout << ERRORS[FailData] << endl;

				cin.clear();
				while (cin.get() != '\n');

				isFail = true;
			}
			else if (pointX[i] > MAX_LIMIT || pointX[i] < MIN_LIMIT || pointY[i] > MAX_LIMIT || pointY[i] < MIN_LIMIT) {
				cout << ERRORS[FailLimitOfData] << endl;				
				isFail = true;
			}
		} while (isFail);

		pointX[i] += MAX_LIMIT;
		pointY[i] += MAX_LIMIT;
	}

	int sign = -2;
	bool isEndOfFor = false;

	for (int i = 0; i < countOfPoints; i++) {
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
		cout << ERRORS[FigureError] << endl;
	else
		cout << "Многоугольник " << (isEndOfFor ? "не " : "") << "является выпуклым." << endl;
	
	delete[] pointX;
	delete[] pointY;

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}