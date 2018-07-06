#ifndef FOURIERTRANSFORMATION_CPP
#define FOURIERTRANSFORMATION_CPP

#include <map>
#include <QVector>
#include <cmath>

std::map<size_t, qreal> ft(const std::map<size_t, qreal>& points) {
    auto N = points.size();
    std::map<size_t, qreal> res;
    for(int k = 0; k < N; ++k) {
        qreal Ak = 0, Bk = 0;
        int n = 0;
        for(auto& x: points) {
            qreal b = 2*M_PI*k*n/N;
            Ak += x.second*cos(-b);
            Bk += x.second*sin(-b);
            ++n;
        }
//        for(int n = 0; n < N; ++n) {
//            qreal b = 2*M_PI*k*n/N;
//            Ak += points.at(n)*cos(-b);
//            Bk += points.at(n)*sin(-b);
//        }
        res[k] = sqrt(Ak*Ak + Bk*Bk);
    }
    return res;
}

#endif
