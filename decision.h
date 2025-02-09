// decision.h

#ifndef DECISION_H
#define DECISION_H

#include "config.h" // Includi le definizioni comuni
#include <stdbool.h>

// Funzione per inizializzare il modulo decisionale
bool decision_init(Config *config);

// Funzione per prendere decisioni sui processi
// I processi passati come parametro sono quelli ottenuti dal modulo di monitoraggio.
// La funzione modifica direttamente il campo 'is_paused' dei processi
// in base alle decisioni prese.
void decision_make_decisions(ProcessInfo *processes, int num_processes);

// Funzione per liberare le risorse allocate dal modulo decisionale (se necessario)
void decision_cleanup();

#endif
