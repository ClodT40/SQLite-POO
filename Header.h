#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <exception>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
using namespace std;

class StringUtils {
public:
	static vector<string> fromCharMatrixToVector(char** matrix, int dim) {
		vector<string> strings;
		for (int i = 0; i < dim; i++)
			strings.push_back(string(matrix[i]));
		return strings;
	}
	static string schimbaTerminatieFisier(string numeFisier, string terminatie) {
		vector<string> numeFisierParsat = StringUtils::splitString(numeFisier, '.', false);
		return numeFisierParsat[0] + '.' + terminatie;
	}
	static vector<string> splitString(string str, char delimiter, bool lower)
	{
		vector<string> tokens;
		string temp;
		for (char letter : str)
		{
			if (letter == delimiter)
			{
				tokens.push_back(temp);
				temp = "";
			}
			else
			{
				if (lower)
					letter = tolower(letter);
				temp += letter;
			}
		}
		if (temp != "")
			tokens.push_back(temp);
		return tokens;
	}
	static list<string> readLinesFromFile(string file)
	{
		ifstream inputFile(file);

		list<string> commands;

		string line;
		while (getline(inputFile, line))
		{
			commands.push_back(line);
		}
		return commands;
	}

	static set<string> removeDuplicates(vector<string> values) {
		set<string> distinctValues;

		for (string value : values) {
			distinctValues.insert(value);
		}
		return distinctValues;
	}

	static map<string, int> getFrequency(vector<string> values) {
		map<string, int> m;

		for (string value : values) {
			if (m.find(value) == m.end())
				m.insert(pair<string, int>(value, 1));
			else
				m[value]++;
		}
		return m;
	}

	static string substring(string str, int indexStart, int indexEnd)
	{
		string newStr;
		if (indexEnd > str.length())
			indexEnd = str.length();
		for (int i = indexStart; i < indexEnd; i++)
			newStr += str[i];
		return newStr;
	}

	static void displayVector(vector<string> v) {
		for (string s : v)
			cout << s << " ";
		cout << '\n';
	}
};
class FileUtils {
public:
	static bool txtExists(string fileName) {
		ifstream f(fileName, ios::in);
		if (f.is_open())
			return true;
		return false;
	}

	static int getNrLinesFile(string fileName) {
		ifstream f(fileName, ios::in);
		char character;
		int countLines = 0;
		if (f.is_open()) {
			while (f.good()) {
				f.get(character);
				if (character == '\n')
					countLines++;
			}
		}
		f.close();
		return countLines;
	}

	static int getNrLinesBinaryFile(string fileName) {
		ifstream f(fileName, ios::binary | ios::in);
		char character;
		int countLines = 0;
		if (f.is_open()) {
			while (f.good()) {
				f.get(character);
				if (character == '\n')
					countLines++;
			}
		}
		f.close();
		return countLines;
	}

	static vector<string> readLinesFromBinaryFile(string fileName) {
		ifstream f(fileName, ios::binary | ios::in);
		vector<string> lines;

		if (!f.is_open())
		{
			cout << "Cannot open " << fileName << " file!" << endl;
		}
		else
		{
			int countLines = FileUtils::getNrLinesBinaryFile(fileName);

			int len;
			char* buffer;

			while (countLines > 1) {
				f.read((char*)&len, sizeof(int));
				buffer = new char[len];
				f.read(buffer, len * sizeof(char));
				string line = string(buffer);
				line = StringUtils::substring(line, 0, line.size() - 1);
				lines.push_back(line);
				countLines--;
			}
		}
		return lines;
	}

	static vector<vector<string>> readVectorLinesFromBinaryFile(string fileName) {
		ifstream f(fileName, ios::binary | ios::in);
		vector<vector<string>> vectorLines;
		vector<string> lines;

		if (!f.is_open())
		{
			cout << "Cannot open " << fileName << " file!" << endl;
		}
		else
		{
			int countLines = FileUtils::getNrLinesBinaryFile(fileName + ".bin");
			int countTxtLines = FileUtils::getNrLinesFile(fileName + ".txt");
			int linesPerParameter = countTxtLines / 4;

			int len;
			char* buffer;
			int count = 0;
			while (countLines > 1) {
				f.read((char*)&len, sizeof(int));
				buffer = new char[len];
				f.read(buffer, len * sizeof(char));
				string line = string(buffer);
				line = StringUtils::substring(line, 0, line.size() - 1);
				lines.push_back(line);

				if (count % linesPerParameter == 0)
				{
					vectorLines.push_back(lines);
					lines.clear();
				}

				countLines--;
				count++;
			}

			if (count % linesPerParameter == 0)
				vectorLines.push_back(lines);

		}
		return vectorLines;
	}

	static int getIndexTxt(string fileName, string parameter) {
		ifstream f(fileName, ios::in);

		if (!f.is_open())
		{
			cout << "Cannot open " << fileName << " file!" << endl;
			return -1;
		}
		else
		{
			string line;
			int index = 0;
			while (getline(f, line)) {
				if (line == parameter)
					return index / 4;
				index++;
			}
		}
		return -1;
	}
	static void functieCSV(char numeFisier[], char numeTabela[])
	{
		string numeFisierCSV = string(numeFisier);
		vector<string> numeFisierSplit = StringUtils::splitString(numeFisierCSV, '.', true);
		string numeFisierBIN = string(numeTabela) + ".bin";

		string line;
		ifstream fileCSV(numeFisierCSV, ios::in);
		ofstream fileBinary(numeFisierBIN, ios::binary | ios::app);
		if (fileCSV.is_open())
		{
			while (!fileCSV.eof())
			{
				getline(fileCSV, line);
				vector<string> valoriDePeLinie = StringUtils::splitString(line, ',', false);
				for (int i = 0; i < valoriDePeLinie.size(); i++)
				{
					string cuvant = valoriDePeLinie[i];
					if (!fileBinary)
					{
						cout << "Nu s-a putut deschide fisierul binar!";
					}
					cuvant += '\n';
					int dim = cuvant.size() + 1;

					fileBinary.write((char*)&dim, sizeof(int));

					fileBinary.write(cuvant.c_str(), dim * sizeof(char));
				}
			}
		}
		fileBinary.close();
		fileCSV.close();
	}
	static void stergereTabela(char** matriceCuvinte, int indice)
	{
		vector<string> buffers;
		string buffer;
		ifstream fis("Tabele.txt", ios::in);
		if (fis.is_open())
		{
			while (getline(fis, buffer))
			{
				if (strcmp(buffer.c_str(), matriceCuvinte[2]) != 0)
					buffers.push_back(buffer);
			}
		}
		else
		{
			cout << "Fisierul nu a putut fi deschis!" << '\n';
		}
		fis.close();

		ofstream fout("Tabele.txt", ios::out);

		for (int i = 0; i < buffers.size(); i++)
			fout << buffers[i] << '\n';

		remove(((string)matriceCuvinte[indice] + ".txt").c_str());
		remove(((string)matriceCuvinte[indice] + ".bin").c_str());
	}
	static void afisareTXT(string numeFisier)
	{
		string line;
		ifstream fileTxt(numeFisier);
		if (fileTxt.is_open())
		{
			while (getline(fileTxt, line))
			{
				cout << line << '\n';
			}
			fileTxt.close();
		}

		else cout << "Unable to open file";

	}
	static bool parcurgereFisier(char** matriceCuvinte, int indice)
	{
		ifstream fis("Tabele.txt", ios::in);
		if (fis.is_open())
		{
			while (!fis.eof())
			{
				char buffer[100];
				char numeTabela[100];
				fis.getline(buffer, 100);
				strcpy(numeTabela, buffer);
				if (strcmp(numeTabela, matriceCuvinte[indice]) == 0)
				{
					return true;
				}
			}
			fis.close();
		}
		return false;
	}
};

