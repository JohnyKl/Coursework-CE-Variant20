#include "stdafx.h"
#include "Circle.h"

using namespace Tyholaz_Kursova;

Circle::Circle(void)
{
	center = gcnew Point();
	radius = 0.0f;

	intersection = false;
	toDraw = false;
}

Circle::Circle(Point ^center, int radius)
{
	this->center = center;
	this->radius = radius;
}

void Circle::checkDistanseToCloserVertexes(int left, int right, int maxDistance)
{
	if(left < maxDistance || right < maxDistance) toDraw = true;
}

void Circle::checkIntersection(System::Array ^vertexes)
{
	for(int i = 0; i < vertexes->Length && !intersection; i++)
	{
		Point ^currentCenter = (Point^)vertexes->GetValue(i);

		int len = System::Math::Sqrt((center->X - currentCenter->X)*(center->X - currentCenter->X) + (center->Y - currentCenter->Y)*(center->Y - currentCenter->Y));//обрахувати відстань між центрами кіл

		if(len <= radius*2 && len > 0)//кола перетинаються, якщо відстань між їх точками знаходиться між сумою та різницею їх радіусів
		{
			intersection = true;
		}	
	}
}

void Circle::drawEllipse(PaintEventArgs^ e)
{
	if(toDraw)
	{
		Pen ^pen = gcnew Pen(Color::Red, 3.0f);

		if(intersection)
		{
			pen = gcnew Pen(Color::Green, 3.0f);
		}

		 e->Graphics->DrawEllipse(pen, (int)(center->X - radius), (int)(center->Y - radius), (int)radius*2, (int)radius*2);
	}
}