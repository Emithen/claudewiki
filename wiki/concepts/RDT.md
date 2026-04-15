# RDT (Reliable Data Transfer)

한 줄 요약: 신뢰할 수 없는 채널 위에서 신뢰성 있는 전송을 구현하기 위한 개념 모델. 실제 구현이 아닌 추상화.

## 직관

RDT는 "채널이 점점 더 나빠진다면 어떻게 대응할까?"를 버전마다 하나씩 추가해가는 사고 실험이다.

소포 배달 비유:
- 배달부가 완벽하면 → 그냥 보내면 됨 (rdt1.0)
- 배달부가 내용물을 구긴다면 → 파손 확인 피드백이 필요 (rdt2.0)
- 확인 쪽지마저 구긴다면 → 쪽지에 번호를 붙여야 함 (rdt2.1)
- 배달부가 소포를 통째로 잃어버린다면 → 일정 시간 후 재발송 (rdt3.0)

각 버전은 이전 버전의 **가정을 하나씩 약화**시키며 더 현실적인 채널을 모델링한다.

## rdt1.0 — 완벽한 채널

**가정**: 비트 오류 없음, 패킷 손실 없음.

```
Sender: rdt_send(data) → make_pkt(data) → udt_send(pkt)
Receiver: rdt_rcv(pkt) → extract(pkt) → deliver_data(data)
```

할 것이 없다. 그냥 보내고 받으면 된다. 피드백도, 재전송도, 상태도 불필요.

## rdt2.0 — 비트 오류 가능

**추가 가정**: 채널에서 비트 flip이 발생할 수 있음. checksum으로 100% 검출 가능하다고 가정.

**해결책**: ACK/NAK 피드백 + 재전송

| 피드백 | 의미 | Sender 동작 |
|--------|------|------------|
| ACK | 정상 수신 | 다음 데이터 전송 |
| NAK | 오류 검출 | 동일 패킷 재전송 |

**Stop-and-Wait**: 전송 후 ACK 또는 NAK을 받을 때까지 기다린다. 한 번에 하나의 패킷만 처리.

```
Sender FSM:
  [대기] → rdt_send(data) → 전송
  [전송 후 대기] → ACK 수신 → [대기]
                → NAK 수신 → 재전송 → [전송 후 대기]
```

**결함**: ACK/NAK 자체가 손상되면?

```
Sender가 NAK를 오해 → 재전송 → Receiver 입장에서 중복 패킷 도착
Receiver: "이게 새 데이터야, 재전송이야?"
```

rdt2.0은 이 상황을 처리하지 못한다.

## rdt2.1 — ACK/NAK 손상 보정

**추가 가정**: ACK/NAK도 손상될 수 있다.

**해결책**: Sequence Number 도입

Sender는 패킷에 번호(0 또는 1)를 붙인다. Receiver는 이미 받은 번호의 패킷이 다시 오면 중복으로 판단해 버린다.

**왜 0, 1 두 개면 충분한가?**

Stop-and-Wait이기 때문이다. 한 번에 하나의 패킷에 대한 판정만 하면 된다. "현재 패킷이 정상인가 / 재전송인가"만 구분하면 되므로 1비트(0/1)면 충분.

```
Sender: pkt0 전송 → ACK0 수신 → pkt1 전송 → ACK1 수신 → pkt0 전송 ...
          손상된 ACK 수신 → pkt0 재전송(seq=0)

Receiver: seq=0 수신 → 정상 처리 → ACK0 전송
          seq=0 또 수신 → 중복 감지 → 버리고 ACK0 전송
```

Sender와 Receiver FSM이 각각 2개 상태를 가지게 된다 (wait-0, wait-1).

## rdt2.2 — NAK 제거

**관찰**: NAK는 사실 "직전 정상 ACK를 다시 보내는 것"과 동일하다.

**해결책**: ACK에 Sequence Number 부착. NAK 없이 ACK만으로 2.1과 동일한 기능.

```
NAK 대신:
  오류 발생 시 → ACK-0 (이전 번호) 전송
  정상 수신 시 → ACK-1 (현재 번호) 전송

Sender가 ACK-0을 받으면 → 재전송 (NAK 효과)
```

