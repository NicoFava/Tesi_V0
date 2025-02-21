#include "funzioni.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

int main(int argc, char* argv[]) {
    TApplication app("app", 0, 0);
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_file.txt>" << endl;
        return 1;
    }
    string filename = argv[1];
    vector<Evento> eventi = load_data(filename);
    vector<Evento> good_eventi = load_good_data(filename);
    TH1F Zenith("Distribuzione dei Punti di Entrata vs Angolo di Zenith", "Distribuzione dei Punti di Entrata vs Angolo di Zenith", 100, 100, 100);
    Zenith.StatOverflows(kTRUE);
    TH1F charge("Distribuzione del valore della carica", "Distribuzione del valore della carica", 100, 100, 100);
    charge.StatOverflows(kTRUE);
    for(const auto& e: eventi){
        charge.Fill(e.charge);
    }
    for(const auto& e:good_eventi){
        // Calcolo della distanza radiale dal centro del rivelatore
        double r = sqrt(e.entry_x * e.entry_x + e.entry_y * e.entry_y + e.entry_z * e.entry_z);
    
        // Evitiamo errori di divisione per zero
        if (r == 0) continue;

        // Calcolo dell'angolo di zenit
        double theta = acos(e.entry_z / r) * 180.0 / M_PI;
        Zenith.Fill(theta);
    }
    TCanvas can("Distribuzione del valore della carica", "Distribuzione del calore della carica");
    can.cd();
    charge.SetTitle("Distribuzione del valore della carica");
    charge.GetXaxis()->SetTitle("Charge [p.e.]");
    charge.GetYaxis()->SetTitle("Conteggi [a.u.]");
    charge.Draw();
    TCanvas canz("Distribuzione dei Punti di Entrata vs Angolo di Zenith", "Distribuzione dei Punti di Entrata vs Angolo di Zenith" );
    canz.cd();
    Zenith.SetTitle("Punti di entrata vs Zenith");
    Zenith.GetXaxis()->SetTitle("Angolo [°]");
    Zenith.GetYaxis()->SetTitle("Conteggi [a.u.]");
    Zenith.Draw();
    print_all_data(good_eventi);
    double efficienza = efficiency(eventi)*100;
    cout << "L'efficienza di rivelazione è del " << efficienza <<"%." << endl; 
    app.Run();
    return 0;

}
