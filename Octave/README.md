# Prise en main

Script réalisé à l'IUT de Cachan pour Octave 6.3.0.  
Développée par Guillaume de Maricourt en 2021/22 sur le projet SaphTeamRacing, course de voiture autonomes.  

Ce programme écrit pour Octave permet de visualiser les données collectées par le LiDAR (voir le dossier [MBED](https://github.com/Widelx/saphteamracing/tree/main/MBED)).

Le programme est à lancer en remplaçcant la valeur du port COM sur lequel est connecté le module bluetooth par le port réel.
Exemple :
```
portCom = "COM10"; (par défaut)
portCom = "COM25";
```

**Guide Windows:**
1. Trouver le bon port COM
  
Il faut aller dans les périphériques connectés pour connaitre le port COM utilisé par votre PC pour la communication avec le module bluetooth.

2. Lancer l'application
  
  
Télécharger Octave : https://www.gnu.org/software/octave/download.html  
Lancer Octave, aller dans le répertoire ou se trouve le dossier `saphteamracing/Octave/` puis lancer dans l'invite de commande `_Lidar_Viewer.m`
```
_Lidar_Viewer.m
Appuyer sur entrée
```
