#include "AlgoritmoGenetico.h"


//AlgoritmoGenetico::AlgoritmoGenetico() {};

AlgoritmoGenetico::~AlgoritmoGenetico()
{
};

void AlgoritmoGenetico::Aptidao(Cromossomo& cromossomo)
{
		for (int i = 0,j=1; j<cromossomo._Caminho.size() ;i++,j++)
		{
			cromossomo._Aptidao += calcDistancia(std::make_pair(cromossomo._Caminho[i].x,cromossomo._Caminho[i].y),
				std::make_pair(cromossomo._Caminho[j].x, cromossomo._Caminho[j].y));
		}
}

void AlgoritmoGenetico::CriarPopulacao()
{
	_rotas =Rotas();
		
	m.lock();
	for (auto& rota : _rotas)
	{
		Cromossomo cromossomo;
		int atPeso = 0;
		for (auto& caminho : rota.caminhoEntrada)
		{
			atPeso += caminho.peso;
			if (atPeso >= cromossomo._Limite)
			{
				cromossomo._Caminho.push_back({ rota.caminhoEntrada[0].indice ,rota.caminhoEntrada[0].x,rota.caminhoEntrada[0].y,rota.caminhoEntrada[0].peso});
				atPeso = 0;
			}
			cromossomo._Caminho.push_back({caminho.indice,caminho.x,caminho.y,caminho.peso});
		}
		
		Aptidao(cromossomo);
		_populacao.push_back(cromossomo);
	}
	m.unlock();
}

void AlgoritmoGenetico::Cruzamento(std::vector<Cromossomo>& populacao)
{
	int i = 0;
	while (i < populacao.size())
	{
		auto aux = populacao[0];
		m.lock();
		for (auto& cromossomo : populacao)
		{
			if (aux._Aptidao <= cromossomo._Aptidao)
				aux = cromossomo;
		}
		m.unlock();
		_Melhorespais.push_back(aux);
		i++;
	}
	_populacao.clear();
	for (int i = 0; i < _Melhorespais.size()/2; i++)
	{
		Cromossomo cromossomo;
		auto cromo1 = Selecao(_Melhorespais);
		auto cromo2 = Selecao(_Melhorespais);
		std::array<CaminhoInfo,6> filho;
		Cromossomo pai1;
		Cromossomo pai2;
		std::vector<int> p1;
		std::vector<int> p2;
		int j = 0;
		m.lock();
		for (int i=0;i<cromo1._Caminho.size();i++)
		{
			int ex1 = std::count(p1.begin(), p1.end(), cromo1._Caminho[i].indice);
			if (ex1 == 0)
			{
				pai1._Caminho.push_back({ cromo1._Caminho[i].indice,cromo1._Caminho[i].x,cromo1._Caminho[i].y,cromo1._Caminho[i].peso });
				p1.push_back(cromo1._Caminho[i].indice);
			}
		}
		for (int i=0;i<cromo2._Caminho.size();i++)
		{
			int ex2 = std::count(p2.begin(), p2.end(), cromo2._Caminho[i].indice);
			
			if (ex2 == 0 )
			{
				pai2._Caminho.push_back({ cromo2._Caminho[i].indice,cromo2._Caminho[i].x,cromo2._Caminho[i].y,cromo2._Caminho[i].peso });
				p2.push_back(cromo2._Caminho[i].indice);
			}
		}
		m.unlock();

		int k = 0;
		bool p = 0;
		std::vector<int> contador1;
		std::vector<int> contador2;
		std::vector<int> indices;
		m.lock();
		filho[0] = { pai1._Caminho[0].indice,pai1._Caminho[0].x,pai1._Caminho[0].y,pai1._Caminho[0].peso };
		contador1.push_back(0);
		contador2.push_back(0);
		indices.push_back(pai1._Caminho[0].indice);
		while (k != 5)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> ran(0, 5);
			int posicao = ran(gen);
			int valor;
			if (p == 0)
			{
				auto exCont1 = std::count(contador1.begin(), contador1.end(), posicao);
				auto exCont2 = std::count(indices.begin(), indices.end(), pai1._Caminho[posicao].indice);
				if (exCont1 == 0 && exCont2 ==0) 
				{
					filho[posicao] = pai1._Caminho[posicao];
					indices.push_back(pai1._Caminho[posicao].indice);
					contador1.push_back(posicao);
					k++;
					valor = pai1._Caminho[posicao].indice;
					if (pai1._Caminho[posicao].indice != pai2._Caminho[posicao].indice)
					{
						int valor2=0;
						[pai2,valor,&valor2]()
							{
								for (auto& pai : pai2._Caminho)
								{

									if (pai.indice == valor)
										return;
									valor2++;
								}
							};
						auto exCont1 = std::count(contador1.begin(), contador1.end(), valor2);
						auto ex2 = std::count(indices.begin(), indices.end(), pai1._Caminho[valor2].indice);
						if (exCont1 == 0 && ex2 ==0)
						{
							filho[valor2] = pai1._Caminho[valor2];
							k++;
							contador1.push_back(valor2);
							indices.push_back(pai1._Caminho[valor2].indice);

						}
					}
					else {
						contador2.push_back(posicao);
					}

				}
				p = 1;
			}
			else
			{
				auto exCont2 = std::count(contador2.begin(), contador2.end(), posicao);
				auto exCont1 = std::count(indices.begin(), indices.end(), pai2._Caminho[posicao].indice);
				if (exCont2 == 0 && exCont1 ==0)
				{

					filho[posicao] = pai2._Caminho[posicao];
					k++;
					contador2.push_back(posicao);
					indices.push_back(pai2._Caminho[posicao].indice);
					valor = pai2._Caminho[posicao].indice;
					if (pai2._Caminho[posicao].indice != pai1._Caminho[posicao].indice)
					{
						int valor2 = 0;
						[pai1, valor, &valor2]()
							{
								for (auto& pai : pai1._Caminho)
								{

									if (pai.indice == valor)
										return;
									valor2++;
								}
							};
						auto exCont2 = std::count(contador2.begin(), contador2.end(), valor2);
						auto ex2 = std::count(indices.begin(), indices.end(), pai2._Caminho[valor2].indice);
						if (exCont2 == 0 && ex2 ==0)
						{
							filho[valor2] = pai2._Caminho[valor2];
							k++;
							contador2.push_back(valor2);
							indices.push_back(pai2._Caminho[valor2].indice);
						}
					}
					else {
						contador1.push_back(posicao);
					}
				}
			p = 0;
			}
		}
		m.unlock();
		m.lock();
		float limiteAtual = 0;
		for(int i =0;i< filho.size();i++)
		{
			limiteAtual += filho[i].peso;
			//std::cout <<"limite: " << limiteAtual << std::endl;
			if (limiteAtual > cromossomo._Limite)
			{
				cromossomo._Caminho.push_back({ filho[0].indice,filho[0].x,filho[0].y,filho[0].peso });
				limiteAtual = 0;
			}
			cromossomo._Caminho.push_back({ filho[i].indice,filho[i].x,filho[i].y,filho[i].peso });
			
		}
		cromossomo._Caminho.push_back({ cromo1._Caminho[0].indice,cromo1._Caminho[0].x,cromo1._Caminho[0].y,cromo1._Caminho[0].peso });
		m.unlock();
		Mutacao(cromossomo);
		Aptidao(cromossomo);
		_populacao.push_back(cromossomo);
		
	}
	for (auto& cromossomo : _Melhorespais)
	{
		if (_populacao.size() != _Melhorespais.size())
			_populacao.push_back(cromossomo);
	}
	_Melhorespais.clear();
}

