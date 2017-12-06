#include "convolutioner.h"

#define double OPdouble

#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))
#define  FT_DIRECT        -1    // Direct transform.
#define  FT_INVERSE        1    // Inverse transform.

Convolutioner::Convolutioner(QObject *parent) : QObject(parent)
{
    lenConvolutionArray = 0;
    lenA = 0;
    lenB = 0;
    stub = 0;
    _lenSection = 0;

    file = new QFile;
    file->setFileName("log.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}

int Convolutioner::getClosestLog(const int number)
{
    if(!NUMBER_IS_2_POW_K(number)) {
        int tmp = 1<<((int)(std::log2(number))+1);
        return tmp;
    }
    else {
        return number;
    }
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


void Convolutioner::setFactor(const QString &lenSection) {
    _lenSection = lenSection.toInt();
}

QString Convolutioner::factor() const {
    return QString::number(std::max(lenA, lenB));
}

void Convolutioner::setPoint(const QString &) {
}


QString Convolutioner::point() const {
    return "";
}


void Convolutioner::parser(const QString &array1, const QString &array2, int logFlag)
{
    emit onPointChanged("clear", 0);

    QStringList list1 = array1.split(' ');
    QStringList list2 = array2.split(' ');

    A = (double*)malloc(list1.size() * sizeof(double));
    B = (double*)malloc(list2.size() * sizeof(double));

    for (auto i = 0; i < list1.size(); ++i) {
        A[i] = std::stod(list1.at(i).toLocal8Bit().constData());
        emit onPointChanged("seq1", A[i].Value());
    }

    for (auto i = 0; i < list2.size(); ++i) {
        B[i] = std::stod(list2.at(i).toLocal8Bit().constData());
        emit onPointChanged("seq2", B[i].Value());
    }

    lenA = list1.size();
    lenB = list2.size();

    if (logFlag == 2) {
        int lenFilter = std::min(lenA, lenB);
        int tmp = (std::max(lenA, lenB))/_lenSection;
        _lenSection = getClosestLog(_lenSection + lenFilter - 1) - lenFilter + 1;
        lenConvolutionArray = _lenSection*tmp+lenFilter-1;
    }
    if (logFlag == 1) {
        lenConvolutionArray = getClosestLog(lenA + lenB -1);
    }
    if (logFlag == 0) {
        lenConvolutionArray = (lenA + lenB - 1);
    }

    convolutionArray = (double*)malloc(lenConvolutionArray * sizeof(double));
    std::fill_n(convolutionArray, lenConvolutionArray, stub);
}


bool Convolutioner::FFT(double *Rdat, double *Idat, int N, int LogN, int Ft_Flag)
{
    if((Rdat == NULL) || (Idat == NULL))                  return false;
    if((N > 16384) || (N < 1))                            return false;
    if(!NUMBER_IS_2_POW_K(N))                             return false;
    if((LogN < 2) || (LogN > 14))                         return false;
    if((Ft_Flag != FT_DIRECT) && (Ft_Flag != FT_INVERSE)) return false;

    register int  i, j, n, k, io, ie, in, nn;
    double         ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

    static const double Rcoef[14] =
    {  -1.0000000000000000F, 0.0000000000000000F,  0.7071067811865475F,
       0.9238795325112867F,  0.9807852804032304F,  0.9951847266721969F,
       0.9987954562051724F,  0.9996988186962042F,  0.9999247018391445F,
       0.9999811752826011F,  0.9999952938095761F,  0.9999988234517018F,
       0.9999997058628822F,  0.9999999264657178F
    };
    static const double Icoef[14] =
    {   0.0000000000000000F,  -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    nn = N >> 1;
    ie = N;
    for(n=1; n<=LogN; n++)
    {
        rw = Rcoef[LogN - n];
        iw = Icoef[LogN - n];
        if(Ft_Flag == FT_INVERSE) iw = -iw;
        in = ie >> 1;
        ru = 1.0F;
        iu = 0.0F;
        for(j=0; j<in; j++)
        {
            for(i=j; i<N; i+=ie)
            {
                io       = i + in;
                rtp      = Rdat[i]  + Rdat[io];
                itp      = Idat[i]  + Idat[io];
                rtq      = Rdat[i]  - Rdat[io];
                itq      = Idat[i]  - Idat[io];
                Rdat[io] = rtq * ru - itq * iu;
                Idat[io] = itq * ru + rtq * iu;
                Rdat[i]  = rtp;
                Idat[i]  = itp;
            }

            sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }

        ie >>= 1;
    }

    for(j=i=1; i<N; i++)
    {
        if(i < j)
        {
            io       = i - 1;
            in       = j - 1;
            rtp      = Rdat[in];
            itp      = Idat[in];
            Rdat[in] = Rdat[io];
            Idat[in] = Idat[io];
            Rdat[io] = rtp;
            Idat[io] = itp;
        }

        k = nn;

        while(k < j)
        {
            j   = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    if(Ft_Flag == FT_DIRECT) return true;

    rw = 1.0F / N;

    for(i=0; i<N; i++)
    {
        Rdat[i] *= rw;
        Idat[i] *= rw;
    }

    return true;
}

void Convolutioner::fhtDitIter(double *x, unsigned long n) {
    unsigned long m;

    for (m = 2; m <= n; m <<= 1) {
        double a, b, c, s, t;
        unsigned long i, j, k, mh, mq;
        mh = m >> 1;
        mq = mh >> 1;
        t = M_PI / (double)mh;
        a = sin(0.5 * t.Value());
        a *= 2.0 * a;
        b = sin(t.Value());
        c = 1.0;
        s = 0.0;
        for (j = 1, k = mh - 1; j < mq; ++j, --k) {
            double tmp;
            double *xj, *xk;
            xj = x + j + mh;
            xk = x + k + mh;
            tmp = c;
            c -= a * c + b * s;
            s -= a * s - b * tmp;
            for (i = 0; i < n; i += m) {
                double u, v;
                u = xj[i];
                v = xk[i];
                xj[i] = u * c + v * s;
                xk[i] = u * s - v * c;
            }
        }
        for (i = 0; i < n; i += m) {
            double *xp;
            xp = x + i;
            for (j = 0, k = mh; j < mh; ++j, ++k) {
                double u, v;
                u = xp[j];
                v = xp[k];
                xp[j] = u + v;
                xp[k] = u - v;
            }
        }
    }
    return;
}

void Convolutioner::bitrevPermuteReal(double *x, unsigned long n) {
    unsigned long i;
    unsigned int ldn = 0;
    unsigned int rshift;

    i = n;
    while (i >>= 1)
        ++ldn;
    rshift = 8 * (unsigned int)sizeof(unsigned long) - ldn;
    for (i = 0; i < n; ++i) {
        unsigned long r;
#if (ULONG_MAX == 0xffffffff)
        r = ((i & 0x55555555) << 1) | ((i & ~0x55555555) >> 1);
        r = ((r & 0x33333333) << 2) | ((r & ~0x33333333) >> 2);
        r = ((r & 0x0f0f0f0f) << 4) | ((r & ~0x0f0f0f0f) >> 4);
        r = ((r & 0x00ff00ff) << 8) | ((r & ~0x00ff00ff) >> 8);
        r = (r << 16) | (r >> 16);
#elif (ULONG_MAX == 0xffffffffffffffff)
        r = ((i & 0x5555555555555555) << 1) | ((i & ~0x5555555555555555) >> 1);
        r = ((r & 0x3333333333333333) << 2) | ((r & ~0x3333333333333333) >> 2);
        r = ((r & 0x0f0f0f0f0f0f0f0f) << 4) | ((r & ~0x0f0f0f0f0f0f0f0f) >> 4);
        r = ((r & 0x00ff00ff00ff00ff) << 8) | ((r & ~0x00ff00ff00ff00ff) >> 8);
        r = ((r & 0x0000ffff0000ffff) << 16) |
                ((r & ~0x0000ffff0000ffff) >> 16);
        r = (r << 32) | (r >> 32);
#endif
        r >>= rshift;
        if (r > i) {
            double tmp;
            tmp = x[i]; x[i] = x[r]; x[r] = tmp;
        }
    }
    return;
}


void Convolutioner::setInput1(const QString &) {
}

QString Convolutioner::input1() {
    QString tmp;
    for (auto i = 0; i < lenA + lenB - 1; ++i) {
        tmp += QString::number(convolutionArray[i].Value()) + "   ";
        emit onPointChanged("seq3", convolutionArray[i].Value());
    }
    return tmp;
}

void Convolutioner::setInput2(const QString &) {
}

QString Convolutioner::input2() const {
    QString tmp;
    QString log;

    tmp = "+ : " + QString::number(OPdouble::Adds())
            + "    - : " + QString::number(OPdouble::Subs())
            + "    * : " + QString::number(OPdouble::Muls())
            + "    / : " + QString::number(OPdouble::Divs())
            + "    n : " + QString::number(OPdouble::Negs());
    log = QString::number(OPdouble::Adds())
            + "\t" + QString::number(OPdouble::Subs())
            + "\t" + QString::number(OPdouble::Muls())
            + "\t" + QString::number(OPdouble::Divs())
            + "\t" + QString::number(OPdouble::Negs()) + "\t";

    QTextStream out(file);
    out << log;
    return tmp;
}

void Convolutioner::statistic()
{
    QString exe = "/usr/bin/python3";
    QStringList args;
    args << "./statistics.py";
    QProcess::execute(exe, args);
}

void Convolutioner::cleaning()
{
    file->close();
    file = new QFile;
    file->setFileName("log.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
}



// Algorythms are not separated by more simple functions
// Of course, if i do, it'll reduce amount of code, but also reduce my own free time, damn

// Свертка в лоб, линейная
void Convolutioner::computeAprioryLine(const QString &array1, const QString &array2)
{
    parser(array1, array2, 0);
    QTextStream out(file);
    QString log = "line\n";
    out << log;

    int ii;
    double tmp;
    _lenSection = 0;


    for ( auto i=0; i<lenConvolutionArray; ++i ) {
        ii = i;
        tmp = 0.0;

        for ( auto j=0; j<lenB; ++j ) {

            if ( ii>=0 && ii<lenA ) tmp += (A[ii]*B[j]);
            --ii;
        }

        convolutionArray[i] = tmp;
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
    parser(array1, array2, 0);
    QTextStream out(file);
    QString log = "circle\n";
    out << log;

    int tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;
    _lenSection = 0;

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

// В лоб, но через БПФ (не в лоб)
void Convolutioner::computeAprioryFFT(const QString &array1, const QString &array2)
{
    parser(array1, array2, 1);
    QTextStream out(file);
    QString log = "FFT\n";
    out << log;

    _lenSection = 0;

    double* section1 = (double*)malloc(lenConvolutionArray * sizeof(double));
    double* section2 = (double*)malloc(lenConvolutionArray * sizeof(double));
    double* Im1 = (double*)malloc(lenConvolutionArray * sizeof(double));
    double* Im2 = (double*)malloc(lenConvolutionArray * sizeof(double));
    double* Im3 = (double*)malloc(lenConvolutionArray * sizeof(double));

    std::fill_n(section1, lenConvolutionArray, stub);
    std::fill_n(section2, lenConvolutionArray, stub);
    std::fill_n(Im1, lenConvolutionArray, stub);
    std::fill_n(Im2, lenConvolutionArray, stub);
    std::fill_n(Im3, lenConvolutionArray, stub);

    memcpy(section1, A, lenA * sizeof *A);
    memcpy(section2, B, lenB * sizeof *B);



    FFT(section1, Im1, lenConvolutionArray, std::log2(lenConvolutionArray), FT_DIRECT);
    FFT(section2, Im2, lenConvolutionArray, std::log2(lenConvolutionArray), FT_DIRECT);

    for ( auto j = 0; j<lenConvolutionArray; ++j) {

        convolutionArray[j] = section1[j]*section2[j] - Im1[j]*Im2[j];
        Im3[j] = section1[j]*Im2[j] + section2[j]*Im1[j];
    }

    FFT(convolutionArray, Im3, lenConvolutionArray, std::log2(lenConvolutionArray),
        FT_INVERSE);

    free(section1);
    free(section2);
    free(Im1);
    free(Im2);
    free(Im3);

    emit onInput1Changed("frame1");
    emit onInput2Changed("frame1");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);

}

// В лоб, но через БПХ (не в лоб)
void Convolutioner::computeAprioryFHT(const QString &array1, const QString &array2)
{
    parser(array1, array2, 1);
    QTextStream out(file);
    QString log = "FHT\n";
    out << log;

    _lenSection = 0;

    std::fill_n(convolutionArray, lenConvolutionArray, stub);

    double* section1 = (double*)malloc(lenConvolutionArray * sizeof(double));
    double* section2 = (double*)malloc(lenConvolutionArray * sizeof(double));

    std::fill_n(section1, lenConvolutionArray, stub);
    std::fill_n(section2, lenConvolutionArray, stub);

    memcpy(section1, A, lenA * sizeof *A);
    memcpy(section2, B, lenB * sizeof *B);



    bitrevPermuteReal(section1, lenConvolutionArray);
    bitrevPermuteReal(section2, lenConvolutionArray);
    fhtDitIter(section1, lenConvolutionArray);
    fhtDitIter(section2, lenConvolutionArray);

    for ( auto i = 0; i<lenConvolutionArray; ++i) {

        convolutionArray[i] =
                (section1[i]
                 * (section2[i] + section2[(lenConvolutionArray-i)%lenConvolutionArray])
                + section1[(lenConvolutionArray-i)%lenConvolutionArray]
                * (section2[i] - section2[(lenConvolutionArray-i)%lenConvolutionArray]))
                * 0.5;
    }

    bitrevPermuteReal(convolutionArray, lenConvolutionArray);
    fhtDitIter(convolutionArray, lenConvolutionArray);

    for ( auto j = 0; j<lenConvolutionArray; ++j) {

        convolutionArray[j] = convolutionArray[j]/(lenConvolutionArray);
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
    parser(array1, array2, 0);
    QTextStream out(file);
    QString log = "line\n";
    out << log;

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
            memcpy(section2, A, lenFilter * sizeof *A);
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
    parser(array1, array2, 0);
    QTextStream out(file);
    QString log = "circle\n";
    out << log;

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
            for ( auto k = 0; k<lenSection + lenFilter - 1; ++k) {

                tmp = ((j-k) < 0)? lenSection + lenFilter - k - 1 : (j-k);
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

// Перекрытие с суммированием, промежуточные последовательности считаются через БПФ
void Convolutioner::computeOverlapAddFFT(const QString &array1, const QString &array2)
{
    parser(array1, array2, 2);
    QTextStream out(file);
    QString log = "FFT\n";
    out << log;

    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;

    double* section1 = (double*)malloc((lenSection+lenFilter-1) * sizeof(double));
    double* section2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* section3 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* Im1 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* Im2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* Im3 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));

    std::fill_n(section1, lenSection+lenFilter-1, stub);
    std::fill_n(section2, lenSection+lenFilter-1, stub);
    std::fill_n(section3, lenSection+lenFilter-1, stub);
    std::fill_n(Im1, lenSection+lenFilter-1, stub);
    std::fill_n(Im2, lenSection+lenFilter-1, stub);
    std::fill_n(Im3, lenSection+lenFilter-1, stub);



    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {
        std::fill_n(section1, lenSection+lenFilter-1, stub);
        std::fill_n(section2, lenSection+lenFilter-1, stub);

        if (lenFilter == lenB) {
            memcpy(section1, A+i, lenSection * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i, lenSection * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        std::fill_n(Im1, lenSection+lenFilter-1, stub);
        std::fill_n(Im2, lenSection+lenFilter-1, stub);
        std::fill_n(Im3, lenSection+lenFilter-1, stub);

        FFT(section1, Im1, lenFilter+lenSection-1, std::log2(lenFilter+lenSection-1),
            FT_DIRECT);
        FFT(section2, Im2, lenFilter+lenSection-1, std::log2(lenFilter+lenSection-1),
            FT_DIRECT);

        for ( auto j = 0; j<lenSection + lenFilter - 1; ++j) {

            section3[j] = section1[j]*section2[j] - Im1[j]*Im2[j];
            Im3[j] = section1[j]*Im2[j] + section2[j]*Im1[j];
        }

        FFT(section3, Im3, lenFilter+lenSection-1, std::log2(lenFilter+lenSection-1),
            FT_INVERSE);

        for ( auto k = 0; k<lenSection + lenFilter - 1; ++k) {

            convolutionArray[k+i] += section3[k];
        }

    }
    free(section1);
    free(section2);
    free(section3);
    free(Im1);
    free(Im2);
    free(Im3);

    emit onInput1Changed("frame2");
    emit onInput2Changed("frame2");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

// Перекрытие с суммированием, промежуточные последовательности считаются через БПХ
void Convolutioner::computeOverlapAddFHT(const QString &array1, const QString &array2)
{
    parser(array1, array2, 2);
    QTextStream out(file);
    QString log = "FHT\n";
    out << log;

    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;
    int lenSubArray = lenSection+lenFilter-1;

    double* section1 = (double*)malloc((lenSubArray) * sizeof(double));
    double* section2 = (double*)malloc((lenSubArray) * sizeof(double));
    double* section3 = (double*)malloc((lenSubArray) * sizeof(double));

    std::fill_n(section1, lenSubArray, stub);
    std::fill_n(section2, lenSubArray, stub);
    std::fill_n(section3, lenSubArray, stub);



    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {
        std::fill_n(section1, lenSubArray, stub);
        std::fill_n(section2, lenSubArray, stub);

        if (lenFilter == lenB) {
            memcpy(section1, A+i, lenSection * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i, lenSection * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        bitrevPermuteReal(section1, lenSubArray);
        bitrevPermuteReal(section2, lenSubArray);
        fhtDitIter(section1, lenSubArray);
        fhtDitIter(section2, lenSubArray);

        for ( auto i = 0; i<lenSubArray; ++i) {

            section3[i] =
                    (section1[i]
                     * (section2[i] + section2[(lenSubArray-i)%lenSubArray])
                    + section1[(lenSubArray-i)%lenSubArray]
                    * (section2[i] - section2[(lenSubArray-i)%lenSubArray]))
                    * 0.5;
        }

        bitrevPermuteReal(section3, lenSubArray);
        fhtDitIter(section3, lenSubArray);

        for ( auto k = 0; k<lenSubArray; ++k) {

            convolutionArray[k+i] += section3[k]/lenSubArray;
        }

    }
    free(section1);
    free(section2);
    free(section3);

    emit onInput1Changed("frame2");
    emit onInput2Changed("frame2");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

// Перекрытие с накоплением, промежуточные свертки круговые (периодические)
void Convolutioner::computeOverlapSaveCircle(const QString &array1, const QString &array2)
{
    parser(array1, array2, 0);
    QTextStream out(file);
    QString log = "circle\n";
    out << log;


    int tmp;
    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;

    double* section1 = (double*)malloc((lenSection+lenFilter-1) * sizeof(double));
    double* section2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* section3 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));



    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {

        std::fill_n(section1, lenSection+lenFilter-1, stub);
        std::fill_n(section2, lenSection+lenFilter-1, stub);
        std::fill_n(section3, lenSection+lenFilter-1, stub);

        for (static bool first = true;first;first=false) {
            if (lenFilter == lenB) {
                memcpy(section1+lenFilter-1, A, lenSection * sizeof *A);
                memcpy(section2, B, lenB * sizeof *B);
            } else {
                memcpy(section1+lenFilter-1, B, lenSection * sizeof *B);
                memcpy(section2, A, lenA * sizeof *A);
            }
        }

        if (lenFilter == lenB) {
            memcpy(section1, A+i-lenFilter+1, (lenSection+lenFilter-1) * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i-lenFilter+1, (lenSection+lenFilter-1) * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        for ( auto j=0; j<lenSection + lenFilter - 1; ++j)
            for ( auto k = 0; k<lenSection + lenFilter - 1; ++k) {
                tmp=((j-k+lenFilter-1)<0)? lenSection+2*lenFilter-k-2 : (j-k+lenFilter-1);
                section3[j] += section1[k]*section2[tmp];
            }

        for ( auto l=0; l<lenSection; ++l)
            convolutionArray[l+i] = section3[l];

    }
    free(section1);
    free(section2);
    free(section3);

    emit onInput1Changed("frame3");
    emit onInput2Changed("frame3");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

// Перекрытие с накоплением, промежуточные - БПФ (и выхлоп обрезается с другой стороны)
void Convolutioner::computeOverlapSaveFFT(const QString &array1, const QString &array2)
{
    parser(array1, array2, 2);
    QTextStream out(file);
    QString log = "FFT\n";
    out << log;

    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;

    double* section1 = (double*)malloc((lenSection+lenFilter-1) * sizeof(double));
    double* section2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* section3 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* Im1 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* Im2 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));
    double* Im3 = (double*)malloc((lenFilter+lenSection-1) * sizeof(double));



    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {

        std::fill_n(section1, lenSection+lenFilter-1, stub);
        std::fill_n(section2, lenSection+lenFilter-1, stub);
        std::fill_n(section3, lenSection+lenFilter-1, stub);
        std::fill_n(Im1, lenSection+lenFilter-1, stub);
        std::fill_n(Im2, lenSection+lenFilter-1, stub);
        std::fill_n(Im3, lenSection+lenFilter-1, stub);

        for (static bool first = true;first;first=false) {
            if (lenFilter == lenB) {
                memcpy(section1+lenFilter-1, A, lenSection * sizeof *A);
                memcpy(section2, B, lenB * sizeof *B);
            } else {
                memcpy(section1+lenFilter-1, B, lenSection * sizeof *B);
                memcpy(section2, A, lenA * sizeof *A);
            }
        }

        if (lenFilter == lenB) {
            memcpy(section1, A+i-lenFilter+1, (lenSection+lenFilter-1) * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i-lenFilter+1, (lenSection+lenFilter-1) * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        FFT(section1, Im1, lenFilter+lenSection-1, std::log2(lenFilter+lenSection-1),
            FT_DIRECT);
        FFT(section2, Im2, lenFilter+lenSection-1, std::log2(lenFilter+lenSection-1),
            FT_DIRECT);

        for ( auto j = 0; j<lenSection + lenFilter - 1; ++j) {

            section3[j] = section1[j]*section2[j] - Im1[j]*Im2[j];
            Im3[j] = section1[j]*Im2[j] + section2[j]*Im1[j];
        }

        FFT(section3, Im3, lenFilter+lenSection-1, std::log2(lenFilter+lenSection-1),
            FT_INVERSE);

        for ( auto k = lenFilter-1; k<lenSection+lenFilter-1; ++k) {

            convolutionArray[k+i-lenFilter+1] += section3[k];
        }

    }
    free(section1);
    free(section2);
    free(section3);
    free(Im1);
    free(Im2);
    free(Im3);

    emit onInput1Changed("frame3");
    emit onInput2Changed("frame3");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

// Перекрытие с накоплением, промежуточные - БПХ (и выхлоп обрезается с другой стороны)
void Convolutioner::computeOverlapSaveFHT(const QString &array1, const QString &array2)
{
    parser(array1, array2, 2);
    QTextStream out(file);
    QString log = "FHT\n";
    out << log;

    int lenFilter = (lenA < lenB)? lenA : lenB;
    int lenSection = _lenSection;
    int lenSubArray = lenSection+lenFilter-1;

    double* section1 = (double*)malloc((lenSubArray) * sizeof(double));
    double* section2 = (double*)malloc((lenSubArray) * sizeof(double));
    double* section3 = (double*)malloc((lenSubArray) * sizeof(double));



    for ( auto i=0; i<lenConvolutionArray+1-lenFilter; i+=lenSection ) {

        std::fill_n(section1, lenSubArray, stub);
        std::fill_n(section2, lenSubArray, stub);
        std::fill_n(section3, lenSubArray, stub);

        for (static bool first = true;first;first=false) {
            if (lenFilter == lenB) {
                memcpy(section1+lenFilter-1, A, lenSection * sizeof *A);
                memcpy(section2, B, lenB * sizeof *B);
            } else {
                memcpy(section1+lenFilter-1, B, lenSection * sizeof *B);
                memcpy(section2, A, lenA * sizeof *A);
            }
        }

        if (lenFilter == lenB) {
            memcpy(section1, A+i-lenFilter+1, (lenSubArray) * sizeof *A);
            memcpy(section2, B, lenB * sizeof *B);
        } else {
            memcpy(section1, B+i-lenFilter+1, (lenSubArray) * sizeof *B);
            memcpy(section2, A, lenA * sizeof *A);
        }

        bitrevPermuteReal(section1, lenSubArray);
        bitrevPermuteReal(section2, lenSubArray);
        fhtDitIter(section1, lenSubArray);
        fhtDitIter(section2, lenSubArray);

        for ( auto i = 0; i<lenSubArray; ++i) {

            section3[i] =
                    (section1[i]
                     * (section2[i] + section2[(lenSubArray-i)%lenSubArray])
                    + section1[(lenSubArray-i)%lenSubArray]
                    * (section2[i] - section2[(lenSubArray-i)%lenSubArray]))
                    * 0.5;
        }

        bitrevPermuteReal(section3, lenSubArray);
        fhtDitIter(section3, lenSubArray);

        for ( auto k = lenFilter-1; k<lenSubArray; ++k) {

            convolutionArray[k+i-lenFilter+1] += section3[k]/lenSubArray;
        }

    }
    free(section1);
    free(section2);
    free(section3);

    emit onInput1Changed("frame3");
    emit onInput2Changed("frame3");

    OPdouble::ClearOps();
    free (A);
    free (B);
    free (convolutionArray);
}

#undef double
