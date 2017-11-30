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
}


void Convolutioner::setInput1(const QString &) {
}

QString Convolutioner::input1() const {
    QString tmp;
    for (auto i = 0; i < lenConvolutionArray; ++i)
        tmp += QString::number(convolutionArray[i].Value()) + "   ";
    return tmp;
}

void Convolutioner::setInput2(const QString &) {
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
    lenConvolutionArray = lenA+lenB-1;
    convolutionArray = (double*)calloc(lenConvolutionArray, sizeof(double));

    int ii;
    double tmp;

    for ( auto i=0; i<lenConvolutionArray; ++i ) {
        ii = i;
        tmp = 0.0;

        for ( auto j=0; j<lenB; ++j ) {
            if ( ii>=0 && ii<lenA ) tmp += (A[ii]*B[j]);
            --ii;
            convolutionArray[i] = tmp;
        }
    }
    //--------------------------------------------------------------------------

    emit onInput1Changed("frame1");
    emit onInput2Changed("frame1");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);

}

void Convolutioner::computeOverlapAdd(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    //--------------------------------------------------------------------------
    int tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = lenFilter*lenFilter;

    lenConvolutionArray = (lenA+lenB-1)*lenSection;
    convolutionArray = (double*)calloc(lenConvolutionArray, sizeof(double));

    double *section1;
    double *section2;

    for ( auto i=0; i<lenConvolutionArray; i+=lenSection ) {
        section1 = (double*)calloc(lenSection + lenFilter - 1, sizeof(double));
        section2 = (double*)calloc(lenFilter + lenSection - 1, sizeof(double));

        if (lenFilter == lenB) {
            memcpy(section1, &A[lenSection], lenSection * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, &B[lenSection], lenSection * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        for ( auto j=0; j<lenSection + lenFilter - 1; ++j)
            for ( auto k = 0; k<lenSection + lenFilter - 1; ++k) {
                tmp = ((j-k) < 0)? lenSection + lenFilter - 1 - k : (j-k);
                convolutionArray[j+i] += section1[k]*section2[tmp];

            }
    }
    //--------------------------------------------------------------------------

    emit onInput1Changed("frame2");
    emit onInput2Changed("frame2");

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

    emit onInput1Changed("frame3");
    emit onInput2Changed("frame3");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

#undef double
