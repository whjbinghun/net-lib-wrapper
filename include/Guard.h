/******************************************************************
一些通用类的实现
******************************************************************/

#pragma once

#include "singletonHandler.h"

/*
通过局部返回类的方式，对每一个->操作进行加锁和解锁操作
*/
template<typename T>
class CLockingProxy  //加锁解锁类
{
public:
    explicit CLockingProxy( T *p ) : m_pObj(p) { m_pObj->Lock(); }
    ~CLockingProxy(){ m_pObj->unLock(); }

    T* operator->() const { return m_pObj; }//为什么要重定义？

private:
    CLockingProxy& operator=(const CLockingProxy& );
    T *m_pObj;
};


/*
线程安全指针
*/
template<typename T>
class CThreadSafePtr
{
public:
    explicit CThreadSafePtr( T *p ) : m_pObj( p ){}
    ~CThreadSafePtr(){}

    CLockingProxy<T> operator->() const
    {
        return CLockingProxy<T>( m_pObj );//？
    }
private:
    T *m_pObj;
};

//from Loki Compile Time Check
template<bool> struct CompileTimeError;
template<> struct CompileTimeError<false>{};
#define STATIC_CHECK(expr)  (CompileTimeError<(expr) != 0>())

template <typename T, typename U>
class CConversion
{
    typedef char Small;
    class Big { char dummy[2]; };
    static Small Test(U);
    static Big Test(...);
    static T MakeT();
public:
    enum { exists = (sizeof(Test(MakeT())) == sizeof(Small)) };
    enum { exists2Way = (exists && CConversion<U, T>::exists) };
    enum { sameType = false };
};

template< typename T>
class CConversion<T, T>
{
public:
    enum { exists = 1, exists2Way = 1, sameType = 1, };
};

#define SUPERSUBCLASS(T, U)\
    (CConversion<const U*, const T*>::exists &&\
    !CConversion<const T*, void*>::sameType )

#define INHERITSCLASS(T, U)\
    (CConversion<const U*, const T*>::exists ||\
    CConversion<const T*, const U*>::exists )


/*
引用计数方式智能指针
*/
template <typename T>
class CRefCountPtr
{
public:
    CRefCountPtr() : m_pObj(0), m_pRef(0) {}
    explicit CRefCountPtr(T* p) : m_pObj(p)
    {
        m_pRef = new long;
        *m_pRef = 1;
    }

    CRefCountPtr(const CRefCountPtr<T>& rcf) : m_pObj(rcf.m_pObj), m_pRef(rcf.m_pRef)
    {
        if (m_pRef){
            __sync_fetch_and_add( m_pRef, 1 );
        }
    }

    template<typename U>
    CRefCountPtr(const CRefCountPtr<U>& rcf) : m_pObj(reinterpret_cast<T*>(rcf.Obj())), m_pRef(rcf.Ref())
    {
        STATIC_CHECK( !INHERITSCLASS(T, U) );
        if( m_pRef ){
            __sync_fetch_and_add( m_pRef, 1 );
        }
    }

    template<typename U>
    operator CRefCountPtr<U>(){ return CRefCountPtr<U>(*this); }


    ~CRefCountPtr()
    {
        Release();
    }

    CRefCountPtr<T>& operator=(const CRefCountPtr<T>& rcf)
    {
        Release();
        m_pObj = rcf.m_pObj;
        if (m_pObj){
            m_pRef = rcf.m_pRef;
            __sync_fetch_and_add( m_pRef, 1 );
        }
        return *this;
    }

    bool operator==(const CRefCountPtr<T>& rcf) const
    {
        if (m_pObj && rcf.m_pObj && m_pRef && rcf.m_pRef &&
            m_pObj == rcf.m_pObj && m_pRef == rcf.m_pRef){
            return true;
        }
        return false;
    }

    template<typename U>
    CRefCountPtr<T>& operator=(const CRefCountPtr<U>& rcf)
    {
        STATIC_CHECK( !INHERITSCLASS(T, U) );
        Release();
        m_pObj = dynamic_cast<T*>(rcf.m_pObj);
        if(m_pObj){
            m_pRef = rcf.m_pRef;
            __sync_fetch_and_add( m_pRef, 1 );
        }
        return *this;
    }

    T* operator->() const { return m_pObj; }
    T& operator*() const { return *m_pObj; }
    T* Obj() const { return m_pObj; }
    long* Ref() const { return m_pRef; }

private:
    void Release()
    {
        if (m_pRef && *m_pRef != 0)
        {
            if ( __sync_sub_and_fetch(m_pRef, 1) == 0 )
            {
                delete m_pObj;
                m_pObj = NULL;
                delete m_pRef;
                m_pRef = NULL;
            }
        }
    }

private:
    T* m_pObj;
    long* m_pRef;
};

template<class T>
bool operator==( const CRefCountPtr<T> &left, const CRefCountPtr<T> &right )
{
    return left.operator ==(right);
}

/*
普通智能指针
*/
template<typename T>
class CSmartlPtr
{
public:
    CSmartlPtr( T *pPtr, bool bArray, unsigned int unBuffSize = -1)
    {
        m_pPtr = pPtr;
        m_bArray = bArray;
        m_unBuffSize = unBuffSize;
    }

    ~CSmartlPtr()
    {
        if( m_bArray ){
            delete []m_pPtr;
        }else{
            delete m_pPtr;
        }

        m_pPtr = NULL;
    }

    T* data()
    {
        return m_pPtr;
    }

    unsigned int getBufferSize()
    {
        return m_unBuffSize;
    }

private:
    CSmartlPtr(const CSmartlPtr<T>& rcf);
    CSmartlPtr<T>& operator=(const CSmartlPtr<T>& rcf);

private:
    T *m_pPtr;
    bool m_bArray;
    unsigned int m_unBuffSize;
};

