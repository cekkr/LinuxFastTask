// control.c

#include "control.h"
#include <signal.h> // Per i segnali (SIGSTOP, SIGCONT)
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


bool control_init() {
    // Nessuna inizializzazione specifica necessaria in questo caso
    return true;
}

void control_apply_decisions(ProcessInfo *processes, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].is_paused) {
            // Metti in pausa il processo (invia SIGSTOP)
            // Controlla che il processo non sia già stato messo in pausa prima di inviare di nuovo il segnale.
             if (kill(processes[i].pid, 0) == 0) //Controlla se il processo esiste
             {
                int ret = kill(processes[i].pid, SIGSTOP);
                if(ret == 0)
                {
                  printf("Processo %d (%s) messo in pausa.\n", processes[i].pid, processes[i].name);
                }
                else {
                  perror("Errore nell'invio del segnale SIGSTOP"); //perror stampa un messaggio di errore più dettagliato.
                }
             }

        } else {
            // Riattiva il processo (invia SIGCONT) se era stato precedentemente messo in pausa.
            if (kill(processes[i].pid, 0) == 0) {
                int ret = kill(processes[i].pid, SIGCONT);

                if(ret == 0)
                {
                  printf("Processo %d (%s) riattivato.\n", processes[i].pid, processes[i].name);
                }
                else{
                    perror("Errore nell'invio del segnale SIGCONT");
                }
            }

        }
    }
}

void control_cleanup() {
    // Nessuna risorsa da liberare in questo esempio
}