#include "stdafx.h"
#include "time.h"
#include "iostream"
#include "AlgGen.h"
using namespace std;
#define popSIZE 50
int Chromosome::iloscKolor()
{
	bool *tK = new bool[rozmiar];
	for (int i = 0; i < rozmiar; ++i)
		tK[i] = false;
	for (int i = 0; i < rozmiar; ++i)
		tK[tabKol[i]] = true;
	int i = 0; 
	for (int j = 0; j < rozmiar; ++j) 
		if(tK[j])
			++i;
	return i;
}

int Chromosome::iloscZlychW()
{
	return graph.zleKraw(tabKol);
}

void Chromosome::genLos()
{
	for (int i = 0; i < rozmiar; ++i)
		tabKol[i] = rand() % rozmiar;
}

void Chromosome::koloruj(Chromosome p1, Chromosome p2, bool W, int point)
{
	for (int i = 0; i < rozmiar; ++i)
		if (i < point)
			tabKol[i] = W ? p1.tabKol[i] : p2.tabKol[i];
		else
			tabKol[i] = W ? p2.tabKol[i] : p1.tabKol[i];
}

void Chromosome::mutation(double f)
{
	if (f > 4.)
		mutacja1();
	else
		mutacja2();
}

void Chromosome::mutacja1()
{
	vector<int> mozliweK;
	bool *allKol=new bool[rozmiar];
	bool change = false;
	for (int i = 0; i < rozmiar; ++i) {
		for (list<int>::iterator ji = graph.pocz(i); ji != graph.kon(i); ++ji)
			if (tabKol[i] == tabKol[*ji]) {
				change = true;
				break;
			}
		if (change) {
			change = false;
			for (int j = 0; j < rozmiar; ++j)
				allKol[j] = true;
			for (list<int>::iterator ji = graph.pocz(i); ji != graph.kon(i); ++ji)
				allKol[tabKol[*ji]] == false;
			for (int j = 0; j < rozmiar; ++j)
				if (allKol[j])
					mozliweK.push_back(j);
			tabKol[i] = mozliweK[rand() % mozliweK.size()];
			mozliweK.erase(mozliweK.begin(), mozliweK.end());
		}
	}
}

void Chromosome::mutacja2()
{
	bool change;
	int kolor;
	for (int i = 0; i < rozmiar; ++i) {
		change = false;
		for (list<int>::iterator ji = graph.pocz(i); ji != graph.kon(i); ++ji)
			if (tabKol[i] == tabKol[*ji])
				change = true;
		if (change) {
			do 
				kolor = rand() % rozmiar;
			while (kolor == tabKol[i]);
			tabKol[i] = kolor;
		}
	}
}

double Chromosome::fitness()
{
	/*bool *tK = new bool[rozmiar];
	for (int i = 0; i < rozmiar; ++i)
		tK[i] = false;
	for (int i = 0; i < rozmiar; ++i)
		tK[tabKol[i]] = true;
	double prod = 1., sum;
	for (int i = 0; i < rozmiar; ++i) {
		sum = 0.;
		for (int j = 0; j < rozmiar; j++)
			if (tK[i])
				if (tabKol[j] == i)
					++sum;
		if (tK[i])
			prod = 1. * prod * sum / rozmiar;
	}
	return 1. * (1. - (1. * iloscKolor() / graph.dajK())) * prod;*/
	return -1. * (iloscZlychW() + iloscKolor());
}

void Population::getParents(Chromosome & p1, Chromosome & p2)
{
	if (avgBadEdges() > 4.)
		parents1(p1, p2);
	else
		parents2(p1, p2);
}

void Population::parents1(Chromosome & p1, Chromosome & p2)
{
	Chromosome temp1 = p1, temp2 = p2;
	temp1 = this->chromosomes[rand() % this->chromosomes.size()];
	temp2 = this->chromosomes[rand() % this->chromosomes.size()];
	if (temp1.fitness() > temp2.fitness())
		p1 = temp1;
	else
		p1 = temp2;
	temp1 = this->chromosomes[rand() % this->chromosomes.size()];
	temp2 = this->chromosomes[rand() % this->chromosomes.size()];
	if (temp1.fitness() > temp2.fitness())
		p2 = temp1;
	else
		p2 = temp2;
}

