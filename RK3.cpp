#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <vector>
#include <iomanip>
#include <string>
//Структура для создания особи
struct point
{
	double x;
	double y;
	double fit;
	point()
	{
		x = 0;
		y = 0;
		fit = 0;
	}
	point(double x_, double y_, double fit_)
	{
		x = x_;
		y = y_;
		fit = fit_;
	}
};
//Заданная функция
double function(double x, double y)
{
	return cos(x) * cos(y) * exp(-(x * x) - (y * y));//(-2,2) x (-2,2)
}
//Функция для генерации случайных чисел
double Random(double left, double right)
{
	return (double)(rand()) / RAND_MAX * (right - left) + left;
}
//Функция мутации
std::vector<point> mutation(std::vector<point> population)
{
	double delta = 0.05;
	int i = rand() % 4;
	if (Random(0, 1) <= 0.5)//x или y
	{
		if (Random(0, 1) <= 0.5)//+delta или -delta
		{
			population[i].x += delta;
		}
		else
		{
			population[i].x -= delta;
		}
	}
	else
	{
		if (Random(0, 1) <= 0.5)//+delta или -delta
		{
			population[i].y += delta;
		}
		else
		{
			population[i].y -= delta;
		}
	}
	return population;
}
//Функция для вывода значений
void print(std::vector<point> v, int N, double fit_max, double fit_sum, bool flag)
{
	double fit_mean;
	std::string flag_s;
	if (flag == 1)
		flag_s = "+  ";
	else
		flag_s = "-  ";
	fit_mean = fit_sum / 4;//Подсчет среднего значения
	for (int i = 0; i < v.size(); i++)
	{
		if (N == 0)
		{
			if (i == 0)
			{
				std::cout << "|" << std::setw(11) << "(исходное)" << "|" << std::fixed << std::setprecision(6) << std::setw(10) << v[i].x << "|" << std::setw(10) << v[i].y << "|" << std::setw(15) << v[i].fit << "|" << std::setw(22) << fit_max << "|" << std::setw(17) << fit_mean << "|" << std::setw(11) << "(исходное)" << "|" << std::endl;
			}
			else
			{
				std::cout << "|" << std::setw(12) << "|" << std::fixed << std::setprecision(6) << std::setw(10) << v[i].x << "|" << std::setw(10) << v[i].y << "|" << std::setw(15) << v[i].fit << "|" << std::setw(23) << "|" << std::setw(18) << "|" << std::setw(12) << "|" << std::endl;
			}
		}
		else
		{
			if (i == 0)
			{
				std::cout << "|" << std::fixed << std::setprecision(0) << std::setw(11) << N << "|" << std::fixed << std::setprecision(6) << std::setw(10) << v[i].x << "|" << std::setw(10) << v[i].y << "|" << std::setw(15) << v[i].fit << "|" << std::setw(22) << fit_max << "|" << std::setw(17) << fit_mean << "|" << std::setw(11) << flag_s << "|" << std::endl;
			}
			else
			{
				std::cout << "|" << std::setw(12) << "|" << std::fixed << std::setprecision(6) << std::setw(10) << v[i].x << "|" << std::setw(10) << v[i].y << "|" << std::setw(15) << v[i].fit << "|" << std::setw(23) << "|" << std::setw(18) << "|" << std::setw(12) << "|" << std::endl;
			}
		}
	}
	std::cout << "+-----------+----------+----------+---------------+----------------------+-----------------+-----------+" << std::endl;
}
void genetic_algorithm()
{
	std::cout << "________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::setw(11) << "№ поколения" << "|" << std::setw(10) << "X" << "|" << std::setw(10) << "Y" << "|" << std::setw(15) << "FIT" << "|" << std::setw(22) << "Максимальный результат" << "|" << std::setw(17) << "Средний результат" << "|" << std::setw(11) << "Мутация" << "|" << std::endl;
	std::cout << "+-----------+----------+----------+---------------+----------------------+-----------------+-----------+" << std::endl;
	//Исходная популяция	
	double sum_fit, max_fit;
	sum_fit = 0;
	std::vector<point> population(4);
	for (int i = 0; i < population.size(); i++)
	{
		population[i].x = Random(-2, 2);
		population[i].y = Random(-2, 2);
		population[i].fit = function(population[i].x, population[i].y);
		if (population[i].fit < 0)
		{
			i--;
		}
	}
	for (auto& pos : population)
		sum_fit += pos.fit;
	max_fit = population[0].fit;
	for (int i = 0; i < population.size(); i++)
	{
		if (population[i].fit >= max_fit)
			max_fit = population[i].fit;
	}
	print(population, 0, max_fit, sum_fit, false);

	std::vector<double> P(4);
	std::vector<double> X(3);
	std::vector<double> Y(3);
	for (auto& pos : P, X, Y)
	{
		pos = 0;
	}
	double sum_P, random_P, fit_max;
	bool flag;
	int position_ignore, position_max, iter;
	int N = 0;
	while (N != 10)
	{
		N++;
		sum_fit = 0;
		for (auto& pos : population)
			sum_fit += pos.fit;

        //Селекция 
		sum_P = 0;
		for (int i = 0; i < population.size(); i++)
		{
			P[i] = 1 - (population[i].fit / sum_fit);//
			sum_P += P[i];
		}
		random_P = Random(0, sum_P);
		position_ignore = 0;
		for (int i = 0; i < 4; i++)
		{
			if (random_P <= P[i])
			{
				position_ignore = i;
				break;
			}
			else
			{
				P[i + 1] = P[i] + P[i + 1];
			}
		}
		//Поиск максимального значения fit 
		if (position_ignore != 0)
		{
			fit_max = population[0].fit;
			position_max = 0;
		}
		else
		{
			fit_max = population[1].fit;
			position_max = 1;
		}
		for (int i = 0; i < population.size(); i++)
		{
			if (i != position_ignore)
			{
				if (population[i].fit > fit_max)
				{
					fit_max = population[i].fit;
					position_max = i;
				}
			}
		}
		//Кроссовер
		iter = 0;
		for (int i = 0; i < population.size(); i++)
		{
			if (i == position_max)
			{
				X[0] = population[i].x;
				Y[0] = population[i].y;
			}
			else
			{
				if (i != position_ignore)
				{
					iter++;
					X[iter] = population[i].x;
					Y[iter] = population[i].y;
				}
			}
		}
		population[0].x = X[0];
		population[0].y = Y[1];
		population[1].x = X[0];
		population[1].y = Y[2];
		population[2].x = X[1];
		population[2].y = Y[0];
		population[3].x = X[2];
		population[3].y = Y[0];
		//Мутация
		if (Random(0, 1) <= 0.25)
		{
			population = mutation(population);
			flag = true;
		}
		else
		{
			flag = false;
		}
		for (int i = 0; i < population.size(); i++)
		{
			population[i].fit = function(population[i].x, population[i].y);
		}
		sum_fit = 0;
		for (auto& pos : population)
			sum_fit += pos.fit;
		max_fit = population[0].fit;
		for (int i = 0; i < population.size(); i++)
		{
			if (population[i].fit >= max_fit)
				max_fit = population[i].fit;
		}
		print(population, N, max_fit, sum_fit, flag);
	}
}
int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "rus");
	genetic_algorithm();
	return 0;
}
