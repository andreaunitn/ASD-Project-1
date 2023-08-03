//Progetto1 con tentativo di aggiunta calcolo studenti colpiti
//Progetto1-1 con aggiunta della funzione di decremento priorità per coda
//Progetto1-1-2 con coda di puntatori a citta
//Progetto1-1-3 con aggiunta di nuovo "grafo" con cammini minimi
//Progetto1-2 con una funzione DFS per contare nodi e struttura per cammini minimi
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

///////////////////////////////////////////////////////////////////////////////
//METODI E STRUTTURE PER ***priority_queue_min***
//
//Elemento della coda
struct citta {
  vector<int> vic; //citta vicine
  vector<int> g; //giorni di distanza dalla citta vicina corrispondente
  vector<int> figli; //nome (interi) dei figli
  vector<int> genitori;
  //bool visitabile = true
  bool visitato = false;
  bool visit = false;
  int value; //Nome della città (il suo numero)
  int pos; //La posizione della città nella coda di priorità
  unsigned int distP = 0-1; //distanza della città da Pogwarts
  int padre = 0; //nodo padre
  int pesoNodo = 1; //numero dei discendenti incluso il nodo
  int controllo;
};
//Struttura coda di priorita (min)
struct Pqueue {
  int capacity;
  int dim;
  vector<citta*> H;
};

Pqueue* PriorityQueue(const int &n);
void insert(Pqueue *q, citta* c);
int deleteMin(Pqueue *q);
int pMin(const Pqueue *q);
int fMin(const Pqueue *q);
void decrease(Pqueue *q, citta *x, int p);
int trovaMassimi();
void stampaCammino(int p);
//
//FINE METODI E STRUTTURE ***priority_queue_min***
////////////////////////////////////////////////////////////////////////////////

vector<citta> regione;
//vector<citta*> grafo_cammini;

void dijkstra(const int & P);
int dfs(const int &n);
int rec(const int &v, const int &V);

ofstream out("output.txt");
int P;
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
int main() {

  //Inizializza orologio
  auto start = high_resolution_clock::now();
  //leggo input
  int N,M;
  ifstream in("input.txt");
  in >> N >> M >> P;
  regione.resize(N);
  for(int i = 0; i < M; i++) {
    int from, to, days;
    in >> from >> to >> days;
    regione[from].vic.push_back(to);
    regione[from].g.push_back(days);
    regione[to].vic.push_back(from);
    regione[to].g.push_back(days);
    regione[from].value = from;
    regione[to].value = to;
  }
  regione[P].padre = -2;
  regione[P].distP = 0;

  dijkstra(P);
  dfs(P);
  regione[P].pesoNodo = 0;

  int cittaColpita = trovaMassimi();

  /*
  for(int i=0; i<regione.size(); i++) {
    cout << i << ": " << regione[i].distP << " padre: " << regione[i].padre << endl << " visite: " << regione[i].visite << endl;
  }*/
  //ofstream out("output.txt");
  //cout << "Numero studenti: " << maxStudenti(P,-3) << endl;

  out << regione[cittaColpita].pesoNodo << endl;
  stampaCammino(cittaColpita);

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Durata " << duration.count() << endl;
  return 0;
}
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN

//da visita_bfs.cpp di lab3 con priority_queue_min
void dijkstra(const int & P) {
  Pqueue *q = PriorityQueue(50000); //Numero scelto arbitrariamente
  for(int i=0; i<regione.size(); i++) {
    insert(q, &regione[i]);
    //cout << "Inserisco: " << regione[i].value << " con priorita " << regione[i].distP << endl;
  }
  /*for(int i=1; i<=q->dim; i++) {
    cout << q->H[i]->value << endl;
  }*/
  while(q->dim > 0) {
    int n = deleteMin(q);
    //cout << "Entro in " << n << " con priorita " << regione[n].distP << endl;
    //Visita tutti i vicini
    for(int i=0; i<regione[n].vic.size(); i++) {
      int v = regione[n].vic[i];
      //cout << "    guardo in " << v << endl;
      int alt = regione[n].distP + regione[n].g[i];
      if(alt < regione[v].distP) {
        regione[v].padre = n;
        //cout << "    aggiungo " << v << endl;
        decrease(q, &regione[v], alt);
      } else if(regione[v].distP == regione[n].distP - regione[n].g[i]) {
        regione[v].figli.push_back(n);
        regione[n].genitori.push_back(v);
        //cout << "Aggiungo a " << v << " il figlio " << n << endl;
      }
    }
  }
}

