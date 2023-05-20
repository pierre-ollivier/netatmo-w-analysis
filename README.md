# netatmo-w-analysis

Le but de ce projet est de fournir un outil de visualisation et d'analyse des données provenant d'une station météo Netatmo.

## Interface
L'interface du programme est une fenêtre, sur laquelle sont affichées les températures extérieure (en haut) et intérieure (en bas).

### Menus
Trois menus sont disponibles :
- Menu Réseau : pour obtenir des informations de connectivité. C'est l'endroit où aller pour vérifier que le nombre d'appels à l'API Netatmo ne dépasse pas les limites (50 appels en 10 secondes glissantes, 500 appels en 60 minutes glissantes).
- Menu Données : c'est ici que les opérations relatives aux données sont disponibles. Il est possible d'ajouter, à partir d'un fichier .csv, les données d'un mois (données toutes les 5 minutes) ou encore de compléter la base de données qui compile, pour chaque journée, les valeurs moyennes et extrêmes de chaque paramètre mesuré ou calculé.
- Menu Climatologie : c'est ici qu'il est possible d'accéder aux résumés mensuels et annuels pour chaque paramètre.

## Données

Les données sont stockées dans la base de données. Celle-ci contient plusieurs tables :

### sqlite-sequence
Cette table est utilisée pour s'assurer de l'unicité des identifiants dans les autres tables.

### IndoorTimestampRecords
Cette table contient l'ensemble des relevés du module intérieur. Elle stocke :
- la date (en incluant le numéro de la semaine et de la décade),
- l'heure,
- la température,
- l'humidité,
- la pression,
- le niveau de CO2,
- le niveau de bruit,
- le point de rosée et l'humidex, calculés en fonction de la température et de l'humidité.
