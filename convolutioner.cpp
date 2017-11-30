#include "convolutioner.h"

#define double OPdouble

Convolutioner::Convolutioner(QObject *parent) : QObject(parent)
{
    lenConvolutionArray = 0;
    lenA = 0;
    lenB = 0;
}

void Convolutioner::parser(const QString &array1, const QString &array2)
{
    QStringList list1 = array1.split(' ');
    QStringList list2 = array2.split(' ');

    A = (double*)malloc(list1.size() * sizeof(double));
    B = (double*)malloc(list2.size() * sizeof(double));

    for (auto i = 0; i < list1.size(); ++i)
        A[i] = std::stod(list1.at(i).toLocal8Bit().constData());

    for (auto i = 0; i < list2.size(); ++i)
        B[i] = std::stod(list2.at(i).toLocal8Bit().constData());

    lenA = list1.size();
    lenB = list2.size();
    lenConvolutionArray = lenA+lenB-1;

    convolutionArray = (double*) calloc(lenConvolutionArray, sizeof(double));
}


void Convolutioner::setInput1(const QString &) {
    emit onInput1Changed();
}

QString Convolutioner::input1() const {
    QString tmp;
    for (auto i = 0; i < lenConvolutionArray; ++i) {
        tmp += QString::number(convolutionArray[i].Value()) + "   ";
    }
    return tmp;
}

void Convolutioner::setInput2(const QString &) {
    emit onInput2Changed();
}

QString Convolutioner::input2() const {
    QString tmp;
    return tmp = "+ : " + QString::number(OPdouble::Adds())
            + "    - : " + QString::number(OPdouble::Subs())
            + "    * : " + QString::number(OPdouble::Muls())
            + "    / : " + QString::number(OPdouble::Divs())
            + "    n : " + QString::number(OPdouble::Negs());
}


void Convolutioner::computeApriory(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    //--------------------------------------------------------------------------
    int ii;
    double tmp;

    for ( auto i=0; i<lenConvolutionArray; ++i ) {

        ii = i;
        tmp = 0.0;
        for ( auto j=0; j<lenB; ++j ) {

            if ( ii>=0 && ii<lenA ) tmp = tmp + (A[ii]*B[j]);
            ii = ii-1;
            convolutionArray[i] = tmp;
        }
    }
    //--------------------------------------------------------------------------

    emit onInput1Changed();
    emit onInput2Changed();

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);

}

void Convolutioner::computeOverlapAdd(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    emit onInput1Changed();
    emit onInput2Changed();

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

void Convolutioner::computeOverlapSave(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    emit onInput1Changed();
    emit onInput2Changed();

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

#undef double
