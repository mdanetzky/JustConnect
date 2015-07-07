#ifndef UIDIAGNOSTICS_H
#define UIDIAGNOSTICS_H

#include <QWidget>

namespace Ui {
class UIDiagnostics;
}

class UIDiagnostics : public QWidget
{
    Q_OBJECT

public:
    explicit UIDiagnostics(QWidget *parent = 0);
    ~UIDiagnostics();

public slots:
    void textChanged();
    void modemAnswer(QString answer);
signals:
    void commandAT(QString);
private:
    Ui::UIDiagnostics *ui;
};

#endif // UIDIAGNOSTICS_H
