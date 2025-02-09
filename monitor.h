// monitor.h

#ifndef MONITOR_H
#define MONITOR_H

#include "config.h" // Includi le definizioni comuni
#include <stdbool.h>

// Funzione per inizializzare il modulo di monitoraggio
bool monitor_init();

// Funzione per ottenere le informazioni sui processi
// Restituisce true se l'operazione ha successo, false altrimenti.
// I dati vengono restituiti tramite i parametri:
//   - processes: puntatore a un array di ProcessInfo (allocato dinamicamente dalla funzione)
//   - num_processes: puntatore a un intero che conterrà il numero di processi trovati
bool monitor_get_processes(ProcessInfo **processes, int *num_processes);

// Funzione per liberare le risorse allocate dal modulo di monitoraggio
void monitor_cleanup();

#endif
