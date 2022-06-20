
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>


using namespace std;

enum Specialty { KOMP_NAYK, INFORMATUKA, MATEM_EKONOM, FIZ_MATEM, TRUD_NAVCH };

string specialtyStr[] = { "Комп'ютерні науки", "Інформатика", "Математика та економіка", "Фізика та інформатика", "Трудове навчання" };

struct Student
{
	char surname[64];
	unsigned course;
	Specialty specialty;
	double SERrate;
	unsigned physics;
	unsigned maths;
	union
	{
		unsigned programming;
		unsigned ch_methods;
		unsigned pedagogy;
	};
};

void CreateBIN(const char* filename);
void PrintBIN(const char* filename);
void fWrite(fstream& f, const int i, Student x);
Student fRead(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);
void SortBIN(const char* filename);
void IndexSortBIN(const char* filename, const char* newfile);
void PrintIndexSortedBIN(const char* filename, const char* newfile);
int BinSearch(const char* filename, const char* surname, const double avg, const int course);

int main()
{



	char filename[100] = "f.dat";
	char newfile[15] = "g.dat";
	char surname[64];
	double avg;
	int found, course;

	int menuItem;
	do {
		cout << endl;
		cout << " Виберіть дію: " << endl << endl;
		cout << " [1] - введення даних з клавіатури " << endl;
		cout << " [2] - вивід даних на екран " << endl;
		cout << " [3] - фізичне впорядкування даних " << endl;
		cout << " [4] - індексне впорядкування та вивід даних " << endl;
		cout << " [5] - бінарний пошук " << endl;
		cout << " [0] - вихід та завершення роботи програми " << endl << endl;
		cout << " Введіть значення: ";
		cin >> menuItem;
		cout << endl;

		switch (menuItem)
		{
		case 1:
			if (strcmp(filename, "") == 0) {
				cout << "Введіть ім'я файлу: ";
				cin >> filename;
			}
			CreateBIN(filename);
			break;
		case 2:
			if (strcmp(filename, "") == 0) {
				cout << "Файл не існує, спочатку створіть файл." << endl;
				break;
			}
			PrintBIN(filename);
			break;
		case 3:
			if (strcmp(filename, "") == 0) {
				cout << "Файл не існує, спочатку створіть файл." << endl;
				break;
			}
			SortBIN(filename);
			PrintBIN(filename);
			break;
		case 4:
			if (strcmp(filename, "") == 0) {
				cout << "Файл не існує, спочатку створіть файл." << endl;
				break;
			}
			IndexSortBIN(filename, newfile);
			PrintIndexSortedBIN(filename, newfile);
			break;
		case 5:
			if (strcmp(filename, "") == 0) {
				cout << "Файл не існує, спочатку створіть файл." << endl;
				break;
			}

			cout << "Введіть ключі пошуку:" << endl;
			cout << " Прізвище: ";
			cin.get();
			cin.sync();
			cin >> surname;
			cout << " Середній бал: ";
			cin >> avg;
			cout << " Курс ";
			cin >> course;

			found = BinSearch(filename, surname, avg, course);

			//cout <<"bin " << found << endl;

			if (found == -1) {
				cout << " Шуканого студента не знайдено" << endl;
			}
			else {
				ifstream fin(filename, ios::binary);
				if (!fin) {
					cout << "Main: Помилка відкриття файлу. " << filename << endl;
					return -1;
				}
				cout << " Знайдено студента в позиції № " << found + 1 << endl;
			}
			break;
		case 0:
			break;
		default:
			cout << " Ви ввели помилкове значення! "
				" Слід ввести число - номер вибраного пункту меню " << endl;
		}
	} while (menuItem != 0);

	return 0;
}

