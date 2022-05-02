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

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      hay_plan = false;
      //bikini = false;
      //zapatillas = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hay_plan = false;
      //bikini = false;
      //zapatillas = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    estado actual;
    list<estado> objetivos;
    list<Action> plan;

    bool hay_plan; // comprobamos si hay plan establecido

    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_AStar(const estado &origen, const estado &destino, list<Action> &plan);

    // distancia Chebyshev teniendo en cuenta las diagonales
    inline int ChebyshevDistance(nodoA& uno, nodoA& dos)
    {
      return abs(uno.actual.st.fila-dos.actual.st.fila) > abs(uno.actual.st.columna-dos.actual.st.columna) ? abs(uno.actual.st.fila-dos.actual.st.fila) : abs(uno.actual.st.columna-dos.actual.st.columna);
    }

    // sobrecarga distancia Chebyshev teniendo en cuenta las diagonales
    inline int ChebyshevDistance(nodoA& uno, const estado& dos)
    {
      return abs(uno.actual.st.fila-dos.fila) > abs(uno.actual.st.columna-dos.columna) ? abs(uno.actual.st.fila-dos.fila) : abs(uno.actual.st.columna-dos.columna);
    }

    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);

};

#endif
