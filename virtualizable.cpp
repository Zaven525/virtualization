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
    std::string typeId;
    std::string TypeName () { return typeId; }
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
        typeId = typeName;
        registerVirtual("foo", func1Ptr);
        registerVirtual("bar", func2Ptr);
    }

    void foo() { std::cout << "Base foo" << std::endl; }
    void bar() { std::cout << "Base bar" << std::endl; }

    static std::string TypeName() {
      return "Base";
    }
};

class Derived : public Base
{
public: 
    Derived() : Base(static_cast<FnPtr>(&Derived::foo), "fooD", static_cast<FnPtr>(&Derived::bar), "barD", "Derived") {}
    void foo() { std::cout << "Derived foo" << std::endl; }
    void bar() { std::cout << "Derived bar" << std::endl; }
    static std::string TypeName() {
        return "Derived";
    }
};

template<typename T>
T* dynamicCast(Virtualizable* type) 
{
    if (type->TypeName() == T::TypeName()) { return static_cast<T*>(type); }
    return nullptr;
};


int main()
{
    Virtualizable* v = new Derived();

    std::cout << "type = " << v->TypeName() << "\n";  // Derived

    // Test virtual calls
    v->callVirtual("foo");   // Derived foo
    v->callVirtual("bar");   // Derived bar

    // Test dynamicCast
    Derived* d = dynamicCast<Derived>(v);
    if (d)
    {
        std::cout << "dynamicCast OK\n";
        d->foo();            // Derived foo
    }
    else
    {
        std::cout << "dynamicCast FAILED\n";
    }

    // Try casting to Base
    Base* b = dynamicCast<Base>(v);
    if (b)
    {
        std::cout << "Cast to Base OK\n";
        b->foo();            // Derived foo  (because override)
    }

    delete v;
}
