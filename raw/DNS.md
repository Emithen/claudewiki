## 개요

- 도메인 이름 -> IP 주소로 변환하는 시스템

## 제공 기능

- host aliasing

- load distribution
	- 하나의 이름에 꼭 하나의 IP 만 대응시킬 필요는 없음
	- 요청이 많으면 서로 다른 IP 로 요청을 분산시키기도 함

## 형식

```
www.hongik.ac.kr.
```

- fully qualified domain name

```
www
```

- partially qualified domain name

## 계층 구조

- Root DNS Servers - Root
	- .com DNS Servers - TLD (Top Level Domain)
		- ...
			- Authorotative DNS Servers
	- .org DNS Servers
		- ...
	- .edu DNS Servers
		- ...

- Root DNS Server 가 모든 걸 다 알고 있지는 않다
	- 하위 DNS Server 들에게 요청에 대한 답변 권한을 이양한다
- TLD DNS Server
	- 옛날에는 종류가 매우 한정되어 있었지만 지금은 꽤 많다
	- Label 이 하나다
- Authoritative DNS Server
	- 공식 답변을 주는 말단 권한 서버
	- 더 이상 권한을 이양하지 않고 IP 주소를 답변으로 제공한다
	- 3 번째일 수도 있고 그 이상일 수도 있다

## 개념 요소

- `name`
- `domain`
- `zone`
- `Local DNS server`
- `records`
- 

## 실제 상호작용 예시 흐름 - Iterated qeury

1. `query` 발생

2. `local DNS name servers`

3. `root DNS server` 로 질의 중계

4. `root` 로부터 `TLD` 주소 받아 옴

5. `TLD` 로부터 `...`

6. `...` 로부터 `authoritative server` 의 주소 받아 옴

7. `authoritative server` 로부터 목적지 `IP` 주소가 포함된 공식 답변 획득

## Records

- type=A
	- hostname 과 IP 주소를 매핑
- type=CNAME
	- hostname 과 canonical name 을 매핑
	- 하나의 IP 를 가리키는 여러 hostname 이 존재할 때 IP 를 변수화 가능
- type=NS
	- 