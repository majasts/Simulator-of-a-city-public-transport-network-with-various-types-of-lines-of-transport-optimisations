#ifndef MREZA_H
#define MREZA_H

#include "Linija.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

#define inf 1000000000

class Mreza :public Stajaliste, public Linija {
public:
	
	Mreza();
	~Mreza();

	void procitajStajalista(string nazivFajla);
	void procitajLinije(string nazivFajla);

	//funkcije za ispis:
	Stajaliste* ispisiStajaliste(int broj);
	Linija* ispisiLiniju(string oznaka);
	Linija* ispisiStatistikuLinije(string oznaka);
	void ispisiPutanju(int brojPocetnogS, int brojKrajnjegS,list<int> prodjenaStajalista);

	//funckije za nalazanje putanje
	Stajaliste* nadjiNajkraciPut(int brojPocetnogS, int brojKrajnjegS);
	vector<int> dijkstraPutanje(int idPocetnogS, vector<vector<int>>& putanje);
	int najmanjeRastojanje(vector<int>& rastojanjeOdPocetnogS, vector<bool> posecenCvor);
	list<int> sacuvajPutanju(vector<vector<int>> putanje, int idPocetnogS, int idKrajnjegS);
	bool potrebnoPresedanje(int idTrenutneLinije, int idStajalista1, int idStajalista2);
	int idLinijeZaPresedanje(int idStajalista1, int idStajalista2);

	void minBrojPresedanja(int idPocetnogS, vector<vector<int>>& putanje);
	///////////////////// ----MODIFIKACIJA-----------
	Stajaliste* nadjiNajduziPut(int brojPocetnogS, int brojKrajnjegS);
	int najduzeRastojanje(vector<int>& rastojanjeOdPocetnogS, vector<bool> posecenCvor);

	//getter funckije
	int dohvatiBrojSvihLinija() const;
	int dohvatiBrojSvihStajalista() const;

	//rad sa matricama povezanosti
	int** inicijalizujMatricuLinija();
	int** inicijalizujMatricuStajalista();
	void dodeliPovezanostLinija();
	void dodeliPovezanostiStajalista();
	void upisiVrednostiUMatricuStajalista(list<Stajaliste*> trenutnaS);

	void obrisiMatricu(int**& matrica, int dimenzija);
	
	
protected:
	//pomocne funckije pri ucitavanju podataka u mrezu
	Stajaliste* poveziStajalisteSaLinijom(Linija* l, string pom);
	vector<int> nadjiLinijuSaMaxZajednickihS(int idTrenutneL);
	
	void dodeliIndekseLinijama();
	void dodeliIndekseStajalistima();



private:
	list<Stajaliste*> svaStajalista;
	list <Linija*> sveLinije;
	int** matricaLinija;
	int** matricaStajalista;
};


#endif
