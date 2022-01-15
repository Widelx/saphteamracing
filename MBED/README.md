Librairie développée à l'IUT de Cachan pour MBED OS v5.15 en C++ (issu de connaissances en C et non de C++).
Développée par Guillaume de Maricourt en 2021/22 sur le projet SaphTeamRacing, course de voiture autonomes.



/!\ Utiliser la librairie SaphTeamRacing_Librairie_working_well : l'autre dossier traite de l'algorithme vu en annexe mais n'est pas une version stable
/!\ Pour voir un exemple d'un programme utilisant correctement la librairie veuillez vous référer au fichier "main.cpp".


************************************************************************************
La librairie "lidar" permet d'utiliser un capteur LiDAR de la marque SLAMTEC en récupérant un jeu de données au degré prêt dans un tableau fourni par l'utilisateur.
ATTENTION : le tableau fourni par l'utilisateur doit ABSOLUMENT être de type "float" et de taille 181 (appelé ici tab).

2fonctions sont à utiliser :
-> initLidar();
-> getLidarData(tab);
Il faut bien entendu ajouter les fichier "lidar.cpp" et "lidar.h" dans le dossier du programme main.cpp et les inclure dans le projet.
Certaines propriétées peuvent être directement modifiées dans le ficher .h comme les pins de liaison vers le LiDAR ou le rapport cyclique du PWM.


************************************************************************************
Par ailleurs les données du LiDAR peuvent être visualisées via une connection série. Celle-ci peut être :
-> bluetooth
-> filaire
La librairie "bluetooth" d'utiliser cette fonctionnalité en parrallèle avec un script Octave(6.3.0) disponible dans ce repo.

ATTENTION : le tableau fourni par l'utilisateur doit ABSOLUMENT être de type "float" et de taille 181 (appelé ici tab).
---> Il y a une consistance entre les deux librairies

Il faut pour bénéficier de cette fonctionnalité ajouter la fonction 
--> initBLE();
AVANT --> sendOverBluetooth(tab);;


Comment utiliser les deux liaisons :
  1) Liaison filaire
La librairie est configurée de base pour transmettre en liaison filaire. Il ne devrait pas y avoir à faire ces modificcations. 
Cependant si un problème subsiste veuillez effectuer ces étapes en priorité.

--> Il faut modifier dans le fichier bluetooth.h les valeurs de TXBLE et RXBLE par les valeurs des pins à utiliser (dans notre cas USBRX et USBTX)
Exemple :
#define RXBLE USBRX<----- c'est cette valeur qui doit être changée
#define TXBLE USBTX<----- et celle-ci

  2) Liaison Bluetooth
La librairie n'est pas configurée de base pour émettre en bluetooth. Il faut pour cela connecter un module qui peut communiquer en RS232 et à 115200 bauds.
--> Il faut modifier dans le fichier bluetooth.h les valeurs de TXBLE et RXBLE par les valeurs des pins connectés sur le module bluetooth.

Exemple :
#define RXBLE PA_0<----- c'est cette valeur qui doit être changée
#define TXBLE PA_1<----- et celle-ci
Ici PA_0 et PA_1 servent d'exemple mais ne correspondent à aucune broche physique de microcontrôleur.

>--------------<
ENFIN : pour visualiser les données en Bluetooth veuillez vous référer à la section OCTAVE de ce git.


************************************************************************************
La librairie "car" permet de contrôler la voiture. Les fichiers ".h" permettent de configurer les PINs de contrôle PWM.