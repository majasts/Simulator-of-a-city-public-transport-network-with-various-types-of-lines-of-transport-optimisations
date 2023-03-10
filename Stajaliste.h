#ifndef STAJALISTE_H
#define STAJALISTE_H


#include <cmath>
#include <list>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Linija;

class Stajaliste {
public:
	Stajaliste();
	Stajaliste(int broj, string naziv);
	~Stajaliste();

	Stajaliste* ucitajStajaliste(string naziv);

	//getter funkcije
	int dohvatiBrojStajalista() const;
	int dohvatiIdStajalista() const;
	string dohvatiNazivStajalista() const;
	list<Linija*> dohvatiLinijeStajalista() const;

	void dodajLinijuUStajaliste(Linija* l);
	void dodajIdStajalistu(int id);

	//funckije za trazenje
	Stajaliste* nadjiStajaliste(int brojS, list<Stajaliste*>);
	Stajaliste* nadjiStajalistePremaId(int id, list<Stajaliste*> listaS);

private:
	int broj;
	int idS;
	string naziv;
	list <Linija*> linije;

};

#endif
