#include "Mreza.h"
#include "Exception.h"

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;


Mreza::Mreza()
{
}

Mreza::~Mreza()
{
	obrisiMatricu(matricaLinija, dohvatiBrojSvihLinija());
	obrisiMatricu(matricaStajalista, dohvatiBrojSvihStajalista());

	while (!sveLinije.empty()) {
		sveLinije.back()->~Linija();
		sveLinije.pop_back();
	}
	while (!svaStajalista.empty()) {
		svaStajalista.back()->~Stajaliste();
		svaStajalista.pop_back();
	}

}

void Mreza::procitajStajalista(string nazivFajla)
{
	fstream ulazniFajl(nazivFajla, ios::in);
	if (!ulazniFajl) throw Neuspesno("Fajl sa datom putanjom nije pronađen\n");
	//proveriti da li je otvoren
	while (ulazniFajl.peek() != EOF) {
		string naziv;
		getline(ulazniFajl, naziv);
		svaStajalista.push_back(ucitajStajaliste(naziv));
	}
	ulazniFajl.close();
}

void Mreza::procitajLinije(string nazivFajla)
{
	fstream ulazniFajl(nazivFajla, ios::in);
	if (!ulazniFajl) throw Neuspesno("Fajl sa datom putanjom nije pronađen\n");

	while (ulazniFajl.peek() != EOF) {
		string tekst, pom;

		getline(ulazniFajl, tekst);
		Linija* novaLinija = ucitajLiniju(tekst);
		sveLinije.push_back(novaLinija);

		istringstream ss(tekst);
		//citanje stajalista u smeruA
		while (getline(ss, pom, ' ')) {
			if (pom.find("#") != string::npos) break;
			Stajaliste* s = poveziStajalisteSaLinijom(novaLinija, pom);
			novaLinija->dodajStajalisteULinijuA(s);
		}

		//krajnje stajaliste za smerA i pocetno za smerB
		Stajaliste* s = poveziStajalisteSaLinijom(novaLinija, pom);
		novaLinija->dodajStajalisteULinijuA(s);
		novaLinija->dodajStajalisteULinijuB(s);

		//citanje stajalista u smeruB
		while (getline(ss, pom, ' ')) {
			Stajaliste* s = poveziStajalisteSaLinijom(novaLinija, pom);
			novaLinija->dodajStajalisteULinijuB(s);
		}
	}
	ulazniFajl.close();
}

Stajaliste* Mreza::poveziStajalisteSaLinijom(Linija* l, string pom)
{
	int brojS = stoi(pom);
	Stajaliste* s = nadjiStajaliste(brojS, svaStajalista);
	if (nadjiLiniju(l->dohvatiOznakuLinije(), s->dohvatiLinijeStajalista()) == nullptr) {
		s->dodajLinijuUStajaliste(l);
	}
	return s;
}

vector<int> Mreza::nadjiLinijuSaMaxZajednickihS(int idTrenutneL)
{
	int max = 1;
	vector<int> maxId;

	for (int i = 0; i < dohvatiBrojSvihLinija(); i++) {

		if (matricaLinija[idTrenutneL][i] >= max) {
			max = matricaLinija[idTrenutneL][i];
			//Za slucaj da vise Linija ima isti broj zajednickih stajalista:
			while (!maxId.empty() && maxId.back() < max) {
				maxId.pop_back();
			}
			maxId.push_back(i);
		}
	}
	return maxId;
}

bool Mreza::potrebnoPresedanje(int idTrenutneLinije, int idStajalista1, int idStajalista2)
{
	Linija* l = nadjiLinijuPremaId(idTrenutneLinije, sveLinije);

	//ukoliko bilo gde u nizu stajalista u okviru linije nadjemo stajaliste2 posle stajalista1 presedanje NIJE potrebno

	list<Stajaliste*> trenutnaS = l->dohvatiStajalistaSmerA();
	for (auto itS = trenutnaS.begin(); itS != trenutnaS.end(); itS++) {
		if ((*itS)->dohvatiIdStajalista() == idStajalista1) {
			if (next(itS, 1) != trenutnaS.end()) {
				if ((*next(itS, 1))->dohvatiIdStajalista() == idStajalista2) return false;
			}
		}
	}
	trenutnaS = l->dohvatiStajalistaSmerB();
	for (auto itS = trenutnaS.begin(); itS != trenutnaS.end(); itS++) {
		if ((*itS)->dohvatiIdStajalista() == idStajalista1) {
			if (next(itS, 1) != trenutnaS.end() && (*next(itS, 1))->dohvatiIdStajalista() == idStajalista2) return false;

		}
	}
	return true;
}

