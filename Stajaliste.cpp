#include "Stajaliste.h"

Stajaliste::Stajaliste()
{
}

Stajaliste::Stajaliste(int broj, string naziv) : broj(broj), naziv(naziv)
{
}

Stajaliste::~Stajaliste()
{
	while (!linije.empty()) {
		linije.back() = nullptr;
		linije.pop_back();
	}

}

int Stajaliste::dohvatiBrojStajalista() const
{
	return this->broj;
}

int Stajaliste::dohvatiIdStajalista() const
{
	return idS;
}

string Stajaliste::dohvatiNazivStajalista() const
{
	return this->naziv;
}

list<Linija*> Stajaliste::dohvatiLinijeStajalista() const
{
	return linije;
}

Stajaliste* Stajaliste::ucitajStajaliste(string naziv)
{
	int broj;
	string brojPom;

	brojPom = naziv.substr(0, naziv.find(" "));
	broj = stoi(brojPom);
	naziv = naziv.substr(naziv.find(" "), naziv.length());
	Stajaliste* s = new Stajaliste(broj, naziv);
	return s;
}

Stajaliste* Stajaliste::nadjiStajaliste(int brojS, list<Stajaliste*> listaS)
{
	list<Stajaliste*>::iterator it;

	for (it = listaS.begin(); it != listaS.end(); it++)
	{
		if ((*it)->broj == brojS) return *it;
	}
	return nullptr;
}

Stajaliste* Stajaliste::nadjiStajalistePremaId(int id, list<Stajaliste*> listaS)
{
	list<Stajaliste*>::iterator it;

	for (it = listaS.begin(); it != listaS.end(); it++)
	{
		if ((*it)->idS == id) return *it;
	}
	return nullptr;
}

void Stajaliste::dodajLinijuUStajaliste(Linija* l)
{
	linije.push_back(l);
}

void Stajaliste::dodajIdStajalistu(int id)
{
	this->idS = id;
}





