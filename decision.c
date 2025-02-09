// decision.c

#include "decision.h"
#include <stdio.h>

static Config *global_config; // Puntatore alla configurazione globale

bool decision_init(Config *config) {
    global_config = config;
    return true;
}

void decision_make_decisions(ProcessInfo *processes, int num_processes) {
    if (!global_config) {
        fprintf(stderr, "Errore: Modulo decisionale non inizializzato.\n");
        return;
    }

    for (int i = 0; i < num_processes; i++) {
        // Se il processo è già in pausa, controlla se deve essere riattivato
        if (processes[i].is_paused) {
            // Riattiva se:
            // 1. È una finestra GUI ed è tornata attiva
            // 2. L'utilizzo della CPU è sceso sotto la soglia
            //3. oppure se la sua priorità è maggiore della soglia
            if ((processes[i].is_gui && processes[i].is_active) ||
                (processes[i].cpu_usage < global_config->cpu_threshold) || processes[i].priority > global_config->priority_threshold)
            {
                processes[i].is_paused = false;
            }
            //Aggiungere il controllo sull'utilizzo della ram
             if (global_config->use_ram && (processes[i].mem_usage < global_config->mem_threshold)) {
                 processes[i].is_paused = false;
             }

        } else { // Se il processo non è in pausa, controlla se deve essere messo in pausa
            // Metti in pausa se:
            // 1. Non è una finestra GUI attiva
            // 2. E l'utilizzo della CPU supera la soglia
            // 3. E ha una priorità inferiore alla soglia
            if ((!processes[i].is_gui || !processes[i].is_active) &&
                processes[i].cpu_usage > global_config->cpu_threshold && processes[i].priority <= global_config->priority_threshold)
            {
                 processes[i].is_paused = true;
            }

            //Aggiungere il controllo sull'utilizzo della ram, si mette in pausa sono se use_ram è true
             if (global_config->use_ram && (processes[i].mem_usage > global_config->mem_threshold)
                && processes[i].priority <= global_config->priority_threshold) {
                 processes[i].is_paused = true;
             }
        }
    }
}

void decision_cleanup() {
    // Nessuna risorsa da liberare in questo esempio semplice
}
