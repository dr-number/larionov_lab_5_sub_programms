#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <cctype>
#include <conio.h>
#include <random>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <vector>
#include <random>

using namespace std;
HANDLE handleConsole;

enum ConsoleColor
{
    Blue = 1,
    Green = 2,
    Red = 4,
    Yellow = 14,
    White = 15,
    Black = 0
};

string GetLine() {
    string line;
    getline(cin, line);
    cin.clear();
    return line;
}

class MyInput {

public:
    bool isNum(string str) {
        return str.find_first_not_of("1234567890") == string::npos;
    }

    int InputIntData(string text, int min, int max, int defaultValue = -1) {

        string xStr = "";
        double result = 0;
        bool isNumber = true;

        while (true) {

            SetConsoleTextAttribute(handleConsole, White);
            cout << text;

            xStr = GetLine();

            if (xStr == "" && defaultValue != -1)
                return defaultValue;


            try {
                result = stod(xStr.c_str());
                isNumber = true;
            }
            catch (...) {
                isNumber = false;
            }

            if (!(isNumber && isNum(xStr))) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << endl << xStr + " - не число!" << endl << endl;
            }
            else if (result > max || result < min) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << endl << "Число должно лежать в промежутке [" << min << "; " << max << "]!" << endl << endl;
            }
            else
                break;
        }

        return result;
    }

};

class MyQuestion {

public:
    const string QUESTION_RANDOM_DATA = "Сгенерировать данные случайным образом [y/n]?";
    const string QUESTION_IN_ORDER_DATA = "Взять числа по порядку [y/n]?";
    const string QUESTION_SHOW_CALC = "Показывать ход вычислений [y/n]?";

    bool isQuestion(string textQuestion) {

        cout << textQuestion << endl;
        string answer = GetLine();

        transform(answer.begin(), answer.end(), answer.begin(), tolower);
        return answer == "y" || answer == "";
    }
};

class MyRandom {

public:
    const double MIN_RANDOM = 10, MAX_RANDOM = 1000;

    double GetRandom(int min, int max) {

        random_device random_device; // Источник энтропии.
        mt19937 generator(random_device()); // Генератор случайных чисел.

        // (Здесь берется одно инициализирующее значение, можно брать больше)
        uniform_int_distribution<> distribution(min, max); // Равномерное распределение [min, max]

        return distribution(generator);

        //return rand() % (max - min) - min;
        //return rand() % max + min;
    }

};

class MyArray {
public:
    int const MIN_VALUE = -10000;
    int const MAX_VALUE = 10000;

    int const DEFAULT_COUNT_VALUE = 20;

    int const MIN_COUNT = 5;
    int const MAX_COUNT = 10000;

    int const DEFAULT_ROW = 10;
    int const DEFAULT_COL = 15;

    vector <int> Swap(vector <int> arr, int index1, int index2) {
        int tmp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = tmp;
        return arr;
    }

    vector <int> CreateRandomArray(int n, int minValue, int maxValue) {

        vector <int> arr;
        MyRandom myRandom = *new MyRandom();

        for (int i = 0; i < n; ++i)
            arr.push_back(myRandom.GetRandom(minValue, maxValue));

        return arr;
    }

    vector <int> CreateInputArray(int n, int minValue, int maxValue) {

        vector <int> arr;
        MyInput myInput = *new MyInput();

        for (int i = 0; i < n; ++i)
            arr.push_back(myInput.InputIntData("Введите целое число (" + to_string(i + 1) + " из " + to_string(n) + "): ", minValue, maxValue));

        return arr;
    }

    vector <int> CreateArray(int n, int minValue, int maxValue) {

        MyQuestion myQuestion = *new MyQuestion();

        bool isRandomData = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);

        if (isRandomData)
            return CreateRandomArray(n, minValue, maxValue);

