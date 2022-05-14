#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	actual.fila = sensores.posF;
	actual.columna = sensores.posC;
	actual.orientacion = sensores.sentido;

	cout << "Fila: " << actual.fila << endl;
	cout << "Col : " << actual.columna << endl;
	cout << "Ori : " << actual.orientacion << endl;

	//pintamos el mapa si estamos en el nivel 3 y 4
	if(sensores.nivel == 3 || sensores.nivel == 4 )
	{
		if(!hay_destinos)
		{
			//destinos.clear();
			//variables auxiliares para los destinos
			estado auxiliar;

			if(primera_creacion_destinos)
			{
				for(int i=3;i<mapaResultado.size()-3;i=i+3)
				{
					// de izquierda a derecha 
					for(int j=3;j<mapaResultado.size()-3;j++)
					{
						auxiliar.fila = i;
						auxiliar.columna = j;
						destinos.push_back(auxiliar);
					}
					 i=i+3;
					// de derecha a izquierda
					for(int j=mapaResultado.size()-3;j>3;j--)
					{
						auxiliar.fila = i;
						auxiliar.columna = j;
						destinos.push_back(auxiliar);
					}
				}
			}
			else
			{
				for(int i=3;i<mapaResultado.size()-3;i++)
				{
					// de izquierda a derecha
					for(int j=3;j<mapaResultado.size()-3;j++)
					{
						auxiliar.fila = i;
						auxiliar.columna = j;
						destinos.push_back(auxiliar);
					}

					i=i+3;
					// de derecha a izquierda
					for(int j=mapaResultado.size()-3;j>3;j--)
					{
						auxiliar.fila = i;
						auxiliar.columna = j;
						destinos.push_back(auxiliar);
					}
				}
			}
			
			
			hay_destinos = true;
		}
		
		/*
		// imprimimos los destinos a ver
		int i = 0;
		for (auto it = destinos.begin() ;it != destinos.end(); ++it) 
		{
    		cout<<"Destino :"<<i<<endl;
			cout<<it->fila<<","<<it->columna;
			cout<<endl;
			i++;
		}
		*/
		// pintamos el mapa
		int index = 1;

		switch (actual.orientacion)
		{
		case norte:
			for (int f = 1; f <= 3; f++)
			{
				for (int c = -f; c <= f; c++)
				{
					mapaResultado[actual.fila - f][actual.columna + c] = sensores.terreno[index];
					index++;
				}
				}
			break;
		case noreste:
			mapaResultado[actual.fila - 1][ actual.columna] = sensores.terreno[1];
			mapaResultado[actual.fila - 1][actual.columna + 1] = sensores.terreno[2];
			mapaResultado[actual.fila][actual.columna + 1] = sensores.terreno[3];

			mapaResultado[actual.fila - 2][actual.columna] = sensores.terreno[4];
			mapaResultado[actual.fila - 2][actual.columna + 1] = sensores.terreno[5];
			mapaResultado[actual.fila - 2][actual.columna + 2] = sensores.terreno[6];
			mapaResultado[actual.fila - 1][actual.columna + 2] = sensores.terreno[7];
			mapaResultado[actual.fila][actual.columna + 2] = sensores.terreno[8];

			mapaResultado[actual.fila - 3][actual.columna] = sensores.terreno[9];
			mapaResultado[actual.fila - 3][actual.columna + 1] =sensores.terreno[10];
			mapaResultado[actual.fila - 3][actual.columna + 2] =sensores.terreno[11];
			mapaResultado[actual.fila - 3][actual.columna + 3] =sensores.terreno[12];
			mapaResultado[actual.fila - 2][actual.columna + 3] =sensores.terreno[13];
			mapaResultado[actual.fila - 1][actual.columna + 3] =sensores.terreno[14];
			mapaResultado[actual.fila][actual.columna + 3] =sensores.terreno[15];
			break;
		case este:
			for (int c = 1; c <= 3; c++)
			{
				for (int f = -c; f <= c; f++)
				{
					mapaResultado[actual.fila + f][actual.columna + c] = sensores.terreno[index];
					index++;
				}
			}
			break;
		case sureste:
			mapaResultado[actual.fila][actual.columna + 1] =sensores.terreno[1];
			mapaResultado[actual.fila + 1][actual.columna + 1] =sensores.terreno[2];
			mapaResultado[actual.fila + 1][actual.columna] =sensores.terreno[3];

			mapaResultado[actual.fila][actual.columna + 2] =sensores.terreno[4];
			mapaResultado[actual.fila + 1][actual.columna + 2] =sensores.terreno[5];
			mapaResultado[actual.fila + 2][actual.columna + 2] =sensores.terreno[6];
			mapaResultado[actual.fila + 2][actual.columna + 1] =sensores.terreno[7];
			mapaResultado[actual.fila + 2][actual.columna] =sensores.terreno[8];

			mapaResultado[actual.fila][actual.columna + 3] =sensores.terreno[9];
			mapaResultado[actual.fila + 1][actual.columna + 3] =sensores.terreno[10];
			mapaResultado[actual.fila + 2][actual.columna + 3] =sensores.terreno[11];
			mapaResultado[actual.fila + 3][actual.columna + 3] =sensores.terreno[12];
			mapaResultado[actual.fila + 3][actual.columna + 2] =sensores.terreno[13];
			mapaResultado[actual.fila + 3][actual.columna + 1] =sensores.terreno[14];
			mapaResultado[actual.fila + 3][actual.columna] =sensores.terreno[15];

			break;
		case sur:
			for (int f = 1; f <= 3; f++)
			{
				for (int c = -f; c <= f; c++)
				{
					mapaResultado[actual.fila + f][actual.columna - c] = sensores.terreno[index];
					index++;
				}
			}
			break;
		case suroeste:
			mapaResultado[actual.fila + 1][actual.columna] =sensores.terreno[1];
			mapaResultado[actual.fila + 1][actual.columna - 1] =sensores.terreno[2];
			mapaResultado[actual.fila][actual.columna - 1] =sensores.terreno[3];

			mapaResultado[actual.fila + 2][actual.columna] =sensores.terreno[4];
			mapaResultado[actual.fila + 2][actual.columna - 1] =sensores.terreno[5];
			mapaResultado[actual.fila + 2][actual.columna - 2] =sensores.terreno[6];
			mapaResultado[actual.fila + 1][actual.columna - 2] =sensores.terreno[7];
			mapaResultado[actual.fila][actual.columna - 2] =sensores.terreno[8];

			mapaResultado[actual.fila + 3][actual.columna] =sensores.terreno[9];
			mapaResultado[actual.fila + 3][actual.columna - 1] =sensores.terreno[10];
			mapaResultado[actual.fila + 3][actual.columna - 2] =sensores.terreno[11];
			mapaResultado[actual.fila + 3][actual.columna - 3] =sensores.terreno[12];
			mapaResultado[actual.fila + 2][actual.columna - 3] =sensores.terreno[13];
			mapaResultado[actual.fila + 1][actual.columna - 3] =sensores.terreno[14];
			mapaResultado[actual.fila][actual.columna - 3] =sensores.terreno[15];

			break;
		case oeste:
			for (int c = 1; c <= 3; c++)
			{
				for (int f = -c; f <= c; f++)
				{
					mapaResultado[actual.fila - f][actual.columna - c] = sensores.terreno[index];
					index++;
				}
			}
			break;
		case noroeste:
			mapaResultado[actual.fila][actual.columna - 1] = sensores.terreno[1];
			mapaResultado[actual.fila - 1][actual.columna - 1] =sensores.terreno[2];
			mapaResultado[actual.fila - 1][actual.columna] =sensores.terreno[3];

			mapaResultado[actual.fila][actual.columna - 2] =sensores.terreno[4];
			mapaResultado[actual.fila - 1][actual.columna - 2] =sensores.terreno[5];
			mapaResultado[actual.fila - 2][actual.columna - 2] =sensores.terreno[6];
			mapaResultado[actual.fila - 2][actual.columna - 1] =sensores.terreno[7];
			mapaResultado[actual.fila - 2][actual.columna] =sensores.terreno[8];

			mapaResultado[actual.fila][actual.columna - 3] =sensores.terreno[9];
			mapaResultado[actual.fila - 1][actual.columna - 3] =sensores.terreno[10];
			mapaResultado[actual.fila - 2][actual.columna - 3] =sensores.terreno[11];
			mapaResultado[actual.fila - 3][actual.columna - 3] =sensores.terreno[12];
			mapaResultado[actual.fila - 3][actual.columna - 2] =sensores.terreno[13];
			mapaResultado[actual.fila - 3][actual.columna - 1] =sensores.terreno[14];
			mapaResultado[actual.fila - 3][actual.columna] =sensores.terreno[15];
			break;
		}

		//comprobamos si hemos visto alguna casilla de bikini o zapatillas
		for(int i=0;i<sensores.terreno.size();i++)
		{
			if(sensores.terreno[i] == 'K')
				casilla_bikini = true;
			if(sensores.terreno[i] == 'D')
				casilla_zapatillas = true;
		}

		// empezamos con la planificacion

		// comprobamos si tenemos bikini o zapatillas
		if(sensores.terreno[0] == 'K')
		{
			tengo_bikini = true;
			tengo_zapatillas = false;
		}
		else if(sensores.terreno[0] == 'D')
		{
			tengo_zapatillas = true;
			tengo_bikini = false;
		}
			
		//comprobamos si ya hemps visto ese destino o no
		while(mapaResultado[destinos.front().fila][destinos.front().columna] != '?')
		{
			hay_plan = false;
			destinos.pop_front();
		}


		// Capturo los destinos
		cout << "Numero de destinos : " << destinos.size() << endl;
		
		cout<<"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii "<<primer_obstaculo<<endl;
		// comprobamos si es la primera vez que encontramos agua o bosque para saltarlo para poder
		//contabilizar el coste de atravesarlo 
		if( ( (sensores.terreno[2] == 'A' && !tengo_bikini) || (sensores.terreno[2] == 'B' && !tengo_zapatillas) ) && (primer_obstaculo) )
		{
			hay_plan = false;
			//destinos.pop_front();

			primer_obstaculo = false;
		}

		cout<<"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee "<<destinos.size()<<endl;
		if(!primer_obstaculo && !(mapaResultado[destinos.front().fila][destinos.front().columna] == 'A'|| mapaResultado[destinos.front().fila][destinos.front().columna] == 'B'))
			primer_obstaculo = true;


		cout<<"tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt"<<endl;
		//comprobamos si hemos llegado ya al destino de arriba de la cola o no es visitable
		if( (actual.fila == destinos.front().fila && actual.columna == destinos.front().columna) || !esVisitable(destinos.front()))
		{
			hay_plan = false;
			destinos.pop_front();
		}
		cout<<"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo "<< casilla_zapatillas << tengo_zapatillas <<endl;
		
		//comprobamos si hay obstaculo delante o si es visitable
		//cout<<"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii "<<sensores.terreno[2]<<esVisitable(sensores.terreno[2])<<endl;
		if(!esVisitable(sensores.terreno[2]) && plan.front() == actFORWARD)
		{	
			hay_plan = false;
			//destinos.pop_front();
		}
		/*
		if(!esVisitable(sensores.terreno[6]))
		{	
			hay_plan = false;
			//destinos.pop_front();
		}
		*/
		//comprobamos que haya destinos
		if(destinos.size() > 0)
		{
			actual.zapatillas = tengo_zapatillas;
			actual.bikini = tengo_bikini;

			if (!hay_plan)
			{	
				// calculamos que no se haya intentado calcular el plan masd e 7 veces por nodo
				if(num_intentos == 0)
					ultimo = destinos.front();
				else if(!esVisitable(sensores.terreno[2]) && plan.front() == actFORWARD)
				{
					destinos.pop_front();
					num_intentos = 0;
				}
				else if(num_intentos > 7)
				{
					destinos.pop_front();
					num_intentos = 0;
				}

				num_intentos++;
				hay_plan = pathFinding(sensores.nivel, actual, destinos, plan);
			}
			

			if (hay_plan && plan.size() > 0)
			{
				accion = plan.front();
				plan.pop_front();
			}
		}
		else
		{
			hay_destinos = false;
			primera_creacion_destinos = false;
		}
			
		

	}

	if(sensores.nivel != 3 && sensores.nivel != 4)
	{
			// Capturo los destinos
		cout << "sensores.num_destinos : " << sensores.num_destinos << endl;
		objetivos.clear();
		for (int i = 0; i < sensores.num_destinos; i++)
		{
			estado aux;
			aux.fila = sensores.destino[2 * i];
			aux.columna = sensores.destino[2 * i + 1];
			objetivos.push_back(aux);
		}

		if (!hay_plan)
			hay_plan = pathFinding(sensores.nivel, actual, objetivos, plan);

		else if (hay_plan && plan.size() > 0)
		{
			accion = plan.front();
			plan.pop_front();

			if(destinos.size() < 1)
				hay_destinos = false;
		}
	}

	
		
	

	

	return accion;
}

// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
	estado un_objetivo;
	un_objetivo = objetivos.front();

	switch (level)
	{
	case 0:
		cout << "Demo\n";
		// estado un_objetivo;
		// un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;

	case 1:
		cout << "Optimo numero de acciones\n";
		// Incluir aqui la llamada al busqueda en anchura
		// estado un_objetivo;
		// un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Anchura(origen, un_objetivo, plan);
		cout << "No implementado aun\n";
		break;
	case 2:
		cout << "Optimo en coste\n";
		// Incluir aqui la llamada al busqueda de costo uniforme/A*
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_AStar(origen, un_objetivo, plan,&ChebyshevDistance);
		cout << "No implementado aun\n";
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 1
		cout << "fila: " << destino.front().fila << " col:" << destino.front().columna << endl;
		return pathFinding_AStar(origen, destino.front(), plan,&ManhattanDistance);
		cout << "No implementado aun\n";
		break;
	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 2
		cout << "No implementado aun\n";
		break;
	}
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M')
		return true;
	else
		return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		fil--;
		col++;
		break;
	case 2:
		col++;
		break;
	case 3:
		fil++;
		col++;
		break;
	case 4:
		fil++;
		break;
	case 5:
		fil++;
		col--;
		break;
	case 6:
		col--;
		break;
	case 7:
		fil--;
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;

		// comprobamos que estemos cayendo en unas zapatillas o bikini
		if (mapaResultado[st.fila][st.columna] == 'K')
		{
			st.zapatillas = false;
			st.bikini = true;
		}
		else if (mapaResultado[st.fila][st.columna] == 'D')
		{
			st.bikini = false;
			st.zapatillas = true;
		}

		return false;
	}
	else
	{
		return true;
	}
}

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini > n.bikini) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.bikini == n.bikini and a.zapatillas > n.zapatillas))
			return true;
		else
			return false;
	}
};

