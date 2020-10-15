En la carpeta resp se tiene la version secuencial, que consta de definiciones.h, funciones.h y k1.cpp.
definiciones.h y funciones.h son los mismos que en la version paralela, pero use la carpeta resp como
respaldo para empezar a crear la paralela. En los k1.cpp se encuentra el main. Ambos reciben 5 argumentos:

arg1: archivo que contiene al grafo (ponderado)
arg2: Numero de canales K
arg3: tiempo de ejecucion en minutos
arg4: estrategia utilizada.
	0 torneo
	1 diversidad Hamming
	2 diversidad SIM
arg5: numero de iteracion

Al final se tienen 4 archivos, todos del estilo

x_arg2_arg3_arg4_arg5.txt, donde x es costo, solucion, distancia o entropia. El tiempo se imprime en
milisegundos. El archivo de costo tiene observaciones del estilo (t, f), donde f es la mejor solucion
encontrada hasta el tiempo t; el archivo de solucion contiene N enteros, que son las asignaciones
correspondientes a los N transmisores; distancia tiene observaciones del estilo (t, Di, Dp), donde
la Di es la distancia deseada a un tiempo t y Dp es la distancia que se tiene en ese tiempo t (si
arg4 es 0, Di y Dp no se calculan, asi que siempre se hace 0); la entropia tiene observaciones del
estilo (t, e), donde e es la estropia al momento t.

Se compilo con C++11, no se requieren de banderas adicionales.

El codigo revisar_solucion.cpp es justo para lo que indica su nombre. Recibe los mismos argumentos que
k1.cpp para recrear el archivo de salida solucion_arg2_arg3_arg4_arg5.txt y valida que sea algo que
tiene sentido e imprime su costo (solo para verificar).