        return CreateInputArray(n, minValue, maxValue);
    }

    vector <int> CreateArray(int n) {
        return CreateArray(n, MIN_VALUE, MAX_VALUE);
    }

    vector <int> CreateArray() {
        MyInput myInput = *new MyInput();
        int size = myInput.InputIntData("Сколько элементов должно быть в массиве? [по умолчанию " + to_string(DEFAULT_COUNT_VALUE) + "]: ", MIN_COUNT, MAX_COUNT, DEFAULT_COUNT_VALUE);
        return CreateArray(size);
    }

    void PrintArray(vector <int> arr) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        int size = arr.size();

        for (int i = 0; i < size; ++i) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "[" << i << "] ";

            SetConsoleTextAttribute(handleConsole, White);
            cout << arr[i] << endl;
        }
    }

    int GetIndexMinElem(vector<int> arr) {

        int index = 0;
        int min = arr[0];
        int size = arr.size();

        for (int i = 1; i < size; ++i)
            if (arr[i] < min) {
                min = arr[i];
                index = i;
            }

        return index;
    }

    int GetIndexMaxElem(vector<int> arr) {

        int index = 0;
        int max = arr[0];
        int size = arr.size();

        for (int i = 1; i < size; ++i)
            if (arr[i] > max) {
                max = arr[i];
                index = i;
            }

        return index;
    }

};

class MyMatrix {
public:
    int const MIN_VALUE = -10000;
    int const MAX_VALUE = 10000;

    int const DEFAULT_COUNT_VALUE = 20;

    int const MIN_COL = 5;
    int const MAX_COL = 10000;

    int const MIN_ROW = 5;
    int const DEFAULT_ROW = 10;

    int const MAX_ROW = 10000;
    int const DEFAULT_COL = 12;

    int const STR_WIDTH = 8;

    struct coordinate {
    public:
        int n;
        int m;
    };

    struct interval {
    public:
        coordinate min;
        coordinate max;
    };

    bool isQual(coordinate a, coordinate b) {
        bool n = a.n == b.n;
        bool m = a.m == b.m;
        return n && m;
    }

    vector <vector <int>> CreateRandomArray(int n, int m, int minValue, int maxValue) {

        vector <vector <int>> arr(n, vector <int>(m));
        MyRandom myRandom = *new MyRandom();

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                arr[i][j] = myRandom.GetRandom(minValue, maxValue);

        return arr;
    }

    vector <vector <int>> CreateInputArray(int n, int m, int minValue, int maxValue) {

        vector <vector <int>> arr(n, vector <int>(m));
        MyInput myInput = *new MyInput();

        int size = m * n;
        int num = 1;

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) {
                arr[i][j] = myInput.InputIntData("Введите целое число (" + to_string(num) + " из " + to_string(size) + "): ", minValue, maxValue);
                ++num;
            }

        return arr;
    }

    vector <vector <int>> Swap(vector <vector <int>> matrix, coordinate coord1, coordinate coord2) {

        int tmp = matrix[coord1.n][coord1.m];
        matrix[coord1.n][coord1.m] = matrix[coord2.n][coord2.m];
        matrix[coord2.n][coord2.m] = tmp;

        return matrix;
    }

    vector <vector <int>> CreateArray(int n, int m, int minValue, int maxValue) {

        MyQuestion myQuestion = *new MyQuestion();

        bool isRandomData = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);

        if (isRandomData)
            return CreateRandomArray(n, m, minValue, maxValue);

        return CreateInputArray(n, m, minValue, maxValue);
    }

    vector <vector <int>> CreateArray(int n, int m) {
        return CreateArray(n, m, MIN_VALUE, MAX_VALUE);
    }

    vector <vector <int>> CreateArray() {

        MyInput myInput = *new MyInput();
        int n = myInput.InputIntData("Сколько строк должно быть в массиве? [по умолчанию " + to_string(DEFAULT_ROW) + "]: ", MIN_ROW, MAX_ROW, DEFAULT_ROW);
        int m = myInput.InputIntData("Сколько столбцов должно быть в массиве? [по умолчанию " + to_string(DEFAULT_COL) + "]: ", MIN_COL, MAX_COL, DEFAULT_COL);

        return CreateArray(n, m);
    }

    void printHeader(vector <vector <int>> arr) {

        int countCol = arr[0].size();
        cout << "   ";

        for (int j = 0; j < countCol; ++j)
            cout << setw(6) << "[" << j << "] ";

        cout << endl;
    }

    void PrintMatrix(vector <vector <int>> matrix, int selectRow = -1, int selectCol = -1) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, Yellow);

        printHeader(matrix);

        int col = matrix[0].size();
        int row = matrix.size();

        for (int i = 0; i < row; ++i) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "[" << i << "] ";

            if (selectRow == i)
                SetConsoleTextAttribute(handleConsole, Green);
            else
                SetConsoleTextAttribute(handleConsole, White);

            for (int j = 0; j < col; ++j) {

                if (selectCol == j)
                    SetConsoleTextAttribute(handleConsole, Green);
                else
                    SetConsoleTextAttribute(handleConsole, White);

                cout << setw(MyMatrix::STR_WIDTH) << matrix[i][j] << " ";
            }

            cout << endl;
        }
    }

    interval GetCoordinateMinMax(vector <vector <int>> matrix) {

        int col = matrix[0].size();
        int row = matrix.size();

        int min = matrix[0][0];
        int max = matrix[0][0];

        coordinate indexMin;
        indexMin.n = 0;
        indexMin.m = 0;

        coordinate indexMax;
        indexMax.n = 0;
        indexMax.m = 0;

        int item;


        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j) {

                item = matrix[i][j];

                if (item < min) {
                    min = item;
                    indexMin.n = i;
                    indexMin.m = j;
                }

                if (item > max) {
                    max = item;
                    indexMax.n = i;
                    indexMax.m = j;
                }
            }



        interval result = *new interval();

        result.min = indexMin;
        result.max = indexMax;

        return result;
    }

};

