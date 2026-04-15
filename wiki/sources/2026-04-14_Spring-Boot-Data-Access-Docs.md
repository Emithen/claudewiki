---
title: "Data Access :: Spring Boot"
type: docs
source: "https://docs.spring.io/spring-boot/how-to/data-access.html"
product: Spring Boot 4.0.5
clipped: 2026-04-14
status: ingested
tags:
  - spring-boot
  - datasource
  - jpa
  - hibernate
---

# 소스 요약: Spring Boot Data Access How-To

## 핵심 주장

- `DataSourceBuilder`가 classpath 기반으로 커넥션 풀을 자동 감지 (기본: HikariCP)
- `DataSourceProperties`가 `url` → `jdbc-url` 변환 처리 (Hikari 같은 풀 대응)
- 다중 DataSource는 `@Qualifier` + `defaultCandidate=false` 조합으로 관리
- Spring Data가 Repository 인터페이스 구현체를 자동 생성 (`@SpringBootApplication` 패키지 기준 스캔)
- Hibernate 기본 네이밍: `CamelCaseToUnderscoresNamingStrategy` (`TelephoneNumber` → `telephone_number`)
- `ddl-auto` 기본값: embedded DB면 `create-drop`, 아니면 `none`

## 다루는 개념

[[DataSource]], [[JPA]], [[Connection-Pool]], [[Repository-Pattern]], [[Spring-Boot-Auto-configuration]]

## 관련 아웃풋

- [[output/guides/Spring-Boot-PostgreSQL]] — 이 소스 기반으로 만든 PostgreSQL 연동 가이드