int Mreza::idLinijeZaPresedanje(int idStajalista1, int idStajalista2)
{
	//Trazimo liniju u kojoj ce posle stajalista1 biti stajaliste2 i to radimo za oba smera
	for (auto it = sveLinije.begin(); it != sveLinije.end(); it++) {
		list<Stajaliste*> trenutnaS = (*it)->dohvatiStajalistaSmerA();
		for (auto itS = trenutnaS.begin(); next(itS) != trenutnaS.end(); itS++) {
			if ((*itS)->dohvatiIdStajalista() == idStajalista1 && (*next(itS, 1))->dohvatiIdStajalista() == idStajalista2) {
				return (*it)->dohvatiIdLinije();
			}
		}
		trenutnaS = (*it)->dohvatiStajalistaSmerB();
		for (auto itS = trenutnaS.begin(); next(itS) != trenutnaS.end(); itS++) {
			if ((*itS)->dohvatiIdStajalista() == idStajalista1 && (*next(itS, 1))->dohvatiIdStajalista() == idStajalista2) {
				return (*it)->dohvatiIdLinije();
			}
		}
	}
	return inf;
}

void Mreza::minBrojPresedanja(int idPocetnogS, vector<vector<int>>& putanje)
{

	vector<bool> posecenCvor;
	posecenCvor.resize(svaStajalista.size(), false);

	vector<int> rastojanjeOdPocetnogS;
	rastojanjeOdPocetnogS.resize(sveLinije.size(), inf);
	rastojanjeOdPocetnogS[idPocetnogS] = 0;

	for (int i = 0; i < sveLinije.size(); i++)
	{
		int m = najmanjeRastojanje(rastojanjeOdPocetnogS, posecenCvor); // vertex not yet included.
		posecenCvor[m] = true;// m with minimum distance included in Tset.
		for (int i = 0; i < sveLinije.size(); i++)
		{
			// Updating the minimum distance for the particular node.
			if (!posecenCvor[i] && matricaLinija[m][i] > 0 && rastojanjeOdPocetnogS[m] != inf && (rastojanjeOdPocetnogS[m] + 1) < rastojanjeOdPocetnogS[i])
			{
				rastojanjeOdPocetnogS[i] = rastojanjeOdPocetnogS[m] + 1;
				putanje[i].push_back(m);
			}
		}
	}
}



Stajaliste* Mreza::ispisiStajaliste(int broj)
{
	Stajaliste* s = nadjiStajaliste(broj, svaStajalista);
	if (s == nullptr) return s;
	ofstream izlazniFajl;
	izlazniFajl.open("stajaliste_" + to_string(broj) + ".txt");
	izlazniFajl << s->dohvatiBrojStajalista() << s->dohvatiNazivStajalista() << " [";

	list<Linija*> l = s->dohvatiLinijeStajalista();
	sortirajLinije(l);
	for (list<Linija*>::iterator it = l.begin(); it != l.end(); it++) {
		if (it != l.begin()) izlazniFajl << " "; //dodavanje znaka razmaka izmedju oznaka linija
		izlazniFajl << (*it)->dohvatiOznakuLinije();

	}
	izlazniFajl << "]";
}

