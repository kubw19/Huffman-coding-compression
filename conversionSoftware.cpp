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
int zmienna;
int bajtow = 0;
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
unsigned int crc, buforu;


static const unsigned int crc32_table[] =
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};
unsigned int CalculateCRC32(char *fileName) {
	unsigned int crc = 0xffffffff;
	const int inputBufferSize = 256;
	//bufor wejsciowy, do którego będą wczytywane bajty pliku wejściowego
	unsigned char inputBuffer[inputBufferSize];
	//licznik bajtów odczytanych z pliku wejściowego
	int bytesRead;
	FILE *inputFileHandle;
	fopen_s(&inputFileHandle, fileName, "rb");

	fseek(inputFileHandle, 4, 0);

	while (bytesRead = fread(inputBuffer, sizeof(unsigned char), inputBufferSize, inputFileHandle)) {
		for (int i = 0; i<bytesRead; i++) {
			crc = crc32_table[(crc ^ (unsigned int)inputBuffer[i]) & 0xFF] ^ (crc >> 8);
		}
	}
	fclose(inputFileHandle);
	return crc ^ 0xffffffff;
}

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
		if (licznik % 8 == 0)bajtow = licznik / 8;
		else bajtow = licznik / 8 + 1;

		cout << " bajtow: " << bajtow << " bitow: " << licznik << endl;

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
			//cout << ptrChar[i];;
		}



		intAmount = bajtow;  //wyznaczenie tablicy intów z kodami ASCII znaków do zapisania
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
			//cout << "(" << ptrInt[i] << ")";
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
		plik2.write("1", sizeof(int)); //zarezerwowanie miejsca
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
		//cout << ptrInt[intAmount];
		plik2.close();
		crc = CalculateCRC32(name);
		plik2.open(name, ios::out | ios::in | ios::binary);
		plik2.seekp(0, ios::beg);
		cout <<hex<< crc<<endl;
		plik2.write((char*)&crc, 4);
		plik2.close();
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
		fread(&buforu, 1, 4, plik);
		crc = buforu;
		fclose(plik);
		if (crc != CalculateCRC32(name)){
			cout << "Niezgodnosc crc! Plik prawdopodobnie uszkodzony. Uruchom program ponownie." << endl;
			system("PAUSE");
			return 0;
		};

		

		fopen_s(&plik, name, "rb");
		string name2 = name;
		if (name2.find(".jwk") == string::npos) {
			cout << "Zly format pliku" << endl;
			system("PAUSE");
			return 0;
		}
		codeTable = new character[256];
		ptrChar = new char[256];
		b = 0;
		fread(&bufor, 1, 4, plik);
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
		
		if (bitsAfterComp % 8 == 0)zmienna = bitsAfterComp;
		else zmienna = bitsAfterComp - bitsAfterComp % 8 + 8;
		ptrChar = new char[zmienna];
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
	for (i = 0; i < zmienna; i++) {
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
			i += 8 - bitsAfterComp % 8;
			cout <<endl<<"Faktycznie bitow: "<< bitsAfterComp;
			cout <<endl<<"zaokraglenie: "<<bitsAfterComp+(8 - bitsAfterComp%8)<<endl<<"Pozycja "<< bitsAfterComp + (8 - bitsAfterComp % 8) -8 << " gdzie skaczemy o "<<8-bitsAfterComp%8<<endl;
		}

	}
	cout << "Pomyslnie rozpakowano plik " << nazwa << endl << endl;
}