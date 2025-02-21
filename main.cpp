#include "funzioni.h"
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
    //dagli eventi veri voglio capire quanti bundle ci sono.
    int bundle = 0;
    string last_timestamp = "";  // Memorizza l'ultimo timestamp visto
    //conta i bundle
    for (const auto& e : good_eventi) {
        if (e.trackID > 0 && e.timestamp != last_timestamp) {
            last_timestamp = e.timestamp;  // Aggiorna il timestamp visto
            bundle++;  // Conta il bundle solo una volta per timestamp unico
        }
    }
    //conta gli eventi singoli (contando una singola volta i bundle)
    int muoni = 0;
    last_timestamp = "";
    for (const auto& e : good_eventi) {
        if ( e.timestamp != last_timestamp) {
            last_timestamp = e.timestamp; 
            muoni++;
        }
    }
    TCanvas can("Distribuzione del valore della carica", "Distribuzione del calore della carica");
    can.cd();
    charge.SetTitle("Distribuzione del valore della carica");
    charge.GetXaxis()->SetTitle("Charge [p.e.]");
    charge.GetYaxis()->SetTitle("Conteggi [a.u.]");
    charge.Draw();
    TCanvas canz("Distribuzione dei Punti di Entrata vs Angolo di Zenith", "Distribuzione dei Punti di Entrata vs Angolo di Zenith" );
    canz.cd();
    Zenith.SetStats(1);
    Zenith.SetLineColor(kBlue);
    Zenith.SetLineWidth(2);
    Zenith.SetFillColorAlpha(kBlue, 0.3);
    Zenith.SetTitle("Distribuzione dei Punti di Entrata vs Angolo Zenith");
    Zenith.GetXaxis()->SetTitle("Angolo #theta [#circ]");
    Zenith.GetYaxis()->SetTitle("Conteggi [a.u.]");
    Zenith.GetXaxis()->SetTitleSize(0.04);
    Zenith.GetYaxis()->SetTitleSize(0.04);
    Zenith.GetXaxis()->SetRangeUser(0, 180);
    gPad->SetGrid();

    TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->AddEntry(&Zenith, "Distribuzione Zenith", "l");
    leg->Draw();

    canz.SetLeftMargin(0.12);
    canz.SetBottomMargin(0.12);
    canz.SetGrid();
    //Zenith.SetTitle("Punti di entrata vs Zenith");
    //Zenith.GetXaxis()->SetTitle("Cos#theta");
    //Zenith.GetYaxis()->SetTitle("Conteggi [a.u.]");
    Zenith.Draw();
    print_all_data(good_eventi);
    double efficienza = efficiency(eventi)*100;
    cout << "L'efficienza di rivelazione è del " << efficienza <<"%." << endl;
    cout << "Il numero di muoni bundle (muoni per traccia>2) sono: " << bundle << " e rappresentano il " <<(double)bundle/muoni*100<< "\% degli eventi buoni (entrata e uscita ricostruita)" << endl;
    plot_3D_distribution(good_eventi);
    app.Run();
    return 0;
}