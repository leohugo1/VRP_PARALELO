#pragma once
#include <iostream>
#include <vector>
namespace Util {
	struct RotaInfo {
		int indice;
		float x, y, peso;
	};
	struct Rota {

		std::vector<RotaInfo> caminhoEntrada;
	};

	std::vector<Rota> Rotas()
	{
		std::vector<RotaInfo> entrada;
		Rota rota;
		std::vector<Rota> rotas;
		std::vector<int> existe;
		entrada.push_back({0,1200,1000,40});
		entrada.push_back({1,400,450,35});
		entrada.push_back({2,890,720,51});
		entrada.push_back({3,910,900,20});
		entrada.push_back({4,502,550,47});
		entrada.push_back({5,690,701,25});
		
		for (int i = 0; i < 100; i++) {
			rota.caminhoEntrada.push_back(entrada[0]);
			existe.push_back(0);
			while (rota.caminhoEntrada.size()!=5)
			{
				int k = rand() % 5;
				if (!std::count(existe.begin(), existe.end(), k))
				{
					rota.caminhoEntrada.push_back(entrada[k]);
					existe.push_back(k);
				}
					
			}
			rotas.push_back(rota);
		}
		
		return rotas;
	}
	float calcDistancia(std::pair<float, float> d1, std::pair<float, float> d2)
	{
		float result = std::pow(d1.first - d2.first,2) +  std::pow(d1.second - d2.second,2);
		float dist = std::sqrt(result);
		return dist;
	}

}