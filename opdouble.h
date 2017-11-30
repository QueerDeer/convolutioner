#ifndef OPDOUBLE_H
#define OPDOUBLE_H


class OPdouble {
public:
    OPdouble();
    OPdouble(const double x);

    static void ClearOps();
    static int Adds();
    static int Subs();
    static int Negs();
    static int Muls();
    static int Divs();

    OPdouble operator++ ();
    OPdouble operator++ (int);
    OPdouble operator-- ();
    OPdouble operator-- (int);

    const OPdouble  operator= (const OPdouble &y );

    OPdouble operator+= (OPdouble y);
    OPdouble operator-= (OPdouble y);
    OPdouble operator*= (OPdouble y);
    OPdouble operator/= (OPdouble y);
    OPdouble operator-  ();
    OPdouble operator+  ();

    bool operator< (OPdouble const & y);
    bool operator> (OPdouble const & y);
    bool operator== (OPdouble const & y);
    bool operator!= (OPdouble const & y);
    bool operator>= (OPdouble const & y);
    bool operator<= (OPdouble const & y);

    double   Value();

private:
    double value;
    static int opadd;
    static int opsub;
    static int opneg;
    static int opmul;
    static int opdiv;
};


inline OPdouble operator+ (OPdouble x, OPdouble y) {
    OPdouble d=x;
    return d +=y;
}
inline OPdouble operator- (OPdouble x, OPdouble y) {
    OPdouble d=x;
    return d -=y;
}
inline OPdouble operator* (OPdouble x, OPdouble y) {
    OPdouble d=x;
    return d *=y;
}
inline OPdouble operator/ (OPdouble x, OPdouble y) {
    OPdouble d=x;
    return d /=y;
}

inline OPdouble operator+ (const double x, OPdouble y) {
    OPdouble d(x);
    return d +=y;
}
inline OPdouble operator- (const double x, OPdouble y) {
    OPdouble d(x);
    return d -=y;
}
inline OPdouble operator* (const double x, OPdouble y) {
    OPdouble d(x);
    return d *=y;
}
inline OPdouble operator/ (const double x, OPdouble y) {
    OPdouble d(x);
    return d /=y;
}


#endif // OPDOUBLE_H
