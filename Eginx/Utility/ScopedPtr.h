#ifndef SCOPEDPTR_H
#define SCOPEDPTR_H

#include <assert.h>
#include <cstddef>
#include <stdlib.h>

template <class C>
class ScopedPtr {
public:
    typedef C element_type;

    explicit ScopedPtr(C* p = NULL) : m_ptr(p) { }

    ~ScopedPtr() {
        enum { type_must_be_complete = sizeof(C) };
        if(m_ptr){
            delete m_ptr;
            m_ptr = NULL;
        }
    }

    void reset(C* p = NULL) {
        if (p != m_ptr) {
            enum { type_must_be_complete = sizeof(C) };
            if(m_ptr){
                delete m_ptr;
                m_ptr = NULL;
            }
            m_ptr = p;
        }
    }

    C& operator*() const {
        assert(m_ptr != NULL);
        return *m_ptr;
    }
    C* operator->() const  {
        assert(m_ptr != NULL);
        return m_ptr;
    }
    C* get() const { return m_ptr; }

    bool operator==(C* p) const { return m_ptr == p; }
    bool operator!=(C* p) const { return m_ptr != p; }

    void swap(ScopedPtr& p2) {
        C* tmp = m_ptr;
        m_ptr = p2.m_ptr;
        p2.m_ptr = tmp;
    }

    C* release() {
        C* retVal = m_ptr;
        m_ptr = NULL;
        return retVal;
    }

private:
    C* m_ptr;

    template <class C2> bool operator==(ScopedPtr<C2> const& p2) const;
    template <class C2> bool operator!=(ScopedPtr<C2> const& p2) const;

    ScopedPtr(const ScopedPtr&);
    void operator=(const ScopedPtr&);
};

template <class C>
void swap(ScopedPtr<C>& p1, ScopedPtr<C>& p2) {
    p1.swap(p2);
}

template <class C>
bool operator==(C* p1, const ScopedPtr<C>& p2) {
    return p1 == p2.get();
}

template <class C>
bool operator!=(C* p1, const ScopedPtr<C>& p2) {
    return p1 != p2.get();
}

template <class C>
class ScopedArray {
public:
    typedef C element_type;

    explicit ScopedArray(C* p = NULL) : m_array(p) { }

    ~ScopedArray() {
        enum { type_must_be_complete = sizeof(C) };
        if(m_array){
            delete[] m_array;
            m_array = NULL;
        }

    }

    void reset(C* p = NULL) {
        if (p != m_array) {
            enum { type_must_be_complete = sizeof(C) };
            if(m_array){
                delete[] m_array;
                m_array = NULL;
            }
            m_array = p;
        }
    }

    C& operator[](std::ptrdiff_t i) const {
        assert(i >= 0);
        assert(m_array != NULL);
        return m_array[i];
    }

    C* get() const {
        return m_array;
    }

    bool operator==(C* p) const { return m_array == p; }
    bool operator!=(C* p) const { return m_array != p; }

    void swap(ScopedArray& p2) {
        C* tmp = m_array;
        m_array = p2.m_array;
        p2.m_array = tmp;
    }

    C* release() {
        C* retVal = m_array;
        m_array = NULL;
        return retVal;
    }

private:
    C* m_array;

    template <class C2> bool operator==(ScopedArray<C2> const& p2) const;
    template <class C2> bool operator!=(ScopedArray<C2> const& p2) const;

    ScopedArray(const ScopedArray&);
    void operator=(const ScopedArray&);
};

template <class C>
void swap(ScopedArray<C>& p1, ScopedArray<C>& p2) {
    p1.swap(p2);
}

template <class C>
bool operator==(C* p1, const ScopedArray<C>& p2) {
    return p1 == p2.get();
}

template <class C>
bool operator!=(C* p1, const ScopedArray<C>& p2) {
    return p1 != p2.get();
}

template <class T>
class ScopedRefPtr {
public:
    ScopedRefPtr() : m_ptr(NULL) {
    }

    ScopedRefPtr(T* p) : m_ptr(p) {
        if (m_ptr)
            m_ptr->AddRef();
    }

    ScopedRefPtr(const ScopedRefPtr<T>& r) : m_ptr(r.m_ptr) {
        if (m_ptr)
            m_ptr->AddRef();
    }

    template <typename U>
    ScopedRefPtr(const ScopedRefPtr<U>& r) : m_ptr(r.get()) {
        if (m_ptr)
            m_ptr->AddRef();
    }

    ~ScopedRefPtr() {
        if (m_ptr)
            m_ptr->Release();
    }

    T* get() const { return m_ptr; }
    operator T*() const { return m_ptr; }
    T* operator->() const { return m_ptr; }

    T* release() {
        T* retVal = m_ptr;
        m_ptr = NULL;
        return retVal;
    }

    ScopedRefPtr<T>& operator=(T* p) {
        // AddRef first so that self assignment should work
        if (p)
            p->AddRef();
        if (m_ptr )
            m_ptr ->Release();
        m_ptr = p;
        return *this;
    }

    ScopedRefPtr<T>& operator=(const ScopedRefPtr<T>& r) {
        return *this = r.m_ptr;
    }

    template <typename U>
    ScopedRefPtr<T>& operator=(const ScopedRefPtr<U>& r) {
        return *this = r.get();
    }

    void swap(T** pp) {
        T* p = m_ptr;
        m_ptr = *pp;
        *pp = p;
    }

    void swap(ScopedRefPtr<T>& r) {
        swap(&r.m_ptr);
    }

protected:
    T* m_ptr;
};

#endif //SCOPEDPTR_H

