#ifndef LINIJA_H
#define LINIJA_H

#include"Stajaliste.h"

#include <algorithm>
#include <list>
#include <string>
using namespace std;

class Linija {
public:
	Linija();
	Linija(string oznaka);
	~Linija();

	Linija* ucitajLiniju(string& tekst);

	//getter funckije
	string dohvatiOznakuLinije() const;
	int dohvatiIdLinije() const;
	list<Stajaliste*> dohvatiStajalistaSmerA() const;
	list<Stajaliste*> dohvatiStajalistaSmerB() const;

	//setter funckije
	void dodajStajalisteULinijuA(Stajaliste* s);
	void dodajStajalisteULinijuB(Stajaliste* s);
	void dodajId(int id);


	//funckije za nalazenje
	Linija* nadjiLiniju(string oznaka, list<Linija*> l);
	Linija* nadjiLinijuPremaId(int id, list<Linija*> l);
	bool operator< (const Linija& l1) const;
	void sortirajLinije(list<Linija*>& linije);



private:
	string oznaka;
	int idL;
	list <Stajaliste*> stajalistaA;
	list <Stajaliste*> stajalistaB;


};
#endif
