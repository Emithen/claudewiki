# DNS

한 줄 요약: 도메인 이름을 IP 주소로 변환하는 분산 계층 시스템.

## 직관

전화번호부와 같다. 사람은 "홍익대학교"라는 이름을 기억하지만, 전화를 걸려면 번호가 필요하다. DNS는 `www.hongik.ac.kr` → `203.xxx.xxx.xxx`로 변환하는 인터넷 전화번호부다.

단일 서버에 모든 도메인을 저장하지 않는 이유도 전화번호부와 같다: 너무 방대하고, 장애 시 전체 인터넷 중단. 그래서 **분산 계층 구조**.

## 제공 기능

- **이름 → IP 변환** (핵심 기능)
- **Host Aliasing** — 하나의 IP에 여러 이름 연결
- **Load Distribution** — 하나의 이름에 여러 IP 대응, 요청 분산

## 계층 구조

```
Root DNS Servers          (.com, .org, .edu 등 TLD 위치 알고 있음)
    ↓
TLD DNS Servers           (Top Level Domain: .com, .kr, .edu)
    ↓
Authoritative DNS Servers (실제 IP 주소를 가진 말단 서버)
```

- **Root**: 모든 걸 직접 알지 않음. TLD 서버 위치를 알려줌 (권한 이양)
- **TLD**: `.com`, `.org`, `.kr` 등. Label 하나짜리
- **Authoritative**: 최종 IP 주소 응답. 더 이상 권한 이양 없음

## 도메인 이름 형식

```
www.hongik.ac.kr.
```
= Fully Qualified Domain Name (FQDN)

```
www
```
= Partially Qualified Domain Name

## Iterated Query 흐름

```
1. 브라우저 → Local DNS (캐시 확인)
2. Local DNS → Root DNS ("root야, .kr 어디 있어?")
3. Root DNS → Local DNS ("TLD 서버 주소 알려줄게")
4. Local DNS → TLD DNS ("hongik.ac.kr 어디 있어?")
5. TLD DNS → Local DNS ("Authoritative 서버 주소 알려줄게")
6. Local DNS → Authoritative DNS ("www.hongik.ac.kr IP 알려줘")
7. Authoritative DNS → Local DNS (IP 주소 반환)
8. Local DNS → 브라우저 (IP 주소 전달)
```

매번 Root까지 올라가지 않도록 **캐싱** 적극 활용.

## DNS Records

| 타입 | 매핑 | 설명 |
|------|------|------|
| A | hostname → IP | 기본 이름 해석 |
| CNAME | hostname → canonical name | 별명. 여러 이름이 하나의 IP를 가리킬 때 IP를 변수화 |
| NS | domain → name server | 어느 DNS 서버가 담당하는지 |
| MX | domain → mail server | 이메일 서버 위치 |

## 왜 이렇게 설계됐는가

중앙화 vs 분산화의 trade-off. 중앙화하면 관리는 쉽지만 단일 장애점, 확장성 한계. DNS는 **계층적 분산**으로 해결:
- 각 TLD가 자기 도메인만 책임
- 캐싱으로 반복 조회 비용 절감
- Root 서버는 전 세계에 다중 복제

## 연결된 개념

- [[Application-Layer]] — DNS는 Application Layer 프로토콜
- [[UDP]] — DNS 질의는 UDP로 전송 (빠른 단순 요청-응답)
- [[TCP-IP-5계층]]
