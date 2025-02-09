// main.c
#include "config.h"
#include "monitor.h"
#include "decision.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    Config config = {
        .cpu_threshold = DEFAULT_CPU_THRESHOLD,
        .mem_threshold = DEFAULT_MEM_THRESHOLD,
        .priority_threshold = DEFAULT_PRIORITY,
        .use_ram = DEFAULT_USE_RAM
    };

    // Inizializzazione dei moduli (da implementare)
    if (!monitor_init()) {
        fprintf(stderr, "Errore durante l'inizializzazione del modulo di monitoraggio.\n");
        return 1;
    }
    if (!decision_init(&config)) {
        fprintf(stderr, "Errore durante l'inizializzazione del modulo decisionale.\n");
        return 1;
    }
    if (!control_init()) {
        fprintf(stderr, "Errore durante l'inizializzazione del modulo di controllo.\n");
        return 1;
    }

    // Ciclo principale
    while (1) {
        // 1. Monitora i processi
        ProcessInfo *processes = NULL;
        int num_processes = 0;
        if (!monitor_get_processes(&processes, &num_processes)) {
            fprintf(stderr, "Errore durante il recupero delle informazioni sui processi.\n");
            continue; // o gestisci l'errore in modo più appropriato
        }

        // 2. Prendi decisioni
        decision_make_decisions(processes, num_processes);

        // 3. Applica le azioni di controllo
        control_apply_decisions(processes, num_processes);

        // 4. Libera la memoria allocata
        free(processes);

        // 5. Attendi un intervallo di tempo (es. 1 secondo)
        sleep(1);
    }

    // Pulizia finale (in teoria, questo punto non viene mai raggiunto nel ciclo infinito)
    monitor_cleanup();
    decision_cleanup();
    control_cleanup();

    return 0;
}
