#include "definiciones.h"


vi permutacion(int n) {

    vi p(n);

    for(int i=0; i<n; i++)
        p[i] = i;

    random_shuffle(p.begin(), p.end());

    return p;

}


vi crear_individuo_rand() {

    vi p(N);

    for(int i=0; i<N; i++)
        p[i] = rand()%K;

    return p;

}


void leer_grafo(string str) {

    ifstream in(str.c_str());
    int a, b, s, p;
    char c1, c2;
    N = 0;

    while(in >> a >> b >> c1 >> c2 >> s >> p) {

        N = max(N, a);
        N = max(N, b);

    }

    in.close();

    G.resize(++N);

    in.open(str.c_str());

    while(in >> a >> b >> c1 >> c2 >> s >> p) {

        G[a].push_back(i_ii(b, ii(s, p)));
        G[b].push_back(i_ii(a, ii(s, p)));

    }

    in.close();

}


ll costo(vi &ind) {

    ll c = 0;

    for(int v=0; v<N; v++)
        for(i_ii dat : G[v])
            if(abs(ind[v] - ind[dat.first]) <= dat.second.first)
                c += dat.second.second;

    return c/2; //cada conflicto se cuenta dos veces

}


ll dif_costo(vi &ind, int v, int c) {

    ll cm = 0;
    ll cM = 0;

    for(i_ii dat : G[v]) {

        if(abs(ind[dat.first] - ind[v]) <= dat.second.first)
            cm += dat.second.second;

        if(abs(ind[dat.first] - c) <= dat.second.first)
            cM += dat.second.second;

    }

    return cM - cm;

}


vi busqueda_local(vi ind) {

    vi p = permutacion(N); // orden en que se visitaran los nodos
    ll dc, u;
    bool ok;

    do {

            ok = true; // se supone que es un optimo

            for(int i=0; i<N && ok; i++) {

                u = p[i];

                for(int c=0; c<K && ok; c++) {

                    if(ind[u] == c)
                        continue;

                    dc = dif_costo(ind, u, c); // costo de vecino

                    if(dc < 0) { // si la diferencia es menor, se toma ese nuevo punto

                        ind[u] = c; // se mueve al nuevo punto
                        ok = false; // se modifica bandera para iterar nuevamente
                        random_shuffle(p.begin(), p.end()); // se toma un nuevo orden de visita

                    }

                }
            }

    } while(!ok);

    return ind;

}


vi mutar(vi ind) {

    int r = rand()%R + 1; // cantidad de veces que se toman canales de manera aleatoria
    int u = rand()%N; // canal de inicio
    vi U, reg(N, 1); // en U se almacenan los canales que han generado conflictos y reg es
                     // para no agregarlos dos veces
    U.push_back(u); // se agrega u y se marca en reg para no agregarlo de nuevo
    reg[u] = 0;

    while(r--) {

        u = U[rand()%U.size()]; //se toma un elemento de U
        ind[u] = rand()%K; // se asigna un color de manera aleatoria

        for(i_ii dat : G[u])
            if(abs(ind[dat.first] - ind[u]) <= dat.second.first) { // vecinos de u que genera conflicto

                if(reg[dat.first] == 1) { // se agrega a U

                    U.push_back(dat.first);
                    reg[dat.first] = 0;

                }

                if(rand()%100 < Pm) // se modifica con probabilidad Pm
                    ind[dat.first] = rand()%K;

            }

    }

    return ind; // se regresa la mutacion

}


vi busqueda_local_iterada(vi ind) {

    int costo_a = costo(ind);
    int costo_n;
    vi ind_;

    for(int i=0; i<BI_n; i++) {

        ind_ = mutar(ind); // mutar individuo actual
        ind_ = busqueda_local(ind_); //busqueda local partiendo de ind_
        costo_n = costo(ind_);

        if(costo_n < costo_a) { // si se alcanza un mejor costo se hace el cambio

            costo_a = costo_n;
            ind = ind_;

        }

    }

    return ind;

}


