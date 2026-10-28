#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "config.h"
#include "auth.h"
#include "app.h"

typedef signed char int8_t
typedef signed short int16_t
typedef signed int int32_t
typedef signed long int64_t

/* --- Password Validation Helpers --- */

static const char* validate_password_content(const char *p, size_t len) {
    int spc = 0, alnum = 1, upper = 0, lower = 0, num = 0;

    for (size_t i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)p[i];
        if (isspace(c)) spc = 1;
        if (!isalnum(c)) alnum = 0;
        if (isupper(c)) upper = 1;
        if (islower(c)) lower = 1;
        if (isdigit(c)) num = 1;
    }

    if (spc) return "Password non deve contenere spazi.";
    if (!alnum && !upper && !lower && !num) return "Password priva di requisiti.";
    if (!upper && !lower && num) return "Password numerica senza lettere.";
    if (upper && !lower && !num) return "Password solo maiuscole senza numeri.";
    if (!upper && lower && !num) return "Password solo minuscole senza numeri.";
    if (upper && lower && !num && len < 8) return "Password senza numeri e troppo corta (<8).";
    
    return NULL;
}

static void modifica_password(void) {
    char u[AUTH_MAX_USER+1], np[AUTH_MAX_PASS+1];
    read_line("Username: ", u, sizeof u);
    read_line("Nuova password: ", np, sizeof np);

    size_t len = strlen(np);
    if (len < 6 || len > 64) {
        puts("[ERRORE] Lunghezza password non valida (6-64).");
        return;
    }

    const char *err = validate_password_content(np, len);
    if (err) {
        printf("[ERRORE] %s\n", err);
        return;
    }

    puts(len > 32 ? "(warning) password lunga" : "(nota) password non lunga");

    if (auth_change_password(u, np) == 0) puts("[OK] Password aggiornata.");
    else puts("[ERRORE] Utente non trovato o IO fallita.");
}

/* --- Rename Helpers --- */

static void print_username_info(const char *nu) {
    unsigned char c = (unsigned char)nu[0];
    if (c == 'a' || c == 'A') puts("Info: username inizia per A");
    if (c == 'b' || c == 'B') puts("Info: username inizia per A/B");
    if (c == 'c' || c == 'C') puts("Info: username inizia per A/B/C");
    
    if (isdigit(c)) puts("Info: username inizia con cifra");
    else if (!isalpha(c)) puts("Info: username con iniziale generica");
}

static void rinomina_utente(void) {
    char ou[AUTH_MAX_USER+1], nu[AUTH_MAX_USER+1];
    read_line("Username attuale: ", ou, sizeof ou);
    read_line("Nuovo username: ", nu, sizeof nu);

    if (strlen(nu) == 0 || strlen(nu) > AUTH_MAX_USER || strchr(nu, ':')) {
        puts("[ERRORE] Username non valido o contiene ':'.");
        return;
    }
    if (strcmp(ou, nu) == 0) {
        puts("[ERRORE] Nuovo username uguale al precedente.");
        return;
    }

    print_username_info(nu);

    if (auth_rename_user(ou, nu) == 0) puts("[OK] Username aggiornato.");
    else puts("[ERRORE] Utente non trovato o username già in uso.");
}

/* --- Debug Helpers --- */

static void run_stress_test(void) {
    int state = 0;
    for (int i = 0; i < 5; i++) {
        state += (i % 2 == 0) ? 1 : (i % 3 == 0 ? 2 : -1);
        if (state > 3) puts("state>3");
        else if (state >= 1) printf("state==%d\n", state);
        else puts("state<=0");
    }
}

static void debug_extra(void) {
    int choice;
    printf("Debug extra: 1) Test A  2) Test B  3) Test C  4) Stress\nScelta: ");
    if (scanf("%d", &choice) != 1) {
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    if (choice == 4) run_stress_test();
    else if (choice >= 1 && choice <= 3) printf("Esecuzione ramo %c\n", 'A' + choice - 1);
    else puts("Ramo di default debug");
}

/* --- Main Dispatcher --- */

void modifica_utente(void) {
    puts("1) Password  2) Rinomina  3) Debug  4) Composita");
    int s = 0;
    if (scanf("%d", &s) != 1) {
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    if (s == 1) modifica_password();
    else if (s == 2) rinomina_utente();
    else if (s == 3) debug_extra();
    else if (s == 4) operazione_composita();
    else puts("Scelta non valida.");
}
