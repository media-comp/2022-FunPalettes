#ifndef ACOPALETTE_SORT_H_
#define ACOPALETTE_SORT_H_

#include <vector>

#include "data_type.h"
#include "color_point.h"
#include "d2array.hpp"

void ACOpaletteSort(std::vector<ColorPoint>& list, d2array<SCALAR>& dismap);

class ACO {
public:
  ACO (int nAnts, int nColors, 
    double alpha, double beta, double q, double ro, double taumax,
    int initColors);
  virtual ~ACO ();
  
  void init ();
  
  void connectCOLORS (int colori, int colorj);
  void setCITYPOSITION (int city, double x, double y);
  
  void printPHEROMONES ();
  void printGRAPH ();
  void printRESULTS ();
  
  void optimize (int ITERATIONS);

  int *BESTROUTE;

private:
  double distance (int colori, int colorj);
  bool exists (int colori, int colorc);
  bool visited (int antk, int c);
  double PHI (int colori, int colorj, int antk);
  
  double length (int antk);
  
  int color ();
  void route (int antk);
  int valid (int antk, int iteration);
  
  void updatePHEROMONES ();

  
  int NUMBEROFANTS, NUMBEROFCOLORS, INITIALCOLOR;
  double ALPHA, BETA, Q, RO, TAUMAX;
  
  double BESTLENGTH;

  int **GRAPH, **ROUTES;
  double **COLORS, **PHEROMONES, **DELTAPHEROMONES, **PROBS;
  double **DISTANCES;

};

#endif  // ACOPALETTE_SORT_H_
