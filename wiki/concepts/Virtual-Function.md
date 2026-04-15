# Virtual Function (가상 함수)

한 줄 요약: Dynamic Binding을 활성화하는 C++의 메커니즘. 내부적으로 vtable이라는 함수 포인터 테이블로 구현된다.

## 직관

책 목차를 생각해보자. 목차(vtable)는 "3장 → 47페이지"처럼 챕터 이름과 실제 위치를 연결한다. `shape->draw()` 호출은 "draw의 실제 구현이 어디 있는지 목차에서 찾아서 거기로 이동"하는 것.

vtable은 클래스마다 하나 존재하고, 각 객체는 그 클래스의 vtable을 가리키는 포인터(vptr)를 갖는다.

## 개념 설명

### virtual 키워드 종류

| 키워드 | 의미 | 용도 |
|--------|------|------|
| `virtual void f()` | 가상 함수 | 재정의 가능, 기본 구현 있음 |
| `virtual void f() = 0` | 순수 가상 함수 | 구현 강제, Abstract Class |
| `override` | 재정의 명시 | 시그니처 오류를 컴파일 타임에 잡음 |
| `final` | 재정의 금지 | 이 클래스 이후 override 막음 |
| `virtual ~Destructor()` | 가상 소멸자 | 다형 소멸 시 필수 |

### vtable 구조

```
[Circle 클래스의 vtable]
+------------------+
| Circle::draw()   |  ← draw 슬롯
| Circle::area()   |  ← area 슬롯
| ~Circle()        |  ← 소멸자 슬롯
+------------------+

[Circle 객체 메모리 레이아웃]
+------------------+
| vptr ──────────► | Circle의 vtable 가리킴
+------------------+
| r (double)       |
+------------------+
```

`Shape* s = new Circle()` → s의 vptr은 Circle vtable을 가리킴
`s->draw()` → vptr 따라가서 vtable에서 draw 슬롯 찾아서 호출

### Abstract Class (추상 클래스)

순수 가상 함수가 하나라도 있으면 추상 클래스. **인스턴스화 불가**.

```cpp
class Shape {
public:
    virtual double area() const = 0;  // 순수 가상 함수
    virtual void draw() const = 0;
    virtual ~Shape() {}
};

// Shape s;         // 컴파일 에러 — 추상 클래스 인스턴스화 불가
// Shape* s = new Shape();  // 컴파일 에러
Shape* s = new Circle(5);   // OK — 구체 타입으로
```

목적: 인터페이스만 정의하고, 구현은 Subclass에 강제.

### 가상 소멸자가 필요한 이유

```cpp
class Base {
public:
    ~Base() { cout << "Base 소멸"; }      // non-virtual
};
class Derived : public Base {
public:
    ~Derived() { cout << "Derived 소멸"; }
};

Base* b = new Derived();
delete b;  // "Base 소멸"만 출력 — Derived 소멸자 미호출 → 메모리 누수
```

```cpp
class Base {
public:
    virtual ~Base() { cout << "Base 소멸"; }  // virtual
};

Base* b = new Derived();
delete b;  // "Derived 소멸" → "Base 소멸" 순서대로 호출 → 정상
```

다형성을 쓸 클래스는 **항상** 가상 소멸자를 선언해야 한다.

### override / final

```cpp
class Shape {
public:
    virtual void draw() const;
};

class Circle : public Shape {
public:
    void draw() const override;   // 시그니처가 다르면 컴파일 에러 → 오타 방지
    // void draw() override;      // 에러: const 빠짐
};

class FixedCircle : public Circle {
public:
    void draw() const override final;  // 이후 상속 클래스에서 재정의 불가
};
```

## 코드 예제

```cpp
#include <iostream>
using namespace std;

// 추상 클래스 — 인터페이스 역할
class Animal {
public:
    virtual void speak() const = 0;      // 구현 강제
    virtual string name() const = 0;
    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void speak() const override { cout << "Woof!"; }
    string name() const override { return "Dog"; }
};

class Cat : public Animal {
public:
    void speak() const override { cout << "Meow!"; }
    string name() const override { return "Cat"; }
};

void makeSpeak(const Animal& a) {
    // a의 실제 타입이 뭔지 몰라도 됨
    cout << a.name() << " says: ";
    a.speak();
    cout << endl;
}

int main() {
    Dog d;
    Cat c;
    makeSpeak(d);  // Dog says: Woof!
    makeSpeak(c);  // Cat says: Meow!
}
```

## 왜 이렇게 설계됐는가

**왜 기본값이 non-virtual인가 (C++ 철학)**

C++의 Zero-overhead principle: "쓰지 않는 기능에 비용을 내지 않는다."

- 모든 함수가 virtual이면 → 모든 객체에 vptr 오버헤드 → 가벼운 struct에도 포인터 하나씩 붙음
- Java/Python은 기본이 dynamic dispatch → 간편하지만 성능 비용 상시 발생
- C++은 명시적으로 `virtual`을 써야만 vtable에 올라감

**vtable의 비용**:
- 클래스당 vtable 하나 (메모리 미비)
- 객체마다 vptr 8bytes
- 간접 호출 1회 추가 (캐시 미스 가능성)

**순수 가상 함수의 목적**:
인터페이스 강제. 추상 클래스를 직접 쓰지 못하게 막음으로써, Subclass가 반드시 구현을 제공하게 강제한다. 런타임이 아닌 **컴파일 타임**에 "구현 빠진 클래스" 오류를 잡는다.

## 연결된 개념

- [[Polymorphism]] — Virtual Function이 Dynamic Binding을 가능하게 함
- Inheritance (상속) — Virtual Function은 상속 계층에서만 의미가 있음
- OCP — Virtual Function이 OCP를 구현하는 도구
