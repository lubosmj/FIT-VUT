## Projekt 1 - Práce s textem

### Popis projektu

Implementujte program pro jednoduché zpracování slov v textu. Program bude implementovat funkce pro detekci čísel, detekci kalendářního data, test na prvočíslo a detekci palindromu. Vstupním textem je standardní vstup (stdin). Vstupní soubor se zpracovává po slovech podle definice konverzního specifikátoru %s funkce scanf.

### Detailní specifikace

Program implementujte ve zdrojovém souboru ''proj1.c''.

#### Překlad a odevzdání zdrojového souboru

Odevzdání: Odevzdejte zdrojový soubor ''proj1.c'' prostřednictvím informačního systému.

Překlad: Program překládejte s následujícími argumenty
```
 $ gcc -std=c99 -Wall -Wextra -pedantic proj1.c -o proj1
```
#### Syntax spuštění

Program se spouští v následující podobě: (./proj1 značí umístění a název programu):
```
 ./proj1
 ```
nebo
```
 ./proj1 libovolné argumenty
```

Pokud je program spuštěn s libovolnými neprázdnými argumenty, vypíše svůj krátký popis a úspěšně skončí. V opačném případě provádí čtení a zpracování dat ze vstupu.

#### Implementační detaily

Program čte data ze standardního vstupu. Data čte po slovech podle definice konverzního specifikátoru %s volání scanf. Slovo je následně zpracováno:

* Pokud se jedná o celé kladné číslo (maximální hodnota INT_MAX), program provede detekci na prvočíslo. Algoritmus detekce na prvočíslo můžete implementovat triviálním způsobem.
* Pokud se jedná o kalendářní datum formátu DDDD-DD-DD (kde D je číslice 0-9), program provede detekci správnosti data a výpočet dne v týdnu. Detekce správnosti a převod na den v týdnu proveďte pomocí volání [http://pubs.opengroup.org/onlinepubs/9699919799/functions/mktime.html mktime] (struktura tm definovaná [http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/time.h.html zde]).
* Všechno ostatní (včetně těch, které neodpovídají ani jednomu typu dat) jsou brána jako obecná slova (včetně symbolů) a program provede test, zda-li se jedná o palindrom.

S každým slovem nebo načtenými daty program na standardní výstup (stdout) vypisuje řádek v následujícím formátu:

* Pokud se jedná o nezáporné celé číslo (např. s hodnotou 123456):
  number: 123456
* Pokud se jedná o prvočíslo (např. s hodnotou 41):
  number: 41 (prime)
* Pokud se jedná o platné kalendářní datum (např. 2015-09-01). Dny v týdnu jsou následující zkratky: Mon, Tue, Wed, Thu, Fri, Sat, Sun:
  date: Tue 2015-09-01
* Pokud se jedná o slovo, které je zároveň palindrom (např. anna):
  word: anna (palindrome)
* Pokud se jedná o slovo, které není palindrom (např. hello):
  word: hello

Maximální přípustná délka slova je omezena na 100 znaků.

#### Omezení v projektu

Je zakázané použít následující funkce:
* všechna volání hlavičkového souboru string.h - hlavičkový soubor je v projektu zakázaný,
* volání z rodiny malloc a free - práce s dynamickou pamětí není v tomto projektu zapotřebí,
* volání z rodiny fopen, fclose, fscanf, ... - práce se soubory (dočasnými) není v tomto projektu žádoucí.

#### Neočekávané chování

Na chyby za běhu programu reagujte obvyklým způsobem: Na neočekávaná vstupní data, formát vstupních dat nebo chyby při volání funkcí reagujte přerušením programu se stručným a výstižným chybovým hlášením na příslušný výstup a odpovídajícím návratovým kódem.

### Příklady vstupů a výstupů

Mějme text:
```
 $ cat data.txt
 Hello, world!
 Created on 2015-08-26
 having 13 words out of which 8 = 4+4 contains o.
```
Výpis programu po spuštění:
```
 $ ./proj1 <data.txt
 word: Hello,
 word: world!
 word: Created
 word: on
 date: Wed 2015-08-26
 word: having
 number: 13 (prime)
 word: words
 word: out
 word: of
 word: which
 number: 8
 word: = (palindrome)
 word: 4+4 (palindrome)
 word: contains
 word: o.
```
### Hodnocení

###### Na výsledném hodnocení mají hlavní vliv následující faktory:

* přeložitelnost zdrojového souboru,
* formát zdrojového souboru (členění, zarovnání, komentáře, vhodně zvolené identifikátory),
* dekompozice problému na podproblémy (vhodné funkce, vhodná délka funkcí a parametry funkcí),
* správná volba datových typů, případně tvorba nových typů,
* vhodně zvolený algoritmus načítání a rozpoznání vstupních dat,
* správná funkcionalita detekčních funkcí a
* ošetření chybových stavů.
