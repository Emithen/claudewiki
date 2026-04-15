# Polymorphism (다형성)

한 줄 요약: 같은 인터페이스 호출이 실제 객체 타입에 따라 다른 동작을 하게 만드는 OOP의 핵심 메커니즘.

## 직관

전원 콘센트를 생각해보자. 콘센트 구멍(인터페이스)은 하나인데, 꽂는 기기(구현체)에 따라 충전기가 될 수도, 선풍기가 될 수도 있다. 콘센트는 꽂힌 게 뭔지 몰라도 된다.

다형성도 같다. `shape.draw()` 호출 하나로, 실제 객체가 Circle이면 원을 그리고, Rectangle이면 사각형을 그린다. **호출하는 쪽은 실제 타입을 몰라도 된다.**

구조적 유사성:
- TCP/IP 계층 분리와 같다 — HTTP는 TCP가 어떻게 전달하는지 몰라도 됨
- 인터페이스가 "무엇을 하는가"를, 구현체가 "어떻게 하는가"를 담당

## 개념 설명

다형성은 두 메커니즘의 조합이다.

### Operation Overriding (재정의)

Subclass가 Superclass의 함수와 **동일한 prototype(시그니처)** 을 가진 함수를 새로 정의하는 것.

```cpp
class Shape {
public:
    virtual void draw();   // 부모: 인터페이스 선언
};

class Circle : public Shape {
public:
    void draw() override;  // 자식: 동일 시그니처로 재정의
};
```

### Dynamic Binding (동적 바인딩)

런타임에 **포인터가 실제 가리키는 객체 타입**을 기준으로 호출할 함수를 결정하는 것.

```cpp
Shape* s = new Circle();
s->draw();  // Circle::draw() 호출 — 컴파일 타임에는 모름
```

`Shape*`지만 실제 객체가 Circle이므로 `Circle::draw()`가 호출된다. C++에서는 `virtual` 키워드가 없으면 **Static Binding** (컴파일 타임에 타입 기준으로 결정) 이 적용된다.

### Static Binding vs Dynamic Binding

| | Static Binding | Dynamic Binding |
|--|--|--|
| **결정 시점** | 컴파일 타임 | 런타임 |
| **기준** | 포인터/참조의 선언 타입 | 실제 객체 타입 |
| **C++ 조건** | 기본값 | `virtual` 키워드 필요 |
| **성능** | 빠름 | vtable 조회 비용 있음 |

```cpp
// Static Binding (virtual 없음)
class Animal {
public:
    void speak() { cout << "..."; }  // non-virtual
};
class Dog : public Animal {
public:
    void speak() { cout << "Woof"; }
};

Animal* a = new Dog();
a->speak();  // "..." 출력 — Animal::speak() 호출됨 (포인터 타입 기준)
```

```cpp
// Dynamic Binding (virtual 있음)
class Animal {
public:
    virtual void speak() { cout << "..."; }  // virtual
};
class Dog : public Animal {
public:
    void speak() override { cout << "Woof"; }
};

Animal* a = new Dog();
a->speak();  // "Woof" 출력 — Dog::speak() 호출됨 (실제 객체 기준)
```

## 코드 예제

```cpp
#include <iostream>
#include <vector>
using namespace std;

class Shape {
public:
    virtual void draw() const {
        cout << "Shape::draw()" << endl;
    }
    virtual double area() const = 0;  // 순수 가상 함수 — 구현 강제
    virtual ~Shape() {}               // 가상 소멸자 필수
};

class Circle : public Shape {
    double r;
public:
    Circle(double r) : r(r) {}
    void draw() const override {
        cout << "Circle(r=" << r << ")" << endl;
    }
    double area() const override { return 3.14 * r * r; }
};

class Rectangle : public Shape {
    double w, h;
public:
    Rectangle(double w, double h) : w(w), h(h) {}
    void draw() const override {
        cout << "Rectangle(" << w << "x" << h << ")" << endl;
    }
    double area() const override { return w * h; }
};

int main() {
    // Shape* 배열에 Circle, Rectangle 혼합
    vector<Shape*> shapes = {
        new Circle(5),
        new Rectangle(3, 4),
        new Circle(2)
    };

    for (Shape* s : shapes) {
        s->draw();                                      // dynamic binding
        cout << "  area = " << s->area() << endl;
    }

    for (Shape* s : shapes) delete s;
    return 0;
}
```

출력:
```
Circle(r=5)
  area = 78.5
Rectangle(3x4)
  area = 12
Circle(r=2)
  area = 12.56
```

## 왜 이렇게 설계됐는가

**문제**: 새로운 타입이 추가될 때마다 호출하는 쪽 코드를 수정해야 한다면 확장이 어렵다.

```cpp
// 다형성 없이
if (type == "circle") drawCircle();
else if (type == "rect") drawRect();
// 새 타입 추가할 때마다 이 if-else를 수정해야 함
```

**해결**: 다형성은 호출하는 쪽을 인터페이스에만 의존하게 만든다.

```cpp
// 다형성 있음 — 새 타입 추가해도 이 코드는 안 바뀜
for (Shape* s : shapes) s->draw();
```

이게 **OCP (Open-Closed Principle)** — 확장에는 열려있고, 수정에는 닫혀있어야 한다.

**C++의 선택**: 기본값이 Dynamic Binding이 아닌 이유는 vtable 비용 때문. C++는 "사용하지 않는 기능에 비용을 내지 않는다"는 철학 (Zero-overhead principle). 필요할 때만 `virtual`로 옵트인.

**Trade-off**:
- `virtual` 사용 시: vtable 포인터(보통 8bytes/객체) + 간접 호출 비용
- 대부분의 경우 무시할 수준이지만, 초고빈도 호출(게임 루프, 임베디드)에서는 고려 대상

## 연결된 개념

- [[Virtual-Function]] — Dynamic Binding의 구현 메커니즘 (vtable)
- [[TCP-IP-5계층]] — 계층 분리도 같은 철학: 인터페이스만 알면 됨
- OCP (Open-Closed Principle) — 소프트웨어 공학에서의 상위 원칙
- Dependency Inversion Principle — "구체 타입이 아닌 인터페이스에 의존하라"
