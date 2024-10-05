#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum TFile {
	FileIn, 
	FileOut
};
enum TErrors {
	FailFileOpen = 0,
	FailFileCreateOrOpen,
	FailData,
	FailLimitOfData
};
const string Errors[4]{
	"Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз."
};

void cinBufClean() {
	cin.clear();
	while (cin.get() != '\n');
}

void cinWithChecking(int& value, const int& MAX_LIMIT, const int& MIN_LIMIT) {
	cin >> value;

	if (cin.fail() || cin.get() != '\n') {
		cout << Errors[FailData] << endl;
		cinBufClean();
		cinWithChecking(value, MAX_LIMIT, MIN_LIMIT);
	}
	else if (value > MAX_LIMIT || value < MIN_LIMIT) {
		cout << Errors[FailLimitOfData] << endl;
		cinWithChecking(value, MAX_LIMIT, MIN_LIMIT);
	}

}

void cinTwiceWithChecking(int& value1, int& value2, const int& MAX_LIMIT, const int& MIN_LIMIT) {
	cin >> value1 >> value2;

	if (cin.fail() || cin.get() != '\n') {
		cout << Errors[FailData] << endl;
		cinBufClean();
		cinTwiceWithChecking(value1, value2, MAX_LIMIT, MIN_LIMIT);
	}
	else if (value1 > MAX_LIMIT || value1 < MIN_LIMIT || value2 > MAX_LIMIT || value2 < MIN_LIMIT) {
		cout << Errors[FailLimitOfData] << endl;
		cinTwiceWithChecking(value1, value2, MAX_LIMIT, MIN_LIMIT);
	}

}

void openFile(fstream& file, TFile fileType) {
	string filename;
	cin >> filename;

	file.open(filename, fileType == FileIn ? ios::in : ios::out);
	if (!file.is_open()) {
		cout << Errors[FailFileOpen];
		file.close();
		openFile(file, fileType);
	}
}

void inData(int& n, int& m, int**& matrix, int inType, fstream& fin, int MAX_LIMIT, int MIN_LIMIT) {
	if (inType == 1) {
		cout << "Введите размер матрицы m строк и n столбцов." << endl;
		cinTwiceWithChecking(n, m, MAX_LIMIT, MIN_LIMIT);

		matrix = new int* [n];
		for (int i = 0; i < n; i++) {
			matrix[i] = new int[m];
			for (int j = 0; j < m; j++) {
				cout << "Введите элемент матрицы [" << i + 1 << ',' << j + 1 << "]:" << endl;
				cinWithChecking(matrix[i][j], MAX_LIMIT, -MAX_LIMIT);
			}
		}
	}
	else {
		bool isFileOpenSeccess;

		cout << "\nВведите путь к файлу .txt для ввода данных:";
		do {
			isFileOpenSeccess = true;
			bool isLimitError = false;

			openFile(fin, FileIn);
			fin >> n >> m;

			matrix = new int* [n];
			for (int i = 0; i < n && !isLimitError; i++) {
				matrix[i] = new int[m];
				for (int j = 0; j < m && !isLimitError; j++) {
					fin >> matrix[i][j];
					if (matrix[i][j] > MAX_LIMIT || matrix[i][j] < MIN_LIMIT)
						isLimitError = true;
				}
			}

			if (fin.fail()) {
				cout << Errors[FailData];
				fin.close();
				isFileOpenSeccess = false;
			}

			if (isLimitError || n > MAX_LIMIT || n < MIN_LIMIT || m > MAX_LIMIT || m < MIN_LIMIT) {
				cout << Errors[FailLimitOfData];
				fin.close();
				isFileOpenSeccess = false;
			}
		} while (!isFileOpenSeccess);
	}

	fin.close();
}

void outSeddlePoints(int n, int m, int**&matrix, int outType, fstream& fout) {
	if (outType == 2)
		openFile(fout, FileOut);

	int* minRows = new int[n];
	int* maxColumns = new int[m] {0};

	for (int i = 0; i < n; i++)
		minRows[i] = 101;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (matrix[i][j] < minRows[i])
				minRows[i] = matrix[i][j];
			if (matrix[i][j] > maxColumns[j])
				maxColumns[j] = matrix[i][j];
		}

	int countOfAnswers = 0;
	cout << endl;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (minRows[i] == maxColumns[j]) {
				countOfAnswers++;
				cout << minRows[i] << " - [" << i + 1 << ',' << j + 1 << ']' << endl;
				if (outType == 2)
					fout << minRows[i] << " - [" << i + 1 << ',' << j + 1 << ']' << endl;
			}

	if (countOfAnswers == 0) {
		cout << "В данный матрице нет седловых точек." << endl;
		if (outType)
			fout << "No saddle point in this matrix." << endl;
	}

	cout << endl;

	delete[] maxColumns;
	delete[] minRows;

	fout.close();
}

void exitProgram(int**&matrix, int matrixLength) {
	for (int i = 0; i < matrixLength; i++)
		delete[] matrix[i];
	delete[] matrix;

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для определения \"седловой\" точки матрицы." << endl;

	const int MIN_LIMIT = 0;
	const int MAX_LIMIT = 100;

	cout << "Все значения матрицы должны быть от " << -MAX_LIMIT << " до " << MAX_LIMIT << '.' << endl;
	cout << "Размеры матрицы MxN быть от " << MIN_LIMIT << " до " << MAX_LIMIT << '.' << endl << endl;

	fstream fin, fout;
	int inType = 0;
	int n, m;
	int** matrix;

	cout << "Введите предпочетаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы)." << endl;
	cinWithChecking(inType, 2, 1);

	inData(n, m, matrix, inType, fin, MAX_LIMIT, MIN_LIMIT);

	int outType;
	cout << "Введите предпочетаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоли,\n\t2 - в консоль и в файл." << endl;
	cinWithChecking(outType, 2, 1);

	outSeddlePoints(n, m, matrix, outType, fout);

	exitProgram(matrix, n);
}