#include <iostream>
#include <memory>
#include <string>
#include <map>

class Traversal;

class Expression {
public:
    virtual int doTraverse(Traversal &trav) = 0;
    virtual ~Expression() {}
};

class Plus; class Variable;

class Traversal {
public:
    virtual int traverse(Variable &expr) = 0;
    virtual int traverse(Plus &expr) = 0;
    virtual ~Traversal() {}
};

class Plus: public Expression {
    Expression *left, *right;
public:
    Plus(Expression *left, Expression *right): left{left}, right{right} {}
    ~Plus() { delete left; delete right; }
    Expression *getLeft() { return left; }
    Expression *getRight() { return right; }

    int doTraverse(Traversal &trav) override {
        return trav.traverse(*this);
    }
};

class Variable: public Expression {
    const std::string name;
public:
    Variable(std::string name): name{name} {};
    const std::string &getName() { return name; }

    int doTraverse(Traversal &trav) override {
        return trav.traverse(*this);
    }
};

class EvaluateTraversal: public Traversal {
    std::map<std::string,int> varValues;
public:
    EvaluateTraversal(std::initializer_list<std::pair<const std::string,int>>vals): varValues{vals}{}
    
    int traverse(Variable &expr) override {
        return varValues[expr.getName()];
    }

    int traverse(Plus &expr) override {
        return expr.getLeft()->doTraverse(*this)
            + expr.getRight()->doTraverse(*this);
    }
};

int main() {
    std::unique_ptr<Expression> ptr = std::make_unique<Plus>(
        new Variable("a"), new Variable("b")
    );
    std::unique_ptr<Traversal> trav {new EvaluateTraversal{{"a", 5}, {"b", 6}}};

    std::cout << ptr->doTraverse(*trav) << std::endl;
}