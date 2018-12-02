#pragma once
#include "Graph.h"
#include "vector"
class Chromosome {
protected:
	int *tabKol;
	int rozmiar;
public:
	Graph graph;
	Chromosome() {};
	Chromosome(Graph g) { graph = g; tabKol = new int[rozmiar = g.dajN()]; };
	int iloscKolor();
	int iloscZlychW();
	void genLos();
	void koloruj(Chromosome p1, Chromosome p2, bool W, int point);
	void mutation(double f);
	void mutacja1();
	void mutacja2();
	void wypisz() { for (int i = 0; i < rozmiar; ++i) cout << tabKol[i] << " "; }
	double fitness();
};
class Population:public Chromosome {
protected:
	vector<Chromosome> chromosomes;
public:
	Population() {};
	void add(Chromosome c) { chromosomes.push_back(c); }
	void getParents(Chromosome &p1, Chromosome &p2);
	void parents1(Chromosome &p1, Chromosome &p2);
	void parents2(Chromosome &p1, Chromosome &p2);
	void firstGen(Graph g);
	void pokoloruj(Graph g, Population &p);
	double avgBadEdges();
	double avgColors();
	double avgFitness();
};

void AlgGen(Graph g);