class Task6 {
private:
    void PrintArray(vector<int> arr, int indexMinElem, int indexMaxElem, bool isEqal) {

        string info;

        int size = arr.size();
        for (int i = 0; i < size; ++i) {

            if (!isEqal) {
                if (i == indexMinElem) {
                    SetConsoleTextAttribute(handleConsole, Green);
                    info = " - минимальный элемент";
                }
                else if (i == indexMaxElem) {
                    SetConsoleTextAttribute(handleConsole, Green);
                    info = " - максимальный элемент";
                }
                else {
                    SetConsoleTextAttribute(handleConsole, Yellow);
                    info = "";
                }
            }

            cout << "[" << i << "] " << arr[i] << info << endl; //форматированный вывод
        }
    }

public:
    void Init() {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Поменять местали максимальный и минимальный элемент массива." << endl << endl;

        MyArray myArray = *new MyArray();
        vector<int> arr = myArray.CreateArray();

        int indexMinElem = myArray.GetIndexMinElem(arr);
        int indexMaxElem = myArray.GetIndexMaxElem(arr);

        bool isEqal = indexMinElem == indexMaxElem;

        if (isEqal) {
            SetConsoleTextAttribute(handleConsole, Blue);
            cout << "Все элементы массива одинаковы!" << endl << endl;
        }

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "Исходный массив:" << endl;

        PrintArray(arr, indexMinElem, indexMaxElem, isEqal);

        if (!isEqal) {

            SetConsoleTextAttribute(handleConsole, Blue);
            cout << endl << "Преобразованный массив:" << endl;

            arr = myArray.Swap(arr, indexMinElem, indexMaxElem);

            PrintArray(arr, indexMaxElem, indexMinElem, isEqal);
        }

    }

};

class Task16 {
private:
    double getAverage(vector <int> arr) {

        double sum = 0;
        int size = arr.size();

        for (int i = 0; i < size; ++i)
            sum += arr[i];

        return sum / size;
    }

public:
    void Init() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Вывести на экран только те его компоненты, которые отклоняются от арифметического" << endl;
        cout << "среднего элементов вектора не более чем на заданную величину E." << endl << endl;

        MyArray myArray = *new MyArray();
        vector<int> arr = myArray.CreateArray();

        MyInput myInput = *new MyInput();

        const int MIN_VALUE = 0;
        const int MAX_VALUE = 90000;
        const int DEFAULT_VALUE = 2000;

        int e = myInput.InputIntData("Введите величину отклонения Е: [по умолчанию " + to_string(DEFAULT_VALUE) + "]", MIN_VALUE, MAX_VALUE, DEFAULT_VALUE);

