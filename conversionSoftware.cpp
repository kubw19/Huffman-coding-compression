// conversionSoftware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>
#include "stdlib.h"
#include <iostream>
#include <cmath>
#include <bitset>
#include "windows.h"
#include <fstream>


using namespace std;
int option;
int testowa;
char name[40];
FILE* plik;
fstream plik2;
int bufor;
int counter;
int id = 257;
int amount = 0;
int tmp, tmp2;
int t;
int intAmount = 0;
int a, b;
int licznik = 1;
int bitsAfterComp;
int lastSize;

struct mdk {
	int father, left, right;
};
struct character {
	unsigned long long int simbol;
	int code;
	char * codeChar;
	int znaki;
	int freq = 0;
};
struct Node {
	int id;
	int simbol;
	int frequency = 0;
	Node* next;
	Node* prev;
};
struct NodeFather {
	int father;
	int left;
	int right;
	NodeFather* next;
	NodeFather* prev;
};

mdk* ptrMdk;
int mdkAmount;
character* codeTable;
Node * ptr, *ptrD;  //wskaźniki pomocnicze
NodeFather *ptrFather, *ptrFatherD;
Node * it;	//wskaźniki iteracyjne
Node * it2;
Node* head; //głowy
NodeFather  *headFather;
Node* tail; //ogony
NodeFather *tailFather;
Node* set; //wskaźnik elementu który się powtarza
Node *s1, *s2;// do zamiany
char * ptrChar;
unsigned int * ptrInt;

bool isset(Node *, Node*);
void insert(Node*, Node**, Node**);
void insertFather(NodeFather*, NodeFather**, NodeFather**);
void display(Node*, Node*);
void displayFather(NodeFather*, NodeFather*);
void sort(Node**, Node**);
int howmany(Node*);
void funkcja(NodeFather*, NodeFather*, int, int);
void makeTDK(Node**, Node**);
void makeTK(NodeFather*, NodeFather*, character**);
void displayTK();
void decode();