void Population::parents2(Chromosome & p1, Chromosome & p2)
{
	Chromosome temp1 = this->chromosomes[0], temp2 = this->chromosomes[1];
	if (temp1.fitness() > temp2.fitness()) {
		p1 = temp1;
		p2 = temp2;
	}
	else {
		p1 = temp2;
		p2 = temp1;
	}
	for (int i = 2; i < chromosomes.size(); ++i)
		if (p1.fitness() <= chromosomes[i].fitness()) {
			p2 = p1;
			p1 = chromosomes[i];
		}
		else
			if (p2.fitness() <= chromosomes[i].fitness())
				p2 = chromosomes[i];
}

void Population::firstGen(Graph g)
{
	while (chromosomes.size() < popSIZE) {
		Chromosome *c = new Chromosome(g);
		c->genLos();
		this->add(*c);
	}
}

void Population::pokoloruj(Graph g, Population &p)
{
	while (this->chromosomes.size() < popSIZE) {
		Chromosome c1(g), c2(g);
		Chromosome p1(g), p2(g);
		p.getParents(p1, p2);
		int point = rand() % g.dajN();
		c1.koloruj(p1, p2, false, point);
		c1.mutation(p.avgBadEdges());
		c2.koloruj(p1, p2, true, point);
		c2.mutation(p.avgBadEdges());
		this->add(c1);
		this->add(c2);
	}
}

double Population::avgBadEdges()
{
	double sum = 0.;
	for (int i = 0; i < chromosomes.size(); ++i)
		sum = 0. + sum + chromosomes[i].iloscZlychW();
	return  1. * sum / chromosomes.size();
}

double Population::avgColors()
{
	double sum = 0.;
	for (int i = 0; i < chromosomes.size(); ++i)
		sum = 0. + sum + chromosomes[i].iloscKolor();
	return 1. * sum / chromosomes.size();
}
double Population::avgFitness()
{
	double sum = 0.;
	for (int i = 0; i < chromosomes.size(); ++i)
		sum = 0. + sum + chromosomes[i].fitness();
	return 1. * sum / chromosomes.size();
}
bool sprawdz(double *tab, int r) {
	for (int i = 1; i < r; ++i)
		if (tab[i - 1] != tab[i])
			return false;
	return true;
}
void AlgGen(Graph g)
{
	srand(time(NULL));
	vector<Population> population;
	Population firstPop;
	firstPop.firstGen(g);
	population.push_back(firstPop);
	double sprK[10] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
	int i = 0;
	sprK[0] = population[i].avgColors();
	cout << "Generacja " << i << " - Ilosc zlych krawedzi: " << population[i].avgBadEdges() << " - Ilosc kolorow: " << population[i].avgColors() << " - Fitness: " << population[i].avgFitness() << endl;
	while (!(population.size()==20000 || (population[i].avgBadEdges()==0 && sprawdz(sprK,10)))) {
		Population *p = new Population;
		p->pokoloruj(g, population[i]);
		population.push_back(*p);
		for (int j = 9; j > 0; --j)
			sprK[j] = sprK[j - 1];
		sprK[0] = population[++i].avgColors();
		cout << "Generacja " << i << " - Ilosc zlych krawedzi: " << population[i].avgBadEdges() << " - Ilosc kolorow: " << population[i].avgColors() << " - Fitness: " << population[i].avgFitness() << endl;
	}
	cout << "Ilosc generacji: " << population.size() << endl;
	cout << "Ilosc zlych krawedzi: " << population[i].avgBadEdges() << endl;
	cout << "Ilosc kolorow: " << population[i].avgColors() << endl;
}
