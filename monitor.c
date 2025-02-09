// monitor.c

#include "monitor.h"
#include <proc/readproc.h>  // libprocps
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h> // Xlib
#include <X11/Xutil.h>  // Per XGetWindowProperty

// Struttura di supporto per tenere traccia delle finestre
typedef struct {
    Window window;
    pid_t  pid;
} WindowInfo;
//Variabile globale per tenere traccia del display
Display *display = NULL;

bool monitor_init() {
    // Inizializza la connessione a X Server, se necessario per rilevare finestre GUI
    display = XOpenDisplay(NULL);
    if (!display)
    {
      fprintf(stderr, "Errore: Impossibile aprire il display X.\n");
      //Se non si riesce ad aprire il display, si potrebbe ritornare un errore, oppure continuare
      //ma la parte di codice per capire se una finestra è in uso dovrà saltare
      //return false;
    }
    return true;
}

// Funzione per ottenere la lista delle finestre e i relativi PID
int get_window_list(WindowInfo **windows, Display *display) {

    Window root = DefaultRootWindow(display);
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop_return = NULL;

    // Ottieni la proprietà _NET_CLIENT_LIST dalla finestra radice
    if (XGetWindowProperty(display, root,
                           XInternAtom(display, "_NET_CLIENT_LIST", False),
                           0, 1024, // Leggi fino a 1024 elementi
                           False, XA_WINDOW,
                           &actual_type, &actual_format,
                           &nitems, &bytes_after, &prop_return) != Success) {
        fprintf(stderr, "Errore: Impossibile ottenere la proprietà _NET_CLIENT_LIST.\n");
        return 0;
    }

    if (actual_type != XA_WINDOW) {
        fprintf(stderr, "Errore: _NET_CLIENT_LIST non è di tipo XA_WINDOW.\n");
        XFree(prop_return);
        return 0;
    }
    // Alloca lo spazio per l'array di WindowInfo
    *windows = (WindowInfo *)malloc(nitems * sizeof(WindowInfo));
       if (!*windows) {
           perror("Errore di allocazione della memoria per la lista di finestre");
           XFree(prop_return);
           return 0;
    }
    // Itera attraverso gli ID delle finestre ottenuti
    Window *window_ids = (Window *)prop_return;
    for (unsigned long i = 0; i < nitems; i++) {
        (*windows)[i].window = window_ids[i];
        // Ottieni il PID associato alla finestra tramite la proprietà _NET_WM_PID
        Atom pid_atom = XInternAtom(display, "_NET_WM_PID", False);
        Atom actual_type_pid;
        int actual_format_pid;
        unsigned long nitems_pid;
        unsigned long bytes_after_pid;
        unsigned char *prop_return_pid = NULL;

        if (XGetWindowProperty(display, window_ids[i],
                               pid_atom, 0, 1, False,
                               XA_CARDINAL, &actual_type_pid, &actual_format_pid,
                               &nitems_pid, &bytes_after_pid, &prop_return_pid) == Success &&
            actual_type_pid == XA_CARDINAL && nitems_pid > 0) {
            (*windows)[i].pid = *(pid_t *)prop_return_pid;
             XFree(prop_return_pid);
        } else
        {
          // Se non riesci a ottenere il PID, imposta a 0 (o un valore non valido)
          (*windows)[i].pid = 0;
        }
    }
     XFree(prop_return);
    return (int)nitems;
}

// Funzione per controllare se una finestra è attiva
bool is_window_active(Window window, Display *display) {
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop_return = NULL;
    Window *active_window = NULL;
    //Ottieni la finestra attiva
    if (XGetWindowProperty(display, DefaultRootWindow(display),
                         XInternAtom(display, "_NET_ACTIVE_WINDOW", False),
                         0, 1,
                         False, XA_WINDOW,
                         &actual_type, &actual_format,
                         &nitems, &bytes_after, &prop_return) == Success)
    {
      if (actual_type == XA_WINDOW && nitems > 0)
      {
        active_window = (Window *)prop_return;
        //Confronta la finestra passata in input con quella attiva
        if (*active_window == window)
        {
          XFree(prop_return);
          return true; // La finestra è attiva
        }
      }
    }
    if(prop_return) XFree(prop_return);
    return false;
}

bool monitor_get_processes(ProcessInfo **processes, int *num_processes) {
    PROCTAB *proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
      if (!proc) {
          perror("Errore nell'apertura di /proc");
          return false;
      }
    // Ottieni la lista delle finestre (se il display X è aperto)
    WindowInfo *windows = NULL;
    int num_windows = 0;
    if(display)
    {
        num_windows = get_window_list(&windows, display);
    }
    // Conta il numero di processi
    proc_t proc_info;
    memset(&proc_info, 0, sizeof(proc_info));
    int count = 0;
    while (readproc(proc, &proc_info) != NULL) {
        count++;
    }
    // Alloca l'array di ProcessInfo
    *processes = (ProcessInfo *)malloc(count * sizeof(ProcessInfo));
    if (!*processes) {
        perror("Errore di allocazione della memoria");
        closeproc(proc);
        return false;
    }
    // Resetta l'iteratore di readproc
    freeproc(proc);
    proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);

    // Popola l'array di ProcessInfo
    int i = 0;
    memset(&proc_info, 0, sizeof(proc_info));

    while (readproc(proc, &proc_info) != NULL && i < count) {
        (*processes)[i].pid = proc_info.tid;
        strncpy((*processes)[i].name, proc_info.cmd, sizeof((*processes)[i].name) - 1);
        (*processes)[i].name[sizeof((*processes)[i].name) - 1] = '\0'; // Assicurati la terminazione
        (*processes)[i].cpu_usage = (float)proc_info.pcpu; // pcpu è già in percentuale
        (*processes)[i].mem_usage = proc_info.rss * getpagesize(); // rss è in pagine, converti in byte
        //(*processes)[i].is_gui = false; // Inizializza a false
        //(*processes)[i].is_active = false;
        //Resetta i valori di is_gui e is_active
        (*processes)[i].is_gui = false;
        (*processes)[i].is_active = false;
        //Imposta la priorità di default
        (*processes)[i].priority = DEFAULT_PRIORITY;

        // Verifica se il processo ha una finestra GUI e se è attiva
        for (int j = 0; j < num_windows; j++) {
            if (windows[j].pid == (*processes)[i].pid) {
                (*processes)[i].is_gui = true;

                if(display)
                {
                    if (is_window_active(windows[j].window, display))
                    {
                        (*processes)[i].is_active = true;

                    }
                    else
                    {
                        (*processes)[i].is_active = false;
                    }
                    break; // Esci dal ciclo interno una volta trovata la finestra
                }

            }
        }
        i++;
    }
    *num_processes = count;
    if(windows) free(windows); // Libera la memoria allocata per la lista di finestre
    closeproc(proc);
    return true;
}

void monitor_cleanup() {
    // Chiudi la connessione a X Server
    if (display)
    {
      XCloseDisplay(display);
      display = NULL;
    }
}