        SetConsoleTextAttribute(handleConsole, Green);
        cout << endl << "Величина отклонения Е: " << e << endl;

        double average = getAverage(arr);
        cout << "Среднее арифмитическое элементов вектора: " << average << endl;

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "Исходный вектор К:" << endl << endl;

        myArray.PrintArray(arr);

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nВычисления:" << endl;

        string sign = "", info = "", symbol = "";
        int size = arr.size();

        vector<int> result;
        double check;

        for (int i = 0; i < size; ++i) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "[" << i << "] ";

            symbol = "";

            check = abs(arr[i] - average);

            if (check < e) {
                SetConsoleTextAttribute(handleConsole, Green);
                sign = " < ";
                result.push_back(arr[i]);
            }
            else if (check == e) {
                SetConsoleTextAttribute(handleConsole, Green);
                sign = " = ";
                result.push_back(arr[i]);
            }
            else {
                SetConsoleTextAttribute(handleConsole, Red);
                sign = " > ";
                symbol = "не";
            }

            info = " | " + to_string(arr[i]) + " - " + to_string(average) + " | = " + to_string(arr[i] - average) + " = " + to_string(check) + sign + to_string(e) + " " + symbol + " подходит по условию задачи";

            cout << arr[i] << info << endl;
        }

        int count = result.size();

        if (count != 0) {
            SetConsoleTextAttribute(handleConsole, Green);
            cout << "\nЭлементы вектора К отвечающие условию задачи (" << count << " из " << size << ")" << endl;

            SetConsoleTextAttribute(handleConsole, Yellow);

            for (int i = 0; i < count; ++i)
                cout << result[i] << endl;
        }
        else {
            SetConsoleTextAttribute(handleConsole, Red);
            cout << "\nНи один элемент вектора К не отвечает условию задачи!" << endl;
        }


    }
};

class Task26 {
private:
    void printArray(vector<int> arr, int index1, int index2) {

        SetConsoleTextAttribute(handleConsole, White);

        int size = arr.size();

        for (int i = 0; i < size; ++i) {

            if (i == index1 || i == index2)
                SetConsoleTextAttribute(handleConsole, Green);
            else
                SetConsoleTextAttribute(handleConsole, White);

            cout << "[" << i << "] " << arr[i] << endl; //форматированный вывод
        }
    }

public:
    void Init() {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Поменять местали указанные элементы массива." << endl << endl;

        MyArray myArray = *new MyArray();
        vector<int> arr = myArray.CreateArray();

        int size = arr.size();
        MyInput myInput = *new MyInput();

        int index1, index2;

        while (true) {

            index1 = myInput.InputIntData("Введите индекс №1: ", 0, size - 1);
            index2 = myInput.InputIntData("Введите индекс №2: ", 0, size - 1);

            if (index1 == index2) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << "Индексы не должны совпадать!" << endl << endl;
            }
            else {
                SetConsoleTextAttribute(handleConsole, White);
                break;
            }

        }

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << endl << "Исходный массив:" << endl;

        printArray(arr, index1, index2);


        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << endl << "Поменять местали элементы массива под индексами " << index1 << " и " << index2 << endl << endl;
        cout << "Преобразованный массив:" << endl;

        arr = myArray.Swap(arr, index1, index2);

        printArray(arr, index1, index2);

    }
};

class Task36 {
private:
    bool isNegativeMatrix(vector <vector <int>> matrix, bool isPrint = true) {

        int col = matrix[0].size();
        int row = matrix.size();

        int countNegative = 0;
        int countPositive = 0;

        int item;

        MyMatrix myMatrix = *new MyMatrix();

        if (isPrint)
            myMatrix.printHeader(matrix);

        for (int i = 0; i < row; ++i) {

            if (isPrint) {
                SetConsoleTextAttribute(handleConsole, Yellow);
                cout << "[" << i << "] " << setw(myMatrix.STR_WIDTH);
            }

            for (int j = 0; j < col; ++j) {

                item = matrix[i][j];

                if (item > 0) {
                    SetConsoleTextAttribute(handleConsole, Green);
                    ++countPositive;
                }
                else if (item < 0) {
                    SetConsoleTextAttribute(handleConsole, Red);
                    ++countNegative;
                }
                else
                    SetConsoleTextAttribute(handleConsole, White);

                if (isPrint)
                    cout << setw(myMatrix.STR_WIDTH) << item << " ";

            }

            if (isPrint)
                cout << endl;
        }

        if (isPrint) {
            SetConsoleTextAttribute(handleConsole, White);
            cout << "\nВсего элементов в массиве:         " << col * row << endl;

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "Положительных элементов в массиве: " << countPositive << endl;

            SetConsoleTextAttribute(handleConsole, Red);
            cout << "Отрицательных элементов в массиве: " << countNegative << endl << endl;
        }

        return countNegative > countPositive;
    }