Linija* Mreza::ispisiLiniju(string oznaka)
{
	Linija* l = nadjiLiniju(oznaka, sveLinije);
	if (l == nullptr) return l;
	list<Stajaliste*> smerA = l->dohvatiStajalistaSmerA();
	list<Stajaliste*> smerB = l->dohvatiStajalistaSmerB();
	ofstream izlazniFajl;
	izlazniFajl.open("linija_" + oznaka + ".txt");
	izlazniFajl << oznaka << smerA.front()->dohvatiNazivStajalista() << "->" << smerA.back()->dohvatiNazivStajalista().erase(0, 1);

	for (; !smerA.empty(); smerA.pop_front()) {
		izlazniFajl << endl << smerA.front()->dohvatiBrojStajalista() << smerA.front()->dohvatiNazivStajalista();
	}
	for (; !smerB.empty(); smerB.pop_front()) {
		izlazniFajl << endl << smerB.front()->dohvatiBrojStajalista() << smerB.front()->dohvatiNazivStajalista();
	}

}

Linija* Mreza::ispisiStatistikuLinije(string oznaka)
{
	ofstream izlazniFajl;
	izlazniFajl.open("statistika_" + oznaka + ".txt");

	Linija* l = nadjiLiniju(oznaka, sveLinije);
	if (l == nullptr) return l;

	izlazniFajl << l->dohvatiOznakuLinije() << endl;
	int idLinije = l->dohvatiIdLinije();

	bool first = true;
	for (int i = 0; i < dohvatiBrojSvihLinija(); i++) {
		if (matricaLinija[idLinije][i] > 0) {

			if (!first) izlazniFajl << " ";
			first = false;
			izlazniFajl << nadjiLinijuPremaId(i, sveLinije)->dohvatiOznakuLinije();
		}
	}
	izlazniFajl << endl;
	vector<int> maxId = nadjiLinijuSaMaxZajednickihS(idLinije);

	//za slucaj da vise linija ima isti broj zajednickih stajalista
	while (!maxId.empty()) {
		izlazniFajl << nadjiLinijuPremaId(maxId.back(), sveLinije)->dohvatiOznakuLinije();
		maxId.pop_back();
		if (!maxId.empty()) izlazniFajl << " ";
	}
	izlazniFajl.close();
}

void Mreza::ispisiPutanju(int brojPocetnogS, int brojKrajnjegS, list<int> prodjenaStajalista)
{

	ofstream izlazniFajl;
	izlazniFajl.open("putanja_" + to_string(brojPocetnogS) + "_" + to_string(brojKrajnjegS) + ".txt");

	//pomocni iteratori za odredjivanje sledbenika
	list<Stajaliste*>::iterator itS;
	itS = svaStajalista.begin();
	list<Linija*>::iterator itL;
	itL = sveLinije.begin();

	int idTrenutnaL = idLinijeZaPresedanje(prodjenaStajalista.back(), *prev(prodjenaStajalista.end(), 2));
	izlazniFajl << "->" << (*next(itL, idTrenutnaL))->dohvatiOznakuLinije() << endl;;

	int brojTrenutnogS = (*next(itS, prodjenaStajalista.back()))->dohvatiBrojStajalista();
	izlazniFajl << brojTrenutnogS;
	prodjenaStajalista.pop_back();

	while (!prodjenaStajalista.empty()) {
		int brojTrenutnogS = (*next(itS, prodjenaStajalista.back()))->dohvatiBrojStajalista();
		izlazniFajl << " " << brojTrenutnogS;


		if (prodjenaStajalista.size() > 1 && potrebnoPresedanje(idTrenutnaL, prodjenaStajalista.back(), *prev(prodjenaStajalista.end(), 2))) {
			//algoritam za ispis staraLinija -> novaLinija
			izlazniFajl << endl << (*next(itL, idTrenutnaL))->dohvatiOznakuLinije() << "->";
			idTrenutnaL = idLinijeZaPresedanje(prodjenaStajalista.back(), *prev(prodjenaStajalista.end(), 2));
			izlazniFajl << (*next(itL, idTrenutnaL))->dohvatiOznakuLinije() << endl;
			izlazniFajl << brojTrenutnogS;
		}

		prodjenaStajalista.pop_back();
	}

}

