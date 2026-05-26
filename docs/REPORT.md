# Project Report

## 1. Runtime Complexity Analysis
* **Initialization of the intersections map:** * Best case: ...
  * Average case: ...
  * Worst case: ...
* **Finding coordinates of a street/place by name:**
  * Best/Average/Worst: ...
* **Path-finding algorithm:**
  * Best/Average/Worst: ...

---

## 2. Experimental Latency Analysis

### 2.1. Latency to find connected streets vs Map Size
#### Raw Data
| Map Size | Sequential Latency (ms) | Intersections Map Latency (ms) |
|---|---|---|
| Small | ... | ... |
| Medium | ... | ... |

#### Plot
![Connected streets latency vs map size](plots/plot1.png)

#### Explanation
[Explica aquí els resultats...]

### 2.2. Latency to find a path vs Map Size (Same Origin/Destination)
#### Raw Data
[Insereix taula aquí...]

#### Plot
![Path latency vs map size](plots/plot2.png)

#### Explanation
[Explica aquí els resultats...]

### 2.3. Latency to find a path vs Distance (Same Map)
#### Raw Data
[Insereix taula aquí...]

#### Plot
![Path latency vs distance](plots/plot3.png)

#### Explanation & Curve Fitting
[Explica els resultats i justifica l'ajust de la corba basat en la complexitat de l'algorisme...]

---

## 3. Improvement to the Visited Data Structure in BFS
* **Proposed Data Structure:** ...
* **Current vs Improved Complexity:** ...
* **Trade-offs:** ...

---

## 4. Improvement to Find Street Segment by Coordinates
* **Proposed Data Structure / Algorithm:** ...
* **Current vs Improved Complexity:** ...
* **Trade-offs:** ...
