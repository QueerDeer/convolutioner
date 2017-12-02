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
    Q_PROPERTY(QString foo READ foo WRITE setFoo NOTIFY onFooChanged)


public:
    explicit Convolutioner(QObject *parent = 0);

    void parser(const QString &array1, const QString &array2);

    void setInput1(const QString &);
    QString input1() const;

    void setInput2(const QString &);
    QString input2() const;

    void setFoo(const QString &lenSection);
    QString foo() const;

signals:
    void onInput1Changed(const QString nFrame);
    void onInput2Changed(const QString nFrame);
    void onFooChanged(const QString lenSeq);

public slots:
    void wound(const QString &array1, const QString &array2);
    void computeAprioryLine(const QString &array1, const QString &array2);
    void computeAprioryCircle(const QString &array1, const QString &array2);
    void computeOverlapAddLine(const QString &array1, const QString &array2);
    void computeOverlapAddCircle(const QString &array1, const QString &array2);
    void computeOverlapSaveLine(const QString &array1, const QString &array2);
    void computeOverlapSaveCircle(const QString &array1, const QString &array2);

private:
#define double OPdouble
    double stub;
    double *A;
    double *B;
    double *convolutionArray;
#undef double

    int lenA;
    int lenB;
    int lenConvolutionArray;
    int _lenSection;
};

#endif // CONVOLUTIONER_H
