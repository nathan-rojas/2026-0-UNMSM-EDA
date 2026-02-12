#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <mutex>
#include <stdexcept>
#include "../general/types.h" 

// FORWARD DECLARATIONS

template <typename T> class CStack;

template <typename T>
std::ostream& operator<<(std::ostream& os, CStack<T>& stack);

template <typename T>
std::istream& operator>>(std::istream& is, CStack<T>& stack);


// NODO PARA STACK

template <typename T> 
class NodeStack {
    friend class CStack<T>;  
    
private:
    T m_data;
    ref_type m_ref;
    NodeStack *m_pNext = nullptr;

public:
    NodeStack() : m_ref(-1), m_pNext(nullptr) {}
    
    // Constructores para facilitar la inserción
    NodeStack(T value, ref_type ref = -1) 
        : m_data(value), m_ref(ref), m_pNext(nullptr) {}
        
    NodeStack(T value, ref_type ref, NodeStack *pNext) 
        : m_data(value), m_ref(ref), m_pNext(pNext) {}
    
    // Getters (Necesarios para que el operador << lea los datos)
    T GetValue() const { return m_data; }
    ref_type GetRef() const { return m_ref; }
    NodeStack *GetNext() const { return m_pNext; }
};


// CLASE STACK 

template <typename T>
class CStack {
    using Node = NodeStack<T>;
    
private:
    Node *m_pTop = nullptr;     
    size_t m_nElements = 0;     
    mutable std::mutex m_mutex; 

public:
    CStack() = default;
    
    
    // Constructor Copia (Deep Copy)
   
    CStack(const CStack &other) {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        
        if (!other.m_pTop) return;
        
        // 1. Contar elementos
        size_t count = other.m_nElements;
        
        // 2. Crear arrays temporales para invertir el orden
        // (Esto demuestra manejo manual de memoria)
        T *tempValues = new T[count];
        ref_type *tempRefs = new ref_type[count];
        
        Node *pCurrent = other.m_pTop;
        for (size_t i = 0; i < count; ++i) {
            tempValues[i] = pCurrent->m_data; 
            tempRefs[i] = pCurrent->m_ref;
            pCurrent = pCurrent->m_pNext;
        }
        
        // 3. Insertar desde el final hacia el principio para mantener LIFO
        for (int i = count - 1; i >= 0; --i) {
            Push(tempValues[i], tempRefs[i]);
        }
        
        // 4. Liberar memoria temporal
        delete[] tempValues;
        delete[] tempRefs;
    }
    
    
    // Move Constructor (Optimización)
   
    CStack(CStack &&other) noexcept {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        // Robamos los punteros
        m_pTop = other.m_pTop;
        m_nElements = other.m_nElements;
        
        // Dejamos la otra pila vacía
        other.m_pTop = nullptr;
        other.m_nElements = 0;
    }
    
    
    // Destructor
    
    virtual ~CStack() {
        Clear();
    }
    
    
    // Métodos Principales
    
    void Push(const T &value, ref_type ref = -1) {
        std::lock_guard<std::mutex> lock(m_mutex);
        Node *pNew = new Node(value, ref, m_pTop);
        m_pTop = pNew;
        ++m_nElements;
    }
    
    T Pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pTop) throw std::out_of_range("Stack is empty");
        
        Node *pTemp = m_pTop;
        T value = pTemp->m_data;
        m_pTop = m_pTop->m_pNext;
        
        delete pTemp;
        --m_nElements;
        return value;
    }
    
    T &Top() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pTop) throw std::out_of_range("Stack is empty");
        return m_pTop->m_data;
    }
    
    const T &Top() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_pTop) throw std::out_of_range("Stack is empty");
        return m_pTop->m_data;
    }
    
    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pTop == nullptr;
    }
    
    size_t getSize() const {
        return m_nElements;
    }
    
    void Clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        while (m_pTop) {
            Node *pTemp = m_pTop;
            m_pTop = m_pTop->m_pNext;
            delete pTemp;
        }
        m_nElements = 0;
    }
    
    
    // Operadores
   
    bool operator<(const CStack &other) const {
        return m_nElements < other.m_nElements;
    }
    
    bool operator>(const CStack &other) const {
        return m_nElements > other.m_nElements;
    }
    
    bool operator==(const CStack &other) const {
        return m_nElements == other.m_nElements;
    }
    
    // Friends
    friend std::ostream& operator<< <T>(std::ostream& os, CStack<T>& stack);
    friend std::istream& operator>> <T>(std::istream& is, CStack<T>& stack);
};


// IMPLEMENTACIÓN DE OPERADORES


template <typename T>
std::ostream& operator<<(std::ostream& os, CStack<T>& stack) {
    std::lock_guard<std::mutex> lock(stack.m_mutex);
    os << "CStack (LIFO): size = " << stack.m_nElements << "\n";
    os << "[ TOP -> ";
    
    auto pCurrent = stack.m_pTop;
    while (pCurrent) {
        // Usamos Getters
        os << "(" << pCurrent->GetValue() << ":" << pCurrent->GetRef() << ")";
        pCurrent = pCurrent->GetNext();
        if (pCurrent) os << " -> ";
    }
    
    os << " ]" << std::endl;
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, CStack<T>& stack) {
    T value;
    ref_type ref;
    if (is >> value >> ref) {
        stack.Push(value, ref);
    }
    return is;
}

#endif // STACK_H