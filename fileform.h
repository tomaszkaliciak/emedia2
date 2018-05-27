#ifndef FILEFORM_H
#define FILEFORM_H

#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QFileDialog>

namespace Ui {
class FIleForm;
}

class FIleForm : public QWidget
{
    Q_OBJECT

public:
    explicit FIleForm(QWidget *parent = 0);
    ~FIleForm();

private:
    Ui::FIleForm *ui;
};

#endif // FILEFORM_H
