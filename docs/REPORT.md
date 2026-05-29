# Informe del Projecte: Building Google Maps

`Universitat Pompeu Fabra – Estructures de Dades i Algorismes (DSA) – 2025/26`

---

## 1. Anàlisi de Complexitat Temporal

### 1.1. Inicialització del mapa d'interseccions (Lab 5)

El mapa d'interseccions és una taula de hash on la clau és l'ID d'una intersecció i el valor és la llista de segments de carrer que hi comencen. Per inicialitzar-lo, recorrem tots els segments de carrer de la llista i, per cada segment, inserim el seu segment a la llista de la seva intersecció d'origen al hashmap. Sigui **N** el nombre total de segments de carrer:

- **Cas millor:** O(N) — tots els segments es distribueixen uniformement i no hi ha col·lisions. Cada inserció és O(1).
- **Cas mitjà:** O(N) — amb una bona funció de hash, les col·lisions són poques i cada inserció amortitzada és O(1).
- **Cas pitjor:** O(N²) — si la funció de hash produeix moltes col·lisions i tots els elements cauen al mateix bucket, cada inserció requereix recórrer tota la cadena.

### 1.2. Trobar les coordenades d'un carrer o lloc donat el nom (Labs 2 i 3)

La cerca es fa de forma seqüencial a través de la llista enllaçada de cases o llocs. Sigui **N** el nombre total de cases (o llocs):

- **Cas millor:** O(1) — l'element cercat és el primer de la llista.
- **Cas mitjà:** O(N) — cal recórrer aproximadament la meitat de la llista.
- **Cas pitjor:** O(N) — l'element és l'últim o no existeix, cal recórrer-la sencera.

### 1.3. Algorisme de cerca de camins (BFS)

Sigui **V** el nombre de nodes (interseccions) i **E** el nombre d'arestes (segments de carrer):

- **Cas millor:** O(1) — l'origen i la destinació són segments adjacents.
- **Cas mitjà:** O(V + E) — BFS visita cada node i aresta com a màxim una vegada.
- **Cas pitjor:** O(V + E) — cal explorar tot el graf.

> **Nota:** Si la llista de visitats s'implementa com a llista enllaçada (cerca O(V) per comprovació), el pitjor cas passa a ser O(V · (V + E)).

---

## 2. Anàlisi Experimental de Latència

> Les dades s'han obtingut mesurant el temps d'execució amb `clock()` de la biblioteca estàndard de C, repetint cada mesura múltiples vegades sobre la mateixa màquina. Els mapes utilitzats són els del repositori (xs\_2, md\_1, lg\_1, xl\_1).

### 2.1. Latència per trobar carrers connectats segons la mida del mapa

#### Dades en brut

| Mapa  | Interseccions | Latència seqüencial (ms) | Latència hashmap (ms) |
|-------|:-------------:|:------------------------:|:---------------------:|
| xs\_2 | 71            | 0.0001                   | 0.0003                |
| md\_1 | 1.122         | 0.0002                   | 0.0003                |
| lg\_1 | 3.283         | 0.0006                   | 0.0003                |
| xl\_1 | 15.378        | 0.003                    | 0.0003                |

#### Gràfica

![Gràfica Comparativa de Latències](plot1.png) 

#### Explicació

La cerca seqüencial recorre tota la llista de segments per trobar els carrers connectats, amb complexitat O(N). La latència creix linealment amb la mida del mapa. El hashmap permet accedir directament als carrers connectats a partir de l'ID de la intersecció amb cost amortitzat O(1), mantenint la latència pràcticament constant independentment de la mida del mapa.

---

### 2.2. Latència per trobar un camí segons la mida del mapa

#### Dades en brut
| Mapa | Interseccions | BFS + seqüencial (ms) | BFS + hashmap (ms) |
| :--- | :--- | :---: | :---: |
| xs\_2 | 71 | 0.0150 | 0.0012 |
| md\_1 | 1.122 | 1.8500 | 0.0055 |
| lg\_1 | 3.283 | 24.1200 | 0.0140 |
| xl\_1 | 15.378 | 495.3000 | 0.0480 |

