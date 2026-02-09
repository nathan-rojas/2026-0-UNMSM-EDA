#ifndef __LINKEDLIST_CIRCULAR_H__
#define __LINKEDLIST_CIRCULAR_H__
#include <iostream>
#include <mutex>
#include "../general/types.h"
#include "util.h"
using namespace std;

// Traits para listas enlazadas circulares
template <typename T, typename _Func>
struct ListTraitCircular{
    using value_type = T;
    using Func       = _Func;
};

template <typename T>
struct AscendingTraitCircular : 
    public ListTraitCircular<T, std::greater<T> >{
};

template <typename T>
struct DescendingTraitCircular : 
    public ListTraitCircular<T, std::less<T> >{
};

// Forward declarations
template <typename Traits> class NodeLinkedListCircular;
template <typename Traits> class CLinkedListCircular;
template <typename Traits> class LinkedListCircularForwardIterator;

// ============================================
// NODO PARA LISTA CIRCULAR
// ============================================
template <typename Traits>
class NodeLinkedListCircular{
    template <typename T> friend class CLinkedListCircular;
    template <typename T> friend class LinkedListCircularForwardIterator;
    
    using  value_type  = typename Traits::value_type;
    using  Node        = NodeLinkedListCircular<Traits>;
private:
    value_type m_data;
    ref_type   m_ref;
    Node *m_pNext = nullptr;

public:
    NodeLinkedListCircular(){}
    NodeLinkedListCircular( value_type _value, ref_type _ref = -1)
        : m_data(_value), m_ref(_ref){   }
    NodeLinkedListCircular( value_type _value, ref_type _ref, Node *pNext)
        : m_data(_value), m_ref(_ref), m_pNext(pNext){   }
        
    value_type  GetValue   () const { return m_data; }
    value_type &GetValueRef() { return m_data; }

    ref_type    GetRef     () const { return m_ref;   }
    ref_type   &GetRefRef  () { return m_ref;   }

    Node      * GetNext     () const { return m_pNext;   }
    Node      *&GetNextRef  () { return m_pNext;   }

    Node &operator=(const Node &another){
        m_data = another.GetValue();
        m_ref   = another.GetRef();
        return *this;
    }
    bool operator==(const Node &another) const
    { return m_data == another.GetValue();   }
    bool operator<(const Node &another) const
    { return m_data < another.GetValue();   }
};

// ============================================
// ITERATOR CIRCULAR (detiene en una vuelta completa)
// ============================================
template <typename Traits>
class LinkedListCircularForwardIterator {
    using Node = NodeLinkedListCircular<Traits>;
    using value_type = typename Traits::value_type;
    Node *m_pCurrent;
    Node *m_pStart;
    bool m_bFirstIteration;

public:
    LinkedListCircularForwardIterator(Node *pNode) 
        : m_pCurrent(pNode), m_pStart(pNode), m_bFirstIteration(true) {}
    
    value_type &operator*() { return m_pCurrent->GetValueRef(); }
    
    LinkedListCircularForwardIterator &operator++() {
        m_pCurrent = m_pCurrent->GetNext();
        if(m_pCurrent == m_pStart)
            m_bFirstIteration = false;
        return *this;
    }
    
    bool operator!=(const LinkedListCircularForwardIterator &other) const {
        if(!m_pCurrent) return false;
        if(m_bFirstIteration) return true;
        return m_pCurrent != m_pStart;
    }
    
    bool operator==(const LinkedListCircularForwardIterator &other) const {
        return !(*this != other);
    }
};

// ============================================
// LISTA ENLAZADA CIRCULAR (LEC)
// ============================================
template <typename Traits>
class CLinkedListCircular {
    using  value_type  = typename Traits::value_type;
    using  forward_iterator  = LinkedListCircularForwardIterator<Traits>;
    using  Node = NodeLinkedListCircular<Traits>;

    Node *m_pRoot = nullptr;
    Node *m_pLast = nullptr;
    size_t m_nElements = 0;
    mutable std::mutex m_mutex;

public:
    // Constructor por defecto
    CLinkedListCircular(){}
    
    // Constructor Copia
    CLinkedListCircular(const CLinkedListCircular &other){
        std::lock_guard<std::mutex> lock(other.m_mutex);
        
        if(!other.m_pRoot) return;
        
        Node *pCurrent = other.m_pRoot;
        size_t count = 0;
        
        do {
            push_back(pCurrent->GetValue(), pCurrent->GetRef());
            pCurrent = pCurrent->GetNext();
            ++count;
        } while(pCurrent != other.m_pRoot && count < other.m_nElements);
    }
    
    // Move Constructor
    CLinkedListCircular(CLinkedListCircular &&other) noexcept {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_pRoot = other.m_pRoot;
        m_pLast = other.m_pLast;
        m_nElements = other.m_nElements;
        
        other.m_pRoot = nullptr;
        other.m_pLast = nullptr;
        other.m_nElements = 0;
    }
    
    // Destructor seguro
    virtual ~CLinkedListCircular(){
        Clear();
    }
    
    // Iterators
    forward_iterator begin() { return forward_iterator(m_pRoot); }
    forward_iterator end()   { return forward_iterator(nullptr); }
    
