#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <iostream>
#include <mutex>
#include <stdexcept>
#include "../general/types.h"
#include "../util.h"

using namespace std;


// TRAITS

template <typename T, typename _Func>
struct ListTrait{
    using value_type = T;
    using Func       = _Func;
};

template <typename T>
struct AscendingTrait : public ListTrait<T, std::greater<T> > {};

template <typename T>
struct DescendingTrait : public ListTrait<T, std::less<T> > {};

// Forward declarations
template <typename Traits> class NodeLinkedList;
template <typename Traits> class CLinkedList;
template <typename Traits> class LinkedListForwardIterator;


// NODE

template <typename Traits>
class NodeLinkedList{
    template <typename T> friend class CLinkedList;
    template <typename T> friend class LinkedListForwardIterator;
    
    using  value_type  = typename Traits::value_type;
    using  Node        = NodeLinkedList<Traits>;
private:
    value_type m_data;
    ref_type   m_ref;
    Node *m_pNext = nullptr;

public:
    NodeLinkedList(){}
    NodeLinkedList( value_type _value, ref_type _ref = -1)
        : m_data(_value), m_ref(_ref){   }
    NodeLinkedList( value_type _value, ref_type _ref, Node *pNext)
        : m_data(_value), m_ref(_ref), m_pNext(pNext){   }
        
    value_type  GetValue   () const { return m_data; }
    value_type &GetValueRef() { return m_data; }

    ref_type    GetRef     () const { return m_ref;   }
    ref_type   &GetRefRef  () { return m_ref;   }

    Node      * GetNext     () const { return m_pNext;   }
    Node      *&GetNextRef  () { return m_pNext;   }
};


// FORWARD ITERATOR

template <typename Traits>
class LinkedListForwardIterator {
    using Node = NodeLinkedList<Traits>;
    using value_type = typename Traits::value_type;
    Node *m_pCurrent;

public:
    LinkedListForwardIterator(Node *pNode) : m_pCurrent(pNode) {}
    
    value_type &operator*() { return m_pCurrent->GetValueRef(); }
    
    LinkedListForwardIterator &operator++() {
        if(m_pCurrent) m_pCurrent = m_pCurrent->GetNext();
        return *this;
    }
    
    bool operator!=(const LinkedListForwardIterator &other) const {
        return m_pCurrent != other.m_pCurrent;
    }
    
    bool operator==(const LinkedListForwardIterator &other) const {
        return m_pCurrent == other.m_pCurrent;
    }
};

template <typename Traits>
class CLinkedList {
    using  value_type  = typename Traits::value_type;
    using  forward_iterator  = LinkedListForwardIterator<Traits>;
    using  Node = NodeLinkedList<Traits>;

    Node *m_pRoot = nullptr;
    Node *m_pLast = nullptr;
    size_t m_nElements = 0;
    
    // Concurrencia (Checklist item)
    mutable std::mutex m_mutex;

public:
    CLinkedList(){}
    
    // Constructor Copia (Checklist item)
    CLinkedList(const CLinkedList &other){
        std::lock_guard<std::mutex> lock(other.m_mutex);
        Node *pCurrent = other.m_pRoot;
        while(pCurrent){
            push_back(pCurrent->GetValue(), pCurrent->GetRef());
            pCurrent = pCurrent->GetNext();
        }
    }
    
    // Move Constructor (Checklist item)
    CLinkedList(CLinkedList &&other) noexcept {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_pRoot = other.m_pRoot;
        m_pLast = other.m_pLast;
        m_nElements = other.m_nElements;
        
        other.m_pRoot = nullptr;
        other.m_pLast = nullptr;
        other.m_nElements = 0;
    }
    
    // Destructor (Checklist item)
    virtual ~CLinkedList(){ Clear(); }
    
    // Iterators (Checklist item)
    forward_iterator begin() { return forward_iterator(m_pRoot); }
    forward_iterator end()   { return forward_iterator(nullptr); }
    