class propozitie {
	char buffer[1000] = "";
	int numarCuvinte = 0;
	char** matriceChar = nullptr;
public:
	propozitie() {
	}
	propozitie(string command) {
		strcpy(this->buffer, command.c_str());

		char copie[1000] = "";
		char* token = nullptr;

		strcpy(copie, command.c_str());
		token = strtok(copie, " ()=,");
		int nrcuv = 1;
		while (token != NULL)
		{
			nrcuv++;
			token = strtok(NULL, " ()=,");
		}
		this->matriceChar = new char* [nrcuv];
		strcpy(copie, command.c_str());
		token = strtok((char*)command.c_str(), " ()=,");
		this->numarCuvinte = 0;
		while (token != NULL)
		{
			this->matriceChar[this->numarCuvinte] = new char[strlen(token) + 1];
			strcpy(this->matriceChar[this->numarCuvinte], token);
			this->numarCuvinte++;
			token = strtok(NULL, " ()=,");
		}
	}

	friend istream& operator>>(istream& consola, propozitie& p);
	friend ostream& operator<<(ostream& consola, propozitie p);
	//get
	char** getMatriceCuvinte()
	{
		return this->matriceChar;
	}
	int getNumarCuvinte()
	{
		return this->numarCuvinte;
	}
	char* getBuffer()
	{
		return this->buffer;
	}
};
istream& operator>>(istream& consola, propozitie& p)
{
	char copie[1000] = "";
	char* token = nullptr;
	cout << "\nDati linia de comanda:" << endl;
	consola.getline(p.buffer, 1000);
	strcpy(copie, p.buffer);
	token = strtok(copie, " ()=,");
	int nrcuv = 1;
	while (token != NULL)
	{
		nrcuv++;
		token = strtok(NULL, " ()=,");
	}
	p.matriceChar = new char* [nrcuv];
	strcpy(copie, p.buffer);
	token = strtok(p.buffer, " ()=,");
	p.numarCuvinte = 0;
	while (token != NULL)
	{
		p.matriceChar[p.numarCuvinte] = new char[strlen(token) + 1];
		strcpy(p.matriceChar[p.numarCuvinte], token);
		p.numarCuvinte++;
		token = strtok(NULL, " ()=,");
	}
	strcpy(p.buffer, copie);

	return consola;
}
ostream& operator<<(ostream& consola, propozitie p)
{
	consola << "numar de cuvinte al liniei de comanda este: " << p.numarCuvinte << endl;
	for (int i = 0; i < p.numarCuvinte; i++)
	{
		consola << endl << p.matriceChar[i] << endl;
	}
	return consola;
}

class coloana {
	const unsigned int id_Coloana = numarColoana;
	char* numeColoana = nullptr;
	string tip = "";
	int dimensiune = 0;
	string valoareImplicita = "";
	static int numarColoana;
public:
	coloana()
	{
		this->numeColoana = nullptr;
		this->tip = "";
		this->valoareImplicita = "";
		this->dimensiune = 0;
		coloana::numarColoana += 1;
	}
	coloana(char* newNumeColoana, string newTip, int newDimensiune, string newValoareImplicita)
	{
		if (this->numeColoana != nullptr) {
			delete[] this->numeColoana;
		}
		this->numeColoana = new char[strlen(newNumeColoana) + 1];
		strcpy(this->numeColoana, newNumeColoana);
		this->tip = newTip;
		this->dimensiune = newDimensiune;
		this->valoareImplicita = newValoareImplicita;
		coloana::numarColoana += 1;
	}
	coloana(char* newNumeColoana, string newValoareImplicita) {
		if (this->numeColoana != nullptr) {
			delete[] this->numeColoana;
		}
		this->numeColoana = new char[strlen(newNumeColoana) + 1];
		strcpy(this->numeColoana, newNumeColoana);
		this->valoareImplicita = newValoareImplicita;
		coloana::numarColoana += 1;
	}

	friend ostream& operator<<(ostream& consola, coloana fc);
	friend istream& operator>>(istream& consola, coloana& fc);

	//copy constructor
	coloana(const coloana& c)
	{
		if (this->numeColoana != nullptr) {
			delete[] this->numeColoana;
		}
		this->numeColoana = new char[strlen(c.numeColoana) + 1];
		strcpy(this->numeColoana, c.numeColoana);
		this->tip = c.tip;
		this->dimensiune = c.dimensiune;
		this->valoareImplicita = c.valoareImplicita;
	}
	//destructor
	~coloana()
	{
		if (this->numeColoana != nullptr) {
			delete[] this->numeColoana;
		}
	}

	char* getNumeColoana()
	{
		return this->numeColoana;
	}
	string getTip()
	{
		return this->tip;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}
	string getValoareImplicita()
	{
		return this->valoareImplicita;
	}

	void setNumeColoana(char* newNumeColoana)
	{
		if (this->numeColoana != nullptr) {
			delete[] this->numeColoana;
		}
		this->numeColoana = new char[strlen(newNumeColoana) + 1];
		strcpy(this->numeColoana, newNumeColoana);
	}
	void setTip(string newTip)
	{
		this->tip = newTip;
	}
	void setDimensiune(int newDimensiune)
	{
		this->dimensiune = newDimensiune;
	}
	void setValoareImplicita(string newValoareImplicita)
	{
		this->valoareImplicita = newValoareImplicita;
	}

