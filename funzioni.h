#ifndef __funzioni_h__
#define __funzioni_h__

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>


//Librerie di ROOT
#include "TH1F.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TH2.h"
#include "TF1.h"

using namespace std;
struct Evento{
    int eventID;
    int trackID;
    string timestamp;
    double charge;
    double ux, uy, uz;
    double entry_x, entry_y, entry_z;
    double entry_time;
    double exit_x, exit_y, exit_z;
    double exit_time;
    double quality;
    double distance;
};

vector<Evento> load_data(const string&);
vector<Evento> load_good_data(const string&);
void print_all_data(const vector<Evento>&);
double efficiency(const vector<Evento>&);
double calculate_distance(const Evento&);








#endif //__funzioni_h__