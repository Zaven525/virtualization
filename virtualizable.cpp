#include<iostream>
#include<vector>

class Virtualizable
{
public:
    using FnPtr = void (Virtualizable::*)();
private:
    std::vector<std::string> names;
    std::vector<FnPtr> funcs;
public:
    Virtualizable() {}
    void registerVirtual(std::string name, FnPtr fn) 
    { 
        names.push_back(name); 
        funcs.push_back(fn); 
    }
    void callVirtual(std::string name) 
    {
        for (size_t i = 0; i < names.size(); i++)
        {   
            if (names[i] == name) { (this->*funcs[i])(); }
        }
    };
};

class Base : public Virtualizable
{
public:
    Base(FnPtr func1Ptr, std::string func1Name, FnPtr func2Ptr, std::string func2Name, std::string typeName = "Base")
    {
        registerVirtual("foo", func1Ptr);
        registerVirtual("bar", func2Ptr);
    }
    void foo() { std::cout << "Base foo" << std::endl; }
    void bar() { std::cout << "Base bar" << std::endl; }
};

class Derived : public Base
{
public:
    static bool is_derived() { return true; }
    Derived() : Base(static_cast<FnPtr>(foo), "fooD", static_cast<FnPtr>(bar), "barD", "Derived") {}
    void foo() { std::cout << "Derived foo" << std::endl; }
    void bar() { std::cout << "Derived bar" << std::endl; }
};

template<typename T>
T* dynamicCast(Virtualizble* type) 
{
    if(type::is_derived()) return static_cast<Virtualizable::FnPtr>(type);
};