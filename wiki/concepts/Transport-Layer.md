# Transport Layer

한 줄 요약: 프로세스 간 end-to-end 논리적 통신 채널을 담당하는 계층.

## 직관

Network Layer(IP)가 "집 주소"까지 배달한다면, Transport Layer는 "몇 호 입주자"까지 정확히 전달한다. IP는 컴퓨터를 찾고, Transport는 그 컴퓨터 위의 프로세스(포트)를 찾는다.

또한 Transport Layer는 "얼마나 믿을 수 있게" 전달할지를 결정한다. TCP는 등기우편(확인 후 재발송), UDP는 일반 엽서(그냥 던짐).

## 주요 기능

| 기능 | 설명 |
|------|------|
| Segmentation & Reassembly | 데이터를 세그먼트로 분할 후 재조립 |
| Port Addressing | 포트 번호로 프로세스 식별 |
| Connection Control | 연결 지향(TCP) / 비연결(UDP) |
| Flow Control | 송수신 속도 조절 |
| Error Control | 데이터 오류 감지·복구 |
| Congestion Control | 네트워크 혼잡 관리 |

## 핵심 개념

- **Port**: 프로세스 식별자. 0~65535. 80=HTTP, 443=HTTPS, 53=DNS
- **Socket**: `IP + Port` 조합. 프로세스의 네트워크 주소
- **Segment**: Transport Layer의 데이터 단위

## 프로토콜

- [[UDP]] — 빠르지만 신뢰성 없음. DNS, 스트리밍, 게임
- **TCP** — 신뢰성 보장, 흐름·혼잡 제어. 웹, 파일 전송

---

## RDT (Reliable Data Transfer)

신뢰성 있는 전송을 구현하기 위한 개념 모델 진화. 실제 구현이 아닌 추상화 모델.

### rdt1.0 — 완벽한 채널 가정
- 오류 없음, 패킷 손실 없음
- 그냥 송수신만 하면 됨

### rdt2.0 — 비트 오류 가능
- checksum으로 오류 검출
- **ACK** (정상) / **NAK** (오류 발견) 피드백
- Stop-and-Wait: ACK/NAK 받을 때까지 대기
- **결함**: ACK/NAK 자체가 손상되면? → 미처리

### rdt2.1 — ACK/NAK 손상 보정
- Sequence Number 도입 (0, 1 두 가지로 충분)
  - Stop-and-Wait이라 한 번에 하나만 판정하면 됨
- Duplicate 패킷을 sequence number로 식별·폐기

### rdt2.2 — NAK 제거
- ACK에 sequence number 부착
- ACK-0 = 정상, ACK-1 = 오류
- NAK 없이 ACK만으로 2.1과 동등한 기능

### rdt3.0 — 패킷 손실 가능
- checksum + sequence만으로는 손실 감지 불가
- **타임아웃** 도입: 일정 시간 ACK 안 오면 재전송
- 문제: 타임아웃 후 늦게 도착한 패킷 → duplicate
  → sequence number로 이미 해결됨
- Stop-and-Wait의 한계: RTT가 Dtrans보다 훨씬 커서 효율 낮음

### Pipelining — Stop-and-Wait 극복
- 여러 세그먼트를 ACK 기다리지 않고 연속 전송
- 효율 공식: `U = (L/R) / (RTT + L/R)`
- Go-Back-N, Selective Repeat 방식으로 구현

## 왜 이렇게 설계됐는가

RDT 진화는 **실패 시나리오를 하나씩 추가하며 최소한의 변경으로 해결**하는 패턴. 소프트웨어 버전 업과 동일한 구조 — 각 버전은 이전 버전의 엣지 케이스를 처리.

TCP는 rdt3.0 + pipelining + 흐름제어 + 혼잡제어를 실제 구현한 것.

## 연결된 개념

- [[TCP-IP-5계층]]
- [[UDP]] — Transport Layer의 경량 선택지
- [[Application-Layer]] — Transport 위에서 동작
- [[Ones-Complement-Sum]] — checksum 계산 방식
