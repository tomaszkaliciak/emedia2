#include "fileform.h"
#include "ui_fileform.h"

FIleForm::FIleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FIleForm)
{
    ui->setupUi(this);
}

FIleForm::~FIleForm()
{
    delete ui;
}
