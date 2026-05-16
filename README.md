\# SafeAccess – Parking Sécurisé



> Projet BTS CIEL Cybersécurité 2024–2026.



Système de contrôle d'accès sécurisé pour parking industriel basé sur une

authentification multi-facteurs et une architecture réseau segmentée.



\## Présentation



SafeAccess sécurise l'accès au parking d'un site industriel sensible

(Jeumont Electric) via trois facteurs combinés :



\- Lecture automatique de plaque (caméra LAPI + OCR).

\- Badge RFID.

\- Empreinte digitale (lecteur biométrique 2-en-1).



Un agent de sécurité supervise chaque accès via une interface dédiée et prend la décision finale (autoriser / refuser).



\## Architecture



| Composant | Technologie |

|-----------|-------------|

| Interface garde | C++ / Qt |

| Base de données | PostgreSQL |

| Réseau | 4 VLAN segmentés, Firewall cœur de réseau |

| Prise de main distante | Bastion + Apache Guacamole |

| Stockage vidéo | Serveur NVR |



\## Structure du repo



\## Fonctionnement



1\. Le véhicule arrive → la caméra LAPI lit la plaque.

2\. L'employé badge + empreinte digitale.

3\. Le serveur vérifie les 3 facteurs en base de données.

4\. Le résultat s'affiche sur l'interface du garde.

5\. Le garde valide ou refuse manuellement.

6\. L'événement est enregistré avec horodatage.



\## Sécurité réseau



\- VLAN 10 – Capteurs (caméras + lecteur RFID/empreinte).

\- VLAN 20 – Serveurs (applicatif/bdd + NVR).

\- VLAN 30 – Bastion / Apache Guacamole (Prise de main).

\- VLAN 40 – Poste de garde (Interface QT + C++).

\- Politique \*\*Deny All\*\* par défaut sur le Firewall, puis flux selon protocoles utilisés.

\- Réseau 100% local, aucune sortie Internet.



\## Auteur



\*\*Esteban HUCKEN\*\* – BTS CIEL Cybersécurité  

Session 2024–2026

