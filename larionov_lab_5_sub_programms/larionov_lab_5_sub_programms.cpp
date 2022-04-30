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
        return str.find_first_not_of("-1234567890") == string::npos;
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

class Task6 {
private:
    void PrintVector(string nameVector, vector <int> array) {

        int size = array.size();

        SetConsoleTextAttribute(handleConsole, Green);
        cout << nameVector;
        
        SetConsoleTextAttribute(handleConsole, Blue);
        cout << " =";

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << " { ";

        for (int i = 0; i < size; ++i) {

            if (i != 0)
                cout << "; ";

            cout << array[i];
        }

        cout << " } " << endl;
    }

    int getMultiplication(vector<int> vector1, vector<int> vector2, bool isPrint) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        int size = vector1.size();

        int result = 0;

        if (!isPrint) {

            for (int i = 0; i < size; ++i) 
                result += vector1[i] * vector2[i];
            
            return result;
        }

        for (int i = 0; i < size; ++i) {

            SetConsoleTextAttribute(handleConsole, White);
            cout << vector1[i] << " * " << vector2[i];

            if (i != size - 1) {
                SetConsoleTextAttribute(handleConsole, Green);
                cout << " + ";
            }
            else {
                SetConsoleTextAttribute(handleConsole, Blue);
                cout << " = ";

                for (int i = 0; i < size; ++i) {
                    SetConsoleTextAttribute(handleConsole, Yellow);
                    cout << vector1[i] * vector2[i];

                    result += vector1[i] * vector2[i];

                    if (i != size - 1) {
                        SetConsoleTextAttribute(handleConsole, Green);
                        cout << " + ";
                    }
                    else {
                        SetConsoleTextAttribute(handleConsole, Blue);
                        cout << " = ";

                        SetConsoleTextAttribute(handleConsole, Green);
                        cout << result << endl;
                    }
                }
            }
        }

        return result;
    }

public:
    void Init() {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Вычислить скалярное произведение векторов произвольной размерности" << endl << endl;

        vector<int> vector1, vector2;

        MyArray myArray = *new MyArray();
        MyQuestion myQuestion = *new MyQuestion();
        MyInput myInput = *new MyInput();

        bool isRandom = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);
        int dimension = myInput.InputIntData("Введите размерность векторов [по умолчанию " + to_string(myArray.DEFAULT_COL) + "]: ", myArray.MIN_COUNT, myArray.MAX_COUNT, myArray.DEFAULT_COL);
        bool isShowCalc = myQuestion.isQuestion(myQuestion.QUESTION_SHOW_CALC);

        if (isRandom) {
            vector1 = myArray.CreateRandomArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);
            vector2 = myArray.CreateRandomArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);
        }
        else {

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "Введение вектора \"a\"" << endl;
            SetConsoleTextAttribute(handleConsole, White);
            vector1 = myArray.CreateInputArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "\nВведение вектора \"b\"" << endl;
            SetConsoleTextAttribute(handleConsole, White);
            vector2 = myArray.CreateInputArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);
        }

        SetConsoleTextAttribute(handleConsole, Blue);
        cout << "\nРазмерность: " << dimension << endl;

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "Вектора: " << endl;
        PrintVector("a", vector1);
        PrintVector("b", vector2);
       

        SetConsoleTextAttribute(handleConsole, Green);
        
        cout << "\nРезультат умножения векторов:" << endl;

        int result = getMultiplication(vector1, vector2, isShowCalc);

        if(!isShowCalc)
            cout << result << endl;
    }

};


class Task16 {
private:
    struct myPoint {
        int x = 0;
        int y = 0;
    };

    bool isQual(myPoint p1, myPoint p2) {
        return p1.x == p2.x && p1.y == p2.y;
    }

    myPoint InputPoint(string namePoint, int min, int max) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nТочка: " << namePoint << endl;

        MyInput myInput = *new MyInput();
        myPoint point;

        point.x = myInput.InputIntData("Введите X: ", min, max);
        point.y = myInput.InputIntData("Введите Y: ", min, max);

        return point;
    }

    myPoint RandomPoint(int min, int max) {

        MyRandom myRandom = *new MyRandom();

        myPoint point;
        point.x = myRandom.GetRandom(min, max);
        point.y = myRandom.GetRandom(min, max);

        return point;
    }

    void PrintPoint(myPoint point, string namePoint) {
        cout << namePoint << "(" << point.x << "; " << point.y << ")" << endl;
    }

public:
    void Init() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        const int R_DEFAULT = 2;
        const int R_MIN = 1;
        const int R_MAX = 10000;

        int r;
        myPoint m1, m2, m3;

        MyQuestion myQuestion = *new MyQuestion();
        bool isRandom = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);

        if (isRandom) {
            r = R_DEFAULT;

            while (isQual(m1, m2) || isQual(m2, m3)) {
                m1 = RandomPoint(-r, r);
                m2 = RandomPoint(-r, r);
                m3 = RandomPoint(-r, r);
            }
        }
        else {
            MyInput myInput = *new MyInput();
            r = myInput.InputIntData("Введите радиус (R) круга [по умолчанию " + to_string(R_DEFAULT) + "]: ", R_MIN, R_MAX);

            bool isGo = true;

            while (isGo) {
                m1 = InputPoint("Точка M1", -r, r);
                m2 = InputPoint("Точка M2", -r, r);
                m3 = InputPoint("Точка M3", -r, r);

                isGo = isQual(m1, m2) || isQual(m2, m3);

                if (isGo) {
                    SetConsoleTextAttribute(handleConsole, Red);
                    cout << "Точки не должны иметь общие координаты!" << endl;
                    SetConsoleTextAttribute(handleConsole, White);
                }
            }

        }

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "Исходные данные:" << endl;

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "R = " << r << endl;
        PrintPoint(m1, "M1");
        PrintPoint(m2, "M2");
        PrintPoint(m3, "M3");

    }
};


int main()
{
    handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setlocale(LC_ALL, "Russian");

    SetConsoleTextAttribute(handleConsole, White);

    cout << "Тема 5. Подпрограммы" << endl;
    cout << "Ларионов Никита Юрьевич. гр. 110з" << endl;

    bool isGo = true;
    string select;

    while (isGo)
    {
        SetConsoleTextAttribute(handleConsole, White);

        cout << "\nВведите номер задачи" << endl;
        cout << "6)	Вычислить скалярное произведение векторов произвольной размерности" << endl << endl;

        cout << "16) Вычислить полярные координаты для точек, лежащих внутри круга радиусом R с центром в начале координат" << endl << endl;
       

        cout << "26) Поменять местали указанные элементы массива." << endl << endl;

        cout << endl << "Для выхода введите \"0\": ";

        select = GetLine();

        if (select == "6") {
            Task6 task6 = *new Task6();
            task6.Init();
        }
        else if (select == "16") {
            Task16 task16 = *new Task16();
            task16.Init();
        } /*
        else if (select == "26") {
            Task26 task26 = *new Task26();
            task26.Init();
        }*/
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