    vector <vector <int>> GetMatrixPositiveToZero(vector <vector <int>> matrix, bool isPrint = true) {

        int col = matrix[0].size();
        int row = matrix.size();

        MyMatrix myMatrix = *new MyMatrix();

        if (isPrint)
            myMatrix.printHeader(matrix);

        for (int i = 0; i < row; ++i) {

            if (isPrint) {
                SetConsoleTextAttribute(handleConsole, Yellow);
                cout << "[" << i << "] " << setw(myMatrix.STR_WIDTH);
            }

            for (int j = 0; j < col; ++j) {

                if (matrix[i][j] > 0)
                    SetConsoleTextAttribute(handleConsole, Green);
                else {
                    SetConsoleTextAttribute(handleConsole, White);
                    matrix[i][j] = 0;
                }

                if (isPrint)
                    cout << setw(myMatrix.STR_WIDTH) << matrix[i][j] << " ";

            }

            if (isPrint)
                cout << endl;
        }

        return matrix;
    }

public:
    void Init() {

        SetConsoleTextAttribute(handleConsole, White);
        cout << "В матрице заменить все положительные элементы нулями," << endl;
        cout << "если количество отрицательных элементов окажется больше, чем количество положительных элементов." << endl << endl;

        MyMatrix myMatrix = *new MyMatrix();
        vector <vector <int>> matrix = myMatrix.CreateArray();

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "\nИсходная матрица:" << endl << endl;


        if (isNegativeMatrix(matrix, true)) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "Количество отрицательных элементов больше, чем количество положительных элементов." << endl;
            cout << "Заменяем все положительные элементы матрицы нулями." << endl << endl;

            cout << "Преобразованная матрица:" << endl << endl;

            GetMatrixPositiveToZero(matrix, true);
        }
        else {
            SetConsoleTextAttribute(handleConsole, Red);
            cout << "Условие не выполняется оставляем все как есть" << endl;
        }

    }
};

class Task46 {
private:
    void printArray(vector <vector <int>> matrix, MyMatrix::coordinate coordMin, MyMatrix::coordinate coordMax, bool isEqal) {

        string info;
        MyMatrix myMatrix = *new MyMatrix();

        myMatrix.printHeader(matrix);

        int col = matrix[0].size();
        int row = matrix.size();

        for (int i = 0; i < row; ++i) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "[" << i << "] ";

            for (int j = 0; j < col; ++j) {

                if (!isEqal) {
                    if (i == coordMin.n && j == coordMin.m) {
                        SetConsoleTextAttribute(handleConsole, Red);
                    }
                    else if (i == coordMax.n && j == coordMax.m) {
                        SetConsoleTextAttribute(handleConsole, Green);
                    }
                    else
                        SetConsoleTextAttribute(handleConsole, White);
                }

                cout << setw(myMatrix.STR_WIDTH) << matrix[i][j] << " ";
            }

            cout << endl;

        }
    }
