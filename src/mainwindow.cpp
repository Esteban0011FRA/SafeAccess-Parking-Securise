#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color:#1e1e1e; color:white;");
    QVBoxLayout *layout = new QVBoxLayout(central);

    // ===== TITRE =====
    QLabel *title = new QLabel("SAFEACCESS - POSTE DE GARDE");
    title->setStyleSheet("font-size:22px; color:#00d4ff; font-weight:bold;");
    layout->addWidget(title);

    // ===== CAMERAS =====
    QHBoxLayout *camLayout = new QHBoxLayout();

    cam1 = new QLabel();
    cam1->setPixmap(QPixmap("C:/videos/camera_voiture.gif"));
    cam1->setScaledContents(true);
    cam1->setFixedSize(300, 180);
    cam1->setStyleSheet("border:1px solid #555;");

    cam2 = new QLabel();
    cam2->setPixmap(QPixmap("C:/videos/camera_garde.gif"));
    cam2->setScaledContents(true);
    cam2->setFixedSize(300, 180);
    cam2->setStyleSheet("border:1px solid #555;");

    camLayout->addWidget(cam1);
    camLayout->addWidget(cam2);
    layout->addLayout(camLayout);

    // ===== STATUTS =====
    QGridLayout *grid = new QGridLayout();

    labelPlaque = new QLabel("PLAQUE");
    labelBadge = new QLabel("BADGE");
    labelEmpreinte = new QLabel("EMPREINTE");

    statusPlaque = new QLabel("●");
    statusBadge = new QLabel("●");
    statusEmpreinte = new QLabel("●");

    statusPlaque->setStyleSheet("color:orange; font-size:24px;");
    statusBadge->setStyleSheet("color:orange; font-size:24px;");
    statusEmpreinte->setStyleSheet("color:orange; font-size:24px;");

    grid->addWidget(labelPlaque, 0, 0);
    grid->addWidget(statusPlaque, 0, 1);

    grid->addWidget(labelBadge, 1, 0);
    grid->addWidget(statusBadge, 1, 1);

    grid->addWidget(labelEmpreinte, 2, 0);
    grid->addWidget(statusEmpreinte, 2, 1);

    layout->addLayout(grid);

    // ===== STATUT GLOBAL =====
    globalStatus = new QLabel("EN ATTENTE");
    globalStatus->setAlignment(Qt::AlignCenter);
    globalStatus->setStyleSheet(
        "background:orange; color:black; font-size:18px; padding:8px; font-weight:bold;"
        );
    layout->addWidget(globalStatus);

    // ===== BOUTONS =====
    QHBoxLayout *btnLayout = new QHBoxLayout();

    btnAuthorize = new QPushButton("AUTORISER");
    btnRefuse = new QPushButton("REFUSER");

    btnAuthorize->setStyleSheet("background:green; color:white; padding:10px; font-weight:bold;");
    btnRefuse->setStyleSheet("background:red; color:white; padding:10px; font-weight:bold;");

    btnLayout->addWidget(btnAuthorize);
    btnLayout->addWidget(btnRefuse);

    layout->addLayout(btnLayout);

    // ===== RECHERCHE MANUELLE =====
    QLabel *searchTitle = new QLabel("RECHERCHE MANUELLE");
    searchTitle->setStyleSheet("font-size:18px; color:#00d4ff; font-weight:bold;");
    layout->addWidget(searchTitle);

    QHBoxLayout *searchLayout = new QHBoxLayout();

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Rechercher par nom, prénom, plaque ou badge...");
    searchInput->setStyleSheet(
        "background:#2b2b2b; color:white; padding:8px; border:1px solid #555;"
        );

    btnSearch = new QPushButton("RECHERCHER");
    btnSearch->setStyleSheet("background:#0078d7; color:white; padding:8px; font-weight:bold;");

    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(btnSearch);

    layout->addLayout(searchLayout);

    searchTable = new QTableWidget();
    searchTable->setColumnCount(5);
    searchTable->setHorizontalHeaderLabels({"Nom", "Prénom", "Badge", "Plaque", "Modèle"});
    searchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    searchTable->setStyleSheet("background:#252525; color:white; gridline-color:#555;");
    layout->addWidget(searchTable);

    // ===== HISTORIQUE =====
    QLabel *historyTitle = new QLabel("HISTORIQUE DES ACCÈS");
    historyTitle->setStyleSheet("font-size:18px; color:#00d4ff; font-weight:bold;");
    layout->addWidget(historyTitle);

    table = new QTableWidget();
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Nom", "Plaque", "Heure", "Type", "Statut"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setStyleSheet("background:#252525; color:white; gridline-color:#555;");
    layout->addWidget(table);

    setCentralWidget(central);
    resize(900, 800);

    // ===== BDD =====
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.20.10");
    db.setDatabaseName("parking_securise");
    db.setUserName("postgres");
    db.setPassword("");

    if (db.open()) {
        qDebug() << "BDD OK";
        loadHistorique();
    } else {
        qDebug() << "BDD ERROR:" << db.lastError().text();
    }

    // ===== SIMULATION AUTO =====
    simTimer = new QTimer(this);
    connect(simTimer, &QTimer::timeout, this, &MainWindow::simulate);
    simTimer->start(5000);

    // ===== CONNEXIONS =====
    connect(btnAuthorize, &QPushButton::clicked, this, &MainWindow::onAuthorize);
    connect(btnRefuse, &QPushButton::clicked, this, &MainWindow::onRefuse);
    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(searchInput, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
}

