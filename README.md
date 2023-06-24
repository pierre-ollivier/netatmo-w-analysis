# netatmo-w-analysis

Le but de ce projet est de fournir un outil de visualisation et d'analyse des données provenant d'une station météo Netatmo.

## Interface
L'interface du programme est une fenêtre, sur laquelle sont affichées les températures extérieure (en haut) et intérieure (en bas). Des graphiques configurables présentent l'évolution d'un paramètre donné (température, humidité, point de rosée ou humidex) sur les dernières 4, 24 ou 192 heures.

Des statistiques sont également affichées dans une zone à droite de la fenêtre.

### Menus
Trois menus sont disponibles :
- Menu Réseau : pour obtenir des informations de connectivité. C'est l'endroit où aller pour vérifier que le nombre d'appels à l'API Netatmo ne dépasse pas les limites (50 appels en 10 secondes glissantes, 500 appels en 60 minutes glissantes).
- Menu Données : c'est ici que les opérations relatives aux données sont disponibles. Il est possible d'ajouter, à partir d'un fichier .csv, les données d'un mois (données toutes les 5 minutes) ou encore de compléter la base de données qui compile, pour chaque journée, les valeurs moyennes et extrêmes de chaque paramètre mesuré ou calculé.
- Menu Climatologie : c'est ici qu'il est possible d'accéder aux résumés mensuels et annuels pour chaque paramètre. Le menu Climatologie donne également accès à la fenêtre Normales.

# Menu Données

## Normales

La page Normales donne accès à une représentation graphique d'un paramètre de mesure donné (température, humidité, point de rosée ou encore humidex) au fil de l'année en cours, ainsi que son écartement vis-à-vis de la normale de chaque jour (moyenne du paramètre sélectionné sur toutes les années précédentes, dans les X jours autour dudit jour).

Quatre options sont proposées pour chaque paramètre : valeur maximale sur une journée (en prenant en compte, dans le cas précis de la température extérieure, les normes de fenêtre temporelle de calcul), valeur minimale, valeur moyenne ou encore amplitude (différence entre le maximum et le minimum). 

Il est possible de choisir entre les données intérieures et les données extérieures.

# Menu Climatologie

## Rapport mensuel

Le rapport mensuel donne les valeurs minimales, maximales et moyennes d'un paramètre (à choisir parmi température, humidité, point de rosée, humidex et pression ; en intérieur ou en extérieur - dans le cas de la pression, les données seront identiques à l'intérieur et à l'extérieur).

La fenêtre de rapport mensuel est interactive et permet de changer de mois, que ce soit en naviguant vers le mois précédent ou suivant ou en sélectionnant soi-même un mois parmi les mois contenant des mesures.

L'absence de données pour un mois ou un jour précis est prise en compte dans l'affichage.

## Rapport annuel

Le rapport annuel de climatologie quotidienne est un tableau associé à un paramètre de mesure, à choisir parmi température, humidité, point de rosée, humidex ou pression. Pour chaque jour de l'année, les valeurs suivantes sont calculées et affichées :
- le minimum du minimum sur la journée du paramètre, ainsi que l'année concernée
- le maximum du minimum sur la journée du paramètre, ainsi que l'année concernée
- le minimum du maximum sur la journée du paramètre, ainsi que l'année concernée
- le maximum du maximum sur la journée du paramètre, ainsi que l'année concernée
- la valeur moyenne du minimum du paramètre
- la valeur moyenne du maximum du paramètre.

# Stockage interne des données

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

### OutdoorTimestampRecords
Cette table contient l'ensemble des données du module extérieur. Elle stocke :
- la date (en incluant le numéro de la semaine et de la décade),
- l'heure,
- la température,
- l'humidité,
- le point de rosée et l'humidex calculés en fonction de la température et de l'humidité.

## IndoorDailyRecords

Cette table agrège les données du module intérieur à l'échelle d'une journée. Elle stocke donc :
- la date (en incluant le numéro de la semaine et de la décade),
- la température maximale et son heure de première occurrence,
- la température minimale et son heure de première occurrence,
- la température moyenne,
- l'humidité maximale et son heure de première occirrence,
- l'humidité minimale et son heure de première occurrence,
- l'humidité moyenne,
- le point de rosée maximal et son heure de première occurrence,
- le point de rosée minimal et son heure de première occurrence,
- le point de rosée moyen,
- l'humidex maximal et son heure de première occurrence,
- l'humidex minimal et son heure de première occurrence,
- l'humidex moyen,
- la pression maximale et son heure de première occurrence,
- la pression minimale et son heure de première occurrence,
- la pression moyenne,
- le taux de CO2 maximal, minimal et miyen,
- le taux de bruit maximal, minimal et moyen.

### OutdoorDailyRecords

Cette table agrège les données du module extérieur à l'échelle d'une journée. Elle stocke donc :
- la température maximale (sur la période 6 UTC - 6 UTC) et son heure de première occurrence,
- la température minimale (sur la plage 18 UTC - 18 UTC) et son heure de première occurrence,
- la température moyenne,
- l'humidité maximale et son heure de première occurrence,
- l'humidité minimale et son heure de première occurrence,
- l'humidité moyenne,
- le point de rosée maximal et son heure de première occurrence,
- le point de rosée minimal et son heure de première occurrence,
- le point de rosée moyen,
- l'humidex maximal et son heure de première occurrence,
- l'humidex minimal et son heure de première occurrence,
- l'humidex moyen.
