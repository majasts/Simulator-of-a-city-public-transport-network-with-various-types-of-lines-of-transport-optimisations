#include "Mreza.h"
#include "Exception.h"

#include<iostream>
using namespace std;

int main() {
	Mreza* m = new Mreza();

	m->procitajStajalista("ulaz/stajalista.txt");
	m->procitajLinije("ulaz/linije.txt");
	m->dodeliPovezanostLinija();
	m->dodeliPovezanostiStajalista();
	m->nadjiNajduziPut(154, 103);

	

	cout << "Dobrodosli u simulator mreze gradskog prevoza.Molimo Vas, odaberite opciju :\n";
	cout << "1. Ucitavanje podataka o mrezi gradskog prevoza\n";
	cout << "0. Kraj rada\n";
	int n;
	while (1) {
		try {
			cin >> n;
			if (n == 0) exit;
			if (n != 1) throw NeispravnoZadataOpcija("Nesipravno zadat broj zeljene opcije. Molim pokusajte ponovo.\n");
			else break;


		}
		catch (NeispravnoZadataOpcija& e) {
			cout << e.what();
		}
	}
	bool ucitano = false;
	while (!ucitano) {
		try {
			string putanja;
			cout << "Molimo Vas, unesite putanju do fajla sa stajalištima\n";
			cin >> putanja;
			m->procitajStajalista(putanja);
			cout << "Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza\n";
			cin >> putanja;
			m->procitajLinije(putanja);
			ucitano = true;

			m->dodeliPovezanostLinija();
			m->dodeliPovezanostiStajalista();
		}
		catch (Neuspesno& e) {
			cout << e.what();
		}
	}

	cout << "Mreža gradskog prevoza je uspešno učitana. Molimo Vas, odaberite opciju:\n";
	cout << "1. Prikaz informacija o stajalištu\n";
	cout << "2. Prikaz osnovnih informacija o liniji gradskog prevoza\n";
	cout << "3. Prikaz statističkih informacija o liniji gradskog prevoza\n";
	cout << "4. Pronalazak putanje između dva stajališta\n";
	cout << "0. Kraj rada\n";
	bool kraj = false;
	while (!kraj) {
		cin >> n;
		try {
			switch (n) {
			case 0:
				kraj = true;
				break;
			case 1:
				cout << "Molimo Vas, unesite broj stajalista cije osnovne informacije zelite da prikazete.\n";
				int broj;
				cin >> broj;
				if (m->ispisiStajaliste(broj) == nullptr) throw Nepostojece("Uneli ste broj stajalista koje ne postoji.\n");
				cout << "Generisan je fajl stajaliste_" << broj << ".txt sa osnovnim informacijama o stajalistu " << broj << endl;;
				break;
			case 2: {
				cout << "Molimo Vas, unesite oznaku linije cije osnovne informacije zelite da prikazete.\n";
				string oznaka;
				cin >> oznaka;
				if (m->ispisiLiniju(oznaka) == nullptr) throw Nepostojece("Uneli ste oznaku linije koja ne postoji.\n");
				cout << "Generisan je fajl linija_" << oznaka << ".txt sa osnovnim informacijama o liniji " << oznaka << endl;
				break;
			}
			case 3: {
				cout << "Molimo Vas, unesite oznaku linije cije statisticke informacije zelite da prikazete.\n";
				string oznaka;
				cin >> oznaka;
				if (m->ispisiStatistikuLinije(oznaka) == nullptr) throw Nepostojece("Uneli ste oznaku linije koja ne postoji.\n");
				cout << "Generisan je fajl statistika_" << oznaka << ".txt sa statictickim informacijama o liniji " << oznaka << endl;;
				break;
			}
			case 4:
				int broj1, broj2;
				cout << ("Molimo Vas, unesite broj pocetnog stajalista\n");
				cin >> broj1;
				cout << ("Molimo Vas, unesite broj krajneg stajalista\n");
				cin >> broj2;
				if (broj1 == broj2) throw Nepostojece("Pocetno i krajnje stajaliste su ista.\n");

				if ((m->nadjiNajkraciPut(broj1, broj2)) == nullptr) throw Nepostojece("Uneli ste broj stajalista koje ne postoji.\n");
				cout << "Generisan je fajl putanja_" << broj1 << "_" << broj2 << ".txt sa planom putanje.\n";
				break;
			default:
				throw Nepostojece("Uneli ste neispravn broj za izbor opcije");
			}
		}
		catch (Nepostojece& e) {
			cout << e.what();
		}
		cout << "Odaberite ponovo opciju:";
	}

	m->~Mreza();

}

