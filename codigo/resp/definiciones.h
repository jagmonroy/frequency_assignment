#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef long long int ll;

int N; // cantidad de nodos
int K; // cantidad de colores
ll INF = numeric_limits<ll>::max()/2 - 1;
double Di; // distancia inicial

#define R 8 // iteraciones en mutacion
#define Pm 2 // probabilidad de mutacion
#define Psm 75 // probabilidad de seleccionar al mayor
#define BI_n 200 // iteraciones en busqueda iterada


typedef pair<ll, ll> ii;
typedef vector<ll> vi;
typedef vector<vi> vvi;
typedef pair<ll, ii> i_ii;
typedef vector<i_ii> vi_ii;
typedef vector<vi_ii> vvi_ii;
typedef vector<double> vd;
typedef unsigned short int uint8;
typedef unsigned long long int ull;


vvi_ii G; // grafo - lista de adyacencia
double **sim; // matriz de similitud
double **dp; // matriz para no calcular distancias mas de una vez
high_resolution_clock::time_point t_ini, t_act; // variables para tomar tiempo
ull tl, ts; // variables para medir tiempo usando las de la linea anterior
int dis_arg; // variable que indica que distancia tomar entre hamming y sim
ll costo_m_global; // variable para almacenar el menor costo hasta el momento
double D, d_promedio;


// genera una permutacion de [0, 1, ..., n-1] con random_shuffle
vi permutacion(int n);

// crea un individio de manera aleatoria (distribución uniforme)
vi crear_individuo_rand();

// leer el grafo del archivo str
void leer_grafo(string str);

// calcular similitud entre cada par de nodos. Se almacena en sim
void calcular_similitud();

// libera la memoria asignada a G_mat y sim
void liberar();

// calcula el costo de ind
ll costo(vi &ind);

// calcula la diferencia de costo que se genera al camviar la v-esima posicion de ind
// por c
ll dif_costo(vi &ind, int v, int c);

// busqueda local tomando a ind como punto inicial (retorna ind)
vi busqueda_local(vi ind);

// regresa la mutacion de un individuo (retorna ind, por eso se pasa por copia)
vi mutar(vi ind);

// regresa la cruza de ind y ind_
vi cruza(vi ind, vi &ind_);

// búsqueda local iterada tomando a ind como punto inicial (retorna ind)
// aplica busqueda local y mutacion BI_n veces
vi busqueda_local_iterada(vi ind);

// recibe una poblacion con cardinalidad par y elige la mitad de los individuos
vvi seleccion_torneo(vvi &poblacion);

// calcula la distancia de hamming entre ind1 y ind2. ind1 siempre es un individuo
// poblacion y ind2 un elemento de nueva poblacion. a es el indice correspondiente a
// a en poblacion y n al indice correspondiente en nueva poblacion. Se almacenan las
// distancias calculadas en dp[][] para no calcularlas dos veces
double distancia_hamming(vi &ind1, vi &ind2, int a, int b);

// calcula la distancia sim entre ind1 y ind2. Los indices son analogos a lo que se
// hace en hamming
double distancia_sim(vi &ind1, vi &ind2, int a, int b);

// Reduce la población a la mitad tomando en cuenta la diversidad. Use hamming o sim
// dependiendo del valor de sim_arg
vvi seleccion_diversidad(vvi &poblacion);

// entropia en poblacion
double entropia(vvi &poblacion);
