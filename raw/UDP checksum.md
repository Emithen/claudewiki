# UDP Checksum

## 개요
UDP Checksum은 UDP 세그먼트 전송 과정에서 **데이터의 무결성(Integrity)을 검증하기 위한 오류 검출 메커니즘**이다.  

송신 측에서 계산된 체크섬 값은 수신 측에서 다시 계산되어 비교되며, 값이 다르면 데이터가 손상된 것으로 판단한다.

---

## 목적
- 데이터 전송 중 발생하는 **비트 오류 검출**
- 헤더 + 데이터 전체에 대한 무결성 확인

---

## 특징
- **16비트 필드**로 구성
- UDP 헤더에 포함됨
- 오류 검출만 가능 (복구 기능 없음)
- IPv4에서는 선택 사항, IPv6에서는 필수

---

## 계산 범위
UDP Checksum은 단순히 UDP 데이터만이 아니라 다음을 포함하여 계산된다:

1. **Pseudo Header (가상 헤더)**
2. UDP Header
3. 데이터 (Payload)

---

## Pseudo Header 구성
UDP의 신뢰성을 보완하기 위해 IP 계층 정보 일부를 포함한다.

| 필드 | 설명 |
|------|------|
| Source IP Address | 송신 IP |
| Destination IP Address | 수신 IP |
| Protocol | UDP 식별 값 (17) |
| UDP Length | UDP 전체 길이 |

---

## 계산 방식
1. 모든 필드를 16비트 단위로 나눔
2. 1의 보수 덧셈 (One's Complement Sum) 수행
3. 결과를 다시 1의 보수 취함 → Checksum 생성

---

## 동작 과정
### 송신 측
1. Checksum 필드를 0으로 설정
2. Pseudo Header + UDP Header + Data를 기반으로 계산
3. 결과를 Checksum 필드에 삽입 후 전송

### 수신 측
1. 동일한 방식으로 Checksum 재계산
2. 결과가 0이면 정상
3. 0이 아니면 오류 발생 → 패킷 폐기

---

## 주의 사항
- Checksum 값이 0인 경우:
  - IPv4에서는 "체크섬 사용 안 함" 의미 가능
- IPv6에서는 반드시 체크섬을 사용해야 함

---

## 정리
UDP Checksum은 **간단하지만 중요한 오류 검출 장치**로,  
UDP의 낮은 신뢰성을 보완하는 최소한의 보호 메커니즘이다.