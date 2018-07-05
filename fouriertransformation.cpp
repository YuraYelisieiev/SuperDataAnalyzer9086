#include <map>
#include <QVector>
#include <cmath>

#include <iostream>

std::map<int, qreal> ft(std::map<int, qreal>& points) {
    auto N = points.size();
    std::map<int, qreal> res;
    for(int k = 0; k < N; ++k) {
        qreal Ak = 0, Bk = 0;
        for(int n = 0; n < N; ++n) {
            qreal b = 2*M_PI*k*n/N;
            Ak += points[n]*cos(-b);
            Ak += points[n]*sin(-b);
        }
        res[k] = sqrt(Ak*Ak + Bk*Bk);
    }
    return res;
}
