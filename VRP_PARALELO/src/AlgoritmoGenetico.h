#pragma once
#include <iostream>
#include<vector>
#include <array>
#include <unordered_map>
#include <random>
#include <mutex>
#include <thread>

struct CaminhoInfo {
	int indice;
	float x, y, peso;
};
struct Cromossomo
{
	std::vector<CaminhoInfo> _Caminho;
	float _Limite;
	float _Aptidao;
	float _pesoAtual;
	Cromossomo() :_Limite(3000), _Aptidao(0), _pesoAtual(0) {};
};

struct RotaInfo {
	int indice;
	float x, y, peso;
};
struct Rota {

	std::vector<RotaInfo> caminhoEntrada;
};

class AlgoritmoGenetico {
public:
	
	AlgoritmoGenetico() :_geracoes(300), _numCromossomos(100) {};
	~AlgoritmoGenetico();
	void Aptidao(Cromossomo& Cromossomo);
	Cromossomo Selecao(std::vector<Cromossomo>& popuplacao);
	void Cruzamento(std::vector<Cromossomo>& populacao);
	void Mutacao(Cromossomo& cromossomo);
	void CriarPopulacao();
	std::vector<Cromossomo> _populacao;
	void fitness(std::vector<Cromossomo>& populacao);
	int _geracoes;
private:
	int _numCromossomos;
	std::vector<Rota> _rotas;
	std::vector<Cromossomo> _Melhorespais;
	std::mutex m;
	std::thread* t1;
	std::thread* t2;
};


std::vector<Rota> Rotas();

float calcDistancia(std::pair<float, float> d1, std::pair<float, float> d2);