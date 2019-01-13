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
	void napraw();
	void mutation(Chromosome c);
	void wypisz() { for (int i = 0; i < rozmiar; ++i) cout << tabKol[i] << " "; }
	double fitness();
	bool operator <(Chromosome &c);
};
class Population:public Chromosome {
protected:
	vector<Chromosome> chromosomes;
public:
	Population() {};
	void add(Chromosome c) { chromosomes.push_back(c); }
	void getParents(Chromosome &p1, Chromosome &p2);
	void firstGen(Graph g);
	void pokoloruj(Graph g, Population &p);
	void mute();
	double avgBadEdges();
	double avgColors();
	double avgFitness();
	void wypiszN() { for (int i = 0; i < chromosomes.size(); ++i) cout << this->chromosomes[i].fitness() << " "; cout << endl; }
	void sortuj();
	Chromosome getP(int i);
};

void AlgGen(Graph g);