public:
    void Init() {

        SetConsoleTextAttribute(handleConsole, White);
        cout << "Поменять местали максимальный и минимальный элементы матрицы." << endl;

        MyMatrix myMatrix = *new MyMatrix();
        vector <vector <int>> matrix = myMatrix.CreateArray();

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "\nИсходная матрица:" << endl << endl;

        MyMatrix::interval coordMinMax = myMatrix.GetCoordinateMinMax(matrix);

        MyMatrix::coordinate coordMin = coordMinMax.min;
        MyMatrix::coordinate coordMax = coordMinMax.max;

        bool isEqal = myMatrix.isQual(coordMin, coordMax);

        if (isEqal) {
            SetConsoleTextAttribute(handleConsole, Blue);
            cout << "Все элементы массива одинаковы!" << endl << endl;
        }

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "Исходный массив:" << endl;

        printArray(matrix, coordMin, coordMax, isEqal);

        if (!isEqal) {

            int min = matrix[coordMin.n][coordMin.m];
            int max = matrix[coordMax.n][coordMax.m];

            SetConsoleTextAttribute(handleConsole, Red);
            cout << "\nМинимальный элемент массива:  [" << coordMin.n << "][" << coordMin.m << "] " << min << endl;

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "Максимальный элемент массива: [" << coordMax.n << "][" << coordMax.m << "] " << max << endl;


            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "\nПреобразованный массив:" << endl;

            matrix[coordMin.n][coordMin.m] = max;
            matrix[coordMax.n][coordMax.m] = min;

            printArray(matrix, coordMax, coordMin, isEqal);
        }


    }
};

class Task56 {
private:
    struct number {
    public:
        int index;
        int value;
    };

    int AbsSumRow(vector <vector <int>> matrix, int j) {

        int sum = 0;
        int row = matrix.size();

        for (int i = 0; i < row; ++i)
            sum += abs(matrix[i][j]);

        return sum;
    }

    number getLNormIndex(vector <vector <int>> matrix) {

        int col = matrix[0].size();

        int sum;
        number max;

        max.index = 0;
        max.value = AbsSumRow(matrix, 0);

        for (int j = 1; j < col; ++j) {
            sum = AbsSumRow(matrix, j);

            if (sum > max.value) {
                max.index = j;
                max.value = sum;
            }
        }

        return max;
    }

public:
    void Init() {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Вычислить L-норму матрицы" << endl << endl;

        MyMatrix myMatrix = *new MyMatrix();
        vector <vector <int>> matrix = myMatrix.CreateArray();

        number lNormIndex = getLNormIndex(matrix);
        int j = lNormIndex.index;

        myMatrix.PrintMatrix(matrix, -1, j);

        SetConsoleTextAttribute(handleConsole, Green);

        int row = matrix.size();

        string strLNorm = "L-норма = ";
        string withModul = strLNorm;
        string modul = strLNorm;

        for (int i = 0; i < row; ++i) {

            if (i != 0) {
                withModul += "+ ";
                modul += "+ ";
            }

            withModul += "|" + to_string(matrix[i][j]) + "| ";
            modul += to_string(abs(matrix[i][j])) + " ";
        }

        withModul += "= " + to_string(lNormIndex.value);
        modul += "= " + to_string(lNormIndex.value);

        cout << endl << withModul << endl;
        cout << modul << endl;

        cout << strLNorm << lNormIndex.value << endl << endl;

    }
};

class Task66 {
private:
    class CalcHide {
    public:
        vector <int> getMultiply(vector <vector <int>> matrix, vector <int> Vector) {

            int matrixCol = matrix[0].size();
            int matrixRow = matrix.size();

            vector <int> result;
            int sum = 0;

            for (int i = 0; i < matrixRow; ++i) {

                for (int j = 0; j < matrixCol; ++j)
                    sum += matrix[i][j] * Vector[j];

                result.push_back(sum);
                sum = 0;
            }

            return result;
        }

    };

    class CalcPrint {
    private:
        int Multiplication(vector <vector <int>> matrix, vector <int> Vector, int i) {

            int result = 0;

            //перемножаем
            int matrixCol = matrix[0].size();
            for (int j = 0; j < matrixCol; ++j) {

                SetConsoleTextAttribute(handleConsole, White);
                cout << matrix[i][j] * Vector[j];

                if (j != matrixCol - 1) {
                    SetConsoleTextAttribute(handleConsole, Yellow);
                    cout << " + ";
                }
                else {
                    SetConsoleTextAttribute(handleConsole, Blue);
                    cout << " = ";
                    result = GetTotal(matrix, Vector, i);
                }

            }

            return result;
        }

