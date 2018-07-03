#ifndef DATAANALYZER_INTEGRATION_CPP
#define DATAANALYZER_INTEGRATION_CPP

#include <map>
#include <QVector>
#include <vector>
#include <thread>
#include <mutex>
#include <iterator>

constexpr short threads_num = 4;

void simple_integrate(std::map<size_t, qreal>::const_iterator st, std::map<size_t, qreal>::const_iterator en, qreal& res) {
    for(auto tmp = st; st != en; ++st) {
        ++tmp;
        res += (tmp->first - st->first)*(tmp->second+st->second)/2.0;
    }
}

qreal integrate(const std::map<size_t, qreal>& points) {
    size_t points_number = std::max(points.size()/threads_num, static_cast<size_t>(1));
    QVector<qreal> results(threads_num);
    std::vector<std::thread> threads;
    size_t cur_num = 0;
    auto tmp = points.begin();
    for(auto st = points.begin(); cur_num*points_number < points.size(); std::advance(tmp, points_number), ++cur_num) {
        threads.emplace_back(simple_integrate, st, tmp, std::ref(results[cur_num]));
        st = tmp;
    }
    for(auto& thread: threads)
        thread.join();
    return std::accumulate(results.begin(), results.end(), 0.0);
}

#endif //DATAANALYZER_INTEGRATION_CPP
