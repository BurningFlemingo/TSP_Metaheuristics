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
int randNum(int min, int max);

