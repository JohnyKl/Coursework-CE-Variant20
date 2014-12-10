#include "stdafx.h"
#include "Polygon.h"


//using namespace std;
using namespace System;
using namespace Tyholaz_Kursova;
using namespace System::Drawing;

Polygon::Polygon(void)//����������� �����
{
    anglesNumber = 0;
    maxLength = 0.0f;
    sidesLengths = new std::vector<int>();
    diagonalsLengths = new std::vector<int>();
}

int Polygon::findMediumY()//��������� ������ �������� �� �
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
int Polygon::findMediumX()//��������� ������ �������� �� �
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

void Polygon::readCoordFromFile(std::string path)// �������, �� ����� � �������� ������� ����� ����������
{
    std::ifstream ifs(path); //��������� ��������� ������ �����

    std::vector<int*> coords;//��������� ��������� �������, � ��� �������������� ����������

    if(ifs.is_open())//���� �������� ����� �������
    {
        while (! ifs.eof() )//���� �� ��������� ���� �����
        {
            string line = "";//���������� �� ����������� ������ ��� ���������� � �����
			getline(ifs,line);//��������� ���� ������ � �����
			
			int* coordsFromLine = new int[2];//���������� �� ����������� ������ ��� ���������� �������� ������� ����������
			findCoordsInLine(line, coordsFromLine);//������ ���������� � ������

			if(coordsFromLine[0] != -1 && coordsFromLine[1] != -1)//���� ���������� ��������
			{
				coords.push_back(coordsFromLine);//������ ���������� � ������
			}		
        }

        int pointsNumber = coords.size();//������� ��������� ������������ �� �������� ������� ������� � �� �

        Polygon::points = Array::CreateInstance(Point::typeid, pointsNumber);//����������� ������ � ������ ���� Point

        for (int i = 0; i < pointsNumber; i++)
        {
            points->SetValue(Point(coords[i][0],coords[i][1]),i);//������ � ����� ����� ���� �������� ����� � ���� ���������� ������� ���� �������� �� �������
        }
    }

    ifs.close();//�������� ��������� ������

	//sortCoords();//����������� ����������
}

void Polygon::findCoordsInLine(string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //������ ������� ������� ������� ���� ������� ��������� ������� "="
	size_t posY = line.find_last_of("=") + 1;//������ ������� ������� ������� ���� ���������� ��������� ������� "="

	coords[0] = -1;//������������ ���������� ����� ����� ��� ������������� ��������� ��������� ���������
	coords[1] = -1;

	if(posX > 0 && posY > 0)//���� ������� ��������
	{
		size_t countX = line.find_first_of(",") - posX;//������ ������ ������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����
		size_t countY = line.length() - posY;//�������� ������� ������ �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����		

		if(countX > 0 && countY > 0)//���� ������� ��������
		{
			int x = stoi(line.substr(posX,countX));//����������� � ����� ������� ������� ������ � ������������
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//��������� ����� ����������
			coords[1] = y;
		}
	}
}


void Polygon::readMaxLengthFromFile(string path)//������� ����������� ������ ������� ������� ��� ��������� � �����
{
    std::ifstream ifs(path); // �������� ��������� ������

    if(ifs.is_open())//���� ���� �������
    {
        while (! ifs.eof() )//���� �� ��������� ���� �����
        {
            string line = "";//���������� �� ����������� ������ ��� ���������� � �����
			getline(ifs,line);//��������� ���� ������ � �����
			
			int* coordsFromLine = new int[2];//���������� �� ����������� ������ ��� ���������� �������� ������� ����������
			findCoordsInLine(line, coordsFromLine);//������ ���������� � ������

			Polygon::maxLength = coordsFromLine[0];//��������� �������� ���� �����
        }
    }

    ifs.close();//�������� ��������� ������
}