vi cruza(vi ind, vi &ind_) {

    // los preparativos son analogos a la cruza
    int r = rand()%R + 1;
    int u = rand()%N;
    vi U, reg(N, 1);
    U.push_back(u);
    reg[u] = 0;

    while(r--) {

        u = U[rand()%U.size()]; // se selecciona un elemento de U
        ind[u] = rand()%K;

        // se registran los elementos que interfieren con u
        for(i_ii dat : G[u])
            if(abs(ind[dat.first] - ind[u]) <= dat.second.first) {

                if(reg[dat.first] == 1) {

                    U.push_back(dat.first);
                    reg[dat.first] = 0;

                }

            }

    }

    // los elementos en U adquieren el valor correspondiente ind_

    for(auto u : U)
        ind[u] = ind_[u];

    return ind;

}


vvi seleccion_torneo(vvi &poblacion) {

    int num_i = poblacion.size(); // se recupera el numero de individuos
    vi p = permutacion(num_i); // acomodo para el torneo!

    // variables para conservar al mejor individuo
    ll costo_m = INF;
    int indice_m;

    // costo de poblacion y arreglo para nueva poblacion
    vi costo_p(num_i);
    vvi nueva_pob(num_i/2);

    // evaluar poblacion actual

    for(int j : p) {

        costo_p[j] = costo(poblacion[j]);

        if(costo_p[j] < costo_m) {

            indice_m = j;
            costo_m = costo_p[j];

        }

    }

    costo_m_global = costo_m; // se actualiza el mejor valor

    // torneo de seleccion

    int a, b, c = 0;

    for(int j=0; j<num_i; j += 2) {

        a = p[j];
        b = p[j+1];

        if(costo_p[a] > costo_p[b]) // a es el mejor entre a y b
            swap(a, b);

        if(a == indice_m) {

            indice_m = c;
            nueva_pob[c++] = poblacion[a];

        } else if(rand()%100 < Psm) // probabilidad de mantener al mejor
            nueva_pob[c++] = poblacion[a];
        else
            nueva_pob[c++] = poblacion[b];

    }

    return nueva_pob;

}


double entropia(vvi &poblacion) {

    double p[N][K];

    for(int i=0; i<N; i++)
        for(int j=0; j<K; j++)
            p[i][j] = 0;

    for(int i=0; i<poblacion.size(); i++)
        for(int j=0; j<N; j++)
            p[j][poblacion[i][j]]++;

    double ans = 0, s, pr;

    for(int i=0; i<N; i++) {

        s = 0;

        for(int j=0; j<K; j++)
            if(p[i][j] > 0) {

                pr = p[i][j]/poblacion.size();
                s -= pr * (log(pr)/log((double)K));

            }

        ans += s;

    }

    ans /= N;
    return ans;

}


void calcular_similitud() {

    vi aux_i(N);
    vi aux_j(N);
    int c1, c2;

    // se asigna memoria a matriz de similitud

    sim = (double**)malloc(N*sizeof(double*));

    for(int i=0; i<N; i++)
        sim[i] = (double*)malloc(N*sizeof(double));

    // Se calcula sim[i][j]

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++) {

            // conjunto correspondientes a i y j
            aux_i.assign(N, 0);
            aux_j.assign(N, 0);

            for(i_ii dat : G[i])
                aux_i[dat.first] = 1;

            for(auto dat : G[j])
                aux_j[dat.first] = 1;

            //aux_i[j] = aux_j[i] = 0;

            // contadores para interseccion y union
            c1 = c2 = 0;

            for(int k=0; k<N; k++) {

                if(aux_i[k] == 1 && aux_j[k] == 1) //interseccion
                    c1++;

                if(aux_i[k] == 1 || aux_j[k] == 1) //union
                    c2++;

            }

            sim[i][j] = (double)c1/c2; //similitud

        }

}


void liberar() {

    for(int i=0; i<N; i++)
        free(sim[i]);

    free(sim);

}


