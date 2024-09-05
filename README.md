# ServerKlientAlgebraPolynomov

Projekt písaný v Clion v2019

cez Cygwin64 3.5.1

Program je rozdelný na Klinta a Server
  - Klient:
    - 1. pripojenie sa na server
    - 2. komunikácia:
                - posiela reťazce znakov (polynóm so zomkami) a operáciu ktorá má byť aplikovaná a prijíma výsledok v tvare polynómu.
    - Pužitie: Používaťel zadá dva polynómi a zvolí aritmetickú operáciu (sčítanie, odčítanie, násobenie, delenie, zvišok po delený polynómov) alebo zadanie nových polynómov a ukončeinie.     
  
  - Server:
    - 1. nastaví server
    - 2.1. komunikácia:
                  - prepína mezdi klintami
                  - prijíma reťazce znakov a ukladá ich ako koeficienty polynómu n-teho stupňa, ukladá operáciu korá sa má vykonať, posiela výsledok 
    - 2.2. počítanie:
                  - vykonáva aritmetickú operáciu s polynómami i-teho klienta, a ukladá výsledko operácie.
      - tieto úkony sa vykonávaju v samostatných vláknach jedno pre každú funkciu, prístup k dátam je kontrolovaný cez mutexi.
