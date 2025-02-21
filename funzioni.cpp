#include "funzioni.h"

vector<Evento>load_data(const string& filename){
    ifstream file(filename); // Apri il file con nome passato da terminale
    if (!file.is_open()) {
        cerr << "Errore nell'apertura del file." << endl;
        return {}; //Ritorna un vettore vuoto in caso di errore
    }


    vector<Evento> eventi;
    string line;

    while (getline(file, line)) {  // Legge ogni riga
        stringstream ss(line);
        Evento e;
        string temp;

        getline(ss, temp, '\t'); e.eventID = stoi(temp);
        getline(ss, temp, '\t'); e.trackID = stoi(temp);
        getline(ss, e.timestamp, '\t'); // Timestamp è una stringa
        getline(ss, temp, '\t'); e.charge = stod(temp);
        getline(ss, temp, '\t'); e.ux = stod(temp);
        getline(ss, temp, '\t'); e.uy = stod(temp);
        getline(ss, temp, '\t'); e.uz = stod(temp);
        getline(ss, temp, '\t'); e.entry_x = stod(temp);
        getline(ss, temp, '\t'); e.entry_y = stod(temp);
        getline(ss, temp, '\t'); e.entry_z = stod(temp);
        getline(ss, temp, '\t'); e.entry_time = stod(temp);
        getline(ss, temp, '\t'); e.exit_x = stod(temp);
        getline(ss, temp, '\t'); e.exit_y = stod(temp);
        getline(ss, temp, '\t'); e.exit_z = stod(temp);
        getline(ss, temp, '\t'); e.exit_time = stod(temp);
        getline(ss, temp, '\t'); e.quality = stod(temp);
        e.distance = calculate_distance(e);


        eventi.push_back(e); // Aggiunge l'evento al vettore
    }

    file.close();
    return eventi;

}

vector<Evento> load_good_data(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore nell'apertura del file " << filename << endl;
        return {};  // Ritorna un vettore vuoto in caso di errore
    }

    vector<Evento> eventi_validi;
    string line;

    while (getline(file, line)) {  // Legge ogni riga del file
        stringstream ss(line);
        Evento e;
        string temp;
            getline(ss, temp, '\t'); e.eventID = stoi(temp);
            getline(ss, temp, '\t'); e.trackID = stoi(temp);
            getline(ss, e.timestamp, '\t'); // Timestamp è una stringa
            getline(ss, temp, '\t'); e.charge = stod(temp);
            getline(ss, temp, '\t'); e.ux = stod(temp);
            getline(ss, temp, '\t'); e.uy = stod(temp);
            getline(ss, temp, '\t'); e.uz = stod(temp);
            getline(ss, temp, '\t'); e.entry_x = stod(temp);
            getline(ss, temp, '\t'); e.entry_y = stod(temp);
            getline(ss, temp, '\t'); e.entry_z = stod(temp);
            getline(ss, temp, '\t'); e.entry_time = stod(temp);
            getline(ss, temp, '\t'); e.exit_x = stod(temp);
            getline(ss, temp, '\t'); e.exit_y = stod(temp);
            getline(ss, temp, '\t'); e.exit_z = stod(temp);
            getline(ss, temp, '\t'); e.exit_time = stod(temp);
            getline(ss, temp, '\t'); e.quality = stod(temp);
            e.distance = calculate_distance(e);

            // FILTRIAMO SOLO GLI EVENTI VALIDi (ENTRATA e USCITA DEFINITE)
            if (e.exit_x != -999999 && e.exit_y != -999999 && e.exit_z != -999999) {
                eventi_validi.push_back(e);
            }
    }

    file.close();
    return eventi_validi;
}


void print_all_data(const vector<Evento>& eventi){
    //Stampo tutti gli eventi per verifica
    cout << "\n=== TUTTI "<< eventi.size() <<" EVENTI CARICATI ===\n";
    for(int i = 0; i < eventi.size(); i++){
        cout << "Evento " << i << "\n";
        cout << "  Event ID: " << eventi[i].eventID << "\n";
        cout << "  Track ID: " << eventi[i].trackID << "\n";
        cout << "  Timestamp: " << eventi[i].timestamp << "\n";
        cout << "  Charge: " << eventi[i].charge << "\n";
        cout << "  Direzione (ux, uy, uz): (" << eventi[i].ux << ", " 
             << eventi[i].uy << ", " << eventi[i].uz << ")\n";
        cout << "  Entry Point (x, y, z): (" << eventi[i].entry_x << ", "
             << eventi[i].entry_y << ", " << eventi[i].entry_z << ")\n";
        cout << "  Entry Time: " << eventi[i].entry_time << " ns\n";
        cout << "  Exit Point (x, y, z): (" << eventi[i].exit_x << ", "
             << eventi[i].exit_y << ", " << eventi[i].exit_z << ")\n";
        cout << "  Exit Time: " << eventi[i].exit_time << " ns\n";
        cout << "  Quality: " << eventi[i].quality << "\n";
        cout << "  Distance: " << eventi[i].distance << "\n";
        cout << "-----------------------------\n";
    }
}

double efficiency(const vector<Evento>&eventi){
    int total_events = eventi.size();
    int valid_events = 0;
    
    for (const auto& e : eventi) {
        // Controlla se almeno una delle coordinate di exit è diversa da -999999
        if (e.exit_x != -999999 || e.exit_y != -999999 || e.exit_z != -999999) {
            valid_events++;
        }
    }
    
    // Se non ci sono eventi, la capacità è zero per evitare divisione per zero
    if (total_events == 0) return 0.0;
    
    // Calcola la capacità
    return static_cast<double>(valid_events) / total_events;
    
}

double calculate_distance(const Evento& e){
    // Se non ci sono dati validi per il punto di uscita, ritorna -1
    if (e.exit_x == -999999 || e.exit_y == -999999 || e.exit_z == -999999) {
        return -1.0;
    }

    // Calcola la distanza percorsa
    double dx = e.exit_x - e.entry_x;
    double dy = e.exit_y - e.entry_y;
    double dz = e.exit_z - e.entry_z;
    
    return sqrt(dx * dx + dy * dy + dz * dz);
}