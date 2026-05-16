-- SafeAccess -- Parking Sécurisé
-- Script de création de la base PostgreSQL 16
-- Esteban HUCKEN -- BTS CIEL 2024-2026

CREATE DATABASE parking_securise;
\c parking_securise;

-- Table salarié
CREATE TABLE salarie (
    id_salarie SERIAL PRIMARY KEY,
    nom VARCHAR(50) NOT NULL,
    prenom VARCHAR(50) NOT NULL,
    badge_id VARCHAR(50) UNIQUE NOT NULL,
    empreinte_hash TEXT
);

-- Table véhicule (relation 1 salarié -> N véhicules)
CREATE TABLE vehicule (
    id_vehicule SERIAL PRIMARY KEY,
    plaque VARCHAR(20) UNIQUE NOT NULL,
    modele VARCHAR(50),
    id_salarie INT NOT NULL,
    FOREIGN KEY (id_salarie) REFERENCES salarie(id_salarie)
);

-- Table accès (historique complet)
CREATE TABLE acces (
    id_acces SERIAL PRIMARY KEY,
    id_salarie INT NOT NULL,
    id_vehicule INT NOT NULL,
    date_heure TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    type VARCHAR(10) CHECK (type IN ('ENTREE','SORTIE')),
    statut VARCHAR(10) CHECK (statut IN ('AUTORISE','REFUSE')),
    validation_garde BOOLEAN NOT NULL,
    FOREIGN KEY (id_salarie) REFERENCES salarie(id_salarie),
    FOREIGN KEY (id_vehicule) REFERENCES vehicule(id_vehicule)
);

-- Données de test
INSERT INTO salarie (nom, prenom, badge_id)
VALUES ('Dupont', 'Jean', 'BADGE001');

INSERT INTO vehicule (plaque, modele, id_salarie)
VALUES ('AB-123-CD', 'Peugeot 308', 1);