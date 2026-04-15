# DataSource

한 줄 요약: DB와의 연결을 추상화한 인터페이스. 커넥션 풀의 관문.

## 직관

DB에 쿼리를 날릴 때마다 새 연결을 맺으면 비용이 너무 크다. TCP 핸드셰이크 + 인증 과정이 매번 필요하기 때문. DataSource는 연결을 미리 여러 개 만들어두고 (커넥션 풀) 요청마다 빌려주는 창구다.

운영체제의 스레드 풀, HTTP 커넥션 풀과 동일한 자원 분배 구조.

```
앱 요청 → DataSource → [conn1, conn2, conn3, ...] → PostgreSQL
                           (커넥션 풀)
```

## 개념 설명

### DataSource vs Connection

| | Connection | DataSource |
|--|--|--|
| 역할 | DB와의 단일 연결 | Connection을 관리하는 팩토리 |
| 생성 비용 | 높음 (TCP + 인증) | 낮음 (풀에서 꺼냄) |
| 수명 | 요청마다 생성/해제 | 앱 생명주기 내내 유지 |

### Spring Boot의 자동 설정 흐름

1. classpath에 `spring-boot-starter-data-jpa` + DB 드라이버 존재
2. `application.yml`의 `spring.datasource.*` 읽기
3. `DataSourceAutoConfiguration`이 HikariDataSource 빈 자동 생성

커스텀 `@Bean DataSource`를 등록하면 자동 설정이 물러남.

### HikariCP가 기본인 이유

Spring Boot 2.x부터 기본 커넥션 풀. 벤치마크에서 가장 빠른 Java 커넥션 풀로 검증됨. 설정도 단순한 편.

### 주요 HikariCP 설정

| 설정 | 기본값 | 의미 |
|------|--------|------|
| `maximum-pool-size` | 10 | 풀의 최대 커넥션 수 |
| `minimum-idle` | 10 | 유휴 커넥션 최소 유지 수 |
| `connection-timeout` | 30000ms | 커넥션 획득 대기 최대 시간 |
| `idle-timeout` | 600000ms | 유휴 커넥션 제거 대기 시간 |
| `max-lifetime` | 1800000ms | 커넥션 최대 수명 |

## 코드 예제

```yaml
# application.yml
spring:
  datasource:
    url: jdbc:postgresql://localhost:5432/mydb
    username: myuser
    password: mypassword
    hikari:
      maximum-pool-size: 10
      connection-timeout: 30000
```

```java
// 커스텀 DataSource가 필요한 경우 (선택)
@Configuration
public class DataSourceConfig {

    @Bean
    @ConfigurationProperties("spring.datasource.hikari")
    public HikariDataSource dataSource() {
        return DataSourceBuilder.create()
            .type(HikariDataSource.class)
            .build();
    }
}
```

## 왜 이렇게 설계됐는가

`DataSource`는 Jakarta EE 표준 인터페이스(`javax.sql.DataSource`). 구현체(HikariCP, DBCP2, c3p0)를 바꿔도 앱 코드가 변하지 않도록 추상화됨. Spring Boot는 이 인터페이스 위에 자동 설정을 얹는 구조.

**Trade-off**: 풀 크기를 너무 크게 잡으면 DB 서버가 연결 유지 부담. 너무 작으면 요청 대기 시간 증가. DB의 `max_connections`와 앱 인스턴스 수를 고려해 설정해야 함.

## 연결된 개념

- [[JPA]] — DataSource 위에서 ORM 레이어가 동작
- [[Spring-Boot-Auto-configuration]] — 자동 설정 원리
- [[LLM-Wiki]] — 구조적 유사성: 커넥션 풀(미리 연결 준비)과 LLM Wiki(미리 지식 컴파일)는 "재사용을 위한 사전 투자"라는 동일한 원리