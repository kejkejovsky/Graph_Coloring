#include "stdafx.h"
#include "Graph.h"
#include "string"
#include "iostream"
using namespace std;

Graph::Graph(int nVec)
{
	this->nVec = nVec;
	this->lS = new list<int>[nVec];
}
Graph::~Graph()
{
}
int Graph::zleKraw(int * tab)
{
	int badEdge = 0;
	for (int i = 0; i < nVec; ++i)
		for (list<int>::iterator j = lS[i].begin(); j != lS[i].end(); ++j)
			if (tab[i] == tab[*j])
				++badEdge;
	return badEdge/2;
}
void Graph::wypisz()
{
	list<int>::iterator j;
	for(int i=0;i<nVec;++i)
	{
		cout << i+1 << ": ";
		for (j = lS[i].begin(); j != lS[i].end(); ++j)
			cout << *j+1 <<", ";
		cout << endl;
	}
}

void Graph::dodajK(int w, int s)
{
	lS[w].push_back(s);
	lS[s].push_back(w);
}

void Graph::algZac()
{
	int i, j, max = 0;
	int *TK = new int[nVec];
	bool *K = new bool[nVec]; //tablica mo¿liwych kolorów
	list<int>::iterator k;
	for (int i = 0; i < nVec; ++i) TK[i] = -1;
	TK[0] = 0;
	for (i = 1; i < nVec; ++i) {
		for (j = 0; j < nVec; j++) K[j] = false;
		for (k = lS[i].begin(); k != lS[i].end(); ++k)
			if (TK[*k] != -1)
				K[TK[*k]] = true;
		for (j = 0; K[j]; ++j);
		TK[i] = j;
	}
	for (i = 0; i < nVec; ++i)
		if (TK[i] > max)
			max = TK[i];
	cout << "Ilosc potrzebnych kolorow: " << max + 1 << endl;
}

