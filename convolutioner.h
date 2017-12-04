#ifndef CONVOLUTIONER_H
#define CONVOLUTIONER_H

#include <iostream>
#include <malloc.h>
#include <math.h>

#include <QObject>
#include <QStringList>

#include "opdouble.h"


class Convolutioner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString input1 READ input1 WRITE setInput1 NOTIFY onInput1Changed)
    Q_PROPERTY(QString input2 READ input2 WRITE setInput2 NOTIFY onInput2Changed)
    Q_PROPERTY(QString factor READ factor WRITE setFactor NOTIFY onFactorChanged)
    Q_PROPERTY(QString point READ point WRITE setPoint NOTIFY onPointChanged)

public:
    explicit Convolutioner(QObject *parent = 0);

    int getClosestLog(const int number);

    void parser(const QString &array1, const QString &array2);

#define double OPdouble
    bool  FFT(double *Rdat, double *Idat, int N, int LogN, int Ft_Flag);
#undef double

    void setFactor(const QString &lenSection);
    QString factor() const;

    void setPoint(const QString &);
    QString point() const;

    void setInput1(const QString &);
    QString input1();

    void setInput2(const QString &);
    QString input2() const;

signals:
    void onInput1Changed(const QString nFrame);
    void onInput2Changed(const QString nFrame);
    void onFactorChanged(const QString lenSeq);
    void onPointChanged(const QString seq, const float coordY);

public slots:
    void getFactor(const QString &array1, const QString &array2);
    void computeAprioryLine(const QString &array1, const QString &array2);
    void computeAprioryCircle(const QString &array1, const QString &array2);
    void computeAprioryFFT(const QString &array1, const QString &array2);
    void computeOverlapAddLine(const QString &array1, const QString &array2);
    void computeOverlapAddCircle(const QString &array1, const QString &array2);
    void computeOverlapAddFFT(const QString &array1, const QString &array2);
    void computeOverlapSaveCircle(const QString &array1, const QString &array2);
    void computeOverlapSaveFFT(const QString &array1, const QString &array2);

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
