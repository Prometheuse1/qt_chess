# Jeu d'Échecs Qt

## Fichiers du projet

| Fichier | Rôle |
|---|---|
| `plateau.h` / `plateau.cpp` | **Inchangés** — logique de jeu originale |
| `chessboard.h` / `chessboard.cpp` | Widget Qt (échiquier + menus) |
| `main_qt.cpp` | Point d'entrée Qt (remplace `main.cpp`) |
| `ChessGame.pro` | Fichier projet Qt (qmake) |

> ⚠️  Ne pas inclure `main.cpp` original dans la compilation — `main_qt.cpp` le remplace.

---

## Prérequis

- Qt 5 ou Qt 6 (avec les widgets)
- MinGW ou MSVC (Windows) / GCC (Linux/Mac)
- `qmake` dans le PATH

### Installation de Qt (si nécessaire)

- **Windows** : [Qt Online Installer](https://www.qt.io/download-open-source)  
  Choisir : Qt 6.x → Desktop → MinGW 64-bit (ou MSVC)
- **Linux** : `sudo apt install qt6-base-dev` (ou `qtbase5-dev` pour Qt5)
- **macOS** : `brew install qt`

---

## Compilation

```bash
# Dans le dossier ChessGame/
qmake ChessGame.pro
make          # Linux/macOS
mingw32-make  # Windows + MinGW
```

L'exécutable `ChessGame` (ou `ChessGame.exe`) sera créé dans le même dossier.

---

## Lancer le jeu

```bash
./ChessGame        # Linux/macOS
ChessGame.exe      # Windows
```

Une fenêtre de menu apparaît :  
- **Joueur vs Joueur** — deux joueurs humains à tour de rôle  
- **Joueur vs IA** — les Blancs jouent, les Noirs sont gérés par l'IA

---

## Contrôles

| Action | Geste |
|---|---|
| Sélectionner une pièce | Clic gauche sur la pièce |
| Déplacer | Clic gauche sur la case cible |
| Changer de pièce | Clic sur une autre pièce de même couleur |
| Désélectionner | Re-clic sur la pièce sélectionnée |

Les cases cibles légales sont indiquées par des **points verts** à chaque sélection.

---

## Architecture

```
main_qt.cpp
    └── MenuDialog  (QDialog)
    └── ChessBoard  (QWidget)
            └── plateau  ← objet original, 100% inchangé
```

`ChessBoard` n'appelle que l'API publique de `plateau` :
- `get_Piece(i,j)` — lecture de l'échiquier
- `get_Tour()` — numéro du tour
- `mov_Piece(ld,cd,la,ca)` — déplacement humain
- `deplacer_IA()` — coup de l'IA
- `fin_Partie()` — détection fin de partie
- `roi_en_echec(couleur)` — état d'échec
- `a_mouvement_legal(couleur)` — pat
- `evaluation(ld,cd,la,ca)` — mouvements légaux (pour surlignage)
