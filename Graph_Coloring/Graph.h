#pragma once
#include "list"
using namespace std;
class Graph {
public:
	int nVec;
	list<int> *lS;
public:
	Graph(int nVec = 0);
	~Graph();
	void wypisz();
	void dodajK(int w, int s);
	void algZac();
};