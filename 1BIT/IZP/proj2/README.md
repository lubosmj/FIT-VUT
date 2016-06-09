## Projekt 2 - Iterační výpočty

### Popis projektu

Implementujte výpočet přirozeného logaritmu pouze pomocí matematických operací +,-,*,/. Implementujte vyhledání požadovaného počtu iterací pro požadovanou přesnost logaritmu z hodnot zadaného intervalu.


### Detailní specifikace

#### Překlad a odevzdání zdrojového souboru

Odevzdání: Program implementujte ve zdrojovém souboru <code>proj2.c</code>. Zdrojový soubor odevzdejte prostřednictvím informačního systému.

Překlad: Program překládejte s následujícími argumenty:
```
 $ gcc -std=c99 -Wall -Wextra -pedantic proj2.c -lm -o proj2
```

#### Syntax spuštění

Program se spouští v následující podobě:
```
 ./proj2 --log X N
 ```
nebo
```
 ./proj2 --iter MIN MAX EPS
 ```

Argumenty programu:
* <code>--log X N</code> požadavek pro výpočet přirozeného logaritmu z čísla X v N iteracích (Taylorova polynomu a zřetězeného zlomku).
* <code>--iter MIN MAX EPS</code> požadavek pro hledání požadovaného počtu iterací pro dostatečně přesný (EPS >= 1e-12) výpočet logaritmu čísla z intervalu <code>&lt;MIN;MAX&gt;</code>.

#### Implementační detaily

Je zakázané použít funkce z matematické knihovny. Jedinou výjimkou je funkce <code>log</code> použitá pouze pro srovnání výpočtů, funkce <code>isnan</code> a <code>isinf</code> a konstanty NAN a INFINITY. Ve všech výpočtech používejte typ <code>double</code>.

#### Implementace logaritmu

Funkci logaritmu implementujte dvakrát a to pomocí Taylorova polynomu a zřetězených zlomků.

###### 1. podúkol - Implementace Taylorova polynomu

Logaritmus pomocí Taylorova polynomu implementujte ve funkci s prototypem:
<syntax>double taylor_log(double x, unsigned int n);</syntax>
kde n udává rozvoj polynomu (počet členů). Taylorův polynom pro funkci logaritmu implementujte podle vzorce

pro 0 < x < 2 a

pro x > 1/2. Doporučená mezní hodnota mezi těmito dvěma polynomy je 1.

###### 2. podúkol - Implementace zřetězeného zlomku

Logaritmus pomocí zřetězených zlomků (viz demonstrační cvičení) implementujte ve funkci s prototypem:
<syntax>double cfrac_log(double x, unsigned int n);</syntax>
kde n udává rozvoj zřetězeného zlomku. Funkci implementujte podle vzorce.

###### 3. podúkol - Hledáni požadovaného počtu iterací výpočtu

Program také hledá požadovaný počet iterací pro výpočet dostatečně přesné hodnoty logaritmu. Uživatel zadá interval hodnot, pro které chce znát požadovaný počet iterací pro Taylorův polynom nebo zřetězený zlomek takový, aby logaritmus libovolné hodnoty z tohoto intervalu odpovídal jemu zadané přesnosti.

### Výstup programu

V případě výpočtu logaritmu (argument <code>--log</code>) program tiskne následující řádky:
```
        log(X) = LOG_X
     cf_log(X) = CF_LOG_X
 taylor_log(X) = TAYLOR_LOG_X
 ```

V případě hledání požadovaného počtu iterací (argument <code>--iter</code>) program tiskne následující řádky:
```
        log(MIN) = LOG_MIN
        log(MAX) = LOG_MAX
 continued fraction iterations = CF_ITER
     cf_log(MIN) = CF_LOG_MIN
     cf_log(MAX) = CF_LOG_MAX
 taylor polynomial iterations = TAYLOR_ITER
 taylor_log(MIN) = TAYLOR_LOG_MIN
 taylor_log(MAX) = TAYLOR_LOG_MAX
 ```

kde:
* <code>X</code>, <code>MIN</code> a <code>MAX</code> jsou hodnoty zadané argumentem příkazové řádky (odpovídají formátu printf <code>%g</code>),
* <code>LOG_</code> jsou hodnoty logaritmu z matematické knihovny,
* <code>CF_LOG_</code> jsou hodnoty logaritmu vypočteného pomocí zřetězeného zlomku,
* <code>TAYLOR_LOG_</code> jsou hodnoty logaritmu vypočteného pomocí Taylorova polynomu,
* všechny <code>*LOG_*</code> hodnoty odpovídají formátu <code>%.12g</code>,
* <code>CF_ITER</code>, resp. <code>TAYLOR_ITER</code> je číslo udávající počet iterací (rozvoje zřetězeného zlomku, resp. Taylorova polynomu) potřebné pro výpočet logaritmu libolného čísla z intervalu <code>&lt;MIN;MAX&gt;</code> tak, aby vypočtený logaritmus byl od reálného výsledku vzdálen maximálně o <code>EPS</code>.

### Příklady vstupů a výstupů

''Číselné údaje nemusí přesně odpovídat vaší implementaci. Výsledek závisí na způsobu implementace a optimalizaci.''
```
 $ ./proj2 --log 1.131401114526 4
        log(1.1314) = 0.123456789012
     cf_log(1.1314) = 0.123456789012
 taylor_log(1.1314) = 0.123452108537
```

```
 $ ./proj2 --iter .31 3 1e-6
        log(0.31) = -1.1711829815
        log(3) = 1.09861228867
 continued fraction iterations = 6
     cf_log(0.31) = -1.17118249665
     cf_log(3) = 1.09861206812
 taylor polynomial iterations = 30
 taylor_log(0.31) = -1.17118199531
 taylor_log(3) = 1.09861197106
```

### Hodnocení

Na výsledném hodnocení mají hlavní vliv následující faktory:

* implementace algoritmických schemat pro iterační výpočty,
* výpočet logaritmu,
* implementace vyhledání požadovaného počtu iterací,
* ošetření neočekávaných stavů.

Prémiové body (max 4) je možné získat implementací vyhledání požadovaného počtu iterací pomocí bisekce/půlení intervalu/binárního vyhledání.
