\# Code source – Interface Qt/C++



Interface de supervision du poste de garde SafeAccess.



\## Fichiers



| Fichier | Rôle |

|---------|------|

| `main.cpp` | Point d'entrée de l'application Qt |

| `mainwindow.h` | Déclaration de la classe MainWindow |

| `mainwindow.cpp` | Implémentation de l'interface et connexion BDD |



\## Fonctionnalités



\- Affichage des statuts en temps réel (plaque / badge / empreinte).

\- Boutons AUTORISER / REFUSER avec mise à jour de la BDD.

\- Historique des accès chargé depuis PostgreSQL.

\- Recherche manuelle par nom, prénom, plaque ou badge.

\- Simulation automatique (reset toutes les 5 secondes).



\## Dépendances



\- Qt / Qt Creator

\- Module Qt SQL avec driver QPSQL

\- PostgreSQL

