#include "stdafx.h"
#include "Polygon.h"


//using namespace std;
using namespace System;
using namespace Tyholaz_Kursova;
using namespace System::Drawing;

Polygon::Polygon(void)//конструктор класу
{
    anglesNumber = 0;
    maxLength = 0.0f;
    sidesLengths = new std::vector<int>();
    diagonalsLengths = new std::vector<int>();
}

int Polygon::findMediumY()//знаходить середнє значення по У
{
	Point currentPoint = (Point) points->GetValue(0);
	int length = points->Length;
	int maxY = currentPoint.Y;
	int minY = currentPoint.Y;
	
	for(int i = 1; i < length - 1; i ++)
	{
		currentPoint = (Point) points->GetValue(i);

		if(maxY < currentPoint.Y)
		{
			maxY = currentPoint.Y;
		}
		if(minY > currentPoint.Y)
		{
			minY = currentPoint.Y;
		}		
	}

	return (int)(maxY + minY)/2;	
}
int Polygon::findMediumX()//знаходить середнє значення по Х
{
	Point currentPoint = (Point) points->GetValue(0);
	int length = points->Length;
	int maxX = currentPoint.X;
	int minX = currentPoint.X;

	for(int i = 1; i < length - 1; i ++)
	{
		currentPoint = (Point) points->GetValue(i);
				
		if(maxX < currentPoint.X)
		{
			maxX = currentPoint.X;
		}
		if(minX > currentPoint.X)
		{
			minX = currentPoint.X;
		}
	}

	return (int)(maxX + minX)/2;
}

void Polygon::readCoordFromFile(std::string path)// функція, що зчитує з заданого стрічкою файлу координати
{
    std::ifstream ifs(path); //створення файлового потоку вводу

    std::vector<int*> coords;//створення структури вектору, у яку додаватимуться координати

    if(ifs.is_open())//якщо файловий опток відкрито
    {
        while (! ifs.eof() )//доки не досягнуто кінця файлу
        {
            string line = "";//оголошення та ініціалізація стрічки для зчитування з файлу
			getline(ifs,line);//отримання однієї стрічки з файлу
			
			int* coordsFromLine = new int[2];//оголошення та ініціалізація масиву для збереження зчитаних значень координати
			findCoordsInLine(line, coordsFromLine);//знайти координати в стрічці

			if(coordsFromLine[0] != -1 && coordsFromLine[1] != -1)//якщо координату знайдено
			{
				coords.push_back(coordsFromLine);//додати координату в вектор
			}		
        }

        int pointsNumber = coords.size();//кількість координат обчислюється як загальна кількість значень Х та У

        Polygon::points = Array::CreateInstance(Point::typeid, pointsNumber);//ініціалізація масиву з даними типу Point

        for (int i = 0; i < pointsNumber; i++)
        {
            points->SetValue(Point(coords[i][0],coords[i][1]),i);//додати у масив точок нову створену точку з двох послідовних значень осей ординати та абсциси
        }
    }

    ifs.close();//закриття файлового потоку

	//sortCoords();//відсортувати координати
}

void Polygon::findCoordsInLine(string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //знайти позицію першого символа після першого входження символа "="
	size_t posY = line.find_last_of("=") + 1;//знайти позицію першого символа після останнього входження символа "="

	coords[0] = -1;//ініціалізувати значеннями масив точок для ідентифікації відсутності знайдених координат
	coords[1] = -1;

	if(posX > 0 && posY > 0)//якщо символи знайдено
	{
		size_t countX = line.find_first_of(",") - posX;//знайти індекс першого входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа
		size_t countY = line.length() - posY;//отримати довжину стрічки та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа		

		if(countX > 0 && countY > 0)//якщо символи знайдено
		{
			int x = stoi(line.substr(posX,countX));//перетворити в число потрібну частину стрічки з координатами
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//заповнити масив значеннями
			coords[1] = y;
		}
	}
}


void Polygon::readMaxLengthFromFile(string path)//считати максимальну задану довжину сторони для порівняння з файлу
{
    std::ifstream ifs(path); // відкриття файлового потоку

    if(ifs.is_open())//якщо потік відкрито
    {
        while (! ifs.eof() )//доки не досягнуто кінця файлу
        {
            string line = "";//оголошення та ініціалізація стрічки для зчитування з файлу
			getline(ifs,line);//отримання однієї стрічки з файлу
			
			int* coordsFromLine = new int[2];//оголошення та ініціалізація масиву для збереження зчитаних значень координати
			findCoordsInLine(line, coordsFromLine);//знайти координати в стрічці

			Polygon::maxLength = coordsFromLine[0];//присвоєння значення полю класу
        }
    }

    ifs.close();//закриття файлового потоку
}

