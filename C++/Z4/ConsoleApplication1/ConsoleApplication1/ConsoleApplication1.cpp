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

void inWithChecking(int& value, const int& MAX_LIMIT, const int& MIN_LIMIT) {
	cin >> value;

	if (cin.fail() || cin.get() != '\n') {
		cout << Errors[FailData] << endl;
		cinBufClean();
		inWithChecking(value, MAX_LIMIT, MIN_LIMIT);
	}
	else if (value > MAX_LIMIT || value < MIN_LIMIT) {
		cout << Errors[FailLimitOfData] << endl;
		inWithChecking(value, MAX_LIMIT, MIN_LIMIT);
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

void inData(int& n, int**& matrix, int inType, fstream& fin, int MAX_LIMIT, int MIN_LIMIT) {
	if (inType == 1) {
		cout << "Введите порядок матрицы 2:" << endl;
		inWithChecking(n, MAX_LIMIT, MIN_LIMIT);

		n *= 2;

		matrix = new int* [n];
		for (int i = 0; i < n; i++) {
			matrix[i] = new int[n];
			for (int j = 0; j < n; j++) {
				cout << "Введите элемент матрицы [" << i + 1 << ',' << j + 1 << "]:" << endl;
				inWithChecking(matrix[i][j], MAX_LIMIT, -MAX_LIMIT);
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
			fin >> n;

			n *= 2;

			matrix = new int* [n];
			for (int i = 0; i < n && !isLimitError; i++) {
				matrix[i] = new int[n];
				for (int j = 0; j < n && !isLimitError; j++) {
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

			if (isLimitError || n > MAX_LIMIT || n < MIN_LIMIT) {
				cout << Errors[FailLimitOfData];
				fin.close();
				isFileOpenSeccess = false;
			}
		} while (!isFileOpenSeccess);
	}
}

void changeMatrix(int **&matrix, int n) {
	int** newMatrix = new int*[n];
	for (int i = 0; i < n; i++)
		newMatrix[i] = new int[n] {0};

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i < n / 2) 
				if (j < n / 2) 
					newMatrix[i + n / 2][j + n / 2] = matrix[i][j];
				else
					newMatrix[i + n / 2][j - n / 2] = matrix[i][j];
			else 
				if (j < n / 2)
					newMatrix[i - n / 2][j] = matrix[i][j];
				else
					newMatrix[i - n / 2][j] = matrix[i][j];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = newMatrix[i][j];

	for (int i = 0; i < n; i++)
		delete[] newMatrix[i];
	delete[] newMatrix;
}

void exitProgram(fstream& fin, fstream& fout, int**& matrix, int matrixLength) {
	for (int i = 0; i < matrixLength; i++)
		delete[] matrix[i];
	delete[] matrix;
	fin.close();
	fout.close();

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для изменения положения подматриц квадратной матрицы порядка 2n." << endl;

	const int MIN_LIMIT = 0;
	const int MAX_LIMIT = 100;
	cout << "Все значения матрицы должны быть от " << -MAX_LIMIT << " до " << MAX_LIMIT << '.' << endl;
	cout << "Порядок матрицы n должен быть от " << MIN_LIMIT << " до " << MAX_LIMIT << '.' << endl << endl;

	fstream fin, fout;
	int inType = 0;
	int n;
	int** matrix;

	cout << "Введите предпочетаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка n, дальше элементы в виде таблицы)." << endl;
	inWithChecking(inType, 2, 1);

	inData(n, matrix, inType, fin, MAX_LIMIT, MIN_LIMIT);

	int outType;
	cout << "Введите предпочетаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоль,\n\t2 - в консоль и в файл." << endl;
	inWithChecking(outType, 2, 1);
	
	changeMatrix(matrix, n);
	
	if (outType == 2)
		openFile(fout, FileOut);

	cout << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << matrix[i][j] << ' ';
			if (outType == 2)
				fout << matrix[i][j] << ' ';
		}

		cout << endl;
		if (outType == 2)
			fout << endl;
	}
	cout << endl;

	exitProgram(fin, fout, matrix, n);
}