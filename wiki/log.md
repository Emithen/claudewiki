# Wiki Log

append-only 작업 기록. 형식: `## [YYYY-MM-DD] 작업유형 | 제목`

빠른 조회: `grep "^## \[" log.md | tail -10`

---

## [2026-04-15] output | Obsidian GitHub 공유 가이드

- 생성: `output/guides/Obsidian-GitHub-공유-가이드.md`
- 내용: git 초기화, .gitignore 설정, Obsidian Git 플러그인, LLM Wiki 공개 전략 3가지 (전체/wiki만/레포 분리), README 초안
- 컨텍스트: LLM Wiki 프로젝트를 GitHub로 공유하기 위한 발판

---

## [2026-04-15] 문서화 | RDT 1.0~3.0 전용 페이지 생성

- 소스: `raw/4월 14일.md`, `wiki/concepts/Transport-Layer.md`
- 생성: `wiki/concepts/RDT.md` (rdt1.0~3.0 + Pipelining 독립 문서)
- 컨텍스트: Transport-Layer.md에 포함된 RDT 내용을 독립 페이지로 분리. FSM 동작, ACK-1 재전송 안 하는 이유, Utilization 공식 포함

---

## [2026-04-15] ingest | OOP 다형성 강의 노트

- 소스: `raw/Polymorphism.md`
- 생성: `wiki/sources/2026-04-15_OOP-다형성.md`
- 생성: `wiki/concepts/Polymorphism.md` (Overriding, Dynamic Binding, Abstract Class)
- 생성: `wiki/concepts/Virtual-Function.md` (vtable, 가상 소멸자, override/final)
- 생성: `output/demos/polymorphism-cpp/main.cpp` (4가지 실험 포함 샌드박스)
- 컨텍스트: OOP + 소프트웨어 공학 수강 중. Polymorphism이 TCP/IP 계층 분리, JPA 추상화와 같은 설계 철학(Separation of Concerns, OCP) 공유

---

## [2026-04-14] ingest | 컴퓨터 네트워크 강의 노트 배치 (14개 파일)

- 소스: `raw/*.md` 루트 직접 작성 파일 14개
- 생성: `wiki/sources/2026-04-14_컴퓨터네트워크-강의노트.md`
- 생성: `wiki/sources/2026-04-14_전공시험-예상-출제유형.md`
- 생성: `wiki/concepts/TCP-IP-5계층.md`
- 생성: `wiki/concepts/Application-Layer.md`
- 생성: `wiki/concepts/Transport-Layer.md` (RDT 1.0→3.0 포함)
- 생성: `wiki/concepts/UDP.md` (segment + checksum 통합)
- 생성: `wiki/concepts/DNS.md`
- 생성: `wiki/concepts/Ones-Complement-Sum.md`
- 생성: `wiki/connections/RDT-진화.md`
- 스텁 처리: Networks.md, Communication Links.md, Packet Switches.md (빈 파일)
- 컨텍스트: 컴퓨터 네트워크 전공 수강 중. 시험 예상: packet switching, socket, SMTP, DNS, UDP checksum, RDT FSM

---

## [2026-04-14] ingest | 카파시 LLM Wiki 실습 영상 (브레인 트리니티)

- 소스: `raw/videos/2026-04-14_카파시의 LLM Wiki로 나만의 AI 세컨드 브레인 만들기...md`
- 생성: `wiki/sources/2026-04-14_카파시-LLM-Wiki-브레인트리니티.md`
- 생성: `wiki/concepts/LLM-Wiki.md`
- 생성: `wiki/connections/RAG-vs-LLM-Wiki.md`
- 업데이트: `wiki/concepts/DataSource.md` (LLM-Wiki 구조적 유사성 역링크 추가)
- 컨텍스트: 이 볼트 자체가 영상의 세팅 프로세스를 직접 실행한 결과물

---

## [2026-04-14] ingest | Spring Boot Data Access 공식 문서

- 소스: `raw/docs/2026-04-14_Data Access Spring Boot.md`
- 생성: `wiki/sources/2026-04-14_Spring-Boot-Data-Access-Docs.md`
- 생성: `wiki/concepts/DataSource.md`
- 생성: `wiki/concepts/JPA.md`
- 생성: `output/guides/Spring-Boot-PostgreSQL.md` (졸업 프로젝트용 PostgreSQL 연동 가이드)
- 컨텍스트: 졸업 프로젝트 백엔드 파트, JPA 설정 + DB mocking 필요

---

## [2026-04-14] init | Wiki 초기화

- 폴더 구조 생성: raw/, wiki/, output/
- index.md, log.md 초기화
- CLAUDE.md 설정 완료