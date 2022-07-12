#include "aco_palette_sort.h"

#include <algorithm>
#include <random>
#include <iostream>

double get_random(){
  default_random_engine generator;
  uniform_real_distribution<double> distribution(0.0, 1.0);
  
  return distribution(generator);
}


ACO::ACO (int nAnts, int nColors, 
  double alpha, double beta, double q, double ro, double taumax,
  int initColor) {
  NUMBEROFANTS   = nAnts;
  NUMBEROFCOLORS   = nColors;
  ALPHA     = alpha;
  BETA     = beta;
  Q     = q;
  RO     = ro;
  TAUMAX     = taumax;
  INITIALCOLOR  = initColor; 
}
ACO::~ACO () {
  for(int i=0; i<NUMBEROFCOLORS; i++) {
    delete [] GRAPH[i];
    delete [] COLORS[i];
    delete [] PHEROMONES[i];
    delete [] DELTAPHEROMONES[i];
    delete [] DISTANCES[i];
    if(i < NUMBEROFCOLORS - 1) {
      delete [] PROBS[i];
    }
  }
  delete [] GRAPH;
  delete [] COLORS;
  delete [] PHEROMONES;
  delete [] DELTAPHEROMONES;
  delete [] DISTANCES;
  delete [] PROBS;
}