TCP가 이 방식을 사용한다. NAK 필드를 따로 두지 않고 ACK 번호로만 관리.

## rdt3.0 — 패킷 손실 가능

**추가 가정**: 패킷 자체가 사라질 수 있음 (손실).

**왜 2.x로는 부족한가?**

손실된 패킷은 피드백이 아예 오지 않는다. checksum도, sequence도 "도착한 패킷"의 오류만 처리할 수 있다. 도착 자체를 안 하면 Receiver는 아무것도 모른다.

**해결책**: Timeout (타이머)

```
Sender: 패킷 전송 → 타이머 시작
  ACK 수신 → 타이머 중지, 다음 전송
  Timeout 발생 → 재전송, 타이머 재시작
```

**새로운 문제**: timeout 후 늦게 도착한 패킷 → 중복

→ Sequence Number로 이미 rdt2.1에서 해결됨. 중복 패킷은 버린다.

**rdt3.0 FSM 포인트 — ACK-1 수신 시 재전송하지 않는 이유**

```
Sender가 pkt0 전송 중 ACK-1 수신:
  → pkt0 전송 상태에서 ACK-0이 와야 다음으로 넘어감
  → ACK-1은 이전 사이클의 잔여 응답
  → 재전송 트리거 안 함. 그냥 기다림.
  → ACK-0이 안 오면 어차피 timeout이 발생하고 그때 재전송
```

ACK-1을 받아도 바로 재전송하지 않고 timeout을 기다리는 게 rdt3.0 FSM의 핵심 동작이다.

## Stop-and-Wait의 한계와 Pipelining

rdt3.0은 논리적으로 완전하지만, 효율이 낮다.

```
Utilization = (L/R) / (RTT + L/R)

L = 패킷 크기 (bits)
R = 링크 속도 (bps)
L/R = 전송 시간 (Dtrans)
RTT = 왕복 지연 시간 (Dprop × 2 포함)
```

RTT가 L/R보다 훨씬 크면 (= 전파 지연이 지배적) → 대부분의 시간을 ACK 기다리는 데 낭비.

**Pipelining**: ACK를 기다리지 않고 여러 패킷을 연속 전송.

```
Stop-and-Wait:  [패킷]---[ACK 대기]---[패킷]---...
Pipelining:     [패킷][패킷][패킷]---[ACK][ACK][ACK]
```

구현 방식:
- **Go-Back-N**: 오류 발생 시 해당 패킷부터 전부 재전송
- **Selective Repeat**: 오류 발생한 패킷만 선택 재전송

TCP가 Pipelining + Selective Repeat 기반.

## 버전별 요약표

| 버전 | 추가 가정 | 해결 도구 | 남은 문제 |
|------|----------|----------|----------|
| rdt1.0 | 완벽한 채널 | — | — |
| rdt2.0 | 비트 오류 | checksum, ACK/NAK | ACK/NAK 손상 |
| rdt2.1 | ACK/NAK도 손상 | Sequence Number (0/1) | NAK 별도 관리 복잡 |
| rdt2.2 | (NAK 제거) | ACK에 번호 부착 | 패킷 손실 |
| rdt3.0 | 패킷 손실 | Timeout | 효율 낮음 (Stop-and-Wait) |
| Pipelining | — | 연속 전송 | Go-Back-N / Selective Repeat 선택 |

## 왜 이렇게 설계됐는가

**점진적 문제 해결**: 각 버전은 이전 버전의 엣지 케이스 하나만 추가로 처리한다. 과잉 설계 없이 최소한의 변경으로 현실적 채널에 수렴.

**TCP와의 관계**: TCP는 rdt3.0 + Pipelining + 흐름제어 + 혼잡제어를 실제 구현한 것이다. RDT는 TCP를 이해하기 위한 추상화 사다리.

→ 자세한 패턴 분석: [[RDT-진화]]

## 연결된 개념

- [[Transport-Layer]] — RDT가 속하는 계층 개요
- [[RDT-진화]] — rdt 버전 진화의 설계 철학과 구조적 유사성
- [[UDP]] — 신뢰성을 포기한 선택 (RDT와 대비)
- [[Ones-Complement-Sum]] — rdt2.0에서 쓰는 checksum 계산 방식
