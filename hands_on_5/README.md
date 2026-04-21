# Modifiche principali ad AuthController
- Implementato l'endpoint `@PostMapping("/login")` per la verifica delle credenziali.
- Aggiunta validazione dei dati di input (null/blank) per username e password nel login.
- Migliorata la gestione delle risposte HTTP con codici di stato appropriati (BAD_REQUEST, UNAUTHORIZED, OK).
