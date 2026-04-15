# Application Layer

한 줄 요약: 사용자와 소프트웨어가 네트워크 서비스에 접근하는 창구. TCP/IP 5계층의 최상단.

## 직관

식당에서 메뉴판이 Application Layer다. 손님(앱)은 무엇을 주문할지만 알면 된다. 주방(Transport)이 어떻게 요리하는지, 재료 유통(Network)이 어떻게 되는지 몰라도 된다.

## 주요 프로토콜

### 웹
- **HTTP/HTTPS** — 웹 페이지 요청/응답

### 이메일
- **SMTP** — 이메일 전송 (Simple Mail Transfer Protocol)
- **IMAP** — 이메일 수신

### 주소 변환
- **DNS** — 도메인 이름 → IP 주소 변환 → [[DNS]]

### 파일
- **FTP** — 파일 전송 (File Transfer Protocol)

### 원격 접속
- **SSH**, **Telnet**

## 전달 방식 & 아키텍처

Application Layer 위에서 동작하는 전달 구조:

- **P2P** — 클라이언트-서버 없이 피어 간 직접 통신
- **Video Streaming** — 버퍼링 기반 대용량 미디어 전달
- **CDN** (Content Delivery Network) — 지리적으로 분산된 캐시 서버

## 네트워크 인터페이스

- **Socket** — 앱이 Transport Layer에 접근하는 API. `IP + Port`의 조합.

## 왜 이렇게 설계됐는가

각 서비스 목적(웹, 메일, 파일)에 맞는 프로토콜을 독립적으로 설계할 수 있도록 계층화. HTTP가 변해도 DNS는 영향 없음.

## 연결된 개념

- [[TCP-IP-5계층]]
- [[Transport-Layer]]
- [[DNS]]
