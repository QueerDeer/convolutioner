#include "convolutioner.h"

#define double OPdouble

Convolutioner::Convolutioner(QObject *parent) : QObject(parent)
{
    lenConvolutionArray = 0;
    lenA = 0;
    lenB = 0;
    stub = 0;
}

void Convolutioner::getFactor(const QString &array1, const QString &array2) {

    QStringList list1 = array1.split(' ');
    QStringList list2 = array2.split(' ');

    lenA = list1.size();
    lenB = list2.size();

    int x = (lenA < lenB)? lenB : lenA;

    for(int k = 1; k <= std::sqrt(x); ++k)
        if (x % k == 0) {
            emit onFactorChanged(QString::number(k));

            if (k != std::sqrt(x))
                emit onFactorChanged(QString::number(x/k));
        }

    lenA = lenB = 0;
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

    lenConvolutionArray = (lenA + lenB - 1);
    convolutionArray = (double*)malloc(lenConvolutionArray * sizeof(double));
    std::fill_n(convolutionArray, lenConvolutionArray, stub);
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


void Convolutioner::setFactor(const QString &lenSection) {
    _lenSection = lenSection.toInt();
}

QString Convolutioner::factor() const {
    return "";
}


// Algorythms are not separated by more simple functions, like 'separate' 'linear', etc.
// Of course, if i do, it'll reduce amount of code, but also reduce my own free time, damn

//Свертка в лоб, линейная
void Convolutioner::computeAprioryLine(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    int ii;
    double tmp;

    for ( auto i=0; i<lenConvolutionArray; ++i ) {
        ii = i;
        tmp = 0.0;

        for ( auto j=0; j<lenB; ++j ) {

            if ( ii>=0 && ii<lenA ) tmp += (A[ii]*B[j]);
            --ii;
        }

        convolutionArray[i] += tmp;
    }

    emit onInput1Changed("frame1");
    emit onInput2Changed("frame1");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);

}
// Свертка в лоб, считаем как круговую (апериодическую)
void Convolutioner::computeAprioryCircle(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    int tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;

    double* section1 = (double*)malloc(lenConvolutionArray * sizeof(double));
    double* section2 = (double*)malloc(lenConvolutionArray * sizeof(double));

    std::fill_n(section1, lenConvolutionArray, stub);
    std::fill_n(section2, lenConvolutionArray, stub);

    memcpy(section1, A, lenA * sizeof *A);
    memcpy(section2, B, lenB * sizeof *B);

    for ( auto i=0; i<lenConvolutionArray; ++i)
        for ( auto j = 0; j<lenConvolutionArray; ++j) {

            tmp = ((i-j) < 0)? lenConvolutionArray - j  + lenFilter - 1: (i-j);
            convolutionArray[i] += section1[j]*section2[tmp];
        }

    free(section1);
    free(section2);

    emit onInput1Changed("frame1");
    emit onInput2Changed("frame1");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);

}
// Перекрытие с суммированием, промежуточные свертки вычисляем линейно
void Convolutioner::computeOverlapAddLine(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    int jj;
    double tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;

    double* section1 = (double*)malloc(lenSection * sizeof(double));
    double* section2 = (double*)malloc(lenFilter * sizeof(double));

    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {

        if (lenFilter == lenB) {
            memcpy(section1, A+i, lenSection * sizeof *A);
            memcpy(section2, B, lenFilter * sizeof *A);
        } else {
            memcpy(section1, B+i, lenSection * sizeof *B);
            memcpy(section1, A, lenFilter * sizeof *A);
        }

        for ( auto j=0; j<lenFilter + lenSection - 1; ++j ) {
            jj = j;
            tmp = 0.0;

            for ( auto k=0; k<lenFilter; ++k ) {

                if ( jj>=0 && jj<lenSection ) tmp += (section1[jj]*section2[k]);
                --jj;
            }

            convolutionArray[j+i] += tmp;
        }
    }

    free(section1);
    free(section2);

    emit onInput1Changed("frame2");
    emit onInput2Changed("frame2");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}
// Перекрытие с суммированием, промежуточные свертки круговые (апериодические)
void Convolutioner::computeOverlapAddCircle(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    int tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;

    double* section1 = (double*)malloc((lenSection+lenFilter-1) * sizeof(double));
    double* section2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));

    std::fill_n(section1, lenSection+lenFilter-1, stub);
    std::fill_n(section2, lenSection+lenFilter-1, stub);

    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {

        if (lenFilter == lenB) {
            memcpy(section1, A+i, lenSection * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i, lenSection * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        for ( auto j=0; j<lenSection + lenFilter - 1; ++j)
            for ( auto k = 0; k<lenSection + lenFilter - 1 + lenFilter - 1; ++k) {

                tmp = ((j-k) < 0)? lenSection + lenFilter - 1 - k : (j-k);
                convolutionArray[j+i] += section1[k]*section2[tmp];
            }

    }
    free(section1);
    free(section2);

    emit onInput1Changed("frame2");
    emit onInput2Changed("frame2");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}
// Перекрытие с накоплением, промежуточные свертки линейные
void Convolutioner::computeOverlapSaveLine(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    //

    emit onInput1Changed("frame3");
    emit onInput2Changed("frame3");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}
// Перекрытие с накоплением, промежуточные свертки круговые (апериодические)
void Convolutioner::computeOverlapSaveCircle(const QString &array1, const QString &array2)
{
    parser(array1, array2);

    int tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;

    double* section1 = (double*)malloc((lenSection+lenFilter-1) * sizeof(double));
    double* section2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* section3 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));


    std::fill_n(section1, lenSection+lenFilter-1, stub);
    std::fill_n(section2, lenSection+lenFilter-1, stub);
    std::fill_n(section3, lenSection+lenFilter-1, stub);


    if (lenFilter == lenB) {
        memcpy(section1+lenFilter-1, A, lenSection * sizeof *A);
        memcpy(section2, B, lenB * sizeof *B);
    } else {
        memcpy(section1+lenFilter-1, B, lenSection * sizeof *B);
        memcpy(section2, A, lenA * sizeof *A);
    }

    for ( auto j=0; j<lenSection + lenFilter - 1; ++j)
        for ( auto k = 0; k<lenSection + lenFilter - 1; ++k) {
            tmp = ((j-k) < 0)? lenSection + lenFilter - 1 - k + lenFilter - 1 : (j-k);
            section3[j] += section1[k]*section2[tmp];
        }

    for ( auto l=lenFilter-1; l<lenSection + lenFilter - 1; ++l)
        convolutionArray[l-lenFilter+1] = section3[l];


    for ( auto i=lenSection; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {

        if (lenFilter == lenB) {
            memcpy(section1, A+i-lenFilter-1, (lenSection+lenFilter) * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i-lenFilter-1, (lenSection+lenFilter) * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        for ( auto j=0; j<lenSection + lenFilter - 1; ++j)
            for ( auto k = 0; k<lenSection + lenFilter - 1; ++k) {
                tmp = ((j-k) < 0)? lenSection + lenFilter - 1 - k + lenFilter - 1+j: (j-k);
                section3[j+i] += section1[k]*section2[tmp];
            }

        for ( auto l=lenFilter-1; l<lenSection + lenFilter - 1; ++l)
            convolutionArray[l-lenFilter+1+i] = section3[l];

    }
    //free(section1);
    //free(section2);
    //free(section3);

    emit onInput1Changed("frame3");
    emit onInput2Changed("frame3");

    OPdouble::ClearOps();
    free (A);
    free (B);
    //free (convolutionArray);
}

#undef double
