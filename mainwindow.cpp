#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebKitWidgets/QWebView>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>
#include "helper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    position(0),
    websites(),
    category_key_index(),
    category_id_index(),
    sites_categories()
{
    ui->setupUi(this);
    load_sites_categories("/home/anhdo/workspace/site_categories.json");
    index_categories();
    load_new_sites("/home/anhdo/workspace/source_2014_12_10.csv");
    connect(ui->button_next,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
    connect(ui->combo_main_cat, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_change_category()));
}

void MainWindow::load_sites_categories(const QString& file_name) {
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QJsonParseError jerror;
    sites_categories = QJsonDocument::fromJson(file.readAll(),&jerror).object();
    file.close();
}

void MainWindow::index_categories(){
    QJsonArray qt_category = sites_categories["categories"].toArray();
    foreach (const QJsonValue & val, qt_category)
    {
        QJsonArray sub_arr = val.toArray();
        int category_id = sub_arr[0].toInt();
        String category_title = sub_arr[1].toString().toStdString();
        int parent_category_id = (sub_arr.size() == 2)? 0 : sub_arr[2].toInt();
        std::shared_ptr<CategoryNode> new_node = std::make_shared<CategoryNode>(category_id, category_title, parent_category_id);
        if(!category_id_index.find(category_id)) {
            String key = "root@";
            while(parent_category_id > 0){
                key += category_title;
                if(!category_key_index.find(key)) {
                    category_key_index[key] = new_node;
                    category_id_index[category_id] = key;
                }
            }// end of parent_category_id > 0
        }
    }
}

void MainWindow::index_old_sites(){
    QJsonObject qt_sites = sites_categories["sites"].toObject();
    QStringList keys_list = qt_sites.keys();
    for(int i = 1; i < keys_list.size()-1; i++) {
        QString key = keys_list[i];
        String std_key = key.toStdString();
        normalize_text(std_key, true);
        std_key.erase(0,3);
        std_key.erase(std_key.size() - 2);
        ui->combo_sub_one->addItem(QString::fromStdString(std_key) + QString::fromStdString(" " + std::to_string(qt_sites[key].toInt())));
    }

}

void MainWindow::load_new_sites(const QString& file_name) {
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }
    QTextStream in(&file);
    unsigned int line_cnt = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split("\t");
        std::vector<String> row_vec;
        for(int i = 0; i < fields.size(); i++) {
            row_vec.push_back(fields[i].toStdString());
        }
        if(row_vec.size() >= 4 && !row_vec[0].empty() && line_cnt++ > 1) {
            RowPtr row = std::make_shared<Row>();
            row->url = row_vec[0];
            row->main_cat = row_vec[1];
            row->sub_cat_first = row_vec[2];
            row->sub_cat_second = row_vec[3];
            websites.push_back(row);
        }
     }
    file.close();
}


void MainWindow::on_change_category() {

}

void MainWindow::on_pushButton_clicked() {
    static int i = 0;
    i++;// = (i+1 > ui->combo_main_cat->size()) ? i+1 : 0;
    ui->combo_main_cat->setCurrentIndex(i);
    /*
    if(position < category.size()) {
        position++;
    }
    RowPtr row = category[position];
    String url = row->url;
    url.erase(0,3);
    url.erase(url.size()-3);
    ui->webView->load(QUrl(("http://"+url).c_str()));
    ui->textEdit->setText(QString::fromStdString(url));
    String size_of_vec(std::to_string(position));
    ui->label_url_size->setText(size_of_vec.c_str());
    ui->combo_main_cat->addItem(QString::fromStdString(row->main_cat));
    ui->combo_sub_one->addItem(QString::fromStdString(row->sub_cat_first));
    ui->combo_sub_two->addItem(QString::fromStdString(row->sub_cat_second));
    */
}


MainWindow::~MainWindow()
{
    delete ui;
}
/*
const String& url("http://vnexpress.net");
ui->txt_url_var->setText(url.c_str());
ui->webView->load(QUrl(ui->txt_url_var->text()));
*/
