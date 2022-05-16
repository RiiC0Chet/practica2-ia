#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <unordered_map>
#include <stdlib.h>
#include <list>

template <class T>
inline void hash_combine(std::size_t & s, const T & v)
{
  std::hash<T> h;
  s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
}

struct estado {
  int fila;
  int columna;
  int orientacion;
  bool bikini = false,
       zapatillas = false;

  bool operator==(const estado& uno) const
  {
    return (fila == uno.fila && columna == uno.columna && orientacion == uno.orientacion);// && bikini == uno.bikini && zapatillas == uno.zapatillas) 
  }
};

template <class T>
class MyHash;

template<>
struct MyHash<estado>
{
    std::size_t operator()(estado const& s) const 
    {
        std::size_t res = 0;
       hash_combine(res,s.fila);
       hash_combine(res,s.columna);
       hash_combine(res,s.orientacion);
       hash_combine(res,s.bikini);
       hash_combine(res,s.zapatillas);

        return res;
    }
};


struct nodo
{
	estado st;
	list<Action> secuencia;
};

// struct de los nodos A*
struct nodoA
{
  //nodo* padre;
  nodo actual;
  int g,h;
  int f; // calculo de la heuristica

  bool operator==(const nodoA& uno) const
  {
    return actual.st==(uno.actual.st);
  }

};


// clase de ordenacion para la cola de prioridad de A*
class mycomparison
{
  bool reverse; // true ordena de menor a mayor
public:
  inline mycomparison(const bool& revparam=true)
    {reverse=revparam;}
  inline bool operator() (const nodoA& lhs, const nodoA& rhs) const
  {
    if (reverse) return (lhs.f > rhs.f);
    else return (lhs.f < rhs.f);
  }
};




    // sobrecarga distancia Chebyshev teniendo en cuenta las diagonales
    inline int ChebyshevDistance(nodoA& uno, const estado& dos)
    {
      return abs(uno.actual.st.fila-dos.fila) > abs(uno.actual.st.columna-dos.columna) ? abs(uno.actual.st.fila-dos.fila) : abs(uno.actual.st.columna-dos.columna);
    }

    // distancia manhattan
    inline int ManhattanDistance(nodoA& uno, const estado& dos)
    {
      return abs(uno.actual.st.fila-dos.fila) + abs(uno.actual.st.columna-dos.columna);
    }


class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      hay_plan = false;
      casilla_zapatillas = false;
      casilla_bikini = false;
      hay_destinos = false;
      primer_obstaculo = true;
      tengo_bikini = false;
      tengo_zapatillas = false;
      primera_creacion_destinos = true;
      spawn = true;
      spawn_2 = true;
      encontrada = 0;
      num_intentos = 0;
      fila_aux = 0;
      columna_aux = 0;
      generar_objetivos=3;
      replanificaciones =0;
      num_replanificaciones = (mapaResultado.size()/2)+10;
      //bikini = false;
      //zapatillas = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hay_plan = false;
      casilla_zapatillas = false;
      casilla_bikini = false;
      hay_destinos = false;
      primer_obstaculo = true;
      tengo_bikini = false;
      tengo_zapatillas = false;
      primera_creacion_destinos = true;
      spawn = true;
      spawn_2 = true;
      encontrada = 0;
      num_intentos = 0;
      fila_aux = 0;
      columna_aux = 0;
      generar_objetivos=3;
      replanificaciones =0;
      num_replanificaciones = (mapaResultado.size()/2)+10;

      //bikini = false;
      //zapatillas = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

    // comprobamos si un extado es visitable o no
    bool esVisitable(estado & posicion);
    bool esVisitable(const char Casilla);

  private:
    // Declarar Variables de Estado
    Action ultimaAccion;

    estado actual;
    list<estado> objetivos;
    list<Action> plan;
    list<estado> destinos;
    list<estado> aux; // auxiliar para el nivel 3

    estado ultimo; // ultimo estado que se ha calculado el plan

    int num_intentos, // numero de intentos por estado repetido
        fila_aux,
        columna_aux,
        encontrada, // auxiliar para lidiar conn la casillaX 
        generar_objetivos, //nivel 4
        replanificaciones;

    int num_replanificaciones ;

    bool hay_plan, // comprobamos si hay plan establecido
         hay_destinos, // comprbamos si tenemos los detinos hechos
        casilla_zapatillas, // comprbamos si hemos visto casilla de zapatillas
        casilla_bikini, // comprbamos si hemos visto casilla de bikini
        primer_obstaculo, // comprobamos si es el primer obstaculo (agua o bosque)
        tengo_bikini,
        tengo_zapatillas,
        primera_creacion_destinos,
        spawn,
        spawn_2;

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_AStar(const estado &origen, const list <estado> &destino, list<Action> &plan,int (*ptr_func)(nodoA& uno, const estado& dos));


    
    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);

};

#endif
