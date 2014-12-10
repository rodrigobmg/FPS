#include <vector>
#include <algorithm>
#include <iostream>

// http://stackoverflow.com/questions/557553/c-bound-method-queue-task-manager-scheduler

/*
	Event, s aprepisuje RUn ale musime vediet ktory event si chcceme 
	registrovat treba definovat ktory je ktory... 
	cize cez type ID a posleme ktore mame.
	
	* Event autoloader, static Register a singleton
*/

template <typename ARG>
class Eventsystem
{
private:
    class DelegateBase
    {
    public:
        virtual ~DelegateBase() { }
        virtual void Invoke(ARG arg) = 0;
    };

    template <typename T>
    class Delegate : public DelegateBase
    {
    public:
        typedef void (T::*Func)(ARG arg);

    private:
        Func m_func;
        T* m_object;

    public:
        Delegate(T* object, Func func)
            : m_object(object), m_func(func)
        { }

        virtual void Invoke(ARG arg) 
        { 
                ((*m_object).*(m_func))(arg);
        }
    };

    typedef std::vector<DelegateBase*> Delegates;
    Delegates m_delegates;

public:
    ~Eventsystem()
    {
        Clear();
    }

    void Clear()
    {
        Delegates::iterator item = m_delegates.begin();

        for (; item != m_delegates.end(); ++item)
        {
            delete *item;
        }

        m_delegates.clear();
    }

    template <typename T>
    void AddDelegate(T& object, typename Delegate<T>::Func func)
    {
        DelegateBase* delegate = new Delegate<T>(&object, func);
        m_delegates.push_back(delegate);
    }

    void Invoke(ARG arg)
    {
        Delegates::iterator item = m_delegates.begin();

        for (; item != m_delegates.end(); ++item)
        {
                (*item)->Invoke(arg);
        }
    }

};

class TaskObject1
{
public:
    void CallOne(const wchar_t* value)
    {
        std::wcout << L"CallOne(): " << value << std::endl;
    }

    void CallTwo(const wchar_t* value)
    {
        std::wcout << L"CallTwo(): " << value << std::endl;
    }
};

class TaskObject2
{
public:
    void CallThree(const wchar_t* value)
    {
        std::wcout << L"CallThree(): " << value << std::endl;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    Eventsystem<const wchar_t*> tasks;

    TaskObject1 obj1;
    TaskObject2 obj2;

    tasks.AddDelegate(obj1, &TaskObject1::CallOne);
    tasks.AddDelegate(obj1, &TaskObject1::CallTwo);
    tasks.AddDelegate(obj2, &TaskObject2::CallThree);

    tasks.Invoke(L"Hello, World!\n");

    return 0;
}
