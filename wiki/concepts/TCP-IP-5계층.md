# TCP/IP 5계층

한 줄 요약: 네트워크 통신을 5단계 책임으로 분리한 설계 구조.

## 직관

편지를 보내는 과정을 생각해보자. 내용 작성(Application) → 봉투에 넣기(Transport) → 주소 경로 결정(Network) → 실제 도로 위 이동(Data Link) → 전기/빛 신호(Physical). 각 단계는 자기 역할만 알고, 위아래 인터페이스만 맞추면 된다.

계층 구조의 핵심: **각 계층은 아래 계층을 블랙박스로 취급한다.** HTTP는 TCP가 어떻게 전달하는지 몰라도 된다.

## 계층 구조

```
5. Application Layer   — 사용자/앱이 네트워크에 접근하는 창구
                          HTTP, DNS, SMTP, FTP
4. Transport Layer     — 프로세스 간 end-to-end 통신
                          TCP, UDP
3. Network Layer       — 호스트 간 경로 결정 (라우팅)
                          IP
2. Data Link Layer     — 인접 노드 간 프레임 전달
                          Ethernet, Wi-Fi
1. Physical Layer      — 비트를 물리 신호로 변환
                          전기, 빛, 전파
```

## 왜 이렇게 설계됐는가

**Separation of Concerns** 원칙. 각 계층이 독립적이면:
- 계층 내부 구현을 바꿔도 다른 계층이 영향받지 않음 (Wi-Fi → 이더넷 교체해도 TCP는 그대로)
- 새 프로토콜을 특정 계층에만 추가 가능
- 계층별로 독립적인 디버깅 가능

Trade-off: 계층을 거칠수록 헤더가 쌓여 오버헤드 증가 (encapsulation). 성능이 극도로 중요하면 계층 분리를 일부 포기하기도 함.

## 각 계층 상세

- [[Application-Layer]] — HTTP, DNS, SMTP
- [[Transport-Layer]] — TCP, UDP, RDT
- [[UDP]] — Transport Layer의 경량 프로토콜

## 연결된 개념

- [[Application-Layer]]
- [[Transport-Layer]]
- [[UDP]]
- [[DNS]]
