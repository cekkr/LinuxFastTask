// control.h

#ifndef CONTROL_H
#define CONTROL_H

#include "config.h" // Includi le definizioni comuni
#include <stdbool.h>

// Funzione per inizializzare il modulo di controllo
bool control_init();

// Funzione per applicare le decisioni prese (mettere in pausa/riattivare i processi)
void control_apply_decisions(ProcessInfo *processes, int num_processes);

// Funzione per liberare le risorse allocate dal modulo di controllo
void control_cleanup();

#endif
