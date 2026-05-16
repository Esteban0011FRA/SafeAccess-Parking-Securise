#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QTimer>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QLabel *cam1;
    QLabel *cam2;

    QLabel *labelPlaque;
    QLabel *labelBadge;
    QLabel *labelEmpreinte;

    QLabel *statusPlaque;
    QLabel *statusBadge;
    QLabel *statusEmpreinte;

    QLabel *globalStatus;

    QPushButton *btnAuthorize;
    QPushButton *btnRefuse;
    QPushButton *btnSearch;

    QLineEdit *searchInput;

    QTableWidget *table;
    QTableWidget *searchTable;

    QSqlDatabase db;

    QTimer *simTimer;
    bool state = false;

    void loadHistorique();
    void insertAcces(QString statut);
    void searchUser();

private slots:
    void simulate();
    void onAuthorize();
    void onRefuse();
    void onSearch();
};

#endif