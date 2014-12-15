#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helper.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <QJsonObject>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void on_change_category();
private:
    Ui::MainWindow *ui;
    unsigned int position;
    std::vector<RowPtr> websites;
    std::map<String, std::shared_ptr<CategoryNode> > category_key_index;
    std::map<unsigned int, String> category_id_index;
    QJsonObject sites_categories;
    void load_sites_categories(const QString& file_name);
    void index_categories();
    void index_old_sites();
    void load_new_sites(const QString& file_name);
};

#endif // MAINWINDOW_H
