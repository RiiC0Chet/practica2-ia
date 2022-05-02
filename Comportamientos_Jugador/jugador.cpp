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

	if (hay_plan && plan.size() > 0)
	{
		accion = plan.front();
		plan.pop_front();
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
		return pathFinding_AStar(origen, un_objetivo, plan);
		cout << "No implementado aun\n";
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 1
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
			cout<<"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"<<endl;
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
					// cout<<"SALIMOS AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<endl;
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


// Implementación de la busqueda A*
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_AStar(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	unordered_map<estado,nodoA,MyHash<estado>> Cerrados;							   // Lista de Cerrados
																		   // using mycomparison:
	priority_queue<nodoA, vector<nodoA>, mycomparison> Abiertos; // Lista de Abiertos

	nodoA current;
	current.actual.st = origen;
	current.actual.secuencia.empty();

	// current.padre = nullptr;
	current.g = 0;
	current.f = 0;
	current.h = ChebyshevDistance(current, destino);

	Abiertos.push(current);

	// coste asociado a la transicion por nodos
	int costeF, costeTURN, costeSEMITURN;

	while (!Abiertos.empty() and (current.actual.st.fila != destino.fila or current.actual.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados[current.actual.st]=current;//Insertamos en el unordered
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
			// cout<<endl;
			// cout<<"A"<<endl;
			// cout<<endl;
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
			// cout<<endl;
			// cout<<"B"<<endl;
			// cout<<endl;
			break;

		case 'T':
			costeF = 2;
			costeTURN = 2;
			costeSEMITURN = 1;
			// cout<<endl;
			// cout<<"T";//<<endl;
			// cout<<endl;
			break;

		default:
			costeF = 1;
			costeTURN = 1;
			costeSEMITURN = 1;
			// cout<<endl;
			// cout<<"DEFAULT"<<endl;
			// cout<<endl;
			break;
		}

		// Generar descendiente de girar a la derecha 90 grados
		nodoA hijoTurnR = current;
		hijoTurnR.actual.st.orientacion = (hijoTurnR.actual.st.orientacion + 2) % 8;

		hijoTurnR.actual.secuencia.push_back(actTURN_R);
		// hijoTurnR.actual.secuencia.push_back(actFORWARD);

		hijoTurnR.g = current.g + costeTURN; // + costeF;
		hijoTurnR.h = ChebyshevDistance(hijoTurnR, destino);
		hijoTurnR.f = hijoTurnR.g + hijoTurnR.h;

		if (Cerrados.find(hijoTurnR.actual.st) == Cerrados.end())
		{
			Abiertos.push(hijoTurnR);
		}
		else
		{
			//cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(Cerrados.find(hijoTurnR.actual.st)->second.g)<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(current.g + costeTURN)<<endl;
			if ((Cerrados.find(hijoTurnR.actual.st)->second.g) > hijoTurnR.g) // + costeF) )
			{
				cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
				Cerrados.erase(hijoTurnR.actual.st);

				Abiertos.push(hijoTurnR);
			}
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodoA hijoSEMITurnR = current;
		hijoSEMITurnR.actual.st.orientacion = (hijoSEMITurnR.actual.st.orientacion + 1) % 8;

		hijoSEMITurnR.actual.secuencia.push_back(actSEMITURN_R);
		// hijoSEMITurnR.actual.secuencia.push_back(actFORWARD);

		hijoSEMITurnR.g = current.g + costeSEMITURN; // + costeF;
		hijoSEMITurnR.h = ChebyshevDistance(hijoSEMITurnR, destino);
		hijoSEMITurnR.f = hijoSEMITurnR.g + hijoSEMITurnR.h;

		if (Cerrados.find(hijoSEMITurnR.actual.st) == Cerrados.end())
		{
			Abiertos.push(hijoSEMITurnR);
		}
		else
		{
			//cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(Cerrados.find(hijoSEMITurnR.actual.st)->second.g)<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(current.g + costeSEMITURN)<<endl;
			if ((Cerrados.find(hijoSEMITurnR.actual.st)->second.g) > hijoSEMITurnR.g) // + costeF) )
			{
				cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
				Cerrados.erase(hijoSEMITurnR.actual.st);

				Abiertos.push(hijoSEMITurnR);
			}
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodoA hijoTurnL = current;
		hijoTurnL.actual.st.orientacion = (hijoTurnL.actual.st.orientacion + 6) % 8;
		hijoTurnL.actual.secuencia.push_back(actTURN_L);
		// hijoTurnL.actual.secuencia.push_back(actFORWARD);

		hijoTurnL.g = current.g + costeTURN; // + costeF;
		hijoTurnL.h = ChebyshevDistance(hijoTurnL, destino);
		hijoTurnL.f = hijoTurnL.g + hijoTurnL.h;

		if (Cerrados.find(hijoTurnL.actual.st) == Cerrados.end())
		{
			Abiertos.push(hijoTurnL);
		}
		else
		{
			//cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(Cerrados.find(hijoTurnL.actual.st)->second.g)<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(current.g + costeTURN)<<endl;
			if ((Cerrados.find(hijoTurnL.actual.st)->second.g) > hijoTurnL.g) // + costeF) )
			{
				cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
				Cerrados.erase(hijoTurnL.actual.st);

				Abiertos.push(hijoTurnL);
			}
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodoA hijoSEMITurnL = current;
		hijoSEMITurnL.actual.st.orientacion = (hijoSEMITurnL.actual.st.orientacion + 7) % 8;

		hijoSEMITurnL.actual.secuencia.push_back(actSEMITURN_L);
		// hijoSEMITurnL.actual.secuencia.push_back(actFORWARD);

		hijoSEMITurnL.g = current.g + costeSEMITURN; //+ costeF;
		hijoSEMITurnL.h = ChebyshevDistance(hijoSEMITurnL, destino);
		hijoSEMITurnL.f = hijoSEMITurnL.g + hijoSEMITurnL.h;

		if (Cerrados.find(hijoSEMITurnL.actual.st) == Cerrados.end())
		{
			Abiertos.push(hijoSEMITurnL);
		}
		else
		{
			//cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(Cerrados.find(hijoSEMITurnL.actual.st)->second.g)<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(current.g + costeSEMITURN)<<endl;
			if ((Cerrados.find(hijoSEMITurnL.actual.st)->second.g) > hijoSEMITurnL.g) // + costeF) )
			{
				cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
				Cerrados.erase(hijoSEMITurnL.actual.st);

				Abiertos.push(hijoSEMITurnL);
			}
		}

		// Generar descendiente de avanzar
		nodoA hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.actual.st))
		{
			hijoForward.actual.secuencia.push_back(actFORWARD);

			hijoForward.g = current.g + costeF;
			hijoForward.h = ChebyshevDistance(hijoForward, destino);
			hijoForward.f = hijoForward.g + hijoForward.h;

			if (Cerrados.find(hijoForward.actual.st) == Cerrados.end())
			{
				Abiertos.push(hijoForward);
			}
			else
			{
				if(Cerrados.find(hijoForward.actual.st)->first.fila == 19 && Cerrados.find(hijoForward.actual.st)->first.columna == 19 && Cerrados.find(hijoForward.actual.st)->first.orientacion == 0 )
					cout<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(Cerrados.find(hijoForward.actual.st)->second.g)<<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<(current.g + costeF)<<endl;
				if ((Cerrados.find(hijoForward.actual.st)->second.g) > hijoForward.g)
				{
					cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
					Cerrados.erase(hijoForward.actual.st);
					Abiertos.push(hijoForward);
				}
			}

			
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();

			// cout<<"El proximo nodo a coger es: "<<current.actual.st.fila<<" "<<current.actual.st.columna<<endl;
		}
	}

	cout << "Terminada la busqueda. "
		 << "El nodo actual tiene bikini : " << current.actual.st.bikini << " Y zapatillas : " << current.actual.st.zapatillas << "\n";

	if (current.actual.st.fila == destino.fila and current.actual.st.columna == destino.columna)
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
