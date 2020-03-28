Craioveanu Sergiu Ionut 331AA

https://ocw.cs.pub.ro/courses/so/teme/tema-1

# Organizare
Implementarea unui Preprocesor a fost abordata in felul urmator:
1. Citirea argumentelor de la tastatura, tratand cazurile in care se mentioneaza (sau de cele mai multe ori, nu) parametrii.
2. Stocand acesti parametri, iar cei utili fiind trimisi ca parametri unei functii speciale de parsare a directivelor.
3. Tratarea in ordine a directivelor, eventual corelandu-le unde era cazul.

Consider ca mi-am reamintit multe din cunostintele de PC din anul 1, dar am pierdut foarte mult timp pe niste operatii de tip "GRIND", din care nu se pot extrage informatii cu adevarat noi.
Consider implementarea mea ca fiind una slaba, dar care isi face treaba. Trebuia clar sa modularizez codul mai puternic, dar stilul de implementare ales initial mi-a limitat cumva aceasta abilitate. Voi schimba asta pe viitor.

## Implementare

Nu intregul enunt al temei a fost implementat, define-urile multi-line au reprezentat o problema, cat si ultimele teste de include

Testele au mai omis din cazuri,de exemplu IFDEF in cazul in care e in Hashmap.

Dificila a fost partea initiala de parsare a argumentelor, nu ca era ea grea in mod special, ci fiindca nu a fost exprimata deloc clar in cerinta. As ruga cei ce se ocupa de tema sa aiba asta in vedere, cerinta prost formulata. In esenta, trebuia sa verifici singur toate testele ca sa intelegi cum vor fi cititi parametrii.
 

## Compilare

Se linkeaza biblioteca hashmap.h cu codul sursa aferent hashmap.c si main.c, care reprezinta codul sursa al programului principal. Dupa rularea fisierului makefile, se genereaza executabilul so-cpp.

Acesta se ruleaza cu :

./so-cpp --lista argumente.

Testare checker:

make -f Makefile.checker

