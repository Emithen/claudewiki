# JPA

한 줄 요약: Java에서 객체-테이블 매핑을 표준화한 인터페이스 명세.

## 직관

SQL로 직접 DB를 다루면 Java 객체와 테이블 행 사이를 손으로 번역해야 한다.

```java
// SQL 직접: 번역을 수동으로
ResultSet rs = stmt.executeQuery("SELECT * FROM users WHERE id = 1");
User user = new User(rs.getLong("id"), rs.getString("name")); // 수작업
```

JPA는 이 번역을 자동화한다. 객체로 쓰면 SQL은 JPA가 생성한다.

```java
// JPA: 번역 자동
User user = userRepository.findById(1L).get(); // SQL 자동 생성
```

## 개념 설명

### 3 레이어 구분

혼동하기 쉬운 세 개념:

```
Jakarta Persistence API (JPA)    ← 인터페이스 명세 (표준, 규칙)
         ↑ 구현
      Hibernate                  ← JPA 구현체 (Spring Boot 기본)
         ↑ 래핑
  Spring Data JPA                ← Repository 패턴 + 편의 추상화
```

JPA는 스펙, Hibernate는 구현체, Spring Data JPA는 그 위의 편의 레이어.

### @Entity 매핑

```java
@Entity
@Table(name = "users")
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY) // DB auto_increment
    private Long id;

    private String name; // 컬럼명 자동: name

    @Column(name = "email_address") // 명시적 컬럼명 지정
    private String email;
}
```

Hibernate 기본 네이밍: `camelCase` → `snake_case` (`emailAddress` → `email_address`)

### Repository 패턴

```java
// 인터페이스만 선언하면 구현체 자동 생성
public interface UserRepository extends JpaRepository<User, Long> {
    List<User> findByName(String name);           // SELECT * FROM users WHERE name = ?
    Optional<User> findByEmail(String email);     // SELECT * FROM users WHERE email = ?
    List<User> findByNameAndAge(String name, int age);
}
```

### ddl-auto 옵션 (중요)

| 값 | 동작 | 언제 사용 |
|----|------|----------|
| `create` | 시작 시 테이블 재생성 (데이터 삭제) | 초기 개발 |
| `create-drop` | 시작 시 생성, 종료 시 삭제 | 테스트 |
| `update` | 변경사항만 반영 (데이터 유지) | 개발 중 |
| `validate` | 엔티티-테이블 일치 여부 검증만 | 운영 |
| `none` | 아무것도 안 함 | 운영 (Flyway/Liquibase 사용 시) |

운영 환경에서는 `validate` 또는 `none`. `update`는 절대 운영에 쓰지 말 것 (예상치 못한 스키마 변경 위험).

## 코드 예제

```java
@Service
@Transactional
public class UserService {

    private final UserRepository userRepository;

    public UserService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    public User createUser(String name, String email) {
        User user = new User(name, email);
        return userRepository.save(user); // INSERT INTO users ...
    }

    public User findUser(Long id) {
        return userRepository.findById(id)
            .orElseThrow(() -> new RuntimeException("User not found"));
    }
}
```

## 왜 이렇게 설계됐는가

객체지향 모델과 관계형 모델은 근본적으로 다르다 (Impedance Mismatch).
- 상속, 다형성, 객체 참조가 테이블에 직접 대응되지 않음
- JPA/ORM이 이 간극을 메우는 중간 계층으로 설계됨

**Trade-off**: 편리하지만 JPA 특유의 함정이 있음.
- **N+1 문제**: 연관 관계 조회 시 쿼리가 N개 추가 발생
- **Lazy Loading**: 트랜잭션 밖에서 연관 객체 접근 시 LazyInitializationException
- 복잡한 집계 쿼리는 JPQL이나 QueryDSL 필요

## 연결된 개념

- [[DataSource]] — JPA가 DataSource를 통해 DB에 접근
- [[Transaction]] — @Transactional 동작 원리
- [[Repository-Pattern]] — Spring Data JPA의 기반 패턴