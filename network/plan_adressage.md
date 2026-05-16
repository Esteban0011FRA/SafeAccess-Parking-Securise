\# Plan d'adressage IP – SafeAccess



Réseau 100% local – Aucune sortie Internet – Politique Deny All



\## VLAN et adressage



| VLAN | Rôle | Réseau | Masque | Passerelle (FW) |

|------|------|--------|--------|-----------------|

| 10 | Capteurs (caméras + RFID) | 192.168.10.0 | /26 | 192.168.10.1 |

| 20 | Serveurs (appli + NVR) | 192.168.20.0 | /26 | 192.168.20.1 |

| 30 | Bastion | 192.168.30.0 | /26 | 192.168.30.1 |

| 40 | Poste de garde | 192.168.40.0 | /26 | 192.168.40.1 |



\## Équipements



| Équipement | VLAN | Adresse IP |

|------------|------|------------|

| Caméra LAPI | 10 | 192.168.10.10 |

| Caméra surveillance | 10 | 192.168.10.11 |

| Lecteur RFID + empreinte | 10 | 192.168.10.20 |

| Serveur Applicatif + PostgreSQL | 20 | 192.168.20.10 |

| Serveur NVR | 20 | 192.168.20.11 |

| Serveur Bastion + Guacamole | 30 | 192.168.30.10 |

| PC Poste de garde | 40 | 192.168.40.10 |



\## Flux autorisés (Firewall)



| Source | Destination | Protocole | Service |

|--------|-------------|-----------|---------|

| VLAN 10 | VLAN 20 (NVR) | RTSP 554 | Flux vidéo |

| VLAN 10 | VLAN 20 (Serveur) | POSTGRESQL 5432| API vérification |

| VLAN 20 | VLAN 30 (Bastion) | SSH 22 / HTTPS 443 / RTSP 554 / POSTGRESQL 5432 | Remontée infos |

| VLAN 30 | VLAN 20 (Serveurs) | SSH 22 / HTTPS 443 | Admin distante |

| VLAN 40 | VLAN 30 (Bastion) | SSH 22 / HTTPS 443 | Accès application |



\## Flux interdits



Tout autre flux est refusé par défaut (Deny All).

Aucune sortie Internet autorisée.