void CreateBIN(const char* filename)
{
	ofstream f(filename, ios::binary); // відкрили файл для запису
	char ch; // відповідь користувача – чи продовжувати введення
	int n = 1;
	int specialty;
	Student student;

	do {
		cout << " Студент № " << n << ":" << endl;
		cin.get(); // очищуємо буфер клавіатури – бо залишаються символи
		cin.sync(); // "кінець рядка", які не дають ввести наступний літерний рядок
		cout << " Прізвище: ";
		cin >> student.surname;
		cout << " Курс: ";
		cin >> student.course;
		cout << " Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
		cin >> specialty;
		student.specialty = (Specialty)specialty;
		cout << " Оцінка з фізики: ";
		cin >> student.physics;
		cout << " Оцінка з математики: ";
		cin >> student.maths;

		switch (student.specialty)
		{
		case KOMP_NAYK:

			cout << " Оцінка з програмування: ";
			cin >> student.programming;
			student.SERrate = ((student.physics + student.maths + student.programming) / 3.0);
			cout << " Середній бал = " << student.SERrate;
			break;
		case INFORMATUKA:
			cout << " Оцінка з чисельних методів: ";
			cin >> student.ch_methods;
			student.SERrate = ((student.physics + student.maths + student.ch_methods) / 3.0);
			cout << " Середній бал = " << student.SERrate;
			break;
		case MATEM_EKONOM:
		case FIZ_MATEM:
		case TRUD_NAVCH:
			cout << " Оцінка з педагогіки: ";
			cin >> student.pedagogy;
			student.SERrate = ((student.physics + student.maths + student.pedagogy) / 3.0);
			cout << " Середній бал = " << student.SERrate;
			break;
		}
		cout << endl;
		f.write((char*)&student, sizeof(Student)); // записали у файл
		n++;
		cout << "Додати ще? (y/n): ";
		cin >> ch;
		cout << endl;
	}

	while (ch == 'y' || ch == 'Y');
	cout << endl;
	f.close();

}

void PrintBIN(const char* filename)
{
	ifstream f(filename, ios::binary); // відкрили файл для зчитування
	if (!f) {
		cout << "PrintBIN: Помилка відкриття файлу." << endl;
		return;
	}

	cout << "=================================================================================================================================" << endl;
	cout << " |  №  |    Прізвище    | Курс |      Спеціальність      | Фізика | Математика | Програмування | Чисельні методи | Педагогіка |" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------------------------" << endl;

	Student student;
	int i = 1;

	while (f.read((char*)&student, sizeof(Student))) {
		cout << " | " << setw(2) << right << i << " ";
		cout << " | " << setw(14) << left << student.surname;
		cout << " |  " << setw(2) << right << student.course << " ";
		cout << " | " << setw(23) << left << specialtyStr[student.specialty];
		cout << " | " << setw(4) << right << student.physics << "  ";
		cout << " | " << setw(6) << right << student.maths << "    ";

		switch (student.specialty)
		{
		case KOMP_NAYK:
			cout << " | " << setw(7) << right << student.programming << "     " << "  | " << setw(18) << right << " | " << setw(12) << right << "|" << endl;
			break;
		case INFORMATUKA:
			cout << " | " << setw(16) << right << "| " << setw(8) << right << student.ch_methods << "      " << "  | " << setw(12) << right << "|" << endl;
			break;
		case MATEM_EKONOM:
		case FIZ_MATEM:
		case TRUD_NAVCH:
			cout << " | " << setw(16) << right << "| " << setw(18) << right << "  | " << setw(6) << right << student.pedagogy << "     " << "|" << endl;
			break;
		}
		i++;
	}
	cout << "=================================================================================================================================" << endl;
	cout << endl;
}

void fWrite(fstream& f, const int i, Student x)
{
	f.seekp(i * (long)sizeof(Student));   // встановили вказівник
	f.write((char*)&x, sizeof(Student));  // записали значення
}

Student fRead(fstream& f, const int i)
{
	Student x;
	f.seekg(i * (long)sizeof(Student));   // встановили вказівник
	f.read((char*)&x, sizeof(Student));   // прочитали значення
	return x;
}

void fChange(fstream& f, const int i, const int j)
{
	Student x = fRead(f, i);
	Student y = fRead(f, j);
	fWrite(f, i, y);
	fWrite(f, j, x);
}

void SortBIN(const char* filename)
{
	fstream f(filename, ios::binary | ios::in | ios::out);

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i0 = 1; i0 < size; i0++) {
		for (int i1 = 0; i1 < size - i0; i1++)
		{
			Student a = fRead(f, i1);
			Student b = fRead(f, i1 + 1);

			if ((a.SERrate > b.SERrate)
				||
				(a.SERrate == b.SERrate &&
					strcmp(a.surname, b.surname) == -1)
				||
				(a.SERrate == b.SERrate &&
					strcmp(a.surname, b.surname) == 0) &&
						a.course > b.course)			{
				fChange(f, i1, i1 + 1);
			}
		}
	}

	f.seekp(0, ios::end);
}