void Mreza::dodeliPovezanostLinija()
{

	dodeliIndekseLinijama();
	inicijalizujMatricuLinija();
	list<Stajaliste*>::iterator itS;
	list<Linija*>::iterator it1, it2;

	for (itS = svaStajalista.begin(); itS != svaStajalista.end(); itS++) {
		//Izaberemo jedno stajaliste i prolazimo kroz sve linije koje prolaze kroz njega dvostrukom for petljom
		list<Linija*> trenutneL = (*itS)->dohvatiLinijeStajalista();

		for (it1 = trenutneL.begin(); it1 != trenutneL.end(); it1++) {
			for (it2 = it1; it2 != trenutneL.end(); it2++) {
				//U matricu povezanosti linija inkrementiramo element na poziciji odredjenoj id-jevima linija
				int idLinije1 = (*it1)->dohvatiIdLinije();
				int idLinije2 = (*it2)->dohvatiIdLinije();

				if (idLinije1 != idLinije2) {
					//inkrementiranje radimo samo ukoliko nije u pitanju element na glavnoj dijagonali jer to ukazuje na povezanost linije sa samom sobom
					matricaLinija[idLinije1][idLinije2] += 1;
					matricaLinija[idLinije2][idLinije1] += 1;
				}
			}
		}
	}

}

void Mreza::dodeliPovezanostiStajalista()
{

	dodeliIndekseStajalistima();
	inicijalizujMatricuStajalista();
	list<Linija*>::iterator itL;

	for (itL = sveLinije.begin(); itL != sveLinije.end(); itL++) {
		//Izaberemo jedno stajaliste i prolazimo kroz sve linije koje prolaze kroz njega dvostrukom for petljom
		list<Stajaliste*> trenutnaS = (*itL)->dohvatiStajalistaSmerA();
		upisiVrednostiUMatricuStajalista(trenutnaS);
		upisiVrednostiUMatricuStajalista((*itL)->dohvatiStajalistaSmerB());
	}

}
void Mreza::upisiVrednostiUMatricuStajalista(list<Stajaliste*> trenutnaS)
{
	list<Stajaliste*>::iterator it1, it2;
	for (it1 = trenutnaS.begin(); it1 != trenutnaS.end(); it1++) {
		for (it2 = next(it1, 1); it2 != trenutnaS.end(); it2++)
		{
			//U matricu povezanosti stajalista inkrementiramo element na poziciji odredjenoj id-jevima stajalista koja su JEDNO PORED DRUGOG u putanji neke linije
			int idSt1 = (*it1)->dohvatiIdStajalista();
			int idSt2 = (*it2)->dohvatiIdStajalista();

			if (idSt1 != idSt2) {
				//inkrementiranje radimo samo ukoliko nije u pitanju element na glavnoj dijagonali jer to ukazuje na povezanost linije sa samom sobom
				matricaStajalista[idSt1][idSt2] += 1;

			}
			break;
		}
	}
}

//pomocna struktura za uporedjivanje linije
struct uporediLinije {
	bool operator()(Linija* l1, Linija* l2) {

		if (strcmp(l1->dohvatiOznakuLinije().c_str(), (l2->dohvatiOznakuLinije()).c_str()) < 0) return true;
		else return false;
	}
};

void Mreza::dodeliIndekseLinijama()
{
	sveLinije.sort(uporediLinije());
	list<Linija*>::iterator it;
	int i = 0;
	for (it = sveLinije.begin(); it != sveLinije.end(); it++) {
		(*it)->dodajId(i);
		i++;
	}

}

void Mreza::dodeliIndekseStajalistima()
{
	list<Stajaliste*>::iterator it;
	int i = 0;
	for (it = svaStajalista.begin(); it != svaStajalista.end(); it++) {
		(*it)->dodajIdStajalistu(i);
		i++;
	}
}

int Mreza::dohvatiBrojSvihLinija() const
{
	return sveLinije.size();
}

int Mreza::dohvatiBrojSvihStajalista() const
{
	return svaStajalista.size();
}

