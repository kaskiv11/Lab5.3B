/*
1. Програма має дати  користувачеві  можливість  фізично  впорядкувати  файл  в  першу чергу – за значенням оцінки з фізики,
   в другу чергу (для однакових оцінок з фізики) – за  номером  курсу,
   в  останню  чергу  (для  однакових  оцінок  з  фізики  і  курсів) – за прізвищем за спаданням – в зворотному до алфавітного порядку.
2. Програма має будувати  індексний  файл,  який  забезпечує  наступне  індексне впорядкування:  в  першу  чергу – за  значенням  оцінки з  фізики,
   в  другу  чергу  (для однакових  оцінок  з  фізики) – за  номером  курсу,
   в  останню  чергу  (для  однакових оцінок з фізики і курсів) – за прізвищем.
3. За  допомогою  бінарного  пошуку  визначити,  чи  навчається  студент  із  вказаним користувачем прізвищем на вказаному курсі та вказаною оцінкою з фізики.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>

using namespace std;

enum Specialty { KN, INF, ME, PI, TD };

string specialtyStr[] = { "Комп'ютерні науки", "Інформатика", "Математика та екноміка", " Фізика та інформатика", "Трудове навчання" };


struct Student {
	char prizv[64];
	unsigned kurs;
	Specialty specialty;
	unsigned gradePhys;
	int gradeMath;
	union {
		unsigned gradeProg;
		unsigned gradeMetod;
		unsigned gradePed;
	};
};


void Create(const char* fName);
void Print(const char* fName);
void fWrite(fstream& f, const int i, Student x);
Student fRead(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);
void Sort(const char* fName);
void indexSort(const char* fName, const char* gName);
void indexPrint(const char* fName, const char* gName);
int binSearch(const char* fName, const char* prizv, const int kurs, const int gradePhys);


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char f[64];
	char g[6] = "g.dat";
	char prizv[64];
	int kurs, gradePhys, binNum;

	int menuItem;
	do {
		cout << "Оберіть дію: " << endl;
		cout << "[1]  введення даних з клавіатури." << endl;
		cout << "[2]  вивід даних на екран." << endl;
		cout << "[3]  сортувати масив за заданими ключами(фізичне сортування)." << endl;
		cout << "[4]  сортувати масив за заданими ключами(індексне сортування) та вивід масиву." << endl;
		cout << "[5]  бінарний пошук." << endl;
		cout << "[6]  ввести назву файла." << endl;
		cout << "[0]  вихід та завершення роботи програми" << endl << endl;
		cout << "Введіть значення: "; cin >> menuItem;
		switch (menuItem) {
		case 1:
			Create(f);
			break;
		case 2:
			Print(f);
			break;
		case 3:
			Sort(f);
			break;
		case 4:
			indexSort(f, g);
			indexPrint(f, g);
			break;
		case 5:
			cout << "Перевірка наявності студента за прізвищем, курсом, та оцінкою з фізики."<<endl;
			cout << "Введіть прізвище: "; cin >> prizv;
			cout << "Введіть курс: "; cin >> kurs;
			cout << "Введіть оцінку з фізики: "; cin >> gradePhys;
			binNum = binSearch(f, prizv, kurs, gradePhys);
			if (binNum == -1) {
				cout << "Не знайдено студента з такими характеристиками!!!" << endl;
			}
			else {
				cout << "Номер цього студента в списку: " << binNum << endl;
			}
			break;
		case 6:
			cout << "Введіть назву файла: "; cin >> f;
			break;
		case 0:
			break;
		default:
			cout << "Ви ввели помилковезначення! Слід ввести число - номер вибраного пункту меню" << endl;
		}
	} while (menuItem != 0);



	system("pause");
	cin.get();
	return 0;
}

void Create(const char* fName)
{
	ofstream f(fName, ios::binary);
	Student student;
	char ch;
	int specialty;
	do {

		cout << "Прізвище: ";
		cin >> student.prizv;
		cout << "Курс: "; cin >> student.kurs;
		cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та екноміка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
		cin >> specialty;
		student.specialty = (Specialty)specialty;
		cout << "Оцінка з фізики: "; cin >> student.gradePhys;
		cout << "Оцінка з математики: "; cin >> student.gradeMath;
		switch (student.specialty) {
		case KN:
			cout << "Оцінка з програмування: "; cin >> student.gradeProg;
			break;
		case INF:
			cout << "Оцінка з чисельних методоів: "; cin >> student.gradeMetod;
			break;
		default:
			cout << "Оцінка педагогіки: "; cin >> student.gradePed;
			break;
		}

		cout << endl;
		if (!f.write((char*)&student, sizeof(Student))) {
			cerr << "Error writing file." << endl;
		}
		cout << "Continue?(Y/N)"; cin >> ch;
	} while (ch == 'y' || ch == 'Y');
}



void Print(const char* fName)
{
	cout << endl << "\nfile \"" << fName << "\":\n";
	cout << "==================================================================================================="
		<< endl;
	cout << "| № |  Прізвище  | Курс |   Спеціальність   | Фізика | Матем. | Програм. | Чис. мет. | Педагогіка |"
		<< endl;
	cout << "==================================================================================================="
		<< endl;
	ifstream f(fName, ios::binary);
	Student student;
	int i = 1;
	while (f.read((char*)&student, sizeof(Student))) {
		cout << "| " << right << i++ << "  ";
		cout << "| " << setw(11) << left << student.prizv
			<< "|   " << setw(4) << right << student.kurs << "  "
			<< "| " << setw(18) << left << specialtyStr[student.specialty]
			<< "| " << setw(6) << right << student.gradePhys << " "
			<< "| " << setw(6) << right << student.gradeMath << " ";
		switch (student.specialty) {
		case KN:
			cout << "| " << setw(8) << fixed << right << student.gradeProg
				<< " |" << setw(12) << "|" << setw(12) << "|" << endl;
			break;
		case INF:
			cout << "|" << setw(11)
				<< "| " << right << fixed << setw(9) << student.gradeMetod << " |" << setw(12) << "|" << endl;
			break;
		default:
			cout << "|" << setw(11)
				<< "|" << setw(12) << "| " << setw(10) << student.gradePed << " |" << endl;
			break;
		}

	}
	cout << "===================================================================================================" << endl;
	cout << endl;
}


void fWrite(fstream& f, const int i, Student x) {
	f.seekp(i * (long)sizeof(Student));
	f.write((char*)&x, sizeof(Student));
}

Student fRead(fstream& f, const int i) {
	Student x;
	f.seekg(i * (long)sizeof(Student));
	f.read((char*)&x, sizeof(Student));
	return x;
}

void fChange(fstream& f, const int i, const int j) {
	Student x = fRead(f, i);
	Student y = fRead(f, j);
	fWrite(f, i, y);
	fWrite(f, j, x);
}

void Sort(const char* fName) {
	fstream f(fName, ios::binary | ios::in | ios::out);

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i0 = 1; i0 < size; i0++) {
		for (int i1 = 0; i1 < size - i0; i1++) {
			Student a = fRead(f, i1);
			Student b = fRead(f, i1 + 1);
			if ((a.gradePhys < b.gradePhys) ||
				(a.gradePhys == b.gradePhys && a.kurs < b.kurs) ||
				(a.gradePhys == b.gradePhys && a.kurs == b.kurs && strcmp(a.prizv, b.prizv) == -1)) {
				fChange(f, i1, i1 + 1);
			}
		}
	}


	f.seekp(0, ios::end);
}

void indexSort(const char* fName, const char* gName) {
	fstream g(gName, ios::binary | ios::in | ios::out | ios::trunc);
	fstream f(fName, ios::binary | ios::in | ios::out);

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i = 0; i < size; i++)
		g.write((char*)&i, sizeof(int));
	g.seekg(0, ios::beg);

	int value1, value2, tmp;

	for (int i0 = 1; i0 < size; i0++) {
		for (int i1 = 0; i1 < size - i0; i1++) {
			;
			g.seekg(i1 * (long)sizeof(int));
			g.read((char*)&value1, sizeof(int));
			g.read((char*)&value2, sizeof(int));
			Student a = fRead(f, value1);
			Student b = fRead(f, value2);
			if ((a.gradePhys < b.gradePhys) ||
				(a.gradePhys == b.gradePhys && a.kurs < b.kurs) ||
				(a.gradePhys == b.gradePhys && a.kurs == b.kurs && strcmp(a.prizv, b.prizv) == -1)) {
				int tmp1;
				int tmp2;
				g.seekg(i1 * (long)sizeof(int));
				g.read((char*)&tmp1, sizeof(int));
				g.seekg((i1 + 1) * (long)sizeof(int));
				g.read((char*)&tmp2, sizeof(int));
				g.seekg(i1 * (long)sizeof(int));
				g.write((char*)&tmp2, sizeof(int));
				g.seekg((i1 + 1) * (long)sizeof(int));
				g.write((char*)&tmp1, sizeof(int));
			}
		}
	}

	g.seekg(0, ios::beg);
	f.seekp(0, ios::end);
	f.close();
	g.close();
}

void indexPrint(const char* fName, const char* gName) {
	cout << endl << "\nfile \"" << fName << "\":\n";
	cout << "==================================================================================================="
		<< endl;
	cout << "| № |  Прізвище  | Курс |   Спеціальність   | Фізика | Матем. | Програм. | Чис. мет. | Педагогіка |"
		<< endl;
	cout << "==================================================================================================="
		<< endl;
	ifstream g(gName, ios::binary);
	fstream f(fName, ios::binary | ios::in | ios::out);
	Student student;

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	int ind, i = 1;

	while (g.read((char*)&ind, sizeof(int))) {
		f.seekp(ind * (long)sizeof(Student));
		f.read((char*)&student, sizeof(Student));
		cout << "| " << right << i++ << "  ";
		cout << "| " << setw(11) << left << student.prizv
			<< "|   " << setw(4) << right << student.kurs << "  "
			<< "| " << setw(18) << left << specialtyStr[student.specialty]
			<< "| " << setw(6) << right << student.gradePhys << " "
			<< "| " << setw(6) << right << student.gradeMath << " ";
		switch (student.specialty) {
		case KN:
			cout << "| " << setw(8) << fixed << right << student.gradeProg
				<< " |" << setw(12) << "|" << setw(12) << "|" << endl;
			break;
		case INF:
			cout << "|" << setw(11)
				<< "| " << right << fixed << setw(9) << student.gradeMetod << " |" << setw(12) << "|" << endl;
			break;
		default:
			cout << "|" << setw(11)
				<< "|" << setw(12) << "| " << setw(10) << student.gradePed << " |" << endl;
			break;
		}

	}
	cout << "===================================================================================================" << endl;
	cout << endl;
}

int binSearch(const char* fName, const char* prizv, const int kurs, const int gradePhys) {
	fstream f(fName, ios::binary | ios::in | ios::out);
	int L = 0, m;
	f.seekg(0, ios::end);
	int R = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);
	do {
		m = (L + R) / 2;
		Student a = fRead(f, m);
		if (a.gradePhys == gradePhys && a.kurs == kurs && strcmp(a.prizv, prizv) == 0) {
			return m + 1;
		}
		else if ((a.gradePhys < gradePhys) ||
			(a.gradePhys == gradePhys && a.kurs < kurs) ||
			(a.gradePhys == gradePhys && a.kurs == kurs && strcmp(a.prizv, prizv) == -1)) {
			R = m - 1;
		}
		else {
			L = m + 1;
		}
	} while (L <= R);
	return -1;
}
