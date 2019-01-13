#include "stdafx.h"
#include "time.h"
#include "iostream"
#include "algorithm"
#include "AlgGen.h"
using namespace std;
#define popSIZE 50
#define mutationRATE 10
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
	int i, j, max = 0, wyb;
	bool *K = new bool[rozmiar]; //tablica mo¿liwych kolorów
	list<int>::iterator k;
	for (int i = 0; i < rozmiar; ++i) tabKol[i] = -1;
	wyb = rand() % rozmiar;
	tabKol[wyb] = 0;
	for (i = 1; i < rozmiar; ++i) {
		for (j = 0; j < rozmiar; j++) K[j] = false;
		for (k = graph.lS[wyb].begin(); k != graph.lS[wyb].end(); ++k)
			if (tabKol[*k] != -1)
				K[tabKol[*k]] = true;
		for (j = 0; K[j]; ++j);
		tabKol[wyb] = j;
		wyb = (wyb + 1) % rozmiar;
	}
}

void Chromosome::koloruj(Chromosome p1, Chromosome p2, bool W, int point)
{
	for (int i = 0; i < rozmiar; ++i)
		if (i < point)
			tabKol[i] = W ? p1.tabKol[i] : p2.tabKol[i];
		else
			tabKol[i] = W ? p2.tabKol[i] : p1.tabKol[i];
}

void Chromosome::napraw()
{
	vector<int> mozliweK;
	bool *allKol = new bool[rozmiar];
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

void Chromosome::mutation(Chromosome c)
{
	int wyb = rand() % rozmiar;
	int tmp = tabKol[wyb];
	double f = this->fitness();
	tabKol[wyb] = c.tabKol[wyb];
	if (this->fitness() < f)
		tabKol[wyb] = tmp;
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
	return -1. * (iloscKolor() + iloscZlychW());
}

bool Chromosome::operator<(Chromosome & c)
{
	return (this->fitness() > c.fitness());
}

void Population::getParents(Chromosome & p1, Chromosome & p2)
{
	Chromosome temp1 = p1, temp2 = p2;
	temp1 = this->chromosomes[rand() % (this->chromosomes.size()/2)];
	temp2 = this->chromosomes[rand() % (this->chromosomes.size()/2)];
	if (temp1.fitness() > temp2.fitness())
		p1 = temp1;
	else
		p1 = temp2;
	temp1 = this->chromosomes[rand() % (this->chromosomes.size() / 2)];
	temp2 = this->chromosomes[rand() % (this->chromosomes.size() / 2)];
	if (temp1.fitness() > temp2.fitness())
		p2 = temp1;
	else
		p2 = temp2;
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
		/*if (c1.iloscZlychW() > 20.)
			c1.napraw();*/
		this->add(c1);
		c2.koloruj(p1, p2, true, point);
		/*if (c2.iloscZlychW() > 20.)
			c2.napraw();*/
		this->add(c2);
	}
}

void Population::mute()
{
	for (int i = 0; i < chromosomes.size(); i++)
		if ((rand() / RAND_MAX) < mutationRATE)
			chromosomes[i].mutation(chromosomes[rand() % chromosomes.size()]);
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

void Population::sortuj()
{
	sort(this->chromosomes.begin(), this->chromosomes.end());
}

Chromosome Population::getP(int i)
{
	return this->chromosomes[i];
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
	double sprK[10] = { 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0 };
	int i = 0;
	sprK[0] = population[i].avgFitness();
	cout << "Generacja " << i << " - Ilosc zlych krawedzi: " << population[i].avgBadEdges() << " - Ilosc kolorow: " << population[i].avgColors() << " - Fitness: " << population[i].avgFitness() << endl;
	while (!(population.size()==20000 || (sprawdz(sprK,10) && population[i].avgBadEdges()==0))) {
		Population *p = new Population;
		population[i].sortuj();
		p->pokoloruj(g, population[i]);
		p->mute();
		population.push_back(*p);
		for (int j = 9; j > 0; --j)
			sprK[j] = sprK[j - 1];
		sprK[0] = population[++i].avgFitness();
		cout << "Generacja " << i << " - Ilosc zlych krawedzi: " << population[i].avgBadEdges() << " - Ilosc kolorow: " << population[i].avgColors() << " - Fitness: " << population[i].avgFitness() << endl;
	}
	cout << "Ilosc generacji: " << population.size() << endl;
	cout << "Ilosc zlych krawedzi: " << population[i].avgBadEdges() << endl;
	cout << "Ilosc kolorow: " << population[i].avgColors() << endl;
}
