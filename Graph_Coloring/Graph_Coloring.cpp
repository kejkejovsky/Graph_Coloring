// Graph_Coloring.cpp: definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include "string"
#include "iostream"
#include "fstream"
#include "random"
#include "Graph.h"
#include "Generator.h"
#include "AlgGen.h"
using namespace std;


void wczytaj(string nazwa, Graph &graphN) 
{
	int n, w, s;
	fstream plik;
	plik.open(nazwa, ios::in);
	if (!plik.good())
	{
		cout << "Nie udalo sie otworzyc pliku" << endl;
		return;
	}
	plik >> n;
	Graph graph(n);
	while (!plik.eof())
	{
		plik >> w >> s;
		--w;
		--s;
		if (w < s)
		{
			graph.dodajK(w, s);
		}
	}
	cout << "Wczytano graf" << endl;
	plik.close();
	graphN = graph;
}


int main()
{
	Graph graph;
	bool exit = true;
	int wyb, n, g;
	string nazwa;
	while (exit) {
		cout << "[1] Generuj graf\n[2] Wczytaj graf z pliku txt\n[3] Wypisanie grafu\n[4] Wykonaj kolorowanie grafu - Algorytm Zachlanny\n[5] Wykonaj kolorowanie grafu - Algorytm Genetyczny\n[6] Wyjscie z programu\nPodaj swoj wybor: ";
		cin >> wyb;
		--wyb;
		switch (wyb) {
		case 0:
			cout << "Podaj ilosc wierzchokow: ";
			cin >> n;
			cout << "Podaj gestosc grafu (0-100%): ";
			cin >> g;
			cout << "Podaj nazwe pliku: ";
			cin >> nazwa;
			generuj(n, g, nazwa, graph);
			break;
		case 1:
			cout << "Podaj nazwe pliku: ";
			cin >> nazwa;
			wczytaj(nazwa, graph);
			break;
		case 2:
			graph.wypisz();
			break;
		case 3:
			graph.algZac();
			break;
		case 4:
			AlgGen(graph);
			break;
		case 5:
			exit = false;
			break;
		default:
			cout << "Zla operacja, sprobuj jeszcze raz" << endl;
			break;
		}
		cout << endl;
	}
	return 0;
}