void Polygon::drawPolygon(System::Windows::Forms::PaintEventArgs^  e)//функція, що малює багатокутник
{
    Pen^ blackPen = gcnew Pen( Color::Black,3.0f);//створення олівця

    int length = Polygon::points->Length - 1;////оголошення і ініціалізація змінної довжиною масиву точок

	array<Point>^ vertexes = (array<Point>^)points;

	e->Graphics->DrawPolygon(blackPen, vertexes);
}

void Polygon::drawEllipses(System::Windows::Forms::PaintEventArgs^ e)//фунція, що малює кола навколо вершин, що відповідають заданим завданням параметрам
{
    int radius = Polygon::findTheLongestDiagonalLength()/2;//ініціалізація і оголошення змінної радіусу як половина найдовшої діагоналі

    int *ptrToElement = (int*)Polygon::sidesLengths->data();//посилання на початок вектору зі значеннями довжин сторін

    for(int i = 1; i < Polygon::sidesLengths->size(); i++)
    {
        //оголошенняя і ініціалізація елементів, яким надаються значення довжин сторін справа та зліва від і-ї точки
        int currentElementLeft  = ptrToElement[i - 1];
        int currentElementRight = ptrToElement[i];

        //створення олівця стандартного кольору для малювання кіл
        Pen^ pen = gcnew Pen(Color::Red, 3.0f);

        if (currentElementLeft/2 < radius || currentElementRight/2 < radius)//якщо колa будуть перетинатись, змінити колір олівця
        {
            pen = gcnew Pen(Color::Green, 3.0f);
        }
        if(currentElementLeft < Polygon::maxLength)//якщо довжина сторони відповідає заданій умові, намалювати коло навколо і-ї та попередньої вершини
        {
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i),radius);
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i - 1),radius);
        }
        if(currentElementRight < Polygon::maxLength)//якщо довжина сторони відповідає заданій умові, намалювати коло навколо і-ї та наступної вершини
        {
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i),radius);
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i + 1),radius);
        }
    }
}

void Polygon::drawEllipse(System::Windows::Forms::PaintEventArgs^ e, Pen^ pen, Point a, int radius)//намалювати коло з центром у заданій вершині відповідного радіусу
{
    e->Graphics->DrawEllipse(pen, (int)(a.X - radius), (int)(a.Y - radius), (int)radius*2, (int)radius*2);
}

void Polygon::countSidesLength()//функція, що обчислює довжини сторін та додає їх у вектор довжин сторін
{
    for(int i = 0; i < Polygon::points->Length - 1; i++)
    {
        int sideLength = Polygon::countVectorAbs((Point^)Polygon::points->GetValue(i), (Point^)Polygon::points->GetValue(i + 1));

        sidesLengths->push_back(sideLength);
    }
}

int Polygon::countVectorAbs(Point ^pA, Point ^pB)//функція, що обраховує довжину вектору (модуль вектору)
{
    int sqrX = (pB->X - pA->X)*(pB->X - pA->X);
    int sqrY = (pB->Y - pA->Y)*(pB->Y - pA->Y);

    return Math::Sqrt(sqrX + sqrY);
}

void Polygon::countDiagonalsLength()//функція, що обраховує довжини всіх діагоналей, яких у багатокутнику n*(n-3)/2, де n кількість вершин багатокутника
{
    int length = Polygon::points->Length - 1;

    for(int i = 0; i < length; i++)
    {
        for(int j = i + 2; j < length + 1; j++)
        {
            if((j == length) && (i == 0)){ }
            else
            {
                int diagonalLength = Polygon::countVectorAbs((Point^)Polygon::points->GetValue(i), (Point^)Polygon::points->GetValue(j));

                diagonalsLengths->push_back(diagonalLength);
            }
        }
    }
}

int Polygon::findTheLongestDiagonalLength()//функція, що знаходить довжину найдовшої діагоналі з усіх діагоналей багатокутника
{
    int theLongestDiagonalLength = Polygon::diagonalsLengths->front();//присвоєння найбільшій величині величини першого елемента вектору

    int *ptrToElement = (int*)Polygon::diagonalsLengths->data();//вказівник на початок даних в векторі

    for(int i = 0; i < Polygon::diagonalsLengths->size(); i++)
    {
        int currentElement = ptrToElement[i];//отримуємо значення і-того елемента

        if(currentElement > theLongestDiagonalLength)//якщо і-ий елемент більший "найбільшого", присвоюєм його значення "найбільшому"
        {
            theLongestDiagonalLength = currentElement;
        }
    }

    return theLongestDiagonalLength;
}