int** Mreza::inicijalizujMatricuLinija()
{
	int dim = dohvatiBrojSvihLinija();
	matricaLinija = new int* [dim];
	for (int i = 0; i < dim; i++)
	{
		matricaLinija[i] = new int[dim];
	}

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			matricaLinija[i][j] = 0;
		}
	}
	return matricaLinija;
}

int** Mreza::inicijalizujMatricuStajalista()
{
	int dim = dohvatiBrojSvihStajalista();
	matricaStajalista = new int* [dim];
	for (int i = 0; i < dim; i++)
	{
		matricaStajalista[i] = new int[dim];
	}

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			matricaStajalista[i][j] = 0;
		}
	}
	return matricaStajalista;
}



Stajaliste* Mreza::nadjiNajkraciPut(int brojPocetnogS, int brojKrajnjegS)
{
	Stajaliste* pocetnoS = nadjiStajaliste(brojPocetnogS, svaStajalista);
	Stajaliste* krajnjeS = nadjiStajaliste(brojKrajnjegS, svaStajalista);
	if (krajnjeS == nullptr || pocetnoS == nullptr) return nullptr;

	int idPocetnogS = pocetnoS->dohvatiIdStajalista();
	int idKrajnjegS = krajnjeS->dohvatiIdStajalista();

	vector<vector<int>> putanje(svaStajalista.size());
	dijkstraPutanje(idPocetnogS, putanje);
	//minBrojPresedanja(idPocetnogS, putanje);
	//obraditi slucaj kada nema putanje

	list<int> prodjenaStajalista = sacuvajPutanju(putanje, idPocetnogS, idKrajnjegS);

	ispisiPutanju(brojPocetnogS, brojKrajnjegS, prodjenaStajalista);
	return pocetnoS;
}

vector<int> Mreza::dijkstraPutanje(int idPocetnogS, vector<vector<int>>& putanje)
{
	vector<bool> posecenCvor;
	posecenCvor.resize(svaStajalista.size(), false);

	vector<int> rastojanjeOdPocetnogS;
	rastojanjeOdPocetnogS.resize(svaStajalista.size(), inf);
	rastojanjeOdPocetnogS[idPocetnogS] = 0;

	for (int i = 0; i < svaStajalista.size(); i++)
	{
		int m = najmanjeRastojanje(rastojanjeOdPocetnogS, posecenCvor); // vertex not yet included.
		posecenCvor[m] = true;// m with minimum distance included in Tset.
		for (int i = 0; i < svaStajalista.size(); i++)
		{
			// Updating the minimum distance for the particular node.
			if (!posecenCvor[i] && matricaStajalista[m][i] > 0 && rastojanjeOdPocetnogS[m] != inf && (rastojanjeOdPocetnogS[m] + 1) < rastojanjeOdPocetnogS[i])
			{
				rastojanjeOdPocetnogS[i] = rastojanjeOdPocetnogS[m] + 1;
				putanje[i].push_back(m);
			}
		}
	}
	return rastojanjeOdPocetnogS;
}

int Mreza::najmanjeRastojanje(vector<int>& rastojanjeOdPocetnogS, vector<bool> posecenCvor)
{
	int min = inf, idTrenutnogS;

	for (int i = 0; i < svaStajalista.size(); i++)
	{
		if (posecenCvor[i] == false && rastojanjeOdPocetnogS[i] <= min)
		{
			min = rastojanjeOdPocetnogS[i];
			idTrenutnogS = i;
		}
	}
	return idTrenutnogS;

}

list<int> Mreza::sacuvajPutanju(vector<vector<int>> putanje, int idPocetnogS, int idKrajnjegS)
{
	list<int> prodjenaStajalista;
	int pomId = idKrajnjegS;
	prodjenaStajalista.push_back(pomId);

	while (pomId != idPocetnogS) {

		prodjenaStajalista.push_back(putanje[pomId].back());
		pomId = putanje[pomId].back();
		//putanje[pomId].pop_back();
	}
	return prodjenaStajalista;
}



