# Server klient Algebra polynomov so zlomkami

Projekt písaný v Clion v2019

cez Cygwin64 3.5.1

Program je rozdelný na Klinta a Server
  - Klient:
    - a. pripojenie sa na server
    - b. komunikácia:
        - posiela reťazce znakov (polynóm so zomkami) a operáciu ktorá má byť aplikovaná a prijíma výsledok v tvare polynómu.
    - Pužitie: Používaťel zadá dva polynómi a zvolí aritmetickú operáciu (sčítanie, odčítanie, násobenie, delenie, zvišok po delený polynómov) alebo zadanie nových polynómov a ukončeinie.     
  
  - Server:
    - a. nastaví server
    - b.1. komunikácia:
        - prepína mezdi klintami,
        - prijíma reťazce znakov a ukladá ich ako koeficienty polynómu n-teho stupňa, ukladá operáciu korá sa má vykonať, posiela výsledok.
    - b.2. počítanie:
                  - vykonáva aritmetickú operáciu s polynómami i-teho klienta, a ukladá výsledko operácie.
      - tieto úkony sa vykonávaju v samostatných vláknach jedno pre každú funkciu, prístup k dátam je kontrolovaný cez mutexi.

  - Main:
    - výkonávanie na jednom zariadení bez serveru.   
