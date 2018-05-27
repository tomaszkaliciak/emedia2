#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString mPath = "./home/tomasz/";
    dirModel = new QFileSystemModel(this);

    dirModel->setRootPath(QDir::currentPath());

    ui->treeView->setModel(dirModel);

    fileModel = new QFileSystemModel(this);

    fileModel->setFilter(QDir::NoDotAndDotDot |
                        QDir::Files);

    fileModel->setRootPath(mPath);

    ui->listView->setModel(fileModel);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_listView_clicked(const QModelIndex &index)
{
    QString mPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(mPath));
}
