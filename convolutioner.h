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
public:
    explicit Convolutioner(QObject *parent = 0);

    void parser(const QString &array1, const QString &array2);

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