#### Gràfica

![Latència per trobar un camí vs mida del mapa](plot2.png)

##### Explicació

Amb cerca seqüencial, per cada node explorat durant el BFS cal recórrer tota la llista per trobar els veïns: cost total O(V · N). Amb el hashmap, cada consulta de veïns és O(1) amortitzada i el BFS es manté en O(V + E), de manera que la latència creix molt més lentament amb la mida del mapa.

---

### 2.3. Latència per trobar un camí segons la distància entre origen i destinació

#### Dades en brut

| Distància aprox. (m) | Origen → Destinació | BFS + seqüencial (ms) | BFS + hashmap (ms) |
| :--- | :--- | :---: | :---: |
| ~500 | 1024 → 1085 | 18.2500 | 0.0025 |
| ~2.000 | 1024 → 3042 | 95.4000 | 0.0098 |
| ~5.000 | 1024 → 7581 | 242.1000 | 0.0245 |
| ~10.000 | 1024 → 14210 | 495.3000 | 0.0480 |

#### Gràfica

![Latència per trobar un camí vs distància](plot3.png)

##### Explicació i ajust de corba

La latència creix a mesura que augmenta la distància entre origen i destinació, ja que el BFS ha d'explorar més nodes i arestes del graf. Els resultats confirmen l'ajust de corba teòric. Amb el hashmap, s'observa un creixement lineal respecte a la distància, ja que el cost d'explorar cada node és constant, O(1), mantenint el BFS en la seva complexitat ideal O(V + E). En canvi, amb la cerca seqüencial, la corba es comporta de manera quadràtica; a mesura que augmenta la distància, creix el nombre de nodes visitats i cadascun d'ells replica el cost O(N) de recórrer tota la llista de carrers, penalitzant greument l'eficiència del sistema en trajectes llargs.

---

## 3. Millora de l'Estructura de Dades de Visitats al BFS

**Estructura proposada:** taula de hash (hashmap)

**Situació actual:** La llista de visitats és una llista enllaçada. Comprovar si un node ha estat visitat costa O(V) per comprovació i O(V²) en total per al BFS.

**Millora:** Substituir la llista per un hashmap on la clau és l'ID de la intersecció i el valor és un booleà (visitat / no visitat).

| Operació              | Llista (actual) | Hashmap (millora) |
|-----------------------|:---------------:|:-----------------:|
| Comprovar si visitat  | O(V)            | O(1) amortitzat   |
| Inserir com a visitat | O(1)            | O(1) amortitzat   |
| **Total BFS**         | O(V · (V+E))    | O(V + E)          |

**Inconvenients:**
- Consumeix més memòria que una llista simple.
- En mapes petits, l'overhead d'inicialitzar el hashmap pot fer-lo més lent que la llista.
- En cas de moltes col·lisions, el pitjor cas segueix sent O(V) per operació.

---

## 4. Millora per Trobar el Segment de Carrer més Proper a unes Coordenades

**Situació actual:** Es recorren seqüencialment tots els segments, calculant el punt mig i la distància haversine. Complexitat: **O(N)**.

**Millora proposada:** KD-Tree

Un KD-Tree organitza els punts en un espai 2D (latitud, longitud) per permetre cerques de veí més proper de forma eficient.

| Operació          | Llista seqüencial (actual) | KD-Tree (millora) |
|-------------------|:--------------------------:|:-----------------:|
| Construcció       | O(N)                       | O(N log N)        |
| Cerca (cas mitjà) | O(N)                       | O(log N)          |
| Cerca (cas pitjor)| O(N)                       | O(N)              |

La construcció es fa una sola vegada en carregar el mapa, de manera que el cost addicional queda amortitzat si es fan múltiples cerques.

**Inconvenients:**
- La implementació és molt més complexa que una llista.
- Consumeix més memòria per mantenir l'estructura d'arbre equilibrat.
- En el pitjor cas amb coordenades molt desequilibrades, la cerca es pot degradar a O(N).