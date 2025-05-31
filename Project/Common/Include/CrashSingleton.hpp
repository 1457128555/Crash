#pragma once

#include <cassert>

namespace Crash
{
    template <typename T>
    class Singleton
    {
    public:
       static T* Instance()
       {
            assert(m_instance != nullptr && "Singleton instance does not exist!");
            return m_instance;
       }

    protected:
        Singleton()                
        {
            assert(m_instance == nullptr && "Singleton instance already exists!");
            m_instance = static_cast<T*>(this);
        }

        virtual ~Singleton()       
        {
            assert(m_instance != nullptr && "Singleton instance does not exist!");
            m_instance = nullptr;
        }
    
    private:
        static inline T* m_instance = nullptr;

    private:
        Singleton(const Singleton&)             = delete;
        Singleton& operator=(const Singleton&)  = delete;
    };
}