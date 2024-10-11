#include <iostream>
#include <fstream>
using namespace std;

enum TFile {
	FileIn,
	FileOut
};

enum TErrors {
	FailFileOpen = 0,
	FailFileCreateOrOpen,
	FailData,
	FailLimitOfData,
	NotTXTFile
};
const string ERRORS[5]{
	"Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.",
	"Расширение файла должно быть TXT. Попробуйте еще раз."
};
const int MIN_LIMIT_SIZE = 0;
const int MAX_LIMIT_SIZE = 100;
const int MIN_LIMIT = -100;
const int MAX_LIMIT = 100;

void cinBufClean() {
	cin.clear();
	while (cin.get() != '\n');
}

void strToLow(string &str) {
	for (char& sym : str) {
		int asciiValue = (int)sym;
		if (asciiValue > 64 && asciiValue < 91)
			sym = (char)(asciiValue + 32);
	}
}

bool checkFileNotTxt(string pathToFile) {
	strToLow(pathToFile);

	int stringLength = pathToFile.length();

	if (stringLength < 4)
		return true;
	if ((pathToFile[stringLength - 1] == 't') &&
		(pathToFile[stringLength - 2] == 'x') &&
		(pathToFile[stringLength - 3] == 't') &&
		(pathToFile[stringLength - 4] == '.'))
		return false;
	return true;
}

void cinWithChecking(int& num, const int MAX_LIMIT_NUM, const int MIN_LIMIT_NUM) {
	cin >> num;

	if (cin.fail() || cin.get() != '\n') {
		cout << ERRORS[FailData] << endl;
		cinBufClean();
		cinWithChecking(num, MAX_LIMIT_NUM, MIN_LIMIT_NUM);
	}
	else if (num > MAX_LIMIT_NUM || num < MIN_LIMIT_NUM) {
		cout << ERRORS[FailLimitOfData] << endl;
		cinWithChecking(num, MAX_LIMIT_NUM, MIN_LIMIT_NUM);
	}

}

bool finWithChecking(fstream& fin, int& num, const int MAX_LIMIT_NUM, const int MIN_LIMIT_NUM) {
	bool osFail = false;

	fin >> num;

	if (fin.fail()) {
		cout << ERRORS[FailData] << endl;
		return true;
	}
	else if (num > MAX_LIMIT_NUM || num < MIN_LIMIT_NUM) {
		cout << ERRORS[FailLimitOfData] << endl;
		return true;
	}
	return false;
}

void openFile(fstream& file, TFile fileType) {
	cout << "\nВведите путь к файлу .txt для " << (fileType == FileIn ? "ввода" : "вывода") << " данных : " << endl;
	string filename;
	cin >> filename;

	if (checkFileNotTxt(filename)) {
		cout << ERRORS[NotTXTFile];
		openFile(file, fileType);
	}
	else {
		file.open(filename, fileType == FileIn ? ios::in : ios::out);
		if (!file.is_open()) {
			cout << ERRORS[FailFileOpen];
			file.close();
			openFile(file, fileType);
		}
	}
}

void inDataWithConsole(int& m, int& n, int**& matrix) {
	cout << "Введите размерность 2M квадратной матрицы: " << endl;
	cinWithChecking(m, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

	m *= 2;
	n = m;

	matrix = new int* [m];
	for (int i = 0; i < m; i++) {
		matrix[i] = new int[n];
		for (int j = 0; j < n; j++) {
			cout << "Введите элемент матрицы [" << i + 1 << ',' << j + 1 << "]:" << endl;
			cinWithChecking(matrix[i][j], MAX_LIMIT, MIN_LIMIT);
		}
	}
}

bool inDataWithFile(int& m, int& n, int**& matrix, fstream& fin) {
	openFile(fin, FileIn);

	if (finWithChecking(fin, m, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE))
		return true;
	
	m *= 2;
	n = m;

	matrix = new int*[m];
	for (int i = 0; i < m; i++) {
		matrix[i] = new int[n];
		for (int j = 0; j < n; j++)
			if (finWithChecking(fin, matrix[i][j], MAX_LIMIT, MIN_LIMIT)) {
				fin.close();
				return true;
			}
	}

	fin.close();
	return false;
}

void inData(int& m, int& n, int**& matrix, int inType) {
	bool isDataOk;

	if (inType == 1)
		inDataWithConsole(m, n, matrix);
	else {
		fstream fin;
		do {
			isDataOk = inDataWithFile(m, n, matrix, fin);
		} while (isDataOk);
	}
}

void outMatrix(int m, int n, int**& arr, int outType) {
	fstream fout;
	if (outType == 2)
		openFile(fout, FileOut);

	cout << endl;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cout << arr[i][j] << ' ';
			if (outType == 2)
				fout << arr[i][j] << ' ';
		}
		cout << endl;
		if (outType == 2)
			fout << endl;
	}

	cout << endl;

	fout.close();
}

void changeMatrix(int**& matrix, int m, int n) {
	int** newMatrix = new int* [m];
	for (int i = 0; i < m; i++)
		newMatrix[i] = new int[n] {0};

	for (int i = 0; i < m; i++)
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

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = newMatrix[i][j];

	for (int i = 0; i < m; i++)
		delete[] newMatrix[i];
	delete[] newMatrix;
}

void exitProgram(int**& matrix, int m) {
	for (int i = 0; i < m; i++)
		delete[] matrix[i];
	delete[] matrix;

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для изменения положения подматриц квадратной матрицы порядка 2n." << endl;

	cout << "Все значения матрицы должны быть от " << MIN_LIMIT << " до " << MAX_LIMIT << '.' << endl;
	cout << "Размеры квадратной матрицы 2M быть от " << MIN_LIMIT_SIZE << " до " << MAX_LIMIT_SIZE << '.' << endl << endl;

	int inType = 0;
	int m, n;
	int** matrix;

	cout << "Введите предпочитаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы)." << endl;
	cinWithChecking(inType, 2, 1);

	inData(m, n, matrix, inType);

	outMatrix(m, n, matrix, 1);

	int outType;
	cout << "Введите предпочитаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоли,\n\t2 - в консоль и в файл." << endl;
	cinWithChecking(outType, 2, 1);

	changeMatrix(matrix, m, n);

	outMatrix(m, n, matrix, outType);

	exitProgram(matrix, m);
}