        int GetTotal(vector <vector <int>> matrix, vector <int> vector, int i) {
            // итоги

            int matrixCol = matrix[0].size();

            int sum = 0;
            for (int j = 0; j < matrixCol; ++j) {

                SetConsoleTextAttribute(handleConsole, White);
                sum += matrix[i][j] * vector[j];

                if (j == matrixCol - 1) {
                    SetConsoleTextAttribute(handleConsole, Green);
                    cout << sum << endl << endl;
                }
            }

            return sum;
        }

    public:
        vector <int> getPrintMultiply(vector <vector <int>> matrix, vector <int> Vector) {

            int matrixCol = matrix[0].size();
            int matrixRow = matrix.size();

            int vectorSize = Vector.size();
            int row = vectorSize;

            if (matrixRow > row)
                row = matrixRow;

            int centerRow = row / 2;

            for (int i = 0; i < row; ++i) {

                cout << "| ";

                for (int j = 0; j < matrixCol; ++j) {

                    if (j != 0)
                        cout << setw(8);

                    if (i < matrixRow)
                        cout << matrix[i][j] << " ";
                    else
                        cout << setw(8) << " ";

                }

                cout << "| ";

                if (i == centerRow)
                    cout << "X ";

                SetConsoleTextAttribute(handleConsole, Green);

                if (i == centerRow)
                    cout << "|";
                else
                    cout << "  |";

                if (i < vectorSize)
                    cout << setw(8) << Vector[i] << " | ";
                else
                    cout << setw(10) << "|";


                SetConsoleTextAttribute(handleConsole, White);

                cout << endl;

            }

            cout << endl;
            vector <int> result;

            for (int i = 0; i < matrixRow; ++i) {

                SetConsoleTextAttribute(handleConsole, Green);
                cout << "c" << (i + 1);

                SetConsoleTextAttribute(handleConsole, Blue);
                cout << " = ";

                for (int j = 0; j < matrixCol; ++j) {

                    SetConsoleTextAttribute(handleConsole, Yellow);
                    cout << matrix[i][j] << " * " << Vector[j];

                    if (j != matrixCol - 1) {
                        SetConsoleTextAttribute(handleConsole, Green);
                        cout << " + ";
                    }
                    else {
                        SetConsoleTextAttribute(handleConsole, Blue);
                        cout << " = ";

                        result.push_back(Multiplication(matrix, Vector, i));
                    }
                }
            }

            return result;
        }

    };

public:
    void Init() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);


        cout << "Для матрицы A и вектора X" << endl;
        cout << "вычислить элементы вектора Z как скалярное произведение строки матрицы A на вектор X" << endl << endl;


        MyQuestion myQuestion = *new MyQuestion();

        bool isRandom = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);

        MyArray myArray = *new MyArray();
        MyMatrix myMatrix = *new MyMatrix();
        MyInput myInput = *new MyInput();

        int n = myInput.InputIntData("Сколько строк должно быть в матрице?: [по умолчанию " + to_string(myMatrix.DEFAULT_ROW) + "]", myMatrix.MIN_ROW, myMatrix.MAX_ROW, myMatrix.DEFAULT_ROW);
        int m = myInput.InputIntData("Сколько столбцов должно быть в матрице?: [по умолчанию " + to_string(myMatrix.DEFAULT_COL) + "]", myMatrix.MIN_COL, myMatrix.MAX_COL, myMatrix.DEFAULT_COL);

        vector <vector <int>> matrix;
        vector <int> Vector;

        if (isRandom) {
            matrix = myMatrix.CreateRandomArray(n, m, myMatrix.MIN_ROW, myMatrix.MAX_ROW);
            Vector = myArray.CreateRandomArray(m, myMatrix.MIN_ROW, myMatrix.MAX_ROW);
        }
        else {
            cout << endl << "Ввод матрицы А" << endl;
            matrix = myMatrix.CreateInputArray(n, m, myMatrix.MIN_ROW, myMatrix.MAX_ROW);

            cout << endl << "Ввод вектора X" << endl;
            Vector = myArray.CreateInputArray(m, myMatrix.MIN_ROW, myMatrix.MAX_ROW);
        }


        if (false) {

            int select = 3;

            if (select == 1) {
                matrix = {
                    {1, 2, 3, 4, 5, 6},
                    {7, 8, 9, 0, 0, 9},
                    {8, 7, 6, 5, 4, 3},
                    {2, 1, 11, 22, 33,44},
                    {55, 66, 77, 88, 99, 111}
                };

                Vector = { 1, 2, 3, 4, 5, 6 };
            }
            else if (select == 2) {

                matrix = {
                    {1, 2, 3, 4, 5},
                    {6, 7, 8, 8, 7},
                    {6, 5, 4, 3, 2},
                    {1, 1, 2, 3, 4},
                    {5, 6, 7, 8, 8},
                    {7, 6, 5, 4, 3},
                    {2, 1, 1, 2, 3},
                    {4, 5, 6, 7, 8}
                };

                Vector = { 0, 9, 8, 7, 6 };
            }
            else if (select == 3) {

                matrix = {
                    {1, 2, 3, 4},
                    {6, 7, 8, 8},
                    {6, 5, 4, 3},
                    {1, 1, 2, 3},
                };

                Vector = { 9, 8, 7, 6 };
            }
        }

        cout << "\nМатрица A:" << endl;
        myMatrix.PrintMatrix(matrix);

        cout << endl << "Вектор Х:" << endl;
        myArray.PrintArray(Vector);

        cout << endl;

        bool isShowCalcProcess = myQuestion.isQuestion(myQuestion.QUESTION_SHOW_CALC);

        vector <int> result;

        if (isShowCalcProcess) {
            CalcPrint calcPrint = *new CalcPrint();
            result = calcPrint.getPrintMultiply(matrix, Vector);
        }
        else {
            CalcHide calcHide = *new CalcHide();
            result = calcHide.getMultiply(matrix, Vector);
        }

        cout << endl << "Результат произведения:" << endl;
        myArray.PrintArray(result);

    }
};


