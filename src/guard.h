#ifndef GUARD_H
#define GUARD_H

#include <QDebug>
#include <utility>

template<typename F>
class Guard {
    F m_onExit;

public:
    explicit Guard(F&& onExit) : m_onExit(std::forward<F>(onExit)) {}
    
    ~Guard() noexcept {
        try {
            m_onExit();
        } catch (...) {
            // Suppress all exceptions
            qDebug() << "Exception in Guard destructor";
        }
    }
    
    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;
};

#endif // GUARD_H


