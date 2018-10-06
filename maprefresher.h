#ifndef __MAP_REFRESHER__
#define __MAP_REFRESHER__

#include <atomic>
#include <chrono>
#include <functional>
#include <map>
#include <thread>

#define MR_DEFAULT_TIMER 60000 // 1 minute of default timer

namespace refresher{

template <typename K, typename V>
class map_refresher {
private:
    std::map<K, V>* map_;
    std::function<V(K,V)> updater_;
    std::atomic<bool> is_running_;
    std::atomic<unsigned int> millis_;
    std::thread* runner_;

public:
    /**
     * @brief Constructor
     * @details Constructor that requires the pointer to the map to update and
     * the function that update entries of the map. Types of the map and the
     * function must agree, if the map types are K and V the function have to
     * take as arguments K and V and to return V. Values are refreshed by default
     * each minute.
     * 
     * @param to_refresh std::map<K,v>* Map to be refreshed
     * @param updater std::function<V(K,V)> Function with which refresh the map
     */
    map_refresher(std::map<K, V>* to_refresh, std::function<V(K,V)> updater)
        : map_(to_refresh), updater_(updater), is_running_(false),
          millis_(MR_DEFAULT_TIMER) {}

    /**
     * @brief Constructor
     * @details Constructor that requires the pointer to the map to update,
     * the function that update entries of the map and the interval of values
     * refreshing. Types of the map and the function must agree, if the map types
     * are K and V the function have to take as arguments K and V and to return
     * V.
     * 
     * @param to_refresh std::map<K,v>* Map to be refreshed
     * @param updater std::function<V(K,V)> Function with which refresh the map
     * @param millisecond unsigned int Interval to wait between two refresh
     */
    map_refresher(std::map<K, V>* to_refresh, std::function<V(K,V)> updater,
                 unsigned int milliseconds)
        : map_(to_refresh), updater_(updater), is_running_(false),
          millis_(milliseconds) {}

    /**
     * @brief Start the map refreshing
     * @details Create a new thread and run the refreshing using
     * {@link map_refresher#updater_} each {@link map_refresher#millis_}ms
     */
    void start() {
        is_running_ = true;
        runner_ = new std::thread(
            [this]{
                while(is_running_) {
                    for (std::pair<K,V> entry : *map_) {
                        (*map_)[entry.first] = updater_(entry.first, entry.second);
                    }
                    std::this_thread::sleep_for( std::chrono::milliseconds(millis_));
                }
            });
        runner_->detach();
    }

    /**
     * @brief Stop the map refreshing
     * @details Stop the refreshing and after delete the refreshing thread
     */
    void stop() {
        is_running_ = false;
        delete runner_;
    }

    /**
     * @brief Change the interval between refreshes
     * 
     * @param milliseconds unsigned int Milliseconds to wait between refreshs
     */
    void change_wait_interval(unsigned int milliseconds) {
        millis_ = milliseconds;
    }

    /**
     * @brief Force the update of the map in the thread in which the method is
     * called
     */
    void sync_refres_now() {
        for (std::pair<K,V> entry : *map_) {
            (*map_)[entry.first] = updater_(entry.first, entry.second);
        }
    }

    /**
     * @brief Destructor
     * @details Stop the refreshs before the object destructor
     */
    ~map_refresher() {
        if (is_running_)
            stop();
    }
};

} // namespace refresher

#endif //__MAP_REFRESHER__