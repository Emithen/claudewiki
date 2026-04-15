# claudewiki

AI가 raw 소스를 컴파일해서 유지하는 지식베이스.  
Claude Code + Obsidian 기반의 [LLM Wiki](wiki/concepts/LLM-Wiki.md) 구현체.

## 구조

```
raw/        원본 노트 (강의, 책, 아티클, 영상)
wiki/       AI가 컴파일한 지식베이스
  concepts/   개념 문서 (한 파일 = 한 개념)
  connections/ 개념 간 연결·비교
output/     wiki에서 파생된 결과물
  guides/     가이드 문서
  demos/      샌드박스 코드
```

## 주제

- **컴퓨터 네트워크** — TCP/IP 5계층, Transport Layer, RDT, UDP, DNS
- **Spring Boot** — DataSource, JPA, HikariCP, 커넥션 풀
- **OOP / C++** — Polymorphism, Virtual Function
- **AI 지식 관리** — LLM Wiki, RAG

## 운영 방식

1. `raw/`에 노트 추가
2. Claude Code에 "인제스트해줘" 요청
3. `wiki/`에 개념 페이지 자동 생성·통합

[Andrej Karpathy의 LLM Wiki 방법론](wiki/concepts/LLM-Wiki.md) 기반.