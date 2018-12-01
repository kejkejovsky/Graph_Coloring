#pragma once
#include "list"
using namespace std;
class Graph {
private:
	int nVec;
public:
	list<int> *lS;
	Graph(int nVec = 0);
	~Graph();
	list<int>::iterator pocz(int v) { return lS[v].begin(); };
	list<int>::iterator kon(int v) { return lS[v].end(); };
	int dajN() { return nVec; };
	int zleKraw(int *tab);
	void wypisz();
	void dodajK(int w, int s);
	void algZac();
};