	coloana operator=(coloana& c)
	{
		if (this->numeColoana != nullptr) {
			delete[] this->numeColoana;
		}
		this->numeColoana = new char[strlen(c.numeColoana) + 1];
		strcpy(this->numeColoana, c.numeColoana);
		this->tip = c.tip;
		this->dimensiune = c.dimensiune;
		this->valoareImplicita = c.valoareImplicita;
		return *this;
	}
	coloana operator +(int value)
	{
		this->dimensiune = this->dimensiune + value;
		return *this;
	}
	/*postincrementare   creste dimensiunea cu 10*/
	coloana operator ++(int)
	{
		this->dimensiune = this->dimensiune + 10;
		return *this;
	}
	/*postincrementare creste dimensiunea cu 10*/
	coloana operator ++()
	{
		this->dimensiune = this->dimensiune + 10;
		return *this;
	}
	//compara dimensiunea coloanelor
	bool operator ==(coloana& c)
	{
		if (this->dimensiune == c.dimensiune)
		{
			return true;
		}
		return false;
	}
	bool operator <=(coloana& c)
	{
		if (this->dimensiune <= c.dimensiune)
		{
			return true;
		}
		return false;
	}
	/*char*& operator[](int value) {
		return this->numeColoana[value];
	}*/
	//returneaza o copie a coloanei cu dimensiunea 0
	coloana operator !()
	{
		this->dimensiune = 0;
		return *this;
	}
};
int coloana::numarColoana = 0;

ostream& operator<<(ostream& consola, coloana fc)
{
	consola << endl << "-----------------" << "\n";
	consola << "Nume coloana: " << fc.numeColoana << "\n";
	consola << "Tip : " << fc.tip << "\n";
	consola << "Dimensiune: " << fc.dimensiune << "\n";
	consola << "Valoare Implicita: " << fc.valoareImplicita << "\n";

	return consola;
}
istream& operator>>(istream& consola, coloana& fc)
{
	cout << "Nume coloana: ";
	consola >> fc.numeColoana;
	cout << "\n";
	cout << "Tip : ";
	consola >> fc.tip;
	cout << "\n";
	cout << "Dimensiune: ";
	consola >> fc.dimensiune;
	cout << "\n";
	cout << "Valoare Implicita:";
	consola >> fc.valoareImplicita;
	return consola;
}

class tabel
{
	const unsigned int id_Tabel = numarTabel;
	char* numeTabel = nullptr;
	bool completat = false;
	static int numarTabel;
public:
	tabel()
	{
		this->numeTabel = nullptr;
		this->completat = true;
		tabel::numarTabel += 1;
	}
	tabel(char* newNumeTabel)
	{
		if (this->numeTabel != nullptr)
		{
			delete[] this->numeTabel;
		}
		this->numeTabel = new char[strlen(newNumeTabel) + 1];
		strcpy(this->numeTabel, newNumeTabel);
		this->completat = true;
		tabel::numarTabel += 1;
	}

	friend ostream& operator<<(ostream& consola, tabel t);
	friend istream& operator>>(istream& consola, tabel& t);

	//copy constructor
	tabel operator=(const tabel& t)
	{
		if (this->numeTabel != nullptr) {
			delete[] this->numeTabel;
		}
		this->numeTabel = new char[strlen(t.numeTabel) + 1];
		strcpy(this->numeTabel, t.numeTabel);
		this->completat = t.completat;

	}
	//destructor
	~tabel()
	{
		if (this->numeTabel != nullptr)
		{
			delete[] this->numeTabel;
		}
	}

	char* getNumeTabel()
	{
		return this->numeTabel;
	}
	bool getCompletat()
	{
		return this->completat;
	}
	void setNumeTabel(char* newNumeTabel)
	{
		if (this->numeTabel != nullptr)
		{
			delete[] this->numeTabel;
		}
		this->numeTabel = new char[strlen(newNumeTabel) + 1];
		strcpy(this->numeTabel, newNumeTabel);
	}
	void setCompletat(bool newCompletat)
	{
		this->completat = newCompletat;
	}

