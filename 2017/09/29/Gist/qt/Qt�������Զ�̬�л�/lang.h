#ifndef LANG_H
#define LANG_H

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTranslator>

class LangUi : public QObject
{
    Q_OBJECT

public:
    void setupUi(QMainWindow* pa)
    {
        this->setParent(pa);
        this->btn = new QPushButton(pa);
        this->setTranslator();
    }

    void setTranslator()
    {
        this->btn->setText(tr("Toggle"));
    }

    QPushButton* btn;
};

class Lang : public QMainWindow
{
    Q_OBJECT

public:
    Lang(QWidget *parent = 0): QMainWindow(parent), ui(new LangUi()), trans(new QTranslator())
    {
        ui->setupUi(this);
        lang = 0;

        connect(this->ui->btn, &QPushButton::clicked, 
                [this]{
                    if (0 == lang)
                    {
                        lang = 1;
                        trans->load("zh.qm");
                    }
                    else if(1 == lang)
                    {
                        lang = 0;
                        trans->load("en.qm");
                    }
                    qApp->installTranslator(trans);
                    this->ui->setTranslator();
                });
    }
    ~Lang(){}

private:
    LangUi* ui;
    QTranslator* trans;
    int lang;
};

#endif // LANG_H
