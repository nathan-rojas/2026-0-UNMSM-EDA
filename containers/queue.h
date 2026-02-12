#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>
#include <mutex>
#include <stdexcept>
#include "../general/types.h"

// FORWARD DECLARATIONS

template <typename T> class CQueue;

template <typename T>
std::ostream& operator<<(std::ostream& os, CQueue<T>& queue);

template <typename T>
std::istream& operator>>(std::istream& is, CQueue<T>& queue);


// NODO PARA QUEUE

template <typename T> 
class NodeQueue {
    friend class CQueue<T>; 
    
private:
    T m_data;
    ref_type m_ref;
    NodeQueue *m_pNext = nullptr;

public:
    NodeQueue() : m_ref(-1), m_pNext(nullptr) {}
    
    NodeQueue(T value, ref_type ref = -1) 
        : m_data(value), m_ref(ref), m_pNext(nullptr) {}
        
    NodeQueue(T value, ref_type ref, NodeQueue *pNext) 
        : m_data(value), m_ref(ref), m_pNext(pNext) {}
    
    // Getters (Necesarios para que el operador << lea los datos)
    T GetValue() const { return m_data; }
    ref_type GetRef() const { return m_ref; }
    NodeQueue *GetNext() const { return m_pNext; }
};


// CLASE QUEUE 

template <typename T>
class CQueue {
    using Node = NodeQueue<T>;
    
private:
    Node *m_pFront = nullptr;   
    Node *m_pRear = nullptr;    
    size_t m_nElements = 0;     
    mutable std::mutex m_mutex; 

public:
    CQueue() = default;
    
    
    // Constructor Copia 
    
    CQueue(const CQueue &other) {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        
        if (!other.m_pFront) return;
        
        // Copiar nodo por nodo manteniendo el orden FIFO
        Node *pCurrent = other.m_pFront;
        while (pCurrent) {
            Push(pCurrent->m_data, pCurrent->m_ref);
            pCurrent = pCurrent->m_pNext;
        }
    }
    
    
    // Move Constructor (Optimización)
    
    CQueue(CQueue &&other) noexcept {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        // Robamos los punteros
        m_pFront = other.m_pFront;
        m_pRear = other.m_pRear;
        m_nElements = other.m_nElements;
        
        // Dejamos la otra cola vacía
        other.m_pFront = nullptr;
        other.m_pRear = nullptr;
        other.m_nElements = 0;
    }
    
    
    // Destructor
    
    virtual ~CQueue() {
        Clear();
    }
    
    
    // Push - Encolar (agregar al final)
    
    void Push(const T &value, ref_type ref = -1) {
        std::lock_guard<std::mutex> lock(m_mutex);
        Node *pNew = new Node(value, ref, nullptr);
        
        if (!m_pRear) {
            // Cola vacía
            m_pFront = m_pRear = pNew;
        } else {
            // Agregar al final
            m_pRear->m_pNext = pNew;
            m_pRear = pNew;
        }
        ++m_nElements;
    }
    
    
    // Pop 
    
    T Pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pFront) throw std::out_of_range("Queue is empty");
        
        Node *pTemp = m_pFront;
        T value = pTemp->m_data;
        m_pFront = m_pFront->m_pNext;
        
        // Si la cola queda vacía, actualizar rear
        if (!m_pFront) {
            m_pRear = nullptr;
        }
        
        delete pTemp;
        --m_nElements;
        return value;
    }
    
    
    // Front - Ver el frente sin sacar
    
    T &Front() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pFront) throw std::out_of_range("Queue is empty");
        return m_pFront->m_data;
    }
    
    const T &Front() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pFront) throw std::out_of_range("Queue is empty");
        return m_pFront->m_data;
    }
    
    // Rear - Ver el final sin sacar
    
    T &Rear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pRear) throw std::out_of_range("Queue is empty");
        return m_pRear->m_data;
    }
    
    const T &Rear() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pRear) throw std::out_of_range("Queue is empty");
        return m_pRear->m_data;
    }
    
    
    // isEmpty - Verificar si está vacía
    
    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pFront == nullptr;
    }
    
    
    // getSize 
    
    size_t getSize() const {
        return m_nElements;
    }
    
    
    // Clear 
    
    void Clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        while (m_pFront) {
            Node *pTemp = m_pFront;
            m_pFront = m_pFront->m_pNext;
            delete pTemp;
        }
        m_pRear = nullptr;
        m_nElements = 0;
    }
    
    
    // Operadores de Comparación
    
    bool operator<(const CQueue &other) const {
        return m_nElements < other.m_nElements;
    }
    
    bool operator>(const CQueue &other) const {
        return m_nElements > other.m_nElements;
    }
    
    bool operator==(const CQueue &other) const {
        return m_nElements == other.m_nElements;
    }
    
    // Friends
    friend std::ostream& operator<< <T>(std::ostream& os, CQueue<T>& queue);
    friend std::istream& operator>> <T>(std::istream& is, CQueue<T>& queue);
};


// IMPLEMENTACIÓN DE OPERADORES

template <typename T>
std::ostream& operator<<(std::ostream& os, CQueue<T>& queue) {
    std::lock_guard<std::mutex> lock(queue.m_mutex);
    os << "CQueue (FIFO): size = " << queue.m_nElements << "\n";
    os << "[ FRONT -> ";
    
    auto pCurrent = queue.m_pFront;
    while (pCurrent) {
        // Usamos Getters
        os << "(" << pCurrent->GetValue() << ":" << pCurrent->GetRef() << ")";
        pCurrent = pCurrent->GetNext();
        if (pCurrent) os << " -> ";
    }
    
    os << " <- REAR ]" << std::endl;
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, CQueue<T>& queue) {
    T value;
    ref_type ref;
    if (is >> value >> ref) {
        queue.Push(value, ref);
    }
    return is;
}

#endif // __QUEUE_H__
