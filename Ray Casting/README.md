# RayCaster — Moteur 3D en C avec SDL2

Un moteur de raycasting minimaliste écrit en C, inspiré de la technique utilisée par des jeux comme **Wolfenstein 3D** et **DOOM**. Il projette une carte 2D en une vue 3D en temps réel à l'aide de lancers de rayons.
https://lodev.org/cgtutor/raycasting.html
---

## Aperçu

Le programme affiche une vue à la première personne d'une carte définie manuellement. Chaque colonne de pixels à l'écran correspond à un rayon lancé depuis la position du joueur — plus le mur touché est proche, plus la colonne affichée est haute.

Les murs peuvent être de différents types (couleurs différentes), et le joueur peut se déplacer et tourner en temps réel.

---

## Fonctionnalités

- Rendu 3D par raycasting (colonne par colonne)
- Carte 2D configurable via un tableau statique
- Différents types de murs avec couleurs distinctes (blanc, bleu, rouge)
- Déplacement du joueur (avant / arrière)
- Rotation du joueur (gauche / droite)
- Distance de rendu limitée configurable

---

## Dépendances

- **GCC** (via MSYS2/MinGW-w64 sur Windows, ou natif sur Linux/macOS)
- **SDL2** — affichage graphique

### Installation SDL2 sur MSYS2

```bash
pacman -S mingw-w64-x86_64-SDL2
```

---

## Compilation

### Windows (MSYS2 / VSCode)

```bash
gcc raycaster.c -o raycaster.exe -mconsole \
    -IC:/msys64/mingw64/include/SDL2 \
    -LC:/msys64/mingw64/lib \
    -lmingw32 -lSDL2main -lSDL2 -lm
```

> Le flag `-mconsole` est important pour voir les messages de debug dans la console.

### Linux / macOS

```bash
gcc raycaster.c -o raycaster -lSDL2 -lm
```

---

## Lancement

```bash
./raycaster.exe   # Windows
./raycaster       # Linux/macOS
```

---

## Contrôles

| Touche       | Action             |
|--------------|--------------------|
| `←` (gauche) | Tourner à gauche   |
| `→` (droite) | Tourner à droite   |
| `↑` (haut)   | Avancer            |
| `↓` (bas)    | Reculer            |
| Croix / Quitter | Fermer la fenêtre |

---

## Structure de la carte

La carte est définie dans le code source sous forme d'un tableau 2D d'entiers :

```c
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 0, 1, 0},
    {0, 0, 2, 2, 0},
    {3, 0, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1}
};
```

| Valeur | Signification     | Couleur  |
|--------|-------------------|----------|
| `0`    | Case vide         | —        |
| `1`    | Mur standard      | Blanc    |
| `2`    | Mur type 2        | Bleu     |
| `3`    | Mur type 3        | Rouge    |

---

## Paramètres configurables

Tous les paramètres principaux sont définis par des `#define` en haut du fichier source :

| Paramètre          | Valeur par défaut | Description                        |
|--------------------|-------------------|------------------------------------|
| `WIDTH`            | 900               | Largeur de la fenêtre (px)         |
| `HEIGHT`           | 600               | Hauteur de la fenêtre (px)         |
| `PLAYER_FOV`       | 80                | Champ de vision (degrés)           |
| `CELL_SIZE`        | 50                | Taille d'une cellule de la carte   |
| `RAY_STEP_SIZE`    | 0.1               | Précision du lancer de rayons      |
| `RENDER_DISTANCE`  | 250               | Distance de rendu maximale         |
| `PLAYER_ROT_SPD`   | 3                 | Vitesse de rotation (degrés/frame) |
| `PLAYER_MVT_SPD`   | 10                | Vitesse de déplacement             |

---

## Principe de fonctionnement

1. Pour chaque colonne de pixels à l'écran, un rayon est lancé depuis la position du joueur dans la direction correspondant à l'angle de vue.
2. Le rayon avance par petits incréments (`RAY_STEP_SIZE`) jusqu'à toucher un mur ou dépasser `RENDER_DISTANCE`.
3. La distance retournée détermine la hauteur de la colonne affichée : `hauteur = (CELL_SIZE × HEIGHT) / distance`.
4. La couleur de la colonne dépend du type de mur touché.

---

## Limitations connues

- Pas de correction de l'effet fisheye (distorsion aux bords du FOV)
- Pas de textures sur les murs
- Pas de détection de collision avec les murs
- Carte de taille fixe (5×5)