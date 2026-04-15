# Spring Boot + PostgreSQL 연동 가이드

졸업 프로젝트 기준. 처음 연동하는 초보자용 + 설정 레퍼런스.

---

## 1. 의존성 추가

**build.gradle (Gradle)**

```groovy
dependencies {
    // JPA + Hibernate
    implementation 'org.springframework.boot:spring-boot-starter-data-jpa'

    // PostgreSQL 드라이버 (런타임에만 필요)
    runtimeOnly 'org.postgresql:postgresql'

    // 테스트용 H2 in-memory DB
    testImplementation 'com.h2database:h2'
}
```

**pom.xml (Maven)**

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
<dependency>
    <groupId>org.postgresql</groupId>
    <artifactId>postgresql</artifactId>
    <scope>runtime</scope>
</dependency>
<dependency>
    <groupId>com.h2database</groupId>
    <artifactId>h2</artifactId>
    <scope>test</scope>
</dependency>
```

---

## 2. 기본 연결 설정

**src/main/resources/application.yml**

```yaml
spring:
  datasource:
    url: jdbc:postgresql://localhost:5432/mydb
    username: myuser
    password: mypassword
    driver-class-name: org.postgresql.Driver

  jpa:
    hibernate:
      ddl-auto: update        # 개발: update | 운영: validate
    show-sql: true            # SQL 로그 출력
    properties:
      hibernate:
        format_sql: true      # SQL 로그 정렬
        dialect: org.hibernate.dialect.PostgreSQLDialect
```

> **주의**: `ddl-auto: update`는 개발 중에만. 운영에서는 반드시 `validate` 또는 `none`.

---

## 3. 엔티티 + Repository 기본 구조

```java
// 엔티티
@Entity
@Table(name = "users")
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false)
    private String name;

    @Column(unique = true, nullable = false)
    private String email;

    // 기본 생성자 필수 (JPA 요구사항)
    protected User() {}

    public User(String name, String email) {
        this.name = name;
        this.email = email;
    }

    // getter...
}

// Repository
public interface UserRepository extends JpaRepository<User, Long> {
    Optional<User> findByEmail(String email);
}
```

---

## 4. HikariCP 커넥션 풀 설정 레퍼런스

```yaml
spring:
  datasource:
    hikari:
      maximum-pool-size: 10        # 최대 커넥션 수 (기본: 10)
      minimum-idle: 5              # 최소 유휴 커넥션 (기본: maximum-pool-size와 동일)
      connection-timeout: 30000    # 커넥션 획득 대기 시간 ms (기본: 30s)
      idle-timeout: 600000         # 유휴 커넥션 제거 시간 ms (기본: 10min)
      max-lifetime: 1800000        # 커넥션 최대 수명 ms (기본: 30min)
      pool-name: MyHikariPool      # 로그에 표시될 풀 이름
```

| 설정 | 기본값 | 언제 조정하나 |
|------|--------|--------------|
| `maximum-pool-size` | 10 | DB max_connections 보고 조정. 앱 인스턴스 수 × 이 값 ≤ DB max |
| `minimum-idle` | maximum과 동일 | 트래픽 변동이 크면 낮게 설정해 유휴 커넥션 낭비 줄이기 |
| `connection-timeout` | 30s | 낮추면 빠른 에러 감지. 너무 낮으면 일시적 부하 시 오류 |
| `max-lifetime` | 30min | DB/방화벽의 idle connection 타임아웃보다 짧게 설정 |

---

## 5. JPA / Hibernate 설정 레퍼런스

| 설정 | 옵션 / 기본값 | 효과 |
|------|--------------|------|
| `ddl-auto` | `none` (기본, 비내장DB) | 아무것도 안 함 |
| | `validate` | 엔티티-테이블 불일치 시 앱 시작 실패 (운영 권장) |
| | `update` | 변경사항만 반영, 데이터 유지 (개발 중) |
| | `create` | 매 시작 시 재생성, 데이터 삭제 |
| | `create-drop` | 시작 시 생성, 종료 시 삭제 (테스트) |
| `show-sql` | `false` | `true`로 하면 실행 SQL 로그 출력 |
| `format_sql` | `false` | SQL 로그 들여쓰기 정렬 |
| `open-in-view` | `true` | HTTP 요청 전체에 걸쳐 영속성 컨텍스트 유지. 운영에서는 `false` 권장 |
| `dialect` | 자동 감지 | 보통 자동 감지되지만 명시하면 안전 |
| `jdbc.batch_size` | 없음 | 대량 INSERT 시 배치 처리. `15`~`30` 추천 |

---

## 6. 테스트 환경 구성 (H2 mocking)

### 테스트용 application.yml 분리

**src/test/resources/application.yml** (또는 `application-test.yml`)

```yaml
spring:
  datasource:
    url: jdbc:h2:mem:testdb;MODE=PostgreSQL  # PostgreSQL 호환 모드
    driver-class-name: org.h2.Driver
    username: sa
    password:
  jpa:
    hibernate:
      ddl-auto: create-drop
    database-platform: org.hibernate.dialect.H2Dialect
    show-sql: true
```

### @DataJpaTest — Repository 단위 테스트

```java
@DataJpaTest  // JPA 관련 빈만 로드, H2 자동 설정
class UserRepositoryTest {

    @Autowired
    private UserRepository userRepository;

    @Test
    void 유저_저장_및_조회() {
        // given
        User user = new User("홍길동", "hong@example.com");

        // when
        User saved = userRepository.save(user);

        // then
        assertThat(saved.getId()).isNotNull();
        assertThat(userRepository.findByEmail("hong@example.com")).isPresent();
    }
}
```

### @SpringBootTest — 전체 통합 테스트

```java
@SpringBootTest
@ActiveProfiles("test")  // application-test.yml 로드
class UserServiceIntegrationTest {

    @Autowired
    private UserService userService;

    @Test
    @Transactional  // 테스트 후 롤백
    void 유저_생성_서비스_테스트() {
        User user = userService.createUser("홍길동", "hong@example.com");
        assertThat(user.getId()).isNotNull();
    }
}
```

---

## 7. 연결 확인 방법

앱 시작 후 로그에서 확인:

```
# 정상 연결 시
HikariPool-1 - Start completed.
Hibernate: create table users (...)  # ddl-auto: create일 때
```

빠른 연결 테스트 (actuator 없이):

```java
@Component
public class DbConnectionChecker implements ApplicationRunner {

    @Autowired
    private DataSource dataSource;

    @Override
    public void run(ApplicationArguments args) throws Exception {
        try (Connection conn = dataSource.getConnection()) {
            System.out.println("DB 연결 성공: " + conn.getMetaData().getURL());
        }
    }
}
```

---

## 참고

- 소스: [[wiki/sources/2026-04-14_Spring-Boot-Data-Access-Docs]]
- 관련 개념: [[wiki/concepts/DataSource]], [[wiki/concepts/JPA]]
- Spring Boot 공식 문서: https://docs.spring.io/spring-boot/how-to/data-access.html