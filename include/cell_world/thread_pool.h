#pragma once
#include <functional>
#include <future>
#include <vector>

namespace cell_world {
    struct Thread_pool {
        template<typename _Tp>
        using __not_same = std::__not_<std::is_same<std::__remove_cvref_t<_Tp>, std::thread>>;

        Thread_pool() : Thread_pool(std::thread::hardware_concurrency()) {};
        explicit Thread_pool(unsigned int worker_count) : workers(worker_count), available_worker(4){
            for (auto &w:available_worker) w = true;
        }

        template<typename... _Tp>
        using __decayed_tuple = std::tuple<typename std::decay<_Tp>::type...>;

        template<typename _Callable, typename... _Args,
                typename = std::_Require<__not_same<_Callable>>>

        void run(_Callable&& __f, _Args&&... __args) {
            while (true){
                for (unsigned int w = 0; w < available_worker.size() ; w++) {
                    auto &available = available_worker[w];
                    if (available) {
                        available = false;
                        if (workers[w].joinable()) workers[w].join();
                        workers[w] = std::thread([&available, &__f, __args...] () { (*__f) (__args...); available = true;});
                        return;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        void wait(){
            for (auto &w:workers) if (w.joinable()) w.join();
        }
        std::vector<std::atomic<bool>> available_worker;
        std::vector<std::thread> workers;
    };
}