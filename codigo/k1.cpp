#include "funciones.h"
#include <mpi.h>


int main(int argn, char **argv) {

    // mpi

    MPI_Init(NULL, NULL);

    int world_size, world_rank;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // argumentos

    srand(stoi(argv[5]) + 1);

    leer_grafo(argv[1]); // se lee grafo de archivo argv[1]
    K = stoi(argv[2]); // numero de colores
    ts = stoi(argv[3])*60*1000; // tiempo en minutos
    dis_arg = argv[4][0]; // metrica utilizada (o torneo)

    if(dis_arg == '1')
        Di = 0.75; // Distancia Hamming
    else
        Di = 0.25; //0.375; // Distancia SIM


    // Archivos donde se almacenaran resultados


    string sufijo = (string) "_" + argv[2] + "_" + argv[3] + "_" + argv[4] + "_" + argv[5] + ".txt";
    string f_ent = "entropia" + sufijo;
    string f_cos = "costo" + sufijo;
    string f_dis = "distancia" + sufijo;
    string f_sol = "solucion" + sufijo;

    if(world_rank == 0) {

        cout << f_ent << '\n';
        cout << f_cos << '\n';
        cout << f_dis << '\n';

    }

    int num_i = 50; // numero de individuos en poblacion
    t_ini = high_resolution_clock::now(); // tiempo inicial

    vvi poblacion;

    if(world_rank == 0)
        poblacion.resize(num_i);

    ll b_s_1 = INF;
    ll b_s_2 = INF;
    ll b_s_3 = INF;
    ll aux_1, aux_2, aux_3;

    if(world_rank == 0) {

        calcular_similitud();

        cout << "Creando poblacion inicial\n";

        for(int i=0; i<num_i; i++) {
            poblacion[i] = crear_individuo_rand();
            aux_1 = costo(poblacion[i]);
            b_s_1 = min(b_s_1, aux_1);
            poblacion[i] = busqueda_local(poblacion[i]);
            aux_2 = costo(poblacion[i]);
            b_s_2 = min(b_s_2, aux_2);

        }

        int mandar = 0, s;
        int resolviendo = world_size - 1;
        vi ok(resolviendo), asignacion(resolviendo), buf(N);

        while(resolviendo) {

            MPI_Recv(&buf[0], N, MPI_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            s = status.MPI_SOURCE;

            // si no es la primera vez que se recibe algo de s, entonces si es un resultado y se almacena
            if(ok[s-1])
                poblacion[asignacion[s-1]] = buf;
            // si es la primera vez, el hijo se esta reportando

            if(mandar < num_i) { // si hay tareas en cola

                ok[s-1] = 1;
                MPI_Send(&poblacion[mandar][0], N, MPI_LONG_LONG, s, 0, MPI_COMM_WORLD);
                asignacion[s-1] = mandar++;

            } else
                // se manda nada a s para que sepa que puede parar.
                --resolviendo; // se decrementa world_size en 1 para saber que hay un hijo m$

        }

        for(int i=0; i<num_i; i++) {

            aux_3 = costo(poblacion[i]);

            if(aux_3 < b_s_3)
                b_s_3 = aux_3;

        }

        cout << '\n';
        cout << "min 1 (aleatorio): " << b_s_1 << '\n';
        cout << "min 2 (busquedaL): " << b_s_2 << ' ' << b_s_1 - b_s_2 << '\n';
        cout << "min 3 (busque LI): " << b_s_3 << ' ' << b_s_2 - b_s_3 << '\n';

    } else {

	vi buf(N);
        int n_recibidos;

        MPI_Send(NULL, 0, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD); // se notifica al nodo maestro que esta disponible

        while(true) {

            MPI_Recv(&buf[0], N, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_LONG_LONG, &n_recibidos); // ver cuantas tareas se reciben

            if(!n_recibidos) // si no se recibe tarea, se puede terminar
                break;

            buf = busqueda_local_iterada(buf);
            MPI_Send(&buf[0], N, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

        }

    }

    if(world_rank == 0) {

        tl = 0;
        vvi nueva_pob;
        double entropia_p;

        while(tl < ts) {

            cout << (float)tl/ts << '\n';

            nueva_pob = seleccion_torneo(poblacion);

            for(int j=0; j<num_i/2; j++)
                nueva_pob.push_back(mutar(nueva_pob[j]));

            int a, b;
            vi p = permutacion(num_i);

            for(int j=0; j<num_i; j += 2) {

                a = p[j];
                b = p[j+1];

                nueva_pob.push_back(cruza(nueva_pob[a], nueva_pob[b]));
                nueva_pob.push_back(cruza(nueva_pob[b], nueva_pob[a]));

            }

            // busqueda local ****************************************************

            num_i *= 2;

            int mandar = 0, s;
            int resolviendo = world_size - 1;
            vi asignacion(resolviendo), buf(N);

            for(int i=0; i<min(num_i, resolviendo); i++) {

                MPI_Send(&nueva_pob[mandar][0], N, MPI_LONG_LONG, i+1, 0, MPI_COMM_WORLD);
                asignacion[i] = mandar++;

            }

            while(resolviendo) {

                MPI_Recv(&buf[0], N, MPI_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                s = status.MPI_SOURCE;

                nueva_pob[asignacion[s-1]] = buf;

                if(mandar < num_i) { // si hay tareas en cola

                    MPI_Send(&nueva_pob[mandar][0], N, MPI_LONG_LONG, s, 0, MPI_COMM_WORLD);
                    asignacion[s-1] = mandar++;

                } else
                    // se manda nada a s para que sepa que puede parar.
                    --resolviendo; // se decrementa world_size en 1 para saber que hay un hijo m$

            }

            // seleccionar nueva poblacion **************************************

	    cout << "seleccion de poblacion\n";

            if(dis_arg == '0') {

                cout << "torneo\n";
                poblacion = seleccion_torneo(nueva_pob);

            } else {

                cout << "diversidad " << ((dis_arg == '1') ? "hamming\n" : "sim\n");
                poblacion = seleccion_diversidad(nueva_pob);

            }

            num_i /= 2;

            entropia_p = entropia(poblacion);
            cout << "entropia: " << entropia_p << "\n";
            cout << "costo: " << costo_m_global << "\n\n";

            t_act = high_resolution_clock::now();
            tl = chrono::duration_cast<chrono::milliseconds>(t_act - t_ini).count();

            ofstream out1(f_ent.c_str(), ios_base::app);
            out1 << tl << ' ' << entropia_p << '\n';
            out1.close();

            ofstream out2(f_cos.c_str(), ios_base::app);
            out2 << tl << ' ' << costo_m_global << '\n';
            out2.close();

            ofstream out3(f_dis.c_str(), ios_base::app);
            out3 << tl << ' ' << D << ' ' << d_promedio << '\n';
            out3.close();

        }

        for(int i=1; i<world_size; i++)
            MPI_Send(NULL, 0, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);

        ll b_s_4 = INF;
        ll ind, aux_c;

        for(int i=0; i<num_i; i++) {

            aux_c = costo(poblacion[i]);

            if(aux_c < b_s_4) {

                b_s_4 = aux_c;
                ind = i;

	    }

        }

        ofstream out4(f_sol.c_str(), ios_base::app);
        for(int i=0; i<N; i++)
            out4 << poblacion[ind][i] << ' ';
        out4 << '\n';
        out4.close();

        cout << '\n';
        cout << "min 1 (aleatorio): " << b_s_1 << '\n';
        cout << "min 2 (busquedaL): " << b_s_2 << ' ' << b_s_1 - b_s_2 << '\n';
        cout << "min 3 (busque LI): " << b_s_3 << ' ' << b_s_2 - b_s_3 << '\n';
        cout << "min 4 (Amemetico): " << b_s_4 << ' ' << b_s_3 - b_s_4 << '\n';

        liberar();

    } else {


    }

    MPI_Finalize();

    return 0;

}
