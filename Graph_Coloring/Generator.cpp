#include "stdafx.h"
#include "Graph.h"
#include "Generator.h"
#include "string"
#include "iostream"
#include "random"
#include "ctime"
#include "fstream"
using namespace std;

void generuj(int n, int g, string nazwa, Graph &graphN) {
	int k, l, m;
	bool all_vis = false;
	//generowanie
	bool **tab = new bool*[n];
	for (int i = 0; i < n; ++i)
		tab[i] = new bool[n];
	while (!all_vis) {
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				tab[i][j] = false;
		k = n * (n - 1)*g / 100 / 2;
		mt19937 generator(time(NULL));
		while (k > 0) {
			uniform_int_distribution<int> dist(0, n - 2);
			l = dist(generator);
			uniform_int_distribution<int> dist2(l + 1, n - 1);
			m = dist2(generator);
			if (tab[l][m] != true) {
				tab[l][m] = tab[m][l] = true;
				k--;
			}
		}
		//sprawdzanie spójnoœci
		int v, vc = 0;
		list<int> S;
		bool *visited = new bool[n];
		for (int i = 1; i < n; ++i)
			visited[i] = false;
		visited[0] = true;
		S.push_front(0);
		while (S.empty() == false) {
			v = S.front();
			S.pop_front();
			++vc;
			for (int i = 0; i < n; ++i) {
				if (tab[v][i] == true) {
					if (visited[i] == false) {
						visited[i] = true;
						S.push_front(i);
					}
				}
			}
		}
		if (vc == n)
			all_vis = true;
	}
	//zapis do pliku
	fstream plik;
	plik.open(nazwa,ios::out);
	plik << n;
	for (int i = 0; i < n-1; ++i)
		for (int j = i+1; j < n; ++j)
			if (tab[i][j])
				plik <<endl<< i + 1 << " " << j + 1;
	plik.close();
}