void ACO::init () {
  GRAPH     = new int*[NUMBEROFCOLORS];
  COLORS     = new double*[NUMBEROFCOLORS];
  PHEROMONES   = new double*[NUMBEROFCOLORS];
  DELTAPHEROMONES = new double*[NUMBEROFCOLORS];
  DISTANCES = new double*[NUMBEROFCOLORS];
  PROBS     = new double*[NUMBEROFCOLORS-1];
  
  for(int i=0; i<NUMBEROFCOLORS; i++) {
    GRAPH[i]     = new int[NUMBEROFCOLORS];
    COLORS[i]     = new double[2];
    PHEROMONES[i]   = new double[NUMBEROFCOLORS];
    DELTAPHEROMONES[i]   = new double[NUMBEROFCOLORS];
    DISTANCES[i] = new double[NUMBEROFCOLORS];
    PROBS[i]     = new double[2];
    for (int j=0; j<2; j++) {
      COLORS[i][j] = -1.0;
      PROBS[i][j]  = -1.0;
    }
    for (int j=0; j<NUMBEROFCOLORS; j++) {
      GRAPH[i][j]     = 0;
      PHEROMONES[i][j]   = 0.0;
      DELTAPHEROMONES[i][j]   = 0.0;
      DISTANCES[[i][j] = 0.0;
    }
  }  

  ROUTES = new int*[NUMBEROFANTS];
  for (int i=0; i<NUMBEROFANTS; i++) {
    ROUTES[i] = new int[NUMBEROFCOLORS];
    for (int j=0; j<NUMBEROFCOLORS; j++) {
      ROUTES[i][j] = -1;
    }
  }
  
  BESTLENGTH = (double) INT_MAX;
  BESTROUTE  = new int[NUMBEROFCOLORS];
  for (int i=0; i<NUMBEROFCOLORS; i++) {
    BESTROUTE[i] = -1;  
  }
}


void ACO::connectCOLORS (int colori, int colorj, double edgedistance) {
  GRAPH[colori][colorj] = 1;
  PHEROMONES[colori][colorj] = get_random() * TAUMAX;
  GRAPH[colorj][colori] = 1;
  PHEROMONES[colorj][colori] = PHEROMONES[colori][colorj];
  DISTANCES[colori][colorj] = DISTANCES[colorj][colori] = edgedistance;
}

bool ACO::visited (int antk, int c) {
  for (int l=0; l<NUMBEROFCOLORS; l++) {
    if (ROUTES[antk][l] == -1) {
      break;
    }
    if (ROUTES[antk][l] == c) {
      return true;
    }
  }
  return false;
}

double ACO::PHI (int colori, int colorj, int antk) {
  double ETAij = (double) pow (1 / DISTANCES[colori][colorj], BETA);
  double TAUij = (double) pow (PHEROMONES[colori][colorj],   ALPHA);

  double sum = 0.0;
  for (int c=0; c<NUMBEROFCOLORS; c++) {
    if (!visited(antk, c)) {
    double ETA = (double) pow (1 / DISTANCES[colori][c], BETA);
    double TAU = (double) pow (PHEROMONES[colori][c],   ALPHA);
    sum += ETA * TAU;
    }   
  }
  return (ETAij * TAUij) / sum;
}

double ACO::length (int antk) {
  double sum = 0.0;
  for (int j=0; j<NUMBEROFCOLORS-1; j++) {
    sum += DISTANCES[ROUTES[antk][j]][ROUTES[antk][j+1]];  
  }
  return sum;
}

int ACO::color () {
  double xi = get_random();
  int i = 0;
  double sum = PROBS[i][0];
  while (sum < xi) {
    i++;
    sum += PROBS[i][0];
  }
  return (int) PROBS[i][1];
}

void ACO::route (int antk) {
  ROUTES[antk][0] = INITIALCOLOR;
  for (int i=0; i<NUMBEROFCOLORS-1; i++) {  
    int colori = ROUTES[antk][i];
    int count = 0;
    for (int c=0; c<NUMBEROFCOLORS; c++) {
      if (colori == c) {
        continue;  
      }
      if (!visited (antk, c)) {
        PROBS[count][0] = PHI (colori, c, antk);
        PROBS[count][1] = (double) c;
        count++;
      }
    }
    // deadlock
    if (0 == count) {
      return;
    }
    ROUTES[antk][i+1] = color();
  }
}

void ACO::printRESULTS () {
  BESTLENGTH += DISTANCES[BESTROUTE[NUMBEROFCOLORS-1]][INITIALCOLOR];
  std::cout << " BEST ROUTE: " << endl;
  for (int i=0; i<NUMBEROFCOLORS; i++) {
    std::cout << BESTROUTE[i] << " ";
  }
  std::cout << endl << " MINIMUM DISTANCE: " << BESTLENGTH << endl;
}

void ACO::updatePHEROMONES () {
  for (int k=0; k<NUMBEROFANTS; k++) {
    double rlength = length(k);
    for (int r=0; r<NUMBEROFCOLORS-1; r++) {
      int colori = ROUTES[k][r];
      int colorj = ROUTES[k][r+1];
      DELTAPHEROMONES[colori][colorj] += Q / rlength;
      DELTAPHEROMONES[colorj][colori] += Q / rlength;
    }
  }
  for (int i=0; i<NUMBEROFCOLORS; i++) {
    for (int j=0; j<NUMBEROFCOLORS; j++) {
      PHEROMONES[i][j] = (1 - RO) * PHEROMONES[i][j] + DELTAPHEROMONES[i][j];
      DELTAPHEROMONES[i][j] = 0.0;
    }  
  }
}


void ACO::optimize (int ITERATIONS) {
  for (int iterations=1; iterations<=ITERATIONS; iterations++) {  
    for (int k=0; k<NUMBEROFANTS; k++) {
      
      route(k);
      
      double rlength = length(k);
      if (rlength < BESTLENGTH) {
        BESTLENGTH = rlength;
        for (int i=0; i<NUMBEROFCOLORS; i++) {
          BESTROUTE[i] = ROUTES[k][i];
        }
      }  
    }

  updatePHEROMONES ();
  
  for (int i=0; i<NUMBEROFANTS; i++) {
    for (int j=0; j<NUMBEROFCOLORS; j++) {
      ROUTES[i][j] = -1;
    }
  }
  }
}


void ACOpaletteSort(std::vector<ColorPoint>& list, d2array<SCALAR>& dismap) {

  int iterations = 20;                      //number of iterations
  int numberofcolors = list.size();         //number of colors
  int numberofants = (int)numberofcolors/2; //number of ants
  double alpha = 0.5;                       //control pheromone deposit
  double beta = 0.8;                        //control desirability of transition
  double q = 80;                            //constant
  double ro = 0.2;                          //pheromone evaporation rate
  int taumax = 2;                           //maximum pheromone
  int initialcolor = 0;                     //starting color
  
  ACO *ANTS = new ACO(numberofants, numberofcolors, alpha, beta, q, ro, taumax, initialcolor);

  ANTS -> init();
  
  for (int i = 0; i < numberofcolors; i++)
    for (int j = i; j < numberofcolors; j++) {
      if (i == j) {
        #dismap(i, j) = 0.0f;
        ANTS -> connectCOLORS(i, j, 0.0);
      } else {
        #dismap(i, j) = list[i].distanceE2000(list[j].lab());
        #dismap(j, i) = dismap(i, j);
        double edgedistance = (double) list[i].distanceE2000(list[j].lab());
        ANTS -> connectCOLORS(i, j, edgedistance);
      }
  }
    
  ANTS -> optimize(iterations);
  
  ANTS -> printRESULTS();

  std::vector<ColorPoint> tmp;
  tmp.resize(size);

  std::copy(list.begin(), list.end(), tmp.begin());
  list.clear();
  for (int i = 0; i < size; i++) list.emplace_back(tmp[ANTS -> BESTROUTE[i]]);
}