int main()
{
	cout << "1. Kompresja pliku" << endl << "2. Dekompresja pliku" << endl;
	cin >> option;
	system("CLS");
	switch (option) {
	case 1:

		head = NULL;
		headFather = NULL;
		amount = 0;
		counter = -1;

		//pierwsze otwarcie pliku
		cout << "Podaj nazwe pliku: " << endl;
		cin >> name;
		cout << endl;
		if (fopen_s(&plik, name, "rb") != 0) {
			cout << "Podany plik nie istnieje!" << endl;
			system("PAUSE");
			return 0;
		};
		while (fread(&bufor, sizeof(unsigned char), 1, plik)) {
			ptr = new Node;
			ptr->simbol = bufor;
			insert(ptr, &head, &tail); // wstawienie nowego symbolu do listy częstości
		}


		sort(&head, &tail);
		sort(&head, &tail);

		//policzenie ile jedno bajtowych znaków jest w pliku
		ptr = head;
		while (ptr != NULL) {
			amount++;
			ptr = ptr->next;
		}
		fclose(plik);
		//zapisanie tablicy częstośći do pliku
		fopen_s(&plik, "czestosc.txt", "w");
		fprintf(plik, "%i\n", howmany(head));
		ptr = head;
		while (ptr != NULL) {
			fprintf(plik, "%i:%i\n", ptr->simbol, ptr->frequency);
			ptr = ptr->next;
		}
		fclose(plik);

		//utworzenie tabeli drzewa kodowania
		makeTDK(&head, &tail);


		//utworzenie TK
		codeTable = new character[amount * 2];//inicjacja tabeli kodowej
		makeTK(headFather, tailFather, &codeTable);


		//kompresja

		//drugie wczytanie pliku w celu kompresji
		fopen_s(&plik, name, "rb");

		licznik = 0;
		//policzenie częśtośći przyszłego wystąpienia zakodowanego słowa po kompresji
		while (fread(&bufor, sizeof(unsigned char), 1, plik)) {
			int i = 0;
			while (codeTable[i].simbol != (int)bufor) {
				i++;
			}
			codeTable[i].freq++;
		}
		for (int i = 0; i < amount; i++) {//policzenie ilości bitów po kompresji
			licznik += codeTable[i].znaki*codeTable[i].freq;
		}

		fclose(plik);

		fopen_s(&plik, name, "rb");

		bitsAfterComp = licznik;
		cout << "SKOMPRESOWANO DO kilobajtow: " << licznik / 8000;
		cout << " bajtow: " << licznik / 8 << " bitow: " << licznik << endl;

		ptrChar = new char[bitsAfterComp]; // tablica znaków na każdy bit po kompresji
		b = 0; //iterator wskazujący na wolne miejsce w ptrChar

		while (fread(&bufor, sizeof(unsigned char), 1, plik)) {
			int i = 0;
			while (codeTable[i].simbol != (int)bufor) {
				i++;
			}						//wypełnienie ptrChar zakodowanym ciągiem
			for (int g = 0; g < codeTable[i].znaki; g++) {
				ptrChar[b] = codeTable[i].codeChar[g];
				b++;
			}
		}
		//
		for (int i = 0; i < bitsAfterComp; i++) {
			cout << ptrChar[i];;
		}



		intAmount = (bitsAfterComp + bitsAfterComp%8)/8;  //wyznaczenie tablicy intów z kodami ASCII znaków do zapisania
		ptrInt = new unsigned int[intAmount];
		for (int i = 0; i < intAmount; i++) {

			tmp = 0;
			t = 0;
			ptrInt[i] = 0;
			for (int k = i * 8; k < (i * 8) + 8; k++) {
				if (ptrChar[i * 8 + 7 - t] == '1') {
					ptrInt[i] += pow(2, t - tmp);
				}
				else if (ptrChar[i * 8 + 7 - t] == '0') 1 == 1;
				else tmp++;
				t++;

			}
			cout << "(" << ptrInt[i] << ")";
		}

		strcat_s(name, ".jwk");
		cout << "Pomyslnie utworzono plik " << name << endl;
		plik2.open(name, ios::out | ios::binary);
		ptrFather = headFather;
		licznik = 0;
		while (ptrFather != NULL) {
			licznik++;
			ptrFather = ptrFather->next;
		}
		ptrFather = headFather;
		plik2.write((char*)&licznik, sizeof(int)); //zapisanie na początek pliku ilość wierszy MDK
		plik2.write((char*)&bitsAfterComp, sizeof(int));//zapisanie na kolejnym miejscu w pliku ilość bitów po kompresji
		while (ptrFather != NULL) {
			plik2.write((char*)&ptrFather->father, sizeof(int));
			plik2.write((char*)&ptrFather->left, sizeof(int));
			plik2.write((char*)&ptrFather->right, sizeof(int));
			ptrFather = ptrFather->next;
		} //zapisanie MDL
		for (int i = 0; i < intAmount; i++) {
			plik2 << (unsigned char)ptrInt[i];
		} //zapisanie skompresowanego pliku
		cout << ptrInt[intAmount];
		plik2.close();
		cout << "asdasd";
		displayTK();
		break;

	case 2:

		cout << "Podaj nazwe pliku do dekompresji: " << endl;
		cin >> name;

		if (fopen_s(&plik, name, "rb") != 0) {
			cout << "Podany plik nie istnieje!" << endl;
			system("PAUSE");
			return 0;
		};

		string name2 = name;
		if (name2.find(".jwk") == string::npos) {
			cout << "Zly format pliku" << endl;
			system("PAUSE");
			return 0;
		}
		codeTable = new character[256];
		ptrChar = new char[256];
		b = 0;
		fread(&bufor, sizeof(int), 1, plik);
		mdkAmount = bufor;
		fread(&bufor, 1, 4, plik);
		bitsAfterComp = bufor;
		ptrMdk = new mdk[mdkAmount];
		for (int i = 0; i < mdkAmount; i++) {
			fread(&ptrMdk[i].father, sizeof(int), 1, plik);
			fread(&ptrMdk[i].left, sizeof(int), 1, plik);
			fread(&ptrMdk[i].right, sizeof(int), 1, plik);

		}
		ptrChar = new char[bitsAfterComp];
		tmp = 0;
		while (fread(&bufor, 1, 1, plik) != NULL) {
			tmp2 = 7;
			for (int g = 0; g < 8; g++) {
				b = bufor % 2;
				if (b == 1)ptrChar[tmp + tmp2] = '1';
				else ptrChar[tmp + tmp2] = '0';
				tmp2 -= 1;
				bufor /= 2;
			}
			tmp += 8;
		}

		for (int i = 0; i < bitsAfterComp; i++) {
			cout << ptrChar[i];
		}
		lastSize = bitsAfterComp % 8;


		decode();

		break;
	}
	system("PAUSE");
	return 0;
}