    // Operator [] (Checklist item)
    value_type &operator[](size_t index){
        std::lock_guard<std::mutex> lock(m_mutex);
        if(index >= m_nElements) throw std::out_of_range("Index out of range");
        Node *pCurrent = m_pRoot;
        for(size_t i = 0; i < index; ++i) pCurrent = pCurrent->GetNext();
        return pCurrent->GetValueRef();
    }
    
    const value_type &operator[](size_t index) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(index >= m_nElements) throw std::out_of_range("Index out of range");
        Node *pCurrent = m_pRoot;
        for(size_t i = 0; i < index; ++i) pCurrent = pCurrent->GetNext();
        return pCurrent->GetValue();
    }

    // Foreach (Checklist item)
    template<typename Func, typename... Args>
    void Foreach(Func fn, Args... args){
        std::lock_guard<std::mutex> lock(m_mutex);
        Node *pCurrent = m_pRoot;
        while(pCurrent){
            fn(pCurrent->GetValueRef(), args...);
            pCurrent = pCurrent->GetNext();
        }
    }
    
    // FirstThat (Checklist item)
    template<typename Predicate>
    value_type *FirstThat(Predicate pred){
        std::lock_guard<std::mutex> lock(m_mutex);
        Node *pCurrent = m_pRoot;
        while(pCurrent){
            if(pred(pCurrent->GetValue())) return &(pCurrent->GetValueRef());
            pCurrent = pCurrent->GetNext();
        }
        return nullptr;
    }
    
    void Clear(){
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

    void push_back(const value_type &val, ref_type ref = -1);
    void Insert(const value_type &val, ref_type ref = -1);
    size_t getSize() const { return m_nElements;  }
    
private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref);

    // Operator << (Output) (Checklist item)
    friend ostream &operator<<(ostream &os, CLinkedList<Traits> &container){
        std::lock_guard<std::mutex> lock(container.m_mutex);
        os << "CLinkedList: size = " << container.getSize() << endl;
        os << "[ ";
        Node *pCurrent = container.m_pRoot;
        while(pCurrent){
            os << "(" << pCurrent->GetValue() << ":" << pCurrent->GetRef() << ")";
            pCurrent = pCurrent->GetNext();
            if(pCurrent) os << ", ";
        }
        os << " ]" << endl;
        return os;
    }

    // Operator >> (Input) (Checklist item - NUEVO)
    friend istream &operator>>(istream &is, CLinkedList<Traits> &container){
        std::lock_guard<std::mutex> lock(container.m_mutex);
        value_type val;
        ref_type ref;
        // Intenta leer valor y referencia
        if(is >> val >> ref){
             // Usa push_back por defecto al leer de stream
            container.push_back(val, ref);
        }
        return is;
    }
};

// IMPLEMENTACIONES

template <typename Traits>
void CLinkedList<Traits>::push_back(const value_type &val, ref_type ref){
    std::lock_guard<std::mutex> lock(m_mutex);
    Node *pNewNode = new Node(val, ref);
    if( !m_pRoot ){
        m_pRoot = pNewNode;
        m_pLast = pNewNode;
    } else {
        m_pLast->GetNextRef() = pNewNode;
        m_pLast = pNewNode;
    }
    ++m_nElements;
}

template <typename Traits>
void CLinkedList<Traits>::InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
    // CORRECCIÓN: Usar Traits::Func() para soportar Ascendente/Descendente
    if( !rParent || typename Traits::Func()(rParent->GetValue(), val) ){
        Node *pNew = new Node(val, ref, rParent);
        rParent = pNew;
        // Si insertamos al final o la lista estaba vacía, actualizar m_pLast
        if(pNew->GetNext() == nullptr) m_pLast = pNew;
        ++m_nElements;
        return;
    }
    InternalInsert(rParent->GetNextRef(), val, ref);
}

template <typename Traits>
void CLinkedList<Traits>::Insert(const value_type &val, ref_type ref){
    std::lock_guard<std::mutex> lock(m_mutex);
    InternalInsert(m_pRoot, val, ref);
}

#endif // __LINKEDLIST_H__