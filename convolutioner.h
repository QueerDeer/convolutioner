#ifndef CONVOLUTIONER_H
#define CONVOLUTIONER_H

#include <iostream>
#include <malloc.h>

#include <QObject>
#include <QStringList>

#include "opdouble.h"


class Convolutioner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString input1 READ input1 WRITE setInput1 NOTIFY onInput1Changed)
    Q_PROPERTY(QString input2 READ input2 WRITE setInput2 NOTIFY onInput2Changed)


public:
    explicit Convolutioner(QObject *parent = 0);

    void parser(const QString &array1, const QString &array2);

    void setInput1(const QString &);
    QString input1() const;

    void setInput2(const QString &);
    QString input2() const;

signals:
    void onInput1Changed();
    void onInput2Changed();

public slots:
    void computeApriory(const QString &array1, const QString &array2);
    void computeOverlapAdd(const QString &array1, const QString &array2);
    void computeOverlapSave(const QString &array1, const QString &array2);

private:
#define double OPdouble
    double *A;
    double *B;
    double *convolutionArray;
#undef double

    int lenA;
    int lenB;
    int lenConvolutionArray;
};

#endif // CONVOLUTIONER_H