double distancia_hamming(vi &ind1, vi &ind2, int a, int b) {

    if(dp[a][b] != -1)
        return dp[a][b];

    double c = 0;

    for(int i=0; i<N; i++)
        c += (ind1[i] != ind2[i]);

    return dp[a][b] = c/N;

}


double distancia_sim(vi &ind1, vi &ind2, int a, int b) {

    if(dp[a][b] != -1)
        return dp[a][b];

    double match, s = 0;

    for(int i=0; i<N; i++) {

        match = 0;

        for(int j=0; j<N; j++)
            if(ind1[i] == ind2[j])
                match = max(match, sim[i][j]);

        s += match;

    }

    for(int i=0; i<N; i++) {

        match = 0;

        for(int j=0; j<N; j++)
            if(ind2[i] == ind1[j])
                match = max(match, sim[i][j]);

        s += match;

    }

    // en este punto 0<=s<=2*N
    s /= 2*N; // ahora 0<=s<=1
    s = 1 - s; // mientras mas cerca sea S a 0 mas similares son

    return dp[a][b] = s;

}


vvi seleccion_diversidad(vvi &poblacion) {

    int num_i = poblacion.size();
    int n = num_i;

    vi costo_p(num_i);
    vd distancia_p(num_i);
    vvi nueva_pob;

    ll costo_m = INF;
    int indice_m;

    // crear e iniciar matriz dp

    dp = (double**)malloc(num_i*sizeof(double*));

    for(int i=0; i<num_i; i++) {

        dp[i] = (double*)malloc(num_i*sizeof(double));

        for(int j=0; j<num_i; j++)
            dp[i][j] = -1;

    }

    // evaluar poblacion actual

    for(int i=0; i<num_i; i++) {

        costo_p[i] = costo(poblacion[i]);

        if(costo_p[i] < costo_m) {

            indice_m = i;
            costo_m = costo_p[i];

        }

    }

    costo_m_global = costo_m;
    swap(poblacion[indice_m], poblacion[n-1]);
    nueva_pob.push_back(poblacion[n-1]);
    n--;

    t_act = high_resolution_clock::now();
    tl = chrono::duration_cast<chrono::milliseconds>(t_act - t_ini).count();
    D = Di - Di*tl/ts;
    d_promedio = 0;

    //cout << "    " << D << '\n';

    while(nueva_pob.size() < num_i/2) {

            // calcular distancias

            for(int i=0; i<n; i++) {

                double cercano = N*N; // No hay problema que no sea inf, la distancia esta normalizada

                for(int j=0; j<nueva_pob.size(); j++)
                    if(dis_arg == '1')
                        cercano = min(cercano, distancia_hamming(poblacion[i], nueva_pob[j], i, j));
                    else
                        cercano = min(cercano, distancia_sim(poblacion[i], nueva_pob[j], i, j));

                distancia_p[i] = cercano;

                if(distancia_p[i] < D)
                    costo_p[i] = INF;

                distancia_p[i] *= -1; //ahora se quiere minimizar la distancia

            }

            // soluciones no dominadas

            vi no_dominados;

            for(int i=0; i<n; i++) {

                bool no_dominado = true;

                for(int j=0; j<n && no_dominado; j++)
                    if(costo_p[j] <= costo_p[i] && distancia_p[j] < distancia_p[i])
                        no_dominado = false;
                    else if(costo_p[j] < costo_p[i] && distancia_p[j] <= distancia_p[i])
                        no_dominado = false;

                if(no_dominado)
                    no_dominados.push_back(i);

            }

            // seleccionar alguna solucion no dominada

            indice_m = no_dominados[rand()%no_dominados.size()];
            d_promedio += -distancia_p[indice_m];
            swap(poblacion[indice_m], poblacion[n-1]);
            swap(dp[indice_m], dp[n-1]);
            nueva_pob.push_back(poblacion[n-1]);
            n--;

    }

    d_promedio /= nueva_pob.size();
    cout << "dpro " << d_promedio << " de " << D << '\n';

    // limpiar dp

    for(int i=0; i<num_i; i++)
        free(dp[i]);

        free(dp);

    return nueva_pob;

}
