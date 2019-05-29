#include<iostream>
#include<random>
#include<stdint.h>
#include<vector>
#include<string>
#include<list>
#include<bitset>
using namespace std;

string przedstawWynik(const vector<bool>& s, int L);

unsigned lfsr1(list<bool> & v1,vector<int> & vFeedback, int howMany)
{
	uint16_t start_state = 0xACE1u;  /* Any nonzero start state will work. */
	std::bitset<16> u(start_state);
	std::cout <<"start_state = "<< u << '\n';
	uint16_t lfsr = start_state;
	uint16_t bit = 0x0000u;                    /* Must be 16-bit to allow bit<<15 later in the code */
	
	for (int i=0; i<vFeedback.size();++i)
	{
		vFeedback[i] = 16-vFeedback[i];
	}

	unsigned period = 0;
	uint16_t x;
	for (int i = 0; i < howMany; ++i)
	{  
		for (int i = 0; i < vFeedback.size(); ++i)
		{
			bit ^= (lfsr >> vFeedback[i]);	
		}
		//cout << endl;
		//bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) /* & 1u */;
		//bit = ((lfsr >> 0));  /* & 1u */
		//cout << "lfsr>>15: " << (lfsr & 0x0001) << endl;
		v1.push_front(lfsr & 0x0001);
		lfsr = (lfsr >> 1) | (bit << 15);
		
		++period;

	}

	return period;
}




string BerlekampMassey(vector<bool> s)
{
	//L - zlozonosc liniowa s (wielomian s postaci: C(x) = c0+c1x+c2x^2+…+ cLx^L )
	//Algorytm BM znajduje L i C(x)
	int L, N, m, d;
	int n = s.size();
	

	vector<bool> c(n);
	vector<bool> b(n);
	vector<bool> t(n);

	//inicjalizacja
	b[0] = c[0] = 1;
	N = L = 0;
	m = -1;

	//algorytm
	while (N < n)
	{
		d = s[N];
		for (int i = 1; i <= L; ++i)
		{
			d ^= c[i] & s[N - i];//(d+=c[i]*s[N-i] mod 2)
		}
		if (d == 1)
		{
			//Array.copy(c, t, n); //c - source array, t - destination array, n - elements to copy 
			t = c;
			for (int i = 0; (i + N - m) < n; ++i)
			{
				c[i + N - m] = c[i + N - m] != b[i];
			}
			if (L <= (N >> 1))
			{
				L = N + 1 - L;
				m = N;
				//Array.Copy(t, b, n);
				b = t;
			}
		}
		N++;
	}
	

	cout << "Linear Span: " << L<<endl;
	string polynomial= przedstawWynik(c,L);
	cout << "LFSR: " << polynomial << endl;

	cout << endl;
	return "";
}

string przedstawWynik(const vector<bool>& s, int L)
{
	string polynomial = "";
	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] != 0)
		{
			if (L==0)
			{
				polynomial += "+ 1";
			}
			else if (i == 0)
			{
				polynomial += "x^" + to_string(L);
			}
			else
			{
				polynomial += "+ x^"+ to_string(L);
			}
			
		}
		L--;
		
	}
	return polynomial;
}

int main()
{
	cout << "LFSR - generowanie ciagu:\n";
	list<bool> l2;
	/* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
	vector<int> vFeedback = { 16,14,13,11 };
	int howLongKey = 20;
	cout << "dlugosc wygenerowanego ciagu = " << lfsr1(l2, vFeedback, howLongKey) << endl;
	cout << "Ciag: \n";
	for (auto i : l2)
	{
		cout << i;
	}


	cout << endl<<endl<<"Berlekamp-Massey:\n";
	//vector<bool> v1 = {1,0,0,0,1,1,1,0};
	vector<bool> v1;
	std::copy(l2.begin(), l2.end(), std::back_inserter(v1));
	BerlekampMassey(v1);

	
	cout << endl;
	system("pause");
	return 0;
}