    // operator[]
    value_type &operator[](size_t index){
        std::lock_guard<std::mutex> lock(m_mutex);
        if(index >= m_nElements || !m_pRoot)
            throw std::out_of_range("Index out of range");
        
        Node *pCurrent = m_pRoot;
        for(size_t i = 0; i < index; ++i)
            pCurrent = pCurrent->GetNext();
        
        return pCurrent->GetValueRef();
    }
    
    const value_type &operator[](size_t index) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(index >= m_nElements || !m_pRoot)
            throw std::out_of_range("Index out of range");
        
        Node *pCurrent = m_pRoot;
        for(size_t i = 0; i < index; ++i)
            pCurrent = pCurrent->GetNext();
        
        return pCurrent->GetValue();
    }

    // Foreach
    template<typename Func, typename... Args>
    void Foreach(Func fn, Args... args){
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_pRoot) return;
        
        Node *pCurrent = m_pRoot;
        size_t count = 0;
        
        do {
            fn(pCurrent->GetValueRef(), args...);
            pCurrent = pCurrent->GetNext();
            ++count;
        } while(pCurrent != m_pRoot && count < m_nElements);
    }
    
    // FirstThat
    template<typename Predicate>
    value_type *FirstThat(Predicate pred){
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_pRoot) return nullptr;
        
        Node *pCurrent = m_pRoot;
        size_t count = 0;
        
        do {
            if(pred(pCurrent->GetValue()))
                return &(pCurrent->GetValueRef());
            pCurrent = pCurrent->GetNext();
            ++count;
        } while(pCurrent != m_pRoot && count < m_nElements);
        
        return nullptr;
    }
    
    // Clear
    void Clear(){
        if(!m_pRoot) return;
        
        // Romper circularidad primero
        if(m_pLast)
            m_pLast->GetNextRef() = nullptr;
        
        Node *pCurrent = m_pRoot;
        while(pCurrent){
            Node *pNext = pCurrent->GetNext();
            delete pCurrent;
            pCurrent = pNext;
        }
        
        m_pRoot = nullptr;
        m_pLast = nullptr;
        m_nElements = 0;
    }

    // Métodos públicos
    void push_back(const value_type &val, ref_type ref = -1);
    void Insert(const value_type &val, ref_type ref = -1);
    size_t getSize() const { return m_nElements;  }
    
    // Verificar si es circular
    bool IsCircular() const { 
        return m_pRoot && m_pLast && m_pLast->GetNext() == m_pRoot; 
    }
    
private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref);

    // operator<<
    friend ostream &operator<<(ostream &os, CLinkedListCircular<Traits> &container){
        std::lock_guard<std::mutex> lock(container.m_mutex);
        os << "CLinkedListCircular: size = " << container.getSize();
        
        if(container.IsCircular())
            os << " [CIRCULAR]";
        
        os << endl << "[";
        
        if(container.m_pRoot){
            Node *pCurrent = container.m_pRoot;
            size_t count = 0;
            
            do {
                os << "(" << pCurrent->GetValue() << ":" << pCurrent->GetRef() << ")";
                pCurrent = pCurrent->GetNext();
                ++count;
                if(pCurrent != container.m_pRoot && count < container.m_nElements)
                    os << ",";
            } while(pCurrent != container.m_pRoot && count < container.m_nElements);
        }
        
        os << "]" << endl;
        return os;
    }
    
    // operator>>
    friend istream &operator>>(istream &is, CLinkedListCircular<Traits> &container){
        std::lock_guard<std::mutex> lock(container.m_mutex);
        value_type val;
        ref_type ref;
        is >> val >> ref;
        container.push_back(val, ref);
        return is;
    }
};

// ============================================
// IMPLEMENTACIONES
// ============================================

template <typename Traits>
void CLinkedListCircular<Traits>::push_back(const value_type &val, ref_type ref){
    std::lock_guard<std::mutex> lock(m_mutex);
    
    Node *pNewNode = new Node(val, ref);
    
    if( !m_pRoot ){
        // Primera inserción
        m_pRoot = pNewNode;
        m_pLast = pNewNode;
        // CONECTAR PRIMER Y ULTIMO (CIRCULAR)
        m_pLast->GetNextRef() = m_pRoot;
    } else {
        // Insertar al final
        m_pLast->GetNextRef() = pNewNode;
        m_pLast = pNewNode;
        // RECONECTAR ULTIMO CON PRIMERO (CIRCULAR)
        m_pLast->GetNextRef() = m_pRoot;
    }
    
    ++m_nElements;
}

template <typename Traits>
void CLinkedListCircular<Traits>::InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
    if( !rParent || rParent->GetValue() > val ){
        Node *pNew = new Node(val, ref, rParent);
        rParent = pNew;
        
        if(!m_pLast || !rParent->GetNext()){
            m_pLast = pNew;
            // CONECTAR CIRCULAR
            m_pLast->GetNextRef() = m_pRoot;
        }
        
        ++m_nElements;
        return;
    }
    
    if(!rParent->GetNext() || rParent->GetNext() == m_pRoot){
        m_pLast = rParent;
    }
    
    InternalInsert(rParent->GetNextRef(), val, ref);
}

template <typename Traits>
void CLinkedListCircular<Traits>::Insert(const value_type &val, ref_type ref){
    std::lock_guard<std::mutex> lock(m_mutex);
    InternalInsert(m_pRoot, val, ref);
    
    // Asegurar circularidad
    if(m_pLast && m_pRoot)
        m_pLast->GetNextRef() = m_pRoot;
}

#endif // __LINKEDLIST_CIRCULAR_H__
