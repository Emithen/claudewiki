// ============================================================
// Polymorphism 샌드박스
// 실험 1: Static vs Dynamic Binding
// 실험 2: 추상 클래스 + 순수 가상 함수
// 실험 3: 가상 소멸자 (virtual destructor)
// 실험 4: override / final
//
// 빌드: g++ -std=c++17 main.cpp -o poly && ./poly
// ============================================================

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// 실험 1: Static Binding vs Dynamic Binding
// ============================================================
namespace binding_demo {

class Base {
public:
    void staticMethod() { cout << "[Static]  Base::staticMethod\n"; }
    virtual void dynamicMethod() { cout << "[Dynamic] Base::dynamicMethod\n"; }
};

class Derived : public Base {
public:
    void staticMethod() { cout << "[Static]  Derived::staticMethod\n"; }
    void dynamicMethod() override { cout << "[Dynamic] Derived::dynamicMethod\n"; }
};

void run() {
    cout << "\n--- 실험 1: Static vs Dynamic Binding ---\n";

    Derived d;
    Base* b = &d;         // Base* 포인터로 Derived 객체 가리킴

    b->staticMethod();    // "Base::staticMethod"  — 포인터 타입(Base) 기준
    b->dynamicMethod();   // "Derived::dynamicMethod" — 실제 객체(Derived) 기준

    // 직접 호출은 항상 해당 타입 기준
    d.staticMethod();     // "Derived::staticMethod"
    d.dynamicMethod();    // "Derived::dynamicMethod"
}

} // namespace binding_demo

// ============================================================
// 실험 2: 추상 클래스 + 다형 컨테이너
// ============================================================
namespace shape_demo {

class Shape {
public:
    virtual void draw() const = 0;          // 순수 가상 함수 — 구현 강제
    virtual double area() const = 0;
    virtual string typeName() const = 0;
    virtual ~Shape() {}                      // 가상 소멸자 필수
};

class Circle : public Shape {
    double r;
public:
    explicit Circle(double r) : r(r) {}
    void draw() const override {
        cout << "  ● Circle (r=" << r << ")\n";
    }
    double area() const override { return 3.14159 * r * r; }
    string typeName() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double w, h;
public:
    Rectangle(double w, double h) : w(w), h(h) {}
    void draw() const override {
        cout << "  ■ Rectangle (" << w << " x " << h << ")\n";
    }
    double area() const override { return w * h; }
    string typeName() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double base, height;
public:
    Triangle(double b, double h) : base(b), height(h) {}
    void draw() const override {
        cout << "  ▲ Triangle (base=" << base << ", h=" << height << ")\n";
    }
    double area() const override { return 0.5 * base * height; }
    string typeName() const override { return "Triangle"; }
};

void run() {
    cout << "\n--- 실험 2: 추상 클래스 + 다형 컨테이너 ---\n";

    // unique_ptr로 소유권 관리 (raw pointer delete 안 해도 됨)
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5.0));
    shapes.push_back(make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(make_unique<Triangle>(6.0, 8.0));
    shapes.push_back(make_unique<Circle>(2.5));

    double total = 0;
    for (const auto& s : shapes) {
        s->draw();
        cout << "    area = " << s->area() << "\n";
        total += s->area();
    }
    cout << "  Total area: " << total << "\n";

    // 새 도형 타입 추가해도 이 for 루프는 수정할 필요 없음 — OCP
}

} // namespace shape_demo

// ============================================================
// 실험 3: 가상 소멸자 — 없을 때의 문제
// ============================================================
namespace destructor_demo {

class WithoutVirtual {
public:
    ~WithoutVirtual() { cout << "  WithoutVirtual 소멸\n"; }
};
class DerivedA : public WithoutVirtual {
    int* data;
public:
    DerivedA() { data = new int[100]; cout << "  DerivedA 생성 (메모리 할당)\n"; }
    ~DerivedA() { delete[] data; cout << "  DerivedA 소멸 (메모리 해제)\n"; }
};

class WithVirtual {
public:
    virtual ~WithVirtual() { cout << "  WithVirtual 소멸\n"; }
};
class DerivedB : public WithVirtual {
    int* data;
public:
    DerivedB() { data = new int[100]; cout << "  DerivedB 생성 (메모리 할당)\n"; }
    ~DerivedB() { delete[] data; cout << "  DerivedB 소멸 (메모리 해제)\n"; }
};

void run() {
    cout << "\n--- 실험 3: 가상 소멸자 ---\n";

    cout << " [가상 소멸자 없음]\n";
    {
        WithoutVirtual* p = new DerivedA();
        delete p;  // DerivedA 소멸자 호출 안 됨 → 메모리 누수
    }

    cout << " [가상 소멸자 있음]\n";
    {
        WithVirtual* p = new DerivedB();
        delete p;  // DerivedB 소멸자 → WithVirtual 소멸자 순서대로 호출
    }
}

} // namespace destructor_demo

// ============================================================
// 실험 4: override / final
// ============================================================
namespace override_final_demo {

class Base {
public:
    virtual void foo() const { cout << "  Base::foo\n"; }
    virtual void bar() { cout << "  Base::bar\n"; }
    virtual ~Base() {}
};

class Middle : public Base {
public:
    void foo() const override { cout << "  Middle::foo\n"; }
    void bar() override final { cout << "  Middle::bar (final)\n"; }
    // final: 이후 상속에서 bar() 재정의 불가
};

class Leaf : public Middle {
public:
    void foo() const override { cout << "  Leaf::foo\n"; }
    // void bar() override {}  // 컴파일 에러 — final 위반
};

void run() {
    cout << "\n--- 실험 4: override / final ---\n";

    Leaf leaf;
    Base* b = &leaf;
    b->foo();  // Leaf::foo
    b->bar();  // Middle::bar (final — Leaf가 재정의 못함)
}

} // namespace override_final_demo

// ============================================================
// main
// ============================================================
int main() {
    binding_demo::run();
    shape_demo::run();
    destructor_demo::run();
    override_final_demo::run();
    return 0;
}
