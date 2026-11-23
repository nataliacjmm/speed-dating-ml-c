# Speed Dating ML (KNN in C)

A **from-scratch implementation in pure C** (with no external Machine Learning libraries) of a supervised classification system using **K-Nearest Neighbors (KNN)** applied to the real *Speed Dating* dataset (`speeddating.csv`), predicting whether a **match (1)** or **no match (0)** occurs based on numerical attributes.

## Architecture & Features

1. **Data Ingestion & Dynamic Memory Management (`main.c`, `matriz.c`, `crearMatriz.c`)**
   - Line-by-line CSV parsing for arbitrary dataset sizes (`fgets` + `strtok`).
   - Dynamic memory allocation (`malloc` / `realloc`) for `Dato` matrices and explicit cleanup (`free`).
   - Dataset splitting and export (`escribirCSV.c`) into training (`entrenamiento50.csv`), testing (`prueba50.csv`), and shuffled subsets (`sdDESORDEN.csv`).

2. **Preprocessing & Min-Max Normalization (`normalizar.c`, `precision.c`)**
   - Computes per-feature minimum (`DBL_MAX`) and maximum (`DBL_MIN`) values on the training set.
   - Scales all features to the **[0, 1]** range to prevent larger-scale attributes from dominating distance calculations.

3. **KNN Algorithm & Euclidean Distance (`precision.c`, `precision1K.c`, `interactivo.c`)**
   - Computes **Euclidean distance** (`calcular_dis`) between feature vectors.
   - Classifies test samples by majority vote among the **k nearest neighbors** (evaluated with k=1, k=5, and k=7).

4. **Evaluation & Class Imbalance Analysis (`contarMatch.c`, `precision.c`)**
   - Analyzes dataset class imbalance (`contarMatch`, `encontrarClaseMinoritaria`).
   - Evaluates model performance using confusion matrix metrics: **True Positives (TP)**, **True Negatives (TN)**, **False Positives (FP)**, and **False Negatives (FN)**.

5. **Interactive Mode (`interactivo.c`)**
   - Real-time console prediction for new user-entered samples normalized with training parameters.

## Build
```bash
make
```
