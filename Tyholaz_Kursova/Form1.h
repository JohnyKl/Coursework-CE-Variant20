#pragma once

#include "stdafx.h"
#include "Polygon.h"

namespace Tyholaz_Kursova {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

		Polygon^ p;

	public:
		Form1(void)
		{
			InitializeComponent();

			p = gcnew Polygon();

			p->readCoordFromFile("coords.txt");//виклик функції считування координат з файлу
			p->readMaxLengthFromFile("side_length.txt");//виклик функції считування максимальної довжини сторони з файлу
			p->countSidesLength();//виклик функції обчислення довжин сторін
			p->countDiagonalsLength();//виклик функцій обчислення довжин діагоналей багатокутника
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1190, 632);
			this->panel1->TabIndex = 0;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel1_Paint);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1214, 656);
			this->Controls->Add(this->panel1);
			this->Name = L"Form1";
			this->Text = L"Тихолаз Анатолій. 1КІ-14мс";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
			 {

			 }
	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
			 {
				 p->drawPolygon(e);//виклик функції малювання багатокутника
				 p->drawEllipses(e);//виклик функції малюванні кіл навколо потрібних вершин багатокутника
			 }
	};
}