// ===== RESET UI =====
void MainWindow::simulate()
{
    state = !state;

    statusPlaque->setStyleSheet("color:orange; font-size:24px;");
    statusBadge->setStyleSheet("color:orange; font-size:24px;");
    statusEmpreinte->setStyleSheet("color:orange; font-size:24px;");

    globalStatus->setText("EN ATTENTE");
    globalStatus->setStyleSheet(
        "background:orange; color:black; font-size:18px; padding:8px; font-weight:bold;"
        );

    qDebug() << "Simulation reset";
}

// ===== INSERT BDD =====
void MainWindow::insertAcces(QString statut)
{
    if (!db.isOpen()) return;

    QSqlQuery q(db);

    q.prepare(
        "INSERT INTO acces "
        "(id_salarie, id_vehicule, date_heure, type, statut, validation_garde) "
        "VALUES (1, 1, NOW(), 'ENTREE', :statut, :val)"
        );

    q.bindValue(":statut", statut);
    q.bindValue(":val", statut == "AUTORISE");

    if (!q.exec()) {
        qDebug() << "INSERT ERROR:" << q.lastError().text();
    } else {
        qDebug() << "INSERT OK";
    }
}

// ===== BOUTON AUTORISER =====
void MainWindow::onAuthorize()
{
    statusPlaque->setStyleSheet("color:green; font-size:24px;");
    statusBadge->setStyleSheet("color:green; font-size:24px;");
    statusEmpreinte->setStyleSheet("color:green; font-size:24px;");

    globalStatus->setText("AUTORISÉ");
    globalStatus->setStyleSheet(
        "background:green; color:white; font-size:18px; padding:8px; font-weight:bold;"
        );

    insertAcces("AUTORISE");
    loadHistorique();
}

// ===== BOUTON REFUSER =====
void MainWindow::onRefuse()
{
    statusPlaque->setStyleSheet("color:red; font-size:24px;");
    statusBadge->setStyleSheet("color:red; font-size:24px;");
    statusEmpreinte->setStyleSheet("color:red; font-size:24px;");

    globalStatus->setText("REFUSÉ");
    globalStatus->setStyleSheet(
        "background:red; color:white; font-size:18px; padding:8px; font-weight:bold;"
        );

    insertAcces("REFUSE");
    loadHistorique();
}

// ===== HISTORIQUE =====
void MainWindow::loadHistorique()
{
    if (!db.isOpen()) return;

    table->setRowCount(0);

    QSqlQuery q(db);

    q.exec(
        "SELECT s.nom, v.plaque, a.date_heure, a.type, a.statut "
        "FROM acces a "
        "JOIN salarie s ON s.id_salarie = a.id_salarie "
        "JOIN vehicule v ON v.id_vehicule = a.id_vehicule "
        "ORDER BY a.date_heure DESC"
        );

    int r = 0;

    while (q.next()) {
        table->insertRow(r);

        for (int c = 0; c < 5; c++) {
            table->setItem(r, c, new QTableWidgetItem(q.value(c).toString()));
        }

        r++;
    }
}

// ===== RECHERCHE MANUELLE =====
void MainWindow::searchUser()
{
    if (!db.isOpen()) return;

    QString search = searchInput->text().trimmed();

    searchTable->setRowCount(0);

    if (search.isEmpty()) {
        return;
    }

    QSqlQuery q(db);

    q.prepare(
        "SELECT s.nom, s.prenom, s.badge_id, v.plaque, v.modele "
        "FROM salarie s "
        "JOIN vehicule v ON v.id_salarie = s.id_salarie "
        "WHERE LOWER(s.nom) LIKE LOWER(:search) "
        "OR LOWER(s.prenom) LIKE LOWER(:search) "
        "OR LOWER(v.plaque) LIKE LOWER(:search) "
        "OR LOWER(s.badge_id) LIKE LOWER(:search) "
        "ORDER BY s.nom ASC"
        );

    q.bindValue(":search", "%" + search + "%");

    if (!q.exec()) {
        qDebug() << "SEARCH ERROR:" << q.lastError().text();
        return;
    }

    int r = 0;

    while (q.next()) {
        searchTable->insertRow(r);

        for (int c = 0; c < 5; c++) {
            searchTable->setItem(r, c, new QTableWidgetItem(q.value(c).toString()));
        }

        r++;
    }
}

void MainWindow::onSearch()
{
    searchUser();
}
