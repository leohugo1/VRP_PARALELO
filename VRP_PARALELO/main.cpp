#include "src/AlgoritmoGenetico.h"


int main()
{
	AlgoritmoGenetico Vrp;
	std::thread t0(&AlgoritmoGenetico::CriarPopulacao,&Vrp);
	t0.join();
	for (int i = 0; i < Vrp._geracoes; i++)
	{
		std::thread t1(&AlgoritmoGenetico::Cruzamento,&Vrp,std::ref(Vrp._populacao));
		t1.join();
		Vrp.fitness(Vrp._populacao);
	}
}