Cromossomo AlgoritmoGenetico::Selecao(std::vector<Cromossomo>& populacao)
{
	float sum = 0;
	for (auto& cromossomo : populacao)
	{
		sum += cromossomo._Aptidao;
	}
	
	while (true)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> probRoleta(0, sum);
		float randV = probRoleta(gen);
		for (auto& cromossomo : populacao) 
		{
			if (randV > cromossomo._Aptidao)
			{
				return cromossomo;

			}

		}
	}
}

void AlgoritmoGenetico::Mutacao(Cromossomo& cromossomo)
{
	float taxa = 0.1;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> prob(0, 1);
	float chance = prob(gen);
	int tam = cromossomo._Caminho.size();
	if (chance > taxa)
	{
		while (true)
		{
			std::uniform_real_distribution<> pos(0, tam);
			int num1 = pos(gen);
			int num2 = pos(gen);
			if ((num1 != 0 && num2 != 0) && num1 != num2)
			{
				auto primeiro = cromossomo._Caminho[num1];
				auto segundo = cromossomo._Caminho[num2];

				cromossomo._Caminho[num1] = segundo;
				cromossomo._Caminho[num2] = primeiro;

				return;
			}

		}
	}
}

void AlgoritmoGenetico::fitness(std::vector<Cromossomo>& populacao)
{
	std::cout << populacao.size() << std::endl;
	Cromossomo melhor = populacao[0];
	for (auto& cromossomo : populacao)
	{
		if (cromossomo._Aptidao < melhor._Aptidao)
			melhor = cromossomo;
	}
	 std::cout << "fitness: " << melhor._Aptidao << std::endl;
}

std::vector<Rota> Rotas()
{
	std::vector<RotaInfo> entrada;
	std::vector<Rota> rotas;
	std::vector<int> existe;
	entrada.push_back({ 0,145,215,0 });
	entrada.push_back({ 1,151,264,1100 });
	entrada.push_back({ 2,159,261,700 });
	entrada.push_back({ 3,130,254,800 });
	entrada.push_back({ 4,128,252,1400 });
	entrada.push_back({ 5,163,247,2100 });
	
	for (int i = 0; i < 100; i++) {
		Rota rota;
		rota.caminhoEntrada.push_back(entrada[0]);
		existe.push_back(0);
		while (rota.caminhoEntrada.size() != entrada.size())
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> probRoleta(0, 5);
			int k = probRoleta(gen);
			auto ex = std::count(existe.begin(), existe.end(), k);
			if ( ex == 0)
			{
				rota.caminhoEntrada.push_back(entrada[k]);
				existe.push_back(k);
			}

		}
		rota.caminhoEntrada.push_back({ 0,145,215,0 });
		rotas.push_back(rota);
		existe.clear();
	}
	return rotas;
}
float calcDistancia(std::pair<float, float> d1, std::pair<float, float> d2)
{
	float result = std::pow(d1.first - d2.first, 2) + std::pow(d1.second - d2.second, 2);
	float dist = std::sqrt(result);
	return dist;
}