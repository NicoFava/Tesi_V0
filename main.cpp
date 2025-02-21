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
    TH1F charge("Distribuzione del valore della carica", "Distribuzione del valore della carica", 100, 100, 100);
    charge.StatOverflows(kTRUE);
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_file.txt>" << endl;
        return 1;
    }
    string filename = argv[1];
    vector<Evento> eventi = load_data(filename);
    for(const auto& e: eventi){
        charge.Fill(e.charge);
    }
    TCanvas can("Distribuzione del valore della carica", "Distribuzione del calore della carica");
    can.cd();
    charge.SetTitle("Distribuzione del valore della carica");
    charge.GetXaxis()->SetTitle("Charge [p.e.]");
    charge.GetYaxis()->SetTitle("Conteggi [a.u.]");
    print_all_data(eventi);
    charge.Draw();
    double efficienza = efficiency(eventi)*100;
    cout << "L'efficienza di rivelazione è del " << efficienza <<"%." << endl; 
    app.Run();
    return 0;

}