void Polygon::drawPolygon(System::Windows::Forms::PaintEventArgs^  e)//�������, �� ����� ������������
{
    Pen^ blackPen = gcnew Pen( Color::Black,3.0f);//��������� �����

    int length = Polygon::points->Length - 1;////���������� � ����������� ����� �������� ������ �����

	array<Point>^ vertexes = (array<Point>^)points;

	e->Graphics->DrawPolygon(blackPen, vertexes);
}

void Polygon::drawEllipses(System::Windows::Forms::PaintEventArgs^ e)//������, �� ����� ���� ������� ������, �� ���������� ������� ��������� ����������
{
    int radius = Polygon::findTheLongestDiagonalLength()/2;//����������� � ���������� ����� ������ �� �������� �������� �������

    int *ptrToElement = (int*)Polygon::sidesLengths->data();//��������� �� ������� ������� � ���������� ������ �����

    for(int i = 1; i < Polygon::sidesLengths->size(); i++)
    {
        //����������� � ����������� ��������, ���� ��������� �������� ������ ����� ������ �� ���� �� �-� �����
        int currentElementLeft  = ptrToElement[i - 1];
        int currentElementRight = ptrToElement[i];

        //��������� ����� ������������ ������� ��� ��������� ��
        Pen^ pen = gcnew Pen(Color::Red, 3.0f);

        if (currentElementLeft/2 < radius || currentElementRight/2 < radius)//���� ���a ������ ������������, ������ ���� �����
        {
            pen = gcnew Pen(Color::Green, 3.0f);
        }
        if(currentElementLeft < Polygon::maxLength)//���� ������� ������� ������� ������ ����, ���������� ���� ������� �-� �� ���������� �������
        {
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i),radius);
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i - 1),radius);
        }
        if(currentElementRight < Polygon::maxLength)//���� ������� ������� ������� ������ ����, ���������� ���� ������� �-� �� �������� �������
        {
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i),radius);
            Polygon::drawEllipse(e, pen, (Point)Polygon::points->GetValue(i + 1),radius);
        }
    }
}

void Polygon::drawEllipse(System::Windows::Forms::PaintEventArgs^ e, Pen^ pen, Point a, int radius)//���������� ���� � ������� � ������ ������ ���������� ������
{
    e->Graphics->DrawEllipse(pen, (int)(a.X - radius), (int)(a.Y - radius), (int)radius*2, (int)radius*2);
}

void Polygon::countSidesLength()//�������, �� �������� ������� ����� �� ���� �� � ������ ������ �����
{
    for(int i = 0; i < Polygon::points->Length - 1; i++)
    {
        int sideLength = Polygon::countVectorAbs((Point^)Polygon::points->GetValue(i), (Point^)Polygon::points->GetValue(i + 1));

        sidesLengths->push_back(sideLength);
    }
}

int Polygon::countVectorAbs(Point ^pA, Point ^pB)//�������, �� �������� ������� ������� (������ �������)
{
    int sqrX = (pB->X - pA->X)*(pB->X - pA->X);
    int sqrY = (pB->Y - pA->Y)*(pB->Y - pA->Y);

    return Math::Sqrt(sqrX + sqrY);
}

void Polygon::countDiagonalsLength()//�������, �� �������� ������� ��� ���������, ���� � ������������� n*(n-3)/2, �� n ������� ������ �������������
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

int Polygon::findTheLongestDiagonalLength()//�������, �� ��������� ������� �������� ������� � ��� ��������� �������������
{
    int theLongestDiagonalLength = Polygon::diagonalsLengths->front();//��������� ��������� ������� �������� ������� �������� �������

    int *ptrToElement = (int*)Polygon::diagonalsLengths->data();//�������� �� ������� ����� � ������

    for(int i = 0; i < Polygon::diagonalsLengths->size(); i++)
    {
        int currentElement = ptrToElement[i];//�������� �������� �-���� ��������

        if(currentElement > theLongestDiagonalLength)//���� �-�� ������� ������ "����������", ��������� ���� �������� "����������"
        {
            theLongestDiagonalLength = currentElement;
        }
    }

    return theLongestDiagonalLength;
}

