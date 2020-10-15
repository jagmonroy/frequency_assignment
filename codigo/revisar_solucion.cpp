#include "funciones.h"


int main(int argn, char **argv) {

	leer_grafo(argv[1]); // se lee grafo de archivo argv[1]
	K = stoi(argv[2]); // numero de colores
	ts = stoi(argv[3])*60*1000; // tiempo en minutos
	dis_arg = argv[4][0]; // metrica utilizada (o torneo)

	string sufijo = (string) "_" + argv[2] + "_" + argv[3] + "_" + argv[4] + "_" + argv[5] + ".txt";
	string f_sol = "solucion" + sufijo;

	ifstream in(f_sol.c_str());

	vi ind(N);

	for(int i=0; i<N; i++)
		if(ind[i]<0 || ind[i]>=K)
			cout << "mala onda\n";

	for(int i=0; i<N; i++)
		in >> ind[i];

	cout << costo(ind) << '\n';

	return 0;

}
