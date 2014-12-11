#pragma once
#include "stdafx.h"

using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace Tyholaz_Kursova
{
	ref class Circle
	{
	public:
		Point ^center;
		int radius;

		bool intersection;
		bool toDraw;

		Circle(void);
		Circle(Point ^center, int radius);

		void checkDistanseToCloserVertexes(int left, int right, int maxDistance);
		void checkIntersection(System::Array ^vertexes);
		void drawEllipse(PaintEventArgs^ e);
	};
}