	tabel operator=(tabel& t)
	{
		if (this->numeTabel != nullptr) {
			delete[] this->numeTabel;
		}
		this->numeTabel = new char[strlen(t.numeTabel) + 1];
		strcpy(this->numeTabel, t.numeTabel);
		this->completat = t.completat;
		return *this;
	}
	tabel operator+(char* nume2)
	{
		this->numeTabel = new char[strlen(nume2) + 1];
		strcat(this->numeTabel, nume2);
		return *this;
	}
	tabel operator++()
	{
		this->numeTabel = new char[strlen("TABEL") + 1];
		strcat(numeTabel, "TABEL");
		return *this;
	}
	tabel operator++(int)
	{
		this->numeTabel = new char[strlen("TABEL") + 1];
		strcat(numeTabel, "TABEL");
		return *this;
	}
	bool operator <=(tabel& t)
	{
		if (strcmp(this->numeTabel, t.numeTabel) < 0 || strcmp(this->numeTabel, t.numeTabel) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator==(tabel& t)
	{
		if (strcmp(this->numeTabel, t.numeTabel) == 0 && this->completat == t.completat)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	tabel operator!()
	{
		if (this->numeTabel != nullptr)
		{
			delete[] this->numeTabel;
		}
		this->completat = false;
		return *this;
	}
};
int tabel::numarTabel = 0;

ostream& operator<<(ostream& consola, tabel t)
{
	consola << endl << "-----------------" << "\n";
	consola << "Nume tabel: " << t.numeTabel << "\n";
	consola << "E completat sau nu : ";
	if (t.completat == true)
	{
		cout << "DA";
	}
	else
	{
		cout << "NU";
	}
	return consola;
}
istream& operator>>(istream& consola, tabel& t)
{
	cout << "Nume Tabel: ";
	consola >> t.numeTabel;
	return consola;
}

class utilizatorBaza {
	const unsigned int id_utilizator = numarUtilizatori;
	string nume = "";
	string prenume = "";
	char* adresa = nullptr;
	static int numarUtilizatori;
public:
	utilizatorBaza()
	{
		this->adresa = nullptr;
		this->nume = "";
		this->prenume = "";
		utilizatorBaza::numarUtilizatori += 1;
	}
	utilizatorBaza(string nume)
	{
		this->nume = nume;
		utilizatorBaza::numarUtilizatori += 1;
	}
	utilizatorBaza(string nume, string prenume)
	{
		this->nume = nume;
		this->prenume = prenume;
		utilizatorBaza::numarUtilizatori += 1;
	}
	friend ostream& operator<<(ostream& consola, utilizatorBaza t);
	friend istream& operator>>(istream& consola, utilizatorBaza& t);
	//copy constructor
	utilizatorBaza operator=(const utilizatorBaza& ub)
	{
		if (this->adresa != nullptr) {
			delete[] this->adresa;
		}
		this->adresa = new char[strlen(ub.adresa) + 1];
		strcpy(this->adresa, ub.adresa);
		this->nume = ub.nume;
		this->prenume = ub.prenume;

	}
	~utilizatorBaza()
	{
		if (this->adresa != nullptr) {
			delete[] this->adresa;
		}
	}

	string getNume()
	{
		return this->nume;
	}
	string getPrenume()
	{
		return this->prenume;
	}
	char* getAdresa()
	{
		return this->adresa;
	}
	void setNume(char* newNume)
	{
		this->nume = newNume;
	}
	void setPrenume(char* newPrenume)
	{
		this->prenume = newPrenume;
	}
	void setAdresa(char* newAdresa)
	{
		if (this->adresa != nullptr) {
			delete[] this->adresa;
		}
		this->adresa = new char[strlen(newAdresa) + 1];
		strcpy(this->adresa, newAdresa);
	}

	utilizatorBaza operator=(utilizatorBaza& ub)
	{
		if (this->adresa != nullptr) {
			delete[] this->adresa;
		}
		this->adresa = new char[strlen(ub.adresa) + 1];
		strcpy(this->adresa, ub.adresa);
		this->nume = ub.nume;
		this->prenume = ub.prenume;
		return*this;
	}
	utilizatorBaza operator+(char* adresa2)
	{
		this->adresa = new char[strlen(adresa2) + 1];
		strcat(this->adresa, adresa2);
		return *this;
	}
	bool operator <=(utilizatorBaza& t)
	{
		if (strcmp(this->adresa, t.adresa) < 0 || strcmp(this->adresa, t.adresa) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator==(utilizatorBaza& t)
	{
		if (strcmp(this->adresa, t.adresa) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	utilizatorBaza operator!()
	{
		if (this->adresa != nullptr)
		{
			delete[] this->adresa;
		}
		this->nume = "";
		this->prenume = "";
		return *this;
	}
};
int utilizatorBaza::numarUtilizatori = 0;

ostream& operator<<(ostream& consola, utilizatorBaza t)
{
	consola << endl << "-----------------" << "\n";
	consola << "Nume : " << t.nume << "\n";
	consola << "Prenume : " << t.prenume << "\n";
	consola << "Adresa : " << t.adresa << "\n";
	return consola;
}
istream& operator>>(istream& consola, utilizatorBaza& t)
{
	cout << "Nume  ";
	consola >> t.nume;
	cout << "Prenume  ";
	consola >> t.prenume;
	cout << "Adresa  ";
	consola >> t.adresa;
	return consola;
}

class Comanda {
private:
	char* numeTabel;
	char** matriceCuvinte;
	int dimensiune;
public:
	virtual void prelucrareComanda() = 0;
	virtual string obtineNumeComanda() = 0;
	virtual string obtineNumeTabel() {
		return string(numeTabel);
	}
	virtual vector<string> obtineVectorCuvinte() {
		vector<string> vectorCuvinte;

		for (int i = 0; i < this->dimensiune; i++)
			vectorCuvinte.push_back(matriceCuvinte[i]);

		return vectorCuvinte;
	}
};

class CREATE : public Comanda {
	char* numeTabel = nullptr;
	coloana coloanaTabel;
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;

public:
	CREATE(char** matriceCuvinte, int dimensiune)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;

	}
	CREATE(char* newNumeTabel, char* newNumeColoana, string newTip, int newDimensiune, string newValoareImplicita)
	{
		if (this->numeTabel != nullptr)
		{
			delete[] numeTabel;
		}
		this->numeTabel = new char[strlen(newNumeTabel) + 1];
		strcpy(this->numeTabel, newNumeTabel);
		coloanaTabel.setNumeColoana(newNumeColoana);
		coloanaTabel.setTip(newTip);
		coloanaTabel.setDimensiune(newDimensiune);
		coloanaTabel.setValoareImplicita(newValoareImplicita);

	}
	CREATE(char* newNumeColoana, string newTip, int newDimensiune, string newValoareImplicita)
	{
		coloanaTabel.setNumeColoana(newNumeColoana);
		coloanaTabel.setTip(newTip);
		coloanaTabel.setDimensiune(newDimensiune);
		coloanaTabel.setValoareImplicita(newValoareImplicita);
	}

	//copy constructor
	CREATE(const CREATE& c)
	{
		if (this->numeTabel != nullptr)
		{
			delete[] numeTabel;
		}
		this->numeTabel = new char[strlen(c.numeTabel) + 1];
		strcpy(this->numeTabel, c.numeTabel);


	}

	/*destructor*/
	~CREATE()
	{
		if (numeTabel != nullptr)
		{
			delete[] numeTabel;
		}
		if (matriceCuvinte != nullptr)
		{
			for (int i = 0; i < dimensiune; i++)
			{
				delete[] matriceCuvinte[i];
			}
			delete[] matriceCuvinte;
		}

	}

	//functii accesor

	char* getNumeTabel()
	{
		return this->numeTabel;
	}
	void setNumeTabel(char* newNumeTabel)
	{
		if (this->numeTabel != nullptr)
		{
			delete[] this->numeTabel;
		}
		this->numeTabel = new char[strlen(newNumeTabel) + 1];
		strcpy(this->numeTabel, newNumeTabel);
	}

	void prelucrareComanda()
	{
		bool eroare = false;
		if (dimensiune < 3)
		{
			cout << "eroare";
		}
		else
		{
			//conditia sa nu fie altceva decat integer, text,float

			if (strcmp(matriceCuvinte[3], "IF") == 0 && strcmp(matriceCuvinte[4], "NOT") == 0 && strcmp(matriceCuvinte[5], "EXISTS") == 0)
			{
				for (int i = 7; i < dimensiune; i = i + 4)
				{
					if (strcmp(matriceCuvinte[i], "text") == 0 || strcmp(matriceCuvinte[i], "integer") == 0 || strcmp(matriceCuvinte[i], "float") == 0) {}
					else
					{
						eroare = true;
						cout << "\n EROARE \n";
						break;
					}
				}
			}
			else
			{
				for (int i = 4; i < dimensiune; i = i + 4)
				{
					if (strcmp(matriceCuvinte[i], "text") == 0 || strcmp(matriceCuvinte[i], "integer") == 0 || strcmp(matriceCuvinte[i], "float") == 0) {}
					else
					{
						eroare = true;
						cout << "\n EROARE \n";
						break;
					}
				}
			}
		}



		ofstream  fis;
		//daca trece de conditia de "integer text,float
		if (strcmp(matriceCuvinte[3], "IF") == 0 && strcmp(matriceCuvinte[4], "NOT") == 0 && strcmp(matriceCuvinte[5], "EXISTS") == 0 && eroare == false)
		{
			cout << "\nNume tabela:  " << matriceCuvinte[2];
			//verificare daca nu exista deja aceasta tabela
			bool existaDenumireDeja = false;
			int indice = 2;
			existaDenumireDeja = FileUtils::parcurgereFisier(matriceCuvinte, indice);
			if (existaDenumireDeja == false)
			{
				fis.open("Tabele.txt", ios::out | ios::app);
				fis << matriceCuvinte[2] << endl;
				fis.close();
				for (int k = 6; k < dimensiune; k = k + 4)
				{
					ofstream file;
					string numeFisier;
					numeFisier = (string)matriceCuvinte[2] + ".txt";
					file.open(numeFisier, ios::out | ios::app);
					file << matriceCuvinte[k] << endl;
					file << matriceCuvinte[k + 1] << endl;
					file << matriceCuvinte[k + 2] << endl;
					file << matriceCuvinte[k + 3];
					file << '\n';
					file.close();
				}
			}
			else {
				cout << endl << "Tabela pe care doriti sa o introduceti:  " << matriceCuvinte[2] << "  exista deja" << endl;
			}
		}
		else
		{
			if (eroare == false)
			{
				if (strcmp(matriceCuvinte[3], "IF") != 0 && strcmp(matriceCuvinte[4], "NOT") != 0 && strcmp(matriceCuvinte[5], "EXISTS") != 0)
				{
					cout << "\nNume tabela:  " << matriceCuvinte[2];

					//verificare daca nu exista deja aceasta tabela
					bool existaDenumireDeja = false;
					int indice = 2;
					existaDenumireDeja = FileUtils::parcurgereFisier(matriceCuvinte, indice);
					if (existaDenumireDeja == false)
					{
						//fisier text cu denumirile tabelelor create cu comanda CREATE
						fis.open("Tabele.txt", ios::out | ios::app);
						fis << matriceCuvinte[2] << endl;
						fis.close();
						for (int k = 3; k < dimensiune; k = k + 4)
						{
							ofstream file;
							string numeFisier;
							numeFisier = (string)matriceCuvinte[2] + ".txt";
							file.open(numeFisier, ios::out | ios::app);
							file << matriceCuvinte[k] << endl;
							file << matriceCuvinte[k + 1] << endl;
							file << matriceCuvinte[k + 2] << endl;
							file << matriceCuvinte[k + 3];
							file << '\n';
							file.close();
							//fisierul binar
						}
					}
					else {
						cout << endl << "Tabela pe care doriti sa o introduceti:  " << matriceCuvinte[2] << "  exista deja" << endl;
					}
				}
				else
				{
					cout << "\n eroare \n";
				}
			}
		}
	}

	string obtineNumeComanda() {
		return "create";
	}
};

class INSERT {
	char** matriceCuvinte = nullptr;
	char** matriceValori = nullptr;
	int dimensiune = 0;
	char* buffer = nullptr;
	int dimensiuneMatriceValori = 0;
public:
	INSERT(char* buffer)
	{
		char copie[1000];
		char* token;
		strcpy(copie, buffer);
		token = strtok(copie, " ");
		int nrcuv = 1;
		while (token != NULL)
		{
			nrcuv++;
			token = strtok(NULL, " ");
		}
		this->matriceCuvinte = new char* [nrcuv];
		strcpy(copie, buffer);
		token = strtok(buffer, " ");
		this->dimensiune = 0;
		while (token != NULL)
		{
			this->matriceCuvinte[this->dimensiune] = new char[strlen(token) + 1];
			strcpy(this->matriceCuvinte[this->dimensiune], token);
			this->dimensiune++;
			token = strtok(NULL, " ");
		}
		strcpy(buffer, copie);
		this->buffer = buffer;

	}
	INSERT()
	{
		this->matriceCuvinte = nullptr;
		this->matriceValori = nullptr;
		this->dimensiune = 0;
		this->buffer = nullptr;
		this->dimensiuneMatriceValori = 0;
	}
	friend ostream& operator<<(ostream& consola, INSERT ins);
	friend istream& operator>>(istream& consola, INSERT& ins);

	/*INSERT(const INSERT& i)
	{
		strcpy(this->buffer, i.buffer);
		this->dimensiune = i.dimensiune;

		if (this->matriceCuvinte != nullptr)
		{
			for (int j = 0; j < this->dimensiune; j++)
			{
				delete[] this->matriceCuvinte[j];
			}
			delete[] this->matriceCuvinte;
		}
		this->dimensiune = i.dimensiune;

		this->matriceCuvinte = new char* [i.dimensiune];
		for (int j = 0; j < i.dimensiune; j++)
		{
			this->matriceCuvinte[j] = i.matriceCuvinte[j];
		}
	}*/
	//~INSERT()
	//{
	//	if ( this->buffer != nullptr)
	//	{
	//		delete[] this->buffer;
	//	}
	//	if (this->matriceCuvinte != nullptr)
	//	{
	//		for (int j = 0; j < this->dimensiune; j++)
	//		{
	//			delete[] this->matriceCuvinte[j];
	//		}
	//		delete[] this->matriceCuvinte;
	//	}
	//}

	char** getMatriceCuvinte()
	{
		return this->matriceCuvinte;
	}
	char** getMatriceValori()
	{
		return this->matriceValori;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}
	int getDimensiuneMatriceValori()
	{
		return this->dimensiuneMatriceValori;
	}
	char* getBuffer()
	{
		return this->buffer;
	}
	//INSERT operator=(INSERT& i)
	//{
	//	strcpy(this->buffer, i.buffer);
	//	this->dimensiune = i.dimensiune;
	//
	//	if (this->matriceCuvinte != nullptr)
	//	{
	//		for (int j = 0; j < this->dimensiune; j++)
	//		{
	//			delete[] this->matriceCuvinte[j];
	//		}
	//		delete[] this->matriceCuvinte;
	//	}
	//	this->dimensiune = i.dimensiune;
	//	this->matriceCuvinte = new char*[i.dimensiune];
	//	for (int j = 0; j < i.dimensiune; j++)
	//	{
	//		this->matriceCuvinte[j] = i.matriceCuvinte[j];
	//	}
	//return *this;
	//}
	void prelucrareComanda()
	{
		if (strcmp(matriceCuvinte[3], "VALUES") == 0 && matriceCuvinte[dimensiune - 1][0] == '('
			&& matriceCuvinte[dimensiune - 1][strlen(matriceCuvinte[dimensiune - 1]) - 1] == ')')
		{
			cout << "\n Nume tabela inserata este:" << matriceCuvinte[2];
			//scap de paranteze in ultimu element al matricei

			matriceCuvinte[dimensiune - 1][strlen(matriceCuvinte[dimensiune - 1]) - 1] = '\0';
			char aux[1000];
			strcpy(aux, matriceCuvinte[dimensiune - 1] + 1);
			strcpy(matriceCuvinte[dimensiune - 1], aux);

			// strtok dupa virgula pe ultimu element
			char copie[1000] = "";
			char* token = nullptr;
			strcpy(copie, matriceCuvinte[dimensiune - 1]);

			////calculeaza cate cuvinte sunt in ultimu vector din matrice
			token = strtok(copie, ",");
			int nrcuv = 1;
			while (token != NULL)
			{
				nrcuv++;
				token = strtok(NULL, ",");
			}

			//aloca memorie pt vector ca sa-l poata imparti
			matriceValori = new char* [nrcuv];
			strcpy(copie, matriceCuvinte[dimensiune - 1]);
			token = strtok(copie, ",");

			while (token != NULL)
			{
				matriceValori[dimensiuneMatriceValori] = new char[strlen(token) + 1];
				strcpy(matriceValori[dimensiuneMatriceValori], token);
				dimensiuneMatriceValori++;
				token = strtok(NULL, ",");
			}
			bool existaDenumireDeja = false;
			int indice = 2;
			existaDenumireDeja = FileUtils::parcurgereFisier(matriceCuvinte, indice);
			if (existaDenumireDeja == false)
			{
				cout << "Aceasta tabela nu exista" << endl;
			}
			else
			{
				string numeFisierBinar = string(matriceCuvinte[2]) + ".bin";
				ofstream fileBinary(numeFisierBinar, ios::app | ios::binary);
				for (int i = 0; i < dimensiuneMatriceValori; i++)
				{
					string cuvant = string(matriceValori[i]);
					if (!fileBinary)
					{
						cout << "Nu s-a putut deschide fisierul binar!";
					}
					cuvant += '\n';
					int dim = cuvant.size() + 1;
					fileBinary.write((char*)&dim, sizeof(int));
					fileBinary.write(cuvant.c_str(), dim * sizeof(char));
				}
				fileBinary.close();
			}
		}
		else
		{
			cout << "\n EROARE \n";
		}
	}
	string obtineNumeComanda() {
		return "insert";
	}
};

class SELECT {
	char** matriceCuvinte = nullptr;
	char** matriceValori = nullptr;
	int dimensiune = 0;
	int dimensiuneMatriceValori = 0;
	char* buffer = nullptr;
public:
	SELECT(char* buffer)
	{
		this->buffer = buffer;
	}

	char** getMatriceCuvinte()
	{
		return this->matriceCuvinte;
	}
	char** getMatriceValori()
	{
		return this->matriceValori;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}
	int getDimensiuneMatriceValori()
	{
		return this->dimensiuneMatriceValori;
	}
	char* getBuffer()
	{
		return this->buffer;
	}

	void getAllValuesFromCommand(char* tableName)
	{
		string table = string(tableName);
		vector<string> lines = FileUtils::readLinesFromBinaryFile(table + ".bin");
		int txtNrLines = FileUtils::getNrLinesFile(table + ".txt") + 1;
		int linesPerParameter = txtNrLines / 4;

		string line = "";
		int i;
		for (i = 0; i < lines.size(); i++)
		{
			line += lines[i] + " ";

			if (i > 0 && i % linesPerParameter == 0) {
				cout << line << '\n';
				line = "";
			}
		}
		if (i > 0 && i % linesPerParameter == 0)
			cout << line << '\n';
	}

	void getValuesFromWhereCommand(char* tableName, char** columns, int dimColumns, char* whereColumn, char* whereValue)
	{
		string table = string(tableName);
		string columnWhere = string(whereColumn);
		string valueWhere = string(whereValue);
		vector<string> vectorColumns = StringUtils::fromCharMatrixToVector(columns, dimColumns);

		int txtNrLines = FileUtils::getNrLinesFile(table + ".txt") + 1;

		vector<vector<string>> vectorLines = FileUtils::readVectorLinesFromBinaryFile(table + ".bin");
		int index;

		index = FileUtils::getIndexTxt(table + ".txt", columnWhere);

		cout << "\nRezultate:\n";

		for (int i = 0; i < vectorLines.size(); i++)
		{
			StringUtils::displayVector(vectorLines[i]);

			cout << "**************";

			if (vectorLines[i][index] == valueWhere) {
				for (int j = 0; j < vectorLines[i].size(); j++)
					cout << vectorLines[i][j] << ' ';
			}
		}
	}

	void getValuesFromCommand(char* tableName, char** columns, int dimColumns)
	{
		string table = string(tableName);
		vector<string> vectorColumns = StringUtils::fromCharMatrixToVector(columns, dimColumns);

		int txtNrLines = FileUtils::getNrLinesFile(table + ".txt") + 1;

		vector<string> lines = FileUtils::readLinesFromBinaryFile(table + ".bin");
		int index;
		for (int i = 0; i < vectorColumns.size(); i++)
		{
			index = FileUtils::getIndexTxt(table + ".txt", vectorColumns[i]);

			cout << "\nRezultatele pentru coloana " << vectorColumns[i] << " sunt:\n";

			for (int i = 0; i < lines.size(); i++)
			{
				if (i % (txtNrLines / 4) == index)
					cout << lines[i] << '\n';
			}
		}
	}

	void prelucrareComanda()
	{

		//portionare propozitie
		char copie[1000] = "";
		char* token = nullptr;
		strcpy(copie, this->buffer);
		token = strtok(copie, " =");
		int nrcuv = 1;
		while (token != NULL)
		{
			nrcuv++;
			token = strtok(NULL, " =");
		}
		this->matriceCuvinte = new char* [nrcuv];
		token = strtok(this->buffer, " =");
		this->dimensiune = 0;
		while (token != NULL)
		{
			this->matriceCuvinte[this->dimensiune] = new char[strlen(token) + 1];
			strcpy(this->matriceCuvinte[this->dimensiune], token);
			this->dimensiune++;
			token = strtok(NULL, " =");
		}

		//divizare coloane din paranteze:
		if (matriceCuvinte[1][strlen(matriceCuvinte[1]) - 1] == ')' && matriceCuvinte[1][0] == '(')
		{
			char copie[1000] = "";
			char* token = nullptr;

			strcpy(copie, this->matriceCuvinte[1]);

			////calculeaza cate cuvinte sunt intre paranteze
			token = strtok(copie, ",()");
			int nrcuv = 1;
			while (token != NULL)
			{
				nrcuv++;
				token = strtok(NULL, ",()");
			}

			//aloca memorie pt vector ca sa-l poata imparti
			this->matriceValori = new char* [nrcuv];


			strcpy(copie, this->matriceCuvinte[1]);
			token = strtok(copie, ",()");

			while (token != NULL)
			{
				this->matriceValori[dimensiuneMatriceValori] = new char[strlen(token) + 1];
				strcpy(this->matriceValori[dimensiuneMatriceValori], token);
				dimensiuneMatriceValori++;
				token = strtok(NULL, ",()");
			}
		}

		//afisare erori sau informatii cu privire la comenzi
		if (strcmp(matriceCuvinte[1], "ALL") != 0 && (matriceCuvinte[1][strlen(matriceCuvinte[1]) - 1] != ')' && matriceCuvinte[1][0] == '('))
		{
			cout << "\n eroare0 \n";
		}
		else
		{

			if (strcmp(matriceCuvinte[2], "FROM") != 0)
			{
				cout << "\n eroare \n";
			}
			else
			{

				if (dimensiune > 7)
				{
					cout << "\n eroare1 \n";
				}
				else
				{
					if (dimensiune > 4)
					{

						if (strcmp(matriceCuvinte[4], "WHERE") != 0)
						{
							cout << "\n eroare2 \n";
						}
						else
						{

							if (strcmp(matriceCuvinte[1], "ALL") == 0)
							{
								cout << "Coloane: ALL" << endl;
							}
							else
							{
								if (matriceValori != nullptr)
								{
									cout << "Coloanele sunt:  " << endl;
									for (int i = 0; i < dimensiuneMatriceValori; i++)
									{
										cout << matriceValori[i] << endl;
									}
								}
								else
								{
									cout << "\n eroare5\n";
								}
							}
							cout << "\n nume tabela (FROM) este: " << matriceCuvinte[3] << endl;
							cout << "\nNume coloana din conditie este: " << matriceCuvinte[5] << "  iar valoarea este:  " << matriceCuvinte[6] << endl;

							//this->getValuesFromWhereCommand(matriceCuvinte[3], matriceValori, dimensiuneMatriceValori, matriceCuvinte[5], matriceCuvinte[6]);
						}
					}
					else
					{
						if (strcmp(matriceCuvinte[1], "ALL") == 0)
						{
							this->getAllValuesFromCommand(matriceCuvinte[3]);
						}
						else
						{
							if (matriceValori == nullptr)
							{
								cout << "\n eroare \n";
							}
							else
							{
								this->getValuesFromCommand(matriceCuvinte[3], matriceValori, dimensiuneMatriceValori);
							}
						}
					}
				}
			}
		}
	}
	string obtineNumeComanda() {
		return "select";
	}
};

class UPDATE {
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;
public:
	UPDATE(char** matriceCuvinte, int dimensiune)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
	}

	char** getMatriceCuvinte()
	{
		return this->matriceCuvinte;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}

	/*UPDATE operator=(UPDATE& fc2)
	{

		this->matriceCuvinte = fc2.matriceCuvinte;
		this->dimensiune = fc2.dimensiune;

		return *this;
	}*/
	void prelucrareComanda()
	{

		if (strcmp(matriceCuvinte[2], "SET") == 0 && strcmp(matriceCuvinte[5], "WHERE") == 0)
		{
			if (dimensiune > 8)
			{
				cout << "\n EROARE \n";
			}
			else
			{
				if (dimensiune < 8)
				{
					cout << "\n eroare \n";
				}
				else
				{
					int indice = 1;
					bool existaDenumireDeja = FileUtils::parcurgereFisier(matriceCuvinte, indice);
					if (existaDenumireDeja == false)
					{
						cout << "Aceasta tabela nu exista" << endl;
					}
					else
					{
						//update tabela
						cout << "Pentru SET: \n nume coloana este:  " << matriceCuvinte[3] << "  iar valoarea este: " << matriceCuvinte[4];
						cout << "\nPentru WHERE: \n nume coloana este:  " << matriceCuvinte[6] << "  iar valoarea este: " << matriceCuvinte[7];
					}

				}

			}

		}
		else
		{

			cout << "\n EROARE \n";
		}
	}
	string obtineNumeComanda() {
		return "update";
	}
};

class DELETE {
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;
public:
	DELETE(char** matriceCuvinte, int dimensiune)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
	}
	char** getMatriceCuvinte()
	{
		return this->matriceCuvinte;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}

	//destructor


	//operator =
	/*DELETE operator=(DELETE& fc2)
	{

		this->matriceCuvinte = fc2.matriceCuvinte;
		this->dimensiune = fc2.dimensiune;

		return *this;
	}*/


	void  prelucrareComanda()
	{
		if (dimensiune < 4)
		{
			cout << "\n eroare\n";
		}
		else
		{
			if (strcmp(matriceCuvinte[3], "WHERE") == 0)
			{
				if (dimensiune > 7)
				{
					cout << "\n eroare \n";
				}
				else
				{
					bool existaDenumireDeja = false;
					int indice = 2;
					existaDenumireDeja = FileUtils::parcurgereFisier(matriceCuvinte, indice);
					if (existaDenumireDeja == false)
					{
						cout << "Aceasta tabela nu exista, nu o puteti sterge" << endl;
					}
					else
					{
						//stergere coloane din fisieru text
						cout << "\n pentru WHERE: \n  nume coloana este:   " << matriceCuvinte[4] << "  cu valoarea: " << matriceCuvinte[5];
					}
				}
			}
			else
			{
				cout << "\n eroare \n";
			}
		}

	}
	string obtineNumeComanda() {
		return "delete";
	}
};

class DROP {
	char* numeTabel = nullptr;
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;
public:
	DROP(char** matriceCuvinte, int dimensiune)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;

	}
	DROP(char** matriceCuvinte, int dimensiune, char* numeTabel)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
		this->numeTabel = new char[1 + strlen(numeTabel)];
		if (this->numeTabel != nullptr)
			strcpy(this->numeTabel, numeTabel);
	}

	char** getMatriceCuvinte()
	{
		return this->matriceCuvinte;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}

	/*DROP operator=(DROP& fc2)
	{

		this->matriceCuvinte = fc2.matriceCuvinte;
		this->dimensiune = fc2.dimensiune;
		strcpy(this->numeTabel, fc2.numeTabel);

		return *this;
	}*/

	void prelucrareComanda()
	{
		if (dimensiune > 3) {
			cout << "\n EROARE:\n";
		}
		else {

			if (dimensiune <= 2)
			{
				cout << "\n EROARE:\n";
			}
			else
			{
				cout << "Tabela de sters: " << matriceCuvinte[2];
				bool existaDenumireDeja = false;
				int indice = 2;
				existaDenumireDeja = FileUtils::parcurgereFisier(matriceCuvinte, indice);
				if (existaDenumireDeja == false)
				{
					cout << "Aceasta tabela nu exista, nu o puteti sterge" << endl;
				}
				else
				{
					FileUtils::stergereTabela(this->matriceCuvinte, indice);
				}

			}

		}
	}
	string obtineNumeComanda() {
		return "drop";
	}
};

class DISPLAY {
	char* numeTabel = nullptr;
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;
public:
	DISPLAY(char** matriceCuvinte, int dimensiune)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
	}
	DISPLAY(char** matriceCuvinte, int dimensiune, char* numeTabel)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
		this->numeTabel = new char[1 + strlen(numeTabel)];
		if (this->numeTabel != nullptr)
			strcpy(this->numeTabel, numeTabel);
	}

