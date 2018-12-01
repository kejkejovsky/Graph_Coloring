#include "stdafx.h"
#include "time.h"
#include "iostream"
#include "AlgGen.h"

int Chromosome::iloscKolor()
{
	int i = 0; 
	for (int j = 0; j < rozmiar; ++j) 
		for (int k = 0; k < rozmiar; ++k) 
			if (tabKol[k] == j) { 
				++i; 
				break; 
			}
	return i;
}

int Chromosome::iloscZlychW()
{
	return graph.zleKraw(tabKol);
}

void Chromosome::genLos()
{
	srand(time(NULL));
	for (int i = 0; i < rozmiar; ++i)
		tabKol[i] = rand() % rozmiar;
}

void Chromosome::koloruj(Chromosome p1, Chromosome p2)
{
	srand(time(NULL));
	int point = rand() % rozmiar;
	for (int i = 0; i < rozmiar; ++i)
		if (i < point)
			tabKol[i] = p1.tabKol[i];
		else
			tabKol[i] = p2.tabKol[i];
}

void Chromosome::mutation()
{
	if (iloscZlychW() > 4)
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
	for (int i = 0; i < rozmiar; ++i) {
		change = false;
		for (list<int>::iterator ji = graph.pocz(i); ji != graph.kon(i); ++ji)
			if (tabKol[i] == tabKol[*ji])
				change = true;
		if (change)
			tabKol[i] = rand() % rozmiar;
	}
}

void Population::getParents(Chromosome & p1, Chromosome & p2)
{
	if (avgBadEdges() > 4)
		parents1(p1, p2);
	else
		parents2(p1, p2);
}

void Population::parents1(Chromosome & p1, Chromosome & p2)
{
	Chromosome temp1 = p1, temp2 = p2;
	srand(time(NULL));
	temp1 = this->chromosomes[rand() % this->chromosomes.size()];
	temp2 = this->chromosomes[rand() % this->chromosomes.size()];
	if (temp1.iloscZlychW() < temp2.iloscZlychW())
		p1 = temp1;
	else if (temp1.iloscZlychW() == temp2.iloscZlychW())
		p1 = temp1.iloscKolor() < temp2.iloscKolor() ? temp1 : temp1.iloscKolor() > temp2.iloscKolor() ? temp2 : rand() % 2 ? temp1 : temp2;
	else
		p1 = temp2;
	srand(time(NULL));
	temp1 = this->chromosomes[rand() % this->chromosomes.size()];
	temp2 = this->chromosomes[rand() % this->chromosomes.size()];
	if (temp1.iloscZlychW() < temp2.iloscZlychW())
		p2 = temp1;
	else if (temp1.iloscZlychW() == temp2.iloscZlychW())
		p2 = temp1.iloscKolor() < temp2.iloscKolor() ? temp1 : temp1.iloscKolor() > temp2.iloscKolor() ? temp2 : rand() % 2 ? temp1 : temp2;
	else
		p2 = temp2;
}

void Population::parents2(Chromosome & p1, Chromosome & p2)
{
	Chromosome temp1 = this->chromosomes[0], temp2 = this->chromosomes[1];
	if (temp1.iloscZlychW() < temp2.iloscZlychW()) {
		p1 = temp1;
		p2 = temp2;
	}
	else if (temp1.iloscZlychW() == temp2.iloscZlychW())
		if (temp1.iloscKolor() < temp2.iloscKolor()) {
			p1 = temp1;
			p2 = temp2;
		}
		else if (temp1.iloscKolor() > temp2.iloscKolor()) {
			p1 = temp2;
			p2 = temp1;
		}
		else
			if (rand() % 2) {
				p1 = temp1;
				p2 = temp2;
			}
			else {
				p1 = temp2;
				p2 = temp1;
			}
	else {
		p1 = temp2;
		p2 = temp1;
	}
	for (int i = 2; i < chromosomes.size(); ++i)
		if (p1.iloscZlychW() > chromosomes[i].iloscZlychW()) {
			p2 = p1;
			p1 = chromosomes[i];
		}
		else if (p1.iloscZlychW() == chromosomes[i].iloscZlychW())
			if (p1.iloscKolor() > chromosomes[i].iloscKolor()) {
				p2 = p1;
				p1 = chromosomes[i];
			}
			else if (p1.iloscKolor() > chromosomes[i].iloscKolor()) {
				if (p2.iloscZlychW() > chromosomes[i].iloscZlychW())
					p2 = chromosomes[i];
				else if (p2.iloscZlychW() == chromosomes[i].iloscZlychW())
					if (p2.iloscKolor() >= chromosomes[i].iloscKolor())
						p2 = chromosomes[i];
			}
			else {
				p2 = p1;
				p1 = chromosomes[i];
			}
}

void Population::firstGen(Graph g)
{
	while (chromosomes.size() < 50) {
		Chromosome *c = new Chromosome(g);
		c->genLos();
		this->add(*c);
	}
}

void Population::pokoloruj(Graph g, Chromosome p1, Chromosome p2)
{
	while (this->chromosomes.size() < 50) {
		Chromosome c(g);
		c.koloruj(p1, p2);
		c.mutation();
		this->add(c);
	}
}

double Population::avgBadEdges()
{
	int sum = 0;
	for (int i = 0; i < chromosomes.size(); ++i)
		sum += chromosomes[i].iloscZlychW();
	return static_cast<double>(sum) / static_cast<double>(chromosomes.size());
}

int Population::avgColors()
{
	int sum = 0;
	for (int i = 0; i < chromosomes.size(); ++i)
		sum += chromosomes[i].iloscKolor();
	return sum / chromosomes.size() + 1;
}

void AlgGen(Graph g)
{
	vector<Population> population;
	Population firstPop;
	firstPop.firstGen(g);
	population.push_back(firstPop);
	int sprK[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int i = 0;
	while (!(population.size()==20000 || (population[i].avgBadEdges()==0 && sprK[0]==sprK[9]))) {
		sprK[i % 10] = population[i].avgColors();
		Population *p = new Population;
		Chromosome p1(g), p2(g);
		population[i].getParents(p1, p2);
		p->pokoloruj(g, p1, p2);
		population.push_back(*p);
		++i;
	}
	cout << "Ilosc zlych krawedzi: " << population[i].avgBadEdges() << endl;
	cout << "Ilosc kolorow: " << population[i].avgColors() << endl;
}
