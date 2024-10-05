#include <iostream>
using namespace std;

enum TErrors {
	FailData,
	FailLimitOfData
};
const string ERRORS[3]{
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.",
};

void cinBufClean() {
	cin.clear();
	while (cin.get() != '\n');
}

void splitNumToArray(unsigned long long num, int* arr, int numLength) {
	while (num > 0) {
		arr[--numLength] = num % 10;
		num = num / 10;
	}
}

void cinWithChecking(int& value, const int& MAX_LIMIT, const int& MIN_LIMIT) {
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

void outElement(char element, int count, bool startEndl = false, bool endEndl = false) {
	if (startEndl) {
		cout << endl;
	}

	for (int i = 0; i < count; i++) {
		cout << element;
	}

	if (endEndl) {
		cout << endl;
	}
}

int getNumLength(unsigned long long num) {
	if (num == 0)
		return 1;

	int length = 0;
	while (num > 0) {
		length++;
		num = num / 10;
	}
	return length;
}

void divisionOfElements(int* arrFirstNum, int& firstNum, int& secondNum, int& firstNumLength) {
	cout << endl << firstNum << " | " << secondNum << endl;

	int pos = 0;
	int underlinedNum = 0;
	int addUnderlinedRange = 1;
	int underlinedNumLength = 0;
	bool isFirst = true;

	while (pos + addUnderlinedRange - 1 < firstNumLength) {
		// Get underlineNum
		while (pos + addUnderlinedRange - 1 < firstNumLength && underlinedNum < secondNum) {
			underlinedNum = underlinedNum * 10 + arrFirstNum[pos + addUnderlinedRange - 1];
			addUnderlinedRange++;
		}
		addUnderlinedRange--;

		// Minus round
		if (!isFirst) {
			outElement(' ', pos);
			outElement('-', addUnderlinedRange, false, true);
			outElement(' ', pos);
			cout << underlinedNum << endl;
		}

		int divisionAns = underlinedNum / secondNum;
		int roundDivision = divisionAns * secondNum;
		int divisionPos = pos + (addUnderlinedRange - getNumLength(roundDivision));

		outElement(' ', divisionPos);
		cout << roundDivision;

		if (isFirst) {
			outElement(' ', firstNumLength - addUnderlinedRange);
			cout << " | " << (int)(firstNum / secondNum) << endl;
		}
		else 
			cout << endl;

		underlinedNum -= roundDivision;

		int newUnderlinedRange = getNumLength(underlinedNum);
		pos += addUnderlinedRange - newUnderlinedRange;
		addUnderlinedRange = newUnderlinedRange + 1;

		if (underlinedNum == 0) {
			pos++;
			addUnderlinedRange--;
		}

		isFirst = false;
	}

	outElement(' ', pos);
	outElement('-', addUnderlinedRange - 1, false, true);
	outElement(' ', pos);

	cout << underlinedNum << endl << endl;
}

void exitProgram() {
	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}

int main() {
	setlocale(LC_ALL, "Russian");

	const int MIN_LIMIT = 1;
	const int MAX_LIMIT = 2000000000;

	cout << "Программа для деления чисел в столбик." << endl << endl;

	int firstNum, secondNum;
	bool isFirst = true;
	bool isFail = true;

	cout << "Введите значение делимого от " << MIN_LIMIT << " до " << MAX_LIMIT << ':';
	cinWithChecking(firstNum, MAX_LIMIT, MIN_LIMIT);

	cout << "Введите значение делителя от " << MIN_LIMIT << " до " << MAX_LIMIT << ':';
	cinWithChecking(secondNum, MAX_LIMIT, MIN_LIMIT);

	int firstNumLength;
	firstNumLength = getNumLength(firstNum);

	int* arr = new int[firstNumLength];
	splitNumToArray(firstNum, arr, firstNumLength);

	divisionOfElements(arr, firstNum, secondNum, firstNumLength);

	delete[] arr;

	exitProgram();
}