struct ComparaNodosA
{
	bool operator()(const nodoA &a, const nodoA &n) const
	{
		return ComparaEstados()(a.actual.st, n.actual.st);
	}
};

// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Implementación de la busqueda en anchura.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan)
{

	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	queue<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);

				if (hijoForward.st.fila == destino.fila && hijoForward.st.columna == destino.columna)
				{
					current = hijoForward;
					break;
				}
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.front();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Implementación de la busqueda A* con functores
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_AStar(const estado &origen, list <estado> &destino, list<Action> &plan,int (*ptr_func)(nodoA& uno, const estado& dos))
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	unordered_map<estado, nodoA, MyHash<estado>> Cerrados;
	priority_queue<nodoA, vector<nodoA>, mycomparison> Abiertos; // Lista de Abiertos

	nodoA current;
	current.actual.st = origen;
	current.actual.secuencia.empty();

	for(auto it = destino.begin(); it != destino.end() ;it++)
	{
		current.g = 0;
		current.f = 0;
		current.h = ptr_func(current, *it);

		Abiertos.push(current);
	}
	

	// coste asociado a la transicion por nodos
	int costeF, costeTURN, costeSEMITURN;
	while (!Abiertos.empty() and current.h == 0)
	{

		Abiertos.pop();

		if (Cerrados.find(current.actual.st) == Cerrados.end())
		{

			Cerrados.insert(pair<estado, nodoA>(current.actual.st, current)); // Insertamos en el unordered
			switch (mapaResultado[current.actual.st.fila][current.actual.st.columna])
			{
			case 'A':
				if (current.actual.st.bikini)
				{
					costeF = 10;
					costeTURN = 5;
					costeSEMITURN = 2;
				}
				else
				{
					costeF = 200;
					costeTURN = 500;
					costeSEMITURN = 300;
				}
				break;

			case 'B':
				if (current.actual.st.zapatillas)
				{
					costeF = 15;
					costeTURN = 1;
					costeSEMITURN = 1;
				}
				else
				{
					costeF = 100;
					costeTURN = 3;
					costeSEMITURN = 2;
				}
				break;

			case 'T':
				costeF = 2;
				costeTURN = 2;
				costeSEMITURN = 1;
				break;
			case 'M':
				costeF = 2000;
				costeTURN = 2000;
				costeSEMITURN = 1000;
				break;

			case 'P':
				costeF = 2000;
				costeTURN = 2000;
				costeSEMITURN = 1000;
				break;
			default:
				costeF = 1;
				costeTURN = 1;
				costeSEMITURN = 1;
				break;
			}

			std::unordered_map<estado, nodoA, MyHash<estado>>::iterator iterator; // creamos el iterator 
			// Generar descendiente de girar a la derecha 90 grados
			for(auto it = destino.begin(); it != destino.end() ;it++)
			{
				nodoA hijoTurnR = current;
				hijoTurnR.actual.st.orientacion = (hijoTurnR.actual.st.orientacion + 2) % 8;

				hijoTurnR.actual.secuencia.push_back(actTURN_R);

				hijoTurnR.g = current.g + costeTURN;
				hijoTurnR.h = ptr_func(hijoTurnR, *it);
				hijoTurnR.f = hijoTurnR.g + hijoTurnR.h;

				iterator = Cerrados.find(hijoTurnR.actual.st);

				if ( iterator == Cerrados.end())
					Abiertos.push(hijoTurnR);
				else if ((iterator->second.g) > hijoTurnR.g)
				{
					Cerrados.erase(iterator);
					Abiertos.push(hijoTurnR);
				}

				// Generar descendiente de girar a la derecha 45 grados
				nodoA hijoSEMITurnR = current;
				hijoSEMITurnR.actual.st.orientacion = (hijoSEMITurnR.actual.st.orientacion + 1) % 8;

				hijoSEMITurnR.actual.secuencia.push_back(actSEMITURN_R);

				hijoSEMITurnR.g = current.g + costeSEMITURN;
				hijoSEMITurnR.h = ptr_func(hijoSEMITurnR, *it);
				hijoSEMITurnR.f = hijoSEMITurnR.g + hijoSEMITurnR.h;

				iterator = Cerrados.find(hijoSEMITurnR.actual.st);
				if ( iterator == Cerrados.end())
					Abiertos.push(hijoSEMITurnR);
				else if ((iterator->second.g) > hijoSEMITurnR.g)
				{
					Cerrados.erase(iterator);
					Abiertos.push(hijoSEMITurnR);
				}
				
				

				// Generar descendiente de girar a la izquierda 90 grados
				nodoA hijoTurnL = current;
				hijoTurnL.actual.st.orientacion = (hijoTurnL.actual.st.orientacion + 6) % 8;
				hijoTurnL.actual.secuencia.push_back(actTURN_L);

				hijoTurnL.g = current.g + costeTURN;
				hijoTurnL.h = ptr_func(hijoTurnL, *it);
				hijoTurnL.f = hijoTurnL.g + hijoTurnL.h;
				
				iterator = Cerrados.find(hijoTurnL.actual.st);
				if ( iterator == Cerrados.end())
				{
					Abiertos.push(hijoTurnL);
				}
				else if ((iterator->second.g) > hijoTurnL.g)
				{
					Cerrados.erase(iterator);
					Abiertos.push(hijoTurnL);
				}
				

				// Generar descendiente de girar a la izquierda 45 grados
				nodoA hijoSEMITurnL = current;
				hijoSEMITurnL.actual.st.orientacion = (hijoSEMITurnL.actual.st.orientacion + 7) % 8;

				hijoSEMITurnL.actual.secuencia.push_back(actSEMITURN_L);

				hijoSEMITurnL.g = current.g + costeSEMITURN;
				hijoSEMITurnL.h = ptr_func(hijoSEMITurnL, *it);
				hijoSEMITurnL.f = hijoSEMITurnL.g + hijoSEMITurnL.h;

				iterator = Cerrados.find(hijoSEMITurnL.actual.st);
				if ( iterator == Cerrados.end())
					Abiertos.push(hijoSEMITurnL);
				else if ((iterator->second.g) > hijoSEMITurnL.g)
				{
					Cerrados.erase(iterator);
					Abiertos.push(hijoSEMITurnL);
				}
				

				// Generar descendiente de avanzar
				nodoA hijoForward = current;
				if (!HayObstaculoDelante(hijoForward.actual.st))
				{
					hijoForward.actual.secuencia.push_back(actFORWARD);

					hijoForward.g = current.g + costeF;
					hijoForward.h = ptr_func(hijoForward, *it);
					hijoForward.f = hijoForward.g + hijoForward.h;

					iterator = Cerrados.find(hijoForward.actual.st);
					if ( iterator == Cerrados.end())
						Abiertos.push(hijoForward);
					else if ((iterator->second.g) > hijoForward.g)
					{
							Cerrados.erase(iterator);
							Abiertos.push(hijoForward);
					}
					
				}
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
			current = Abiertos.top();
	}

	cout << "Terminada la busqueda. " << endl;

	if (current.h == 0)
	{
		cout << "Cargando el plan\n";
		plan = current.actual.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}




// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actSEMITURN_R)
		{
			cout << "d ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else if (*it == actSEMITURN_L)
		{
			cout << "i "; // giro pequenio de 45grados
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.fila--;
				cst.columna++;
				break;
			case 2:
				cst.columna++;
				break;
			case 3:
				cst.fila++;
				cst.columna++;
				break;
			case 4:
				cst.fila++;
				break;
			case 5:
				cst.fila++;
				cst.columna--;
				break;
			case 6:
				cst.columna--;
				break;
			case 7:
				cst.fila--;
				cst.columna--;
				break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion + 2) % 8;
		}
		else if (*it == actSEMITURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 8;
		}
		else if (*it == actTURN_L)
		{
			cst.orientacion = (cst.orientacion + 6) % 8;
		}
		else if (*it == actSEMITURN_L)
		{
			cst.orientacion = (cst.orientacion + 7) % 8;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

// con esta funcion comprobamos si una posicion es visitable
// teniendo en cuenta si hemos visto anteriormente una casilla de bikini o de zapatillas
bool ComportamientoJugador::esVisitable(estado & posicion)
{
	
	if(mapaResultado[posicion.fila][posicion.columna] == 'M' || mapaResultado[posicion.fila][posicion.columna] == 'P')
		return false;
	
	if(!casilla_bikini && mapaResultado[posicion.fila][posicion.columna] == 'A')
		return false;

	if(!casilla_zapatillas && mapaResultado[posicion.fila][posicion.columna] == 'B')
		return false;

	return true;
}

// sobrecarga del de arriba
bool ComportamientoJugador::esVisitable(const char Casilla)
{
	if(Casilla == 'M' || Casilla == 'P')
		return false;
	
	if(!casilla_bikini &&Casilla == 'A')
		return false;

	if(!casilla_zapatillas && Casilla == 'B')
		return false;

	return true;
}