	char** getMatriceCuvinte()
	{
		return this->matriceCuvinte;
	}
	int getDimensiune()
	{
		return this->dimensiune;
	}
	char* getNumeTabel()
	{
		return this->numeTabel;
	}

	/*~DISPLAY()
	{
		if (matriceCuvinte != nullptr)
		{
			for (int i = 0; i < dimensiune; i++)
			{
				delete[] matriceCuvinte[i];
			}
			delete[] matriceCuvinte;
		}

		delete numeTabel;
	}*/
	void prelucrareComanda()
	{
		if (dimensiune > 3) {
			cout << "\n EROARE:\n";
		}
		else {

			if (dimensiune <= 2)
			{
				cout << "\n EROARE:\n";
			}
			else
			{
				cout << "Tabela de (sters)/afisat: " << matriceCuvinte[2];
				string numeFisierTxt = string(matriceCuvinte[2]) + ".txt";
				FileUtils::afisareTXT(numeFisierTxt);
			}

		}
	}
	string obtineNumeComanda() {
		return "display";
	}
};

class ImportInsert {
private:
	char* numeTabel = nullptr;
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;
public:
	ImportInsert(char** matriceCuvinte, int dimensiune)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
	}
	void prelucrareComanda()
	{
		if (FileUtils::txtExists(string(matriceCuvinte[1]) + ".txt"))
			FileUtils::functieCSV(matriceCuvinte[2], matriceCuvinte[1]);
	}
	string obtineNumeComanda() {
		return "import";
	}

};

