# Prise en main

Librairies développées à l'IUT de Cachan pour MBED OS v5.15 en C++ (issu de connaissances en C et non de C++).
Développée par Guillaume de Maricourt en 2021/22 sur le projet SaphTeamRacing, course de voiture autonomes.

**/!\ Utiliser la librairie SaphTeamRacing_Librairie_working_well** : l'autre dossier traite de l'algorithme vu en annexe du rapport mais n'est pas une version stable
/!\ Pour voir un exemple d'un programme utilisant correctement la librairie veuillez vous référer au fichier "main.cpp".


### Lidar
La librairie "lidar" permet d'utiliser un capteur LiDAR de la marque SLAMTEC en récupérant un jeu de données au degré prêt dans un tableau fourni par l'utilisateur.
**ATTENTION :** le tableau fourni par l'utilisateur doit ABSOLUMENT être de type "float" et de taille 181 (ici tab).

2fonctions sont à utiliser :
```
initLidar();
getLidarData(tab);
```
Il faut bien entendu ajouter les fichier `lidar.cpp` et `lidar.h` dans le dossier du programme main.cpp et les inclure dans le projet.
Certaines propriétées peuvent être directement modifiées dans le ficher .h le rapport cyclique du PWM ou la distance maximale d'acquisition.


### BLE (Bluetooth Low Energy)
Par ailleurs les données du LiDAR peuvent être visualisées via une connection série. Celle-ci peut être :
- bluetooth
- filaire
La librairie "bluetooth" permet d'utiliser cette fonctionnalité en parrallèle avec un [script](https://github.com/Widelx/saphteamracing/tree/main/Octave) créé sur [Octave](https://www.gnu.org/software/octave/index)(6.3.0) disponible dans ce repoistory.

**ATTENTION :** le tableau fourni par l'utilisateur doit ABSOLUMENT être de type "float" et de taille 181 (ici tab).
_Consistance entre les deux librairies._

Il faut pour bénéficier de cette fonctionnalité ajouter les fonctions suivantes :
```
initBLE();
sendOverBluetooth(tab);
```
Il faut bien entendu ajouter les fichier `bluetooth.cpp` et `bluetooth.h` dans le dossier du programme main.cpp et les inclure dans le projet.

Comment utiliser les deux types liaisons :
1. Liaison filaire
La librairie est configurée de base pour transmettre en liaison filaire. Il ne devrait pas y avoir à faire ces modificcations. 

Cependant si un problème subsiste veuillez effectuer les étapes suivantes.
Modifier dans le fichier `bluetooth.h` les valeurs de `TXBLE` et `RXBLE` par les valeurs des pins à utiliser (dans notre cas `USBRX` et `USBTX`)
Exemple :
```
#define RXBLE USBRX
#define TXBLE USBTX
```

2. Liaison Bluetooth
La librairie n'est pas configurée de base pour émettre en bluetooth. Il faut pour cela connecter un module qui peut communiquer en RS232 et à 115200 bauds.
Il faut modifier dans le fichier `bluetooth.h` les valeurs de `TXBLE` et `RXBLE` par les valeurs des pins connectés sur le module bluetooth.

Exemple :
```
#define RXBLE PA_0
#define TXBLE PA_1
```

Visualisation des données en Bluetooth 
Veuillez vous référer à la section [OCTAVE](https://github.com/Widelx/saphteamracing/tree/main/Octave) de ce git.

### Car
La librairie "car" permet de contrôler la voiture. Les fichiers ".h" permettent de configurer les PINs de contrôle des signaux PWM.