int dfs(const int & n) {
  regione[n].visitato = true;

  for(auto iter: regione[n].figli) {
    //int v = regione[n].figli[i];
    if(!regione[iter].visitato) {
      regione[n].pesoNodo += dfs(iter);
    }
  }

  if(regione[n].genitori.size() > 1) {
    int x = rec(n,n);
    regione[x].pesoNodo += regione[n].pesoNodo;
    if(x == P)
      regione[n].visit = true;
    return 0;
  }
  //cout << "PesoNodo " << n << " :" << regione[n].pesoNodo << endl;
  return regione[n].pesoNodo;
}

int rec(const int &v, const int &V) {
  for(auto iter: regione[v].genitori) {
    if(regione[iter].controllo == V) {
      return iter;
    }
    regione[iter].controllo = V;
    rec(iter, V);
  }
}

int trovaMassimi() {
  int massimo = 0;
  int cittaMassimo = 0;
  for(int i=0; i<regione.size(); i++) {
    if(regione[i].pesoNodo > massimo) {
      massimo = regione[i].pesoNodo;
      cittaMassimo = i;
    }
  }
  return cittaMassimo;
}

void stampaCammino(int p) {
  regione[p].visit = true;
  out << p << endl;
  for(auto iter: regione[p].figli) {
    //int v = regione[n].figli[i];
    if(!regione[iter].visit) {
      stampaCammino(iter);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//METODI PER priority_queue_min
//
  //Funzione ausiliaria priority_queue_min
  int l(int i) {
    return 2*i;
  }

  //Funzione ausiliaria priority_queue_main
  int r(int i) {
    return 2*i+1;
  }

  //Scambia H[i] con H[j]
  void swap(vector<citta*> &H, const int &i, const int &j) {
    citta* temp = H[i];
    H[i] = H[j];
    H[j] = temp;
    H[i]->pos = i;
    H[j]->pos = j;
  }

  //Inizializza la coda con dimensione massima n (capacity)
  //n = capacità della coda
  Pqueue* PriorityQueue(const int &n) {
    Pqueue *t = new Pqueue();
    t->capacity = n;
    t->dim = 0;
    t->H.resize(n);
    return t;
  }

  //Inserisce un elemento nella coda in base alla priorità
  //q = coda, x = valore, p = priorità, f = nodo padre
  void insert(Pqueue *q, citta* c) {
    if(q->dim < q->capacity) {
      q->dim++;
      q->H[q->dim] = c;
      q->H[q->dim]->pos = q->dim;
      int i = q->dim;

      while(i > 1 && q->H[i]->distP < q->H[i/2]->distP) {
        int p_i = i/2;
        swap(q->H,i,p_i);
        i = i/2;
      }
    }
  }

  //Funzione ausiliaria per riordinare gli elementi della coda nella heap
  //A = vettore della coda, i = indice, dim = dimensione attuale
  void minHeapRestore(vector<citta*> &A, const int &i, const int &dim) {
    int min1 = i;
    if(l(i) <= dim && A[l(i)]->distP < A[min1]->distP) {
      min1 = l(i);
    }
    if(r(i) <= dim && A[r(i)]->distP < A[min1]->distP) {
      min1 = r(i);
    }
    if(i != min1) {
      swap(A,i,min1);
      minHeapRestore(A,min1,dim);
    }
  }

  //Elimina l'elemento con priorità minore e lo ritorna
  // q = coda
  int deleteMin(Pqueue *q) {
    if (q->dim > 0) {
      swap(q->H, 1, q->dim);
      q->dim--;
      minHeapRestore(q->H, 1, q->dim);
      return q->H[q->dim+1]->value;
    }
    return -1; //(*)
  }

  //Ritorna la priorità del primo elemento in coda
  int pMin(const Pqueue *q) {
    if(q->dim > 0) {
      return q->H[1]->distP;
    }
    return -1; //(*)
  }

  //NEW!! Ritorna il padre del primo nodo in coda
  int fMin(const Pqueue *q) {
    if(q->dim > 0) {
      return q->H[1]->padre;
    }
    return -1; //(*)
  }

  void decrease(Pqueue *q, citta *x, int p) {
    if (p < x->distP) {
      x->distP = p;
      int i = x->pos;
      while(i > 1 && q->H[i]->distP < q->H[i/2]->distP) {
        int p_i = i/2;
        swap(q->H,i,p_i);
        i = i/2;
      }
    }
  }
//
//FINE METODI priority_queue_min
////////////////////////////////////////////////////////////////////////////////
