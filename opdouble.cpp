#include "opdouble.h"

int OPdouble::opadd = 0;
int OPdouble::opsub = 0;
int OPdouble::opneg = 0;
int OPdouble::opmul = 0;
int OPdouble::opdiv = 0;


OPdouble::OPdouble() : value( 0.0 ) {
}

OPdouble::OPdouble(const double x) {
    value = x;
}


void OPdouble::ClearOps() {
    opadd = opmul = opsub = opneg = opdiv = 0;
}

int OPdouble::Adds() {
    return opadd;
}

int OPdouble::Subs() {
    return opsub;
}

int OPdouble::Negs() {
    return opneg;
}

int OPdouble::Muls() {
    return opmul;
}

int OPdouble::Divs() {
    return opdiv;
}


OPdouble OPdouble::operator++(int) {
    OPdouble old(value);
    value++;
    ++opadd;
    return old;
}

OPdouble OPdouble::operator--(int) {
    OPdouble old(value);
    value--;
    ++opneg;
    return old;
}


const OPdouble OPdouble::operator=(const OPdouble &y){
    value = y.value;
    return *this;
}


OPdouble OPdouble::operator+=(OPdouble y) {
    ++opadd;
    value += y.value;
    return *this;
}

OPdouble OPdouble::operator-=(OPdouble y) {
    ++opsub;
    value -= y.value;
    return *this;
}

OPdouble OPdouble::operator*=(OPdouble y) {
    ++opmul;
    value *= y.value;
    return *this;
}

OPdouble OPdouble::operator/=(OPdouble y) {
    ++opdiv;
    value /= y.value;
    return *this;
}

OPdouble OPdouble::operator-() {
    ++opneg;
    return OPdouble(-value);
}

OPdouble OPdouble::operator+() {
    return OPdouble( value );
}


bool OPdouble::operator<(const OPdouble &y) {
    return (value < y.value);
}

bool OPdouble::operator>(const OPdouble &y) {
    return (value > y.value);
}

bool OPdouble::operator==(const OPdouble &y) {
    return (value == y.value);
}

bool OPdouble::operator!=(const OPdouble &y) {
    return !(*this == y);
}

bool OPdouble::operator>=(const OPdouble &y) {
    return !(*this < y);
}

bool OPdouble::operator<=(const OPdouble &y) {
    return !(*this > y);
}


double OPdouble::Value() {
    return value;
}


OPdouble OPdouble::operator--() {
    value--;
    ++opneg;
    return *this;
}

OPdouble OPdouble::operator++() {
    value++;
    ++opadd;
    return *this;
}