int main()
{
    handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setlocale(LC_ALL, "Russian");

    SetConsoleTextAttribute(handleConsole, White);

    cout << "Тема 4. Структурный тип данных \"массив\"" << endl;
    cout << "Ларионов Никита Юрьевич. гр. 110з" << endl;

    bool isGo = true;
    string select;

    while (isGo)
    {
        SetConsoleTextAttribute(handleConsole, White);

        cout << "\nВведите номер задачи" << endl;
        cout << "6)	Поменять местали максимальный и минимальный элементы массива." << endl << endl;

        cout << "16) Вывести на экран только те его компоненты, которые отклоняются от арифметического" << endl;
        cout << "среднего элементов вектора не более чем на заданную величину." << endl << endl;

        cout << "26) Поменять местали указанные элементы массива." << endl << endl;

        cout << "36) В матрице заменить все положительные элементы нулями," << endl;
        cout << "если количество отрицательных элементов окажется больше, чем количество положительных элементов." << endl << endl;

        cout << "46) Поменять местали максимальный и минимальный элементы матрицы." << endl << endl;

        cout << "56) Вычислить L-норму матрицы" << endl << endl;

        cout << "66) Для матрицы A и вектора X" << endl;
        cout << "вычислить элементы вектора Z как скалярное произведение строки матрицы A на вектор X" << endl << endl;

        cout << endl << "Для выхода введите \"0\": ";

        select = GetLine();

        if (select == "6") {
            Task6 task6 = *new Task6();
            task6.Init();
        }
        else if (select == "16") {
            Task16 task16 = *new Task16();
            task16.Init();
        }
        else if (select == "26") {
            Task26 task26 = *new Task26();
            task26.Init();
        }
        else if (select == "36") {
            Task36 task36 = *new Task36();
            task36.Init();
        }
        else if (select == "46") {
            Task46 task46 = *new Task46();
            task46.Init();
        }
        else if (select == "56") {
            Task56 task56 = *new Task56();
            task56.Init();
        }
        else if (select == "66") {
            Task66 task66 = *new Task66();
            task66.Init();
        }
        else if (select == "0") {
            isGo = false;
        }
        else {
            SetConsoleTextAttribute(handleConsole, Red);
            cout << "\nНекорректные данные!" << endl;
            SetConsoleTextAttribute(handleConsole, White);
        }

    }
}

