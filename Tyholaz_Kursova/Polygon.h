#pragma once

#include "stdafx.h"

using namespace std;
using namespace System;

namespace Tyholaz_Kursova
{

	ref class Polygon
	{
	private:
		size_t anglesNumber;
		int maxLength;
		System::Array ^points;
		System::Array ^circles;
		vector<int> *sidesLengths;
		vector<int> *diagonalsLengths;

		int findMediumY();
		int findMediumX();
        int countVectorAbs(System::Drawing::Point ^pA, System::Drawing::Point ^pB);
		int findTheLongestDiagonalLength();
		void findCoordsInLine(string line, int* coords);
		bool checkIntersection(System::Drawing::Point centerA, System::Drawing::Point centerB, int radius);

		void createCircles();
	public:
		Polygon(void);

		void readMaxLengthFromFile(string path);
		void readCoordFromFile(string path);
		void drawPolygon(System::Windows::Forms::PaintEventArgs^ e);
		void countSidesLength();
		void countDiagonalsLength();
		void drawEllipses(System::Windows::Forms::PaintEventArgs^ e);
		void drawEllipse(System::Windows::Forms::PaintEventArgs^ e, System::Drawing::Pen^ pen, System::Drawing::Point a, int radius);
	};
}