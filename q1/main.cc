class C{
public:
    virtual void f() = 0;
    virtual ~C() = 0;
};
C::~C() {}

class D: public C {
public:
    void f() override {}
    // ~D() override {}
};

int main() {
    D d;
}