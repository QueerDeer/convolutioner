#include "convolutioner.h"

Convolutioner::Convolutioner(QObject *parent) : QObject(parent)
{

}

void Convolutioner::computeApriory(const QString &array1, const QString &array2)
{
    QStringList list1 = array1.split(' ');
    QStringList list2 = array2.split(' ');

    double A[list1.size()];
    double B[list2.size()];

    for (auto i = 0; i < list1.size(); ++i)
             A[i] = std::stod(list1.at(i).toLocal8Bit().constData());

    for (auto i = 0; i < list2.size(); ++i)
             B[i] = std::stod(list2.at(i).toLocal8Bit().constData());

    int lenA = sizeof(A)/sizeof(A[0]);
    int lenB = sizeof(B)/sizeof(B[0]);
    int lenConvolutionArray = lenA+lenB-1;

    double *convolutionArray = (double*) calloc(lenConvolutionArray, sizeof(double));

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

    for (auto i=0; i<lenConvolutionArray; ++i) {
        std::cout << convolutionArray[i] << " ";
    }

    std::cout << "computed by definition" << std::endl;

}

void Convolutioner::computeOverlapAdd(const QString &array1, const QString &array2)
{
    std::cout << "computed by overlap-add method" << std::endl;
}

void Convolutioner::computeOverlapSave(const QString &array1, const QString &array2)
{
    std::cout << "computed by overlap-save method" << std::endl;
}
