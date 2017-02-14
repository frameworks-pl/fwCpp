#ifndef FWCORE_H_INCLUDED
#define FWCORE_H_INCLUDED

namespace fw
{
    namespace core
    {

        //--------------------------------------------------------------------------------
        template <class T>
        class NullableVar
        {
            public:
                virtual ~NullableVar() { }
                NullableVar() : m_pPointer(NULL) { }
                NullableVar(const T& value) {
                    *this = value;
                }

                NullableVar(const NullableVar<T>& pValue) {
                    *this = pValue;
                }

                NullableVar<T>& operator=(const T &value)
                {
                    m_pPointer = &m_Value;
                    *m_pPointer = value;
                    return *this;
                }

                const NullableVar<T>& operator=(const NullableVar<T>& pOrgNullableVar)
                {
                    if (pOrgNullableVar.IsSet())
                        *this = pOrgNullableVar();
                    else
                        m_pPointer = NULL;
                    return *this;
                }

                bool IsSet() const { return m_pPointer != NULL; }

                const T& operator ()() const
                {
                    if (m_pPointer)
                        return *m_pPointer;

                    throw std::string("Trying to read nullable value that is not set. Use IsSet first.");
                }

            private:
                T* m_pPointer;
                T m_Value;

        }; //class NullableVar
        //--------------------------------------------------------------------------------


        //--------------------------------------------------------------------------------
        class StateLocker
        {
            public:
                StateLocker(bool* pFlagToLock, bool bNewState)
                {
                    m_pFlag = pFlagToLock;
                    m_bNewState = bNewState;
                    if (m_pFlag)
                    {
                        m_bAlreadySet = (bNewState == *m_pFlag);
                        if (false == m_bAlreadySet)
                            *m_pFlag = bNewState;

                    }
                }

                virtual ~StateLocker()
                {
                    if (m_pFlag != NULL && false == m_bAlreadySet)
                        *m_pFlag = !m_bNewState;
                    m_pFlag = NULL;
                }
            protected:


                bool* m_pFlag;
                bool m_bAlreadySet;
                bool m_bNewState;
        };
        //--------------------------------------------------------------------------------


        //--------------------------------------------------------------------------------
        template <class T>
        class RefCountPtr
        {
            public:
                RefCountPtr()
                {
                    m_pObject = NULL;
                    m_pRefCount = new int(0);
                }

                RefCountPtr(T* pT)
                {
                    m_pObject = pT;
                    m_pRefCount = new int(1);
                }

                RefCountPtr(const RefCountPtr<T>& pT)
                {
                    *this = pT;
                }

                const RefCountPtr<T>& operator=(const RefCountPtr<T>& pT)
                {
                    m_pRefCount = pT.m_pRefCount;
                    m_pObject = pT.m_pObject;
                    (*m_pRefCount)++;
                    return *this;
                }

                virtual ~RefCountPtr()
                {
                    if ((*m_pRefCount) > 0)
                        *m_pRefCount -= 1;
                    if (*m_pRefCount == 0)
                    {
                        delete m_pObject;
                        m_pObject = NULL;

                        delete m_pRefCount;
                    }
                }

                int GetRefCount()
                {
                    return *m_pRefCount;
                }
            protected:
                int* m_pRefCount;
                T* m_pObject;
        };
        //--------------------------------------------------------------------------------



    } //namespace core
} //namespace fw

#endif //FWCORE_H_INCLUDED