void IndexSortBIN(const char* filename, const char* newfile)
{	// сортування файлу – за допомогою послідовного доступу: зчитуємо з одного файлу і записуємо в інший
	fstream g(newfile, ios::binary | ios::in | ios::out | ios::trunc);
	fstream f(filename, ios::binary | ios::in | ios::out);

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i = 0; i < size; i++)
		g.write((char*)&i, sizeof(int));
	g.seekg(0, ios::beg);

	int x, y;

	for (int i0 = 1; i0 < size; i0++) {
		for (int i1 = 0; i1 < size - i0; i1++) {
			;
			g.seekg(i1 * (long)sizeof(int));
			g.read((char*)&x, sizeof(int));
			g.read((char*)&y, sizeof(int));
			Student a = fRead(f, x);
			Student b = fRead(f, y);
			if ((a.SERrate > b.SERrate)
				||
				(a.SERrate == b.SERrate &&
					strcmp(a.surname, b.surname) == -1)
				||
				(a.SERrate == b.SERrate &&
					strcmp(a.surname, b.surname) == 0 &&
						a.course > b.course))
			{
				int  i2;
				g.seekg(i1 * (long)sizeof(int));
				g.read((char*)&i1, sizeof(int));
				g.seekg((i1 + 1) * (long)sizeof(int));
				g.read((char*)&i2, sizeof(int));
				g.seekg(i1 * (long)sizeof(int));
				g.write((char*)&i2, sizeof(int));
				g.seekg((i1 + 1) * (long)sizeof(int));
				g.write((char*)&i1, sizeof(int));
			}
		}
	}

	g.seekg(0, ios::beg);
	f.seekp(0, ios::end);
	f.close();
	g.close();
}

void PrintIndexSortedBIN(const char* filename, const char* newfile)
{
	cout << "=================================================================================================================================" << endl;
	cout << " |  №  |    Прізвище    | Курс |      Спеціальність      | Фізика | Математика | Програмування | Чисельні методи | Педагогіка |" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------------------------" << endl;

	ifstream g(newfile, ios::binary);		// відкрили файл для запису
	if (!g) {
		cout << "PrintIndexSorted: Помилка відкриття файлу. " << newfile << endl;
		return;
	}
	fstream f(filename, ios::binary | ios::in | ios::out);	// відкрили файл для зчитування
	if (!f) {
		cout << "PrintIndexSorted: Помилка відкриття файлу. " << filename << endl;
		return;
	}

	Student student;
	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	int i = 1;
	int idx;

	while (g.read((char*)&idx, sizeof(int))) {
		f.seekg(idx * (long)sizeof(Student));
		f.read((char*)&student, sizeof(Student));
		cout << " | " << setw(2) << right << i++ << " ";
		cout << " | " << setw(14) << left << student.surname;
		cout << " |  " << setw(2) << right << student.course << " ";
		cout << " | " << setw(23) << left << specialtyStr[student.specialty];
		cout << " | " << setw(4) << right << student.physics << "  ";
		cout << " | " << setw(6) << right << student.maths << "    ";

		switch (student.specialty)
		{
		case KOMP_NAYK:
			cout << " | " << setw(7) << right << student.programming << "     " << "  | " << setw(18) << right << " | " << setw(12) << right << "|" << endl;
			break;
		case INFORMATUKA:
			cout << " | " << setw(16) << right << "| " << setw(8) << right << student.ch_methods << "      " << "  | " << setw(12) << right << "|" << endl;
			break;
		case MATEM_EKONOM:
		case FIZ_MATEM:
		case TRUD_NAVCH:
			cout << " | " << setw(16) << right << "| " << setw(18) << right << "  | " << setw(6) << right << student.pedagogy << "     " << "|" << endl;
			break;
		}
	}
	cout << "=================================================================================================================================" << endl;
	cout << endl;
}

int BinSearch(const char* filename, const char* surname, const double avg, const int course)
{   // повертає індекс знайденого елемента або -1, якщо шуканий елемент відсутній
	fstream f(filename, ios::binary | ios::in | ios::out);
	if (!f) {
		cout << "Print_I: Помилка відкриття файлу. " << filename << endl;
		return -1;
	}
	int L = 0, m;
	f.seekg(0, ios::end);
	int R = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	do {
		m = (L + R) / 2;
		Student a = fRead(f, m);

		if (a.SERrate == avg && strcmp(a.surname, surname) == 0 && a.course == course) {
			return m;
		}
		else if ((a.SERrate > avg) ||
			(a.SERrate == avg && strcmp(a.surname, surname) == -1) ||
			(a.SERrate == avg && strcmp(a.surname, surname) == 0 && a.course > course))
		{
			R = m - 1;
		}
		else {
			L = m + 1;
		}
	} while (L <= R);

	return -1;

}
