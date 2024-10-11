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
	FailLimitOfData
};
const string ERRORS[4]{
	"Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз."
};
const int MIN_LIMIT_SIZE = 0;
const int MAX_LIMIT_SIZE = 100;
const int MIN_LIMIT = -100;
const int MAX_LIMIT = 100;

void cinBufClean() {
	cin.clear();
	while (cin.get() != '\n');
}

void cinWithChecking(int &num, const int MAX_LIMIT_NUM, const int MIN_LIMIT_NUM) {
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

bool finWithChecking(fstream &fin, int& num, const int MAX_LIMIT_NUM, const int MIN_LIMIT_NUM) {
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

void openFile(fstream &file, TFile fileType) {
	cout << "\nВведите путь к файлу .txt для " << (fileType == FileIn ? "ввода" : "вывода") << " данных : " << endl;
	string filename;
	cin >> filename;

	file.open(filename, fileType == FileIn ? ios::in : ios::out);
	if (!file.is_open()) {
		cout << ERRORS[FailFileOpen];
		file.close();
		openFile(file, fileType);
	}
}

void inDataWithConsole(int &m, int &n, int **&matrix) {
	cout << "Введите колчество строк M матрицы: " << endl;
	cinWithChecking(m, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

	cout << "Введите колчество столбцов N матрицы: " << endl;
	cinWithChecking(n, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

	matrix = new int*[m];
	for (int i = 0; i < m; i++) {
		matrix[i] = new int[n];
		for (int j = 0; j < n; j++) {
			cout << "Введите элемент матрицы [" << i + 1 << ',' << j + 1 << "]:" << endl;
			cinWithChecking(matrix[i][j], MAX_LIMIT, MIN_LIMIT);
		}
	}
}

bool inDataWithFile(int& m, int& n, int**& matrix, fstream &fin) {
	
	openFile(fin, FileIn);
	
	if (finWithChecking(fin, m, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE))
		return true;
	if (finWithChecking(fin, n, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE))
		return true;

	matrix = new int* [m];
	for (int i = 0; i < m; i++) {
		cout << i << endl;
		matrix[i] = new int[n];
		for (int j = 0; j < n; j++) 
			if (finWithChecking(fin, matrix[i][j], MAX_LIMIT, MIN_LIMIT))
				return true;
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

void findSeddlePoints(int m, int n, int**&matrix, int outType) {
	fstream fout;

	if (outType == 2) 
		openFile(fout, FileOut);

	int* minRows = new int[m];
	int* maxColumns = new int[n] {0};

	for (int i = 0; i < m; i++)
		minRows[i] = 101;

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] < minRows[i])
				minRows[i] = matrix[i][j];
			if (matrix[i][j] > maxColumns[j])
				maxColumns[j] = matrix[i][j];
		}

	int countOfAnswers = 0;
	cout << endl;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			if (minRows[i] == maxColumns[j]) {
				countOfAnswers++;
				cout << minRows[i] << " - [" << i + 1 << ',' << j + 1 << ']' << endl;
				if (outType == 2)
					fout << minRows[i] << " - [" << i + 1 << ',' << j + 1 << ']' << endl;
			}

	if (countOfAnswers == 0) {
		cout << "В данный матрице нет седловых точек." << endl;
		if (outType == 2)
			fout << "No saddle point in this matrix." << endl;
	}

	cout << endl;

	delete[] maxColumns;
	delete[] minRows;

	fout.close();
}

void outMatrix(int m, int n, int**&arr) {
	cout << endl;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			cout << arr[i][j] << ' ';
		cout << endl;
	}

	cout << endl;
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

	cout << "Программа для определения \"седловой\" точки матрицы.\n" << endl;

	cout << "Все значения матрицы должны быть от " << MIN_LIMIT << " до " << MAX_LIMIT << '.' << endl;
	cout << "Размеры матрицы MxN быть от " << MIN_LIMIT_SIZE << " до " << MAX_LIMIT_SIZE << '.' << endl << endl;

	int inType = 0;
	int m, n;
	int** matrix;

	cout << "Введите предпочитаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли (по элементу),\n\t2 - из файла (одна строка m и n, дальше элементы в виде таблицы)." << endl;
	cinWithChecking(inType, 2, 1);
		
	inData(m, n, matrix, inType);

	outMatrix(m, n, matrix);

	int outType;
	cout << "Введите предпочитаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоли,\n\t2 - в консоль и в файл." << endl;
	cinWithChecking(outType, 2, 1);

	findSeddlePoints(m, n, matrix, outType);

	exitProgram(matrix, m);
	return 0;
}