#include "Header.h"
int main(int argc, char* argv[])
{

	//citirea din linia de comanda a fisierelor:
	/*if (argc > 1)
	{
		if (argc > 5) cout << "puteti da maxim 5 fisiere de intrare";
		else
		{
			for (int i = 1; i < argc; i++)
			{

				Files::citireCMD(argv[i]);

			}
		}
	}*/



	propozitie p;

	while (1) {
		cin >> p;
		ParametriComanda g(p.getMatriceCuvinte(), p.getNumarCuvinte(), p.getBuffer());
		g.afisareParametriComanda(p.getMatriceCuvinte(), p.getNumarCuvinte(), p.getBuffer());
	}

	return 0;
}