bool isset(Node *ptr, Node* head) {
	Node * it = head;
	while (it != NULL) {
		if (ptr->simbol == it->simbol) {
			set = it;
			return true;
		}
		it = it->next;
	}
	return false;
}
void insert(Node* ptr, Node** head, Node** tail) {

	ptr->frequency = 1;

	if (*head == NULL)
	{
		*head = ptr;
		*tail = ptr;
		ptr->next = NULL;
		ptr->prev = NULL;
	}

	else {
		if (!isset(ptr, *head)) {
			(*tail)->next = ptr;
			ptr->prev = *tail;
			ptr->next = NULL;
			*tail = ptr;
		}
		else {
			set->frequency++;
			delete ptr;
		}
	}
}
void insertFather(NodeFather* ptr, NodeFather** headFather, NodeFather** tailFather) {


	if (*headFather == NULL)
	{
		*headFather = ptr;
		*tailFather = ptr;
		ptr->next = NULL;
		ptr->prev = NULL;
	}

	else {

		(*tailFather)->next = ptr;
		ptr->prev = *tailFather;
		ptr->next = NULL;
		*tailFather = ptr;


	}
}
void display(Node* head, Node* tail) {
	int c = 0;
	Node* ptrD = head;
	cout << endl << endl << "Display:   head: " << head << "   tail: " << tail << endl;
	cout << "id	|	adress		|	simbol	|frequency	|	next		|	previous	" << endl;
	while (ptrD != NULL) {
		cout << c << "	|	" << ptrD << "	|	" << ptrD->simbol << "	|	" << ptrD->frequency << "	|	" << ptrD->next << "	|	" << ptrD->prev << endl;
		c++;
		ptrD = ptrD->next;
	}


}
void displayFather(NodeFather* headFather, NodeFather* tailFather) {
	int c = 0;
	NodeFather* ptrFatherD = headFather;
	cout << endl << endl << "DisplayFather:    head: " << headFather << "   tail: " << tailFather << endl;
	cout << "id	|	adress		|	father	|	left		|	right	" << endl;
	while (ptrFatherD != NULL) {
		cout << c << "	|	" << ptrFatherD << "	|	" << ptrFatherD->father << "	|	" << ptrFatherD->left << "	|	" << ptrFatherD->right << "	|	" << ptrFatherD->next << "|" << ptrFatherD->prev << endl;
		c++;
		ptrFatherD = ptrFatherD->next;
	}
}
void sort(Node** head, Node** tail) {
	Node* s1, *s2;
	Node* it2 = *head;
	while (it2 != NULL) {
		it = *head;
		while (it->next != NULL) {

			if (it->frequency < it->next->frequency && it == *head && it->next == *tail) {

				s1 = it->next;
				s2 = it->prev;
				it->next = it->next->next;
				it->prev = s1;
				s1->next = it;
				s1->prev = NULL;
				*head = s1;
				*tail = it;

			}

			else if (it->frequency < it->next->frequency && it == *head) {

				*head = it->next;
				s1 = it->next;
				s2 = it->prev;
				it->next = s1->next;
				s1->next = it;
				s1->prev = s2;
				it->next->prev = it;
				it->prev = s1;

			}


			else if (it->frequency < it->next->frequency && it->next == *tail) {
				*tail = it;
				s1 = it->next;
				s2 = it->prev;
				it->prev->next = it->next;
				it->next->next = it;
				it->next = NULL;
				it->prev = s1;
				s1->prev = s2;
			}
			else if (it->next != NULL && it->frequency < it->next->frequency) {
				s1 = it->next;
				s2 = it->prev;
				it->prev->next = s1;
				it->next = it->next->next;
				s1->next = it;
				it->next->prev = it;
				it->prev = s1;
				s1->prev = s2;
			}

			else {
				it = it->next;
			}
		}
		it2 = it2->next;
	}

}
int howmany(Node* head) {
	ptr = head;
	int counter = 0;
	while (ptr != NULL) {
		counter++;
		ptr = ptr->next;
	}
	return counter;
}
void funkcja(NodeFather* ptr, NodeFather* headFather, int bytes, int znaki) {
	NodeFather* ptr2;

	if (ptr->left > 256 && ptr->right <= 256) {
		ptr2 = headFather;
		while (ptr2->father != ptr->left) {
			ptr2 = ptr2->next;
		}
		counter++;
		bytes *= 2;
		codeTable[counter].simbol = ptr->right;
		codeTable[counter].code = bytes + 1;
		codeTable[counter].znaki = znaki;

		funkcja(ptr2, headFather, bytes, znaki + 1);
	}

	else if (ptr->left <= 256 && ptr->right > 256) {
		ptr2 = headFather;
		while (ptr2->father != ptr->right) {
			ptr2 = ptr2->next;
		}
		counter++;
		codeTable[counter].simbol = ptr->left;
		codeTable[counter].znaki = znaki;
		bytes *= 2;
		codeTable[counter].code = bytes;
		bytes++;
		funkcja(ptr2, headFather, bytes, znaki + 1);
	}
	else if (ptr->left > 256 && ptr->right > 256) {
		bytes *= 2;
		ptr2 = headFather;
		while (ptr2->father != ptr->left) {
			ptr2 = ptr2->next;
		}

		funkcja(ptr2, headFather, bytes, znaki + 1);
		bytes++;
		ptr2 = headFather;
		while (ptr2->father != ptr->right) {
			ptr2 = ptr2->next;
		}

		funkcja(ptr2, headFather, bytes, znaki + 1);

	}
	else if (ptr->left <= 256 && ptr->right <= 256) {
		bytes *= 2;
		counter++;
		codeTable[counter].simbol = ptr->left;
		codeTable[counter].code = bytes;
		codeTable[counter].znaki = znaki;
		counter++;
		codeTable[counter].simbol = ptr->right;
		codeTable[counter].code = bytes + 1;
		codeTable[counter].znaki = znaki;
	}
}
void makeTDK(Node** head, Node** tail) {
	ptr = *tail;
	while (ptr->prev != NULL) {
		sort(&*head, &*tail);
		sort(&*head, &*tail);
		ptr = *tail;
		ptrFather = new NodeFather;
		ptrFather->left = ptr->simbol;
		ptrFather->right = ptr->prev->simbol;
		ptrFather->father = id;
		insertFather(ptrFather, &headFather, &tailFather);
		ptr->prev->frequency += ptr->frequency;
		ptr->prev->next = NULL;
		ptr->prev->simbol = id;
		*tail = ptr->prev;
		ptr = *tail;
		id++;
	}
}
void makeTK(NodeFather* headFather, NodeFather* tailFathe, character** codeTable) {


	funkcja(tailFather, headFather, 0, 1);

	for (int i = 0; i < amount; i++) {
		char zapas2, *zapas;
		int a = (*codeTable)[i].code;
		(*codeTable)[i].codeChar = new char[(*codeTable)[i].znaki];
		zapas = new char[(*codeTable)[i].znaki];
		for (int g = 0; g < (*codeTable)[i].znaki; g++) {
			b = a % 2;
			if (b == 1)strcpy_s((*codeTable)[i].codeChar + g, 4, "1");
			else strcpy_s((*codeTable)[i].codeChar + g, 4, "0");
			a /= 2;
		}

		for (int g = 0; g < (*codeTable)[i].znaki; g++) {
			zapas2 = *((*codeTable)[i].codeChar + ((*codeTable)[i].znaki - 1 - g));
			if (zapas2 == '1')strcpy_s(zapas + g, 4, "1");
			else  strcpy_s(zapas + g, 4, "0");

		}

		for (int k = 0; k < (*codeTable)[i].znaki; k++) {
			(*codeTable)[i].codeChar[k] = zapas[k];
		}
	}
}
void displayTK() {
	for (int i = 0; i < amount; i++) {
		int a = codeTable[i].code;
		bitset<65> x(a);
		cout << codeTable[i].simbol << "	:	" << x << " znaki: " << codeTable[i].znaki << " size: " << codeTable[i].codeChar << endl;
	}
}
void decode() {
	fstream plik;

	string nazwa = name;
	nazwa = nazwa.substr(0, nazwa.length() - 4);
	
	plik.open(nazwa, ios::out | ios::binary);
	int ptr = mdkAmount - 1;
	int ptr2;
	int i;
	for (i = 0; i < bitsAfterComp+bitsAfterComp % 8; i++) {
		if (ptrChar[i] == '1' && ptrMdk[ptr].right < 256) {
			plik << char(ptrMdk[ptr].right);
			ptr = mdkAmount - 1;
		}
		else if (ptrChar[i] == '0' && ptrMdk[ptr].left < 256) {
			plik << char(ptrMdk[ptr].left);
			ptr = mdkAmount - 1;
		}
		else if (ptrChar[i] == '1' && ptrMdk[ptr].right >= 256) {
			ptr2 = 0;
			while (ptrMdk[ptr2].father != ptrMdk[ptr].right)ptr2++;
			ptr = ptr2;
		}
		else if (ptrChar[i] == '0' && ptrMdk[ptr].left >= 256) {
			ptr2 = 0;
			while (ptrMdk[ptr2].father != ptrMdk[ptr].left)ptr2++;
			ptr = ptr2;
		}
		if (i == (bitsAfterComp - bitsAfterComp % 8 - 1)) {
			i += 8 - lastSize;
			cout <<endl<<"Faktycznie bitow: "<< bitsAfterComp;
			cout <<endl<<"zaokraglenie: "<<bitsAfterComp+(8 - bitsAfterComp%8)<<endl<<"Pozycja "<< bitsAfterComp + (8 - bitsAfterComp % 8) -8 << " gdzie skaczemy o "<<8-bitsAfterComp%8<<endl;
		}

	}
	cout << "Pomyslnie rozpakowano plik " << nazwa << endl << endl;
}