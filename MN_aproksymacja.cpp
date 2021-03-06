// MN_aproksymacja.cpp 
// Heizig Patryk i Tomasz Ratajczak, sekcja 3, laboratoria z dnia 8.12.2018r
// Aproksymacja potęgowa
// wzor sin(2x) * cos(x)
// parametry startowe	-2 do 2
//						n = 49 (50 węzłów)
//						stopien 2 do 6




#include "pch.h"
#include <iostream>
#include <iomanip>
#include <math.h> 
#include <string>

using namespace std;


void drukuj_macierz(double macierz[], int stopien_d, string nazwa)
{
	cout << nazwa << endl;
	cout << "+--------+" << endl;
	for (int i = 0; i < stopien_d + 1; i++)
	{
		cout << "| ";
		cout.setf(ios::fixed);
		//cout.precision(4);
		cout << macierz[i] << " |" << endl;
	}
	cout << "+--------+" << endl;
}

void drukuj_macierz(double macierz[7][7], int stopien_d, string nazwa)
{
	cout << nazwa << endl;
	for (int i = 0; i < stopien_d + 1 ; i++)
	{
		cout << "| ";
		for (int j = 0; j < stopien_d + 1; j++)
		{
			cout.setf(ios::fixed);
			//cout.precision(4);
			cout << macierz[i][j] << " |";
		}
		cout << endl;
	}
}




int main(int argc, char** argv) {

	//parametry startowe pracy programu
	const double aa = -2;
	const double bb = 2;
	const int n_wezlow = 49;
	const int stopien_beg = 2;
	const int stopien_end = 6;
	const int stopien = 16;

	cout << "Stopien: " << stopien << endl << endl;

	//dyskretyzacja
	double dyskr[n_wezlow + 1];  //f(x)
	//double przedzial = 4.0/49;	
	double przedzial = (bb - aa) / (n_wezlow);
	double x[n_wezlow + 1];
	x[0] = aa;
	for (int i = 0; i < n_wezlow +1 ; i++)
	{
		dyskr[i] = sin(2 * x[i]) * cos(x[i]); //użyty wzór
		if (i != n_wezlow)
		{
			
			x[i + 1] = x[i] + przedzial;
		}
	}
	for (int i = 0; i < n_wezlow + 1; i++)
	{
		cout << x[i] << endl;
	}

	//wzor [6]
	double g[stopien + 1][stopien + 1] = { 0 };
	double r[stopien + 1] = { 0 };
	for (int k = 0; k < (stopien + 1); k++)
	{
		for (int i = 0; i < (n_wezlow + 1); i++)
		{
			r[k] += dyskr[i] * pow(x[i], k); //obliczenie r, suma f pomnozonych przez x do potęga k
		}
		//if ((r[k] < 0.0001) && (r[k] > -0,0001)) r[k] = 0.0;
		for (int j = 0; j < (stopien + 1); j++)
		{
			for (int i = 0; i < (n_wezlow + 1); i++)
			{
				g[k][j] += pow(x[i], (k + j));  //obliczenie g, suma x do potęgi k+j
				//if (k == 0 && j == 1) cout << "x[" << i << "] = " << x[i] << " g = " << g[k][j] << endl;
			}
			//if ((g[k][j] < 0.0001) && (g[k][j] > -0,0001)) g[k][j] = 0.0;
		}
	}

	drukuj_macierz(r, stopien, "Macierz r");
	cout << endl;
	//drukuj_macierz(g, stopien, "Macierz g");

	//rozwiązanie układu liniowego metoda Gaussa
	double a[stopien + 1] = { 0.0 };
	double U[stopien + 1][stopien + 1] = { 0.0 };  //macierz U - trojkatna gorna
	double L[stopien + 1][stopien + 1] = { 0.0 };  //macierz L - trojkatna dolna

	for (int i = 0; i < stopien + 1; i++)
	{
		for (int j = 0; j < stopien + 1; j++)
		{
			U[i][j] = g[i][j]; //Przepisujemy zawartosc macierzy A do macierzy U 
			L[i][j] = 0.0;  	// zerowanie macierzy L
		}
	}
	for (int i = 0; i < stopien + 1; i++)
	{
		L[i][i] = 1;
		if (U[i][i] == 0) //Sprawdzanie czy na głównej przekątnej nie ma 0 
			cout << "U" << i << i << " dzielenie przez 0!!!!!" << endl;
	}

	for (int k = 0; k < stopien + 1; k++)
	{
		for (int i = k +1; i < stopien + 1; i++)
		{
			if (U[k][k] == 0.0)
			{
				cout << "U" << k << k << " dzielenie przez 0!!!!!" << endl; // sprawdzenie czy dzielimy przez 0
			}

			L[i][k] = U[i][k] / U[k][k]; // obliczanie macierzy L

			for (int j = k + 2; j < stopien + 1; j++) 	 	// obliczanie macierzy U 
			{
				U[i][j] = U[i][j] - ((U[i][k] * U[k][j]) / U[k][k]);
			}
			U[i][k] = 0;
		}
	}
	cout << endl;
	//drukuj_macierz(U, stopien, "Macierz U");

	//rozwiazanie macierzy UL
	double Y[stopien + 1];
	Y[0] = r[0];
	for (int i = 1; i < stopien + 1; i++)
	{
		double suma = 0.0;
		for (int k = 0; k < i; k++)  	 	 	
			suma += L[i][k] * Y[k];
		Y[i] = r[i] - suma;
	}
	a[stopien] = Y[stopien] / U[stopien][stopien];
	for (int i = stopien - 1; i >= 0; i--)
	{
		double suma = 0.0;  	 	
		for (int k = i + 1; k <= stopien; k++)
			suma += U[i][k] * a[k];
		a[i] = (Y[i] - suma) / U[i][i];
		cout << "a" << i << ": " << a[i] << endl;
	}


	//Błąd aproksymacji
	double s = 0;
	double s_gora = 0;
	double wfa[n_wezlow + 1] = { 0.0 }; //wartosc funckji aproksymujacej


	for (int i = 0; i < n_wezlow + 1; i++)
	{
		for (int j = 0; j < stopien; j++)
		{
			wfa[i] += a[j] * pow(x[i], j);
		}
		s_gora += pow(((wfa[i]) - dyskr[i]), 2);
	}

	cout << endl;
	drukuj_macierz(wfa, n_wezlow, "WFA");

	s = sqrt(s_gora / (n_wezlow + 1));
	cout << endl << "S = " << s << endl;

	return 0;
}