class ParametriComanda {
	char** matriceCuvinte = nullptr;
	int dimensiune = 0;
	char* comanda;
	char* buffer;

public:
	ParametriComanda(char** matriceCuvinte, int dimensiune, char* buffer)
	{
		this->matriceCuvinte = matriceCuvinte;
		this->dimensiune = dimensiune;
		this->comanda = new char[100];
		strcpy(this->comanda, matriceCuvinte[0]);
		this->buffer = new char[1000];
		this->buffer = buffer;
	}
	void afisareParametriComanda(char** matriceCuvinte, int dimensiune, char* buffer)
	{

		if (strcmp(comanda, "DROP") == 0 || strcmp(comanda, "CREATE") == 0 || strcmp(comanda, "DISPLAY") == 0 || strcmp(comanda, "SELECT") == 0 || strcmp(comanda, "INSERT") == 0 || strcmp(comanda, "UPDATE") == 0 || strcmp(comanda, "DELETE") == 0 || strcmp(comanda, "IMPORT") == 0)
		{

			if (strcmp(comanda, "SELECT") == 0)
			{
				SELECT comanda(buffer);
				comanda.prelucrareComanda();
			}
			if (strcmp(comanda, "INSERT") == 0)
			{
				if (strcmp(matriceCuvinte[1], "INTO") == 0)
				{
					INSERT comanda(buffer);
					comanda.prelucrareComanda();
				}
				else
				{
					cout << "\n EROARE \n";
				}
			}
			if (strcmp(comanda, "CREATE") == 0)
			{

				if (strcmp(matriceCuvinte[1], "TABLE") == 0)
				{
					CREATE comanda(matriceCuvinte, dimensiune);
					comanda.prelucrareComanda();
				}
				else {
					cout << "\n EROARE \n";
				}
			}
			if (strcmp(comanda, "UPDATE") == 0)
			{
				UPDATE comanda(matriceCuvinte, dimensiune);
				comanda.prelucrareComanda();
			}
			if (strcmp(comanda, "DELETE") == 0)
			{
				if (strcmp(matriceCuvinte[1], "FROM") == 0)
				{
					DELETE comanda(matriceCuvinte, dimensiune);
					comanda.prelucrareComanda();
				}
				else
				{
					cout << "\n EROARE \n";
				}
			}
			if (strcmp(comanda, "DROP") == 0)
			{
				if (strcmp(matriceCuvinte[1], "TABLE") == 0)
				{
					DROP comanda(matriceCuvinte, dimensiune);
					comanda.prelucrareComanda();
				}
				else {
					cout << "\n EROARE \n";
				}
			}
			if (strcmp(comanda, "DISPLAY") == 0)
			{
				if (strcmp(matriceCuvinte[1], "TABLE") == 0)
				{
					DISPLAY comanda(matriceCuvinte, dimensiune);
					comanda.prelucrareComanda();
				}
				else {
					cout << "\n EROARE \n";
				}
			}
			if (strcmp(comanda, "IMPORT") == 0)
			{
				if (dimensiune == 3)
				{
					ImportInsert comanda(matriceCuvinte, dimensiune);
					comanda.prelucrareComanda();
				}
				else {
					cout << "\n EROARE \n";
				}
			}
		}
		else {
			cout << "\n Eroare \n";
		}
	}
};

class Files {
private:
	vector<string> files;
	vector<string> files2;
public:
	static void citireCMD(string numeFisier)
	{
		ifstream fis(numeFisier, ios::in);
		string line;
		if (fis.is_open())
		{
			while (!fis.eof())
			{
				getline(fis, line);
				propozitie p(line);

				ParametriComanda g(p.getMatriceCuvinte(), p.getNumarCuvinte(), p.getBuffer());
				g.afisareParametriComanda(p.getMatriceCuvinte(), p.getNumarCuvinte(), p.getBuffer());
			}
			fis.close();
		}
	}
};
