#pragma once

#include "pch.h"

template<typename T, uint J>
class CircularBuffer {
public:
    CircularBuffer() : m_Size(J) {}
    void push(T value) {
        m_Queue.push_front(value);
        if (m_Size < m_Queue.size()) {
            m_Queue.pop_back();
        }
    }

    T operator [](uint index) { return m_Queue[index]; }

    inline uint size() { return m_Size; }
    inline uint sizeUsed() { return m_Queue.size(); }
        
private:
    std::deque<T> m_Queue;
    uint m_Size;
};

glm::mat4 modelPivotSquare(glm::mat4& model, float rotation, float w, float h, uint pivot);
void seedRandNum(uint64_t seed);
int randNum(int min, int max);

template<typename T>
class ChronoClock {
public:
    inline void startTimer() {
        m_StartTime = std::chrono::high_resolution_clock::now();
    };
    inline uint64_t endTimer() {
        return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
    }
    inline uint64_t getTime() {
        return std::chrono::duration_cast<T>(std::chrono::time_point_cast<T>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count(); 
    }
private:
    std::chrono::high_resolution_clock::time_point m_StartTime;
};

void printOpenglInfo();