void Mreza::obrisiMatricu(int**& matrica, int dimenzija)
{
	for (int i = 0; i < dimenzija; i++)
	{
		delete[] matricaStajalista[i];
	}
	delete matricaLinija;
}

//-------MODIFIKACIJA

Stajaliste* Mreza::nadjiNajduziPut(int brojPocetnogS, int brojKrajnjegS)
{

	Stajaliste* pocetnoS = nadjiStajaliste(brojPocetnogS, svaStajalista);
	Stajaliste* krajnjeS = nadjiStajaliste(brojKrajnjegS, svaStajalista);
	if (krajnjeS == nullptr || pocetnoS == nullptr) return nullptr;

	int idPocetnogS = pocetnoS->dohvatiIdStajalista();
	int idKrajnjegS = krajnjeS->dohvatiIdStajalista();
	vector<vector<int>> putanje(svaStajalista.size());
	
	vector<int> rastojanjeOdPocetnogS = dijkstraPutanje(idPocetnogS, putanje);
	

	vector<bool> posecenCvor;
	posecenCvor.resize(svaStajalista.size(), false);
	posecenCvor[0] = true;
	vector<vector<int>> putanjeDuze(svaStajalista.size());
	int idTrenutnogS = idPocetnogS;

	for (int i = 0; i < svaStajalista.size(); i++) {

		int idTrenutnogS = najduzeRastojanje(rastojanjeOdPocetnogS, posecenCvor);
		if (idTrenutnogS != -1) {
			posecenCvor[idTrenutnogS] = true;

		
			for (int i = 0; i < svaStajalista.size(); i++)
			{

				if (!posecenCvor[i] && matricaStajalista[idTrenutnogS][i] > 0 && (rastojanjeOdPocetnogS[idTrenutnogS] + 1) >= rastojanjeOdPocetnogS[i] && rastojanjeOdPocetnogS[idTrenutnogS] != -1000000)
				{
					rastojanjeOdPocetnogS[i] = rastojanjeOdPocetnogS[idTrenutnogS] + 1;
					putanjeDuze[i].push_back(idTrenutnogS);
				}
			}
		}
	}
	rastojanjeOdPocetnogS[idKrajnjegS];
	list<int> prodjenaS=sacuvajPutanju(putanjeDuze, idPocetnogS, idKrajnjegS);
	ispisiPutanju(brojPocetnogS, brojKrajnjegS, prodjenaS);
}

int Mreza::najduzeRastojanje(vector<int>& rastojanjeOdPocetnogS, vector<bool> posecenCvor)
{
	int min= -1000000, idTrenutnogS=-1;
	for (int i = 0; i < svaStajalista.size(); i++) {
		if (posecenCvor[i] == false  && rastojanjeOdPocetnogS[i]>=min) {
			min = rastojanjeOdPocetnogS[i];
			idTrenutnogS = i;
		}
	}
	return idTrenutnogS;


	//for (int i = 0; i < svaStajalista.size(); i++)
	//{
	//	int m = najmanjeRastojanje(rastojanjeOdPocetnogS, posecenCvor); // vertex not yet included.
	//	posecenCvor[m] = true;// m with minimum distance included in Tset.
	//	for (int i = 0; i < svaStajalista.size(); i++)
	//	{
	//		// Updating the minimum distance for the particular node.
	//		if (!posecenCvor[i] && matricaStajalista[m][i] > 0 && rastojanjeOdPocetnogS[m] != inf && (rastojanjeOdPocetnogS[m] + 1) < rastojanjeOdPocetnogS[i])
	//		{
	//			rastojanjeOdPocetnogS[i] = rastojanjeOdPocetnogS[m] + 1;
	//			putanje[i].push_back(m);
	//		}
	//	}
	//}

	//int min = inf, idTrenutnogS;

	//for (int i = 0; i < svaStajalista.size(); i++)
	//{
	//	if (posecenCvor[i] == false && rastojanjeOdPocetnogS[i] <= min)
	//	{
	//		min = rastojanjeOdPocetnogS[i];
	//		idTrenutnogS = i;
	//	}
	//}
	//return idTrenutnogS;
}





