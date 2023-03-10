#include "Linija.h"

Linija::Linija()
{
}

Linija::Linija(string oznaka) : oznaka(oznaka)
{
}

Linija::~Linija()
{
	while (!this->stajalistaA.empty()) {
		stajalistaA.back() = nullptr;
		stajalistaA.pop_back();
	}
	while (!this->stajalistaB.empty()) {
		stajalistaB.back() = nullptr;
		stajalistaB.pop_back();
	}

	//delete this;
}
struct uporediLinije {
	bool operator()(Linija* l1, Linija* l2) {

		if (strcmp(l1->dohvatiOznakuLinije().c_str(), (l2->dohvatiOznakuLinije()).c_str()) < 0) return true;
		else return false;
	}
};

string Linija::dohvatiOznakuLinije() const
{
	return this->oznaka;
}

int Linija::dohvatiIdLinije() const
{
	return this->idL;
}

Linija* Linija::ucitajLiniju(string& tekst)
{
	int broj;
	string oznaka;
	oznaka = tekst.substr(0, tekst.find(" "));
	tekst.erase(0, oznaka.length() + 1);
	Linija* l = new Linija(oznaka);
	return l;
}

void Linija::dodajStajalisteULinijuA(Stajaliste* s)
{
	stajalistaA.push_back(s);
}

void Linija::dodajStajalisteULinijuB(Stajaliste* s)
{
	stajalistaB.push_back(s);
}

void Linija::dodajId(int id)
{
	this->idL = id;
}

list<Stajaliste*> Linija::dohvatiStajalistaSmerA() const
{
	return stajalistaA;
}

list<Stajaliste*> Linija::dohvatiStajalistaSmerB() const
{
	return stajalistaB;
}


Linija* Linija::nadjiLiniju(string oznaka, list<Linija*> l)
{
	list<Linija*>::iterator it;

	for (it = l.begin(); it != l.end(); it++)
	{
		if ((*it)->oznaka == oznaka) return *it;

	}
	return nullptr;
}

Linija* Linija::nadjiLinijuPremaId(int id, list<Linija*> l)
{
	list<Linija*>::iterator it;

	for (it = l.begin(); it != l.end(); it++)
	{
		if ((*it)->idL == id) return *it;

	}
	return nullptr;

}

bool Linija::operator<(const Linija& l1) const
{
	return (this->oznaka.c_str() > l1.oznaka.c_str());
}



void Linija::sortirajLinije(list<Linija*>& linije)
{
	linije.sort(uporediLinije());


}



