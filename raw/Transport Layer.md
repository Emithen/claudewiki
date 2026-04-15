## 개요
Transport Layer(전송 계층)는 OSI 7계층 모델에서 4번째 계층으로, 종단 간(End-to-End) 통신을 담당한다.  
데이터를 송신 측 프로세스에서 수신 측 프로세스까지 전달하며, 신뢰성, 흐름 제어, 오류 제어, 혼잡 제어 등의 기능을 제공한다.

주요 목적은 애플리케이션 간의 **논리적 통신 채널**을 구축하고, 데이터가 올바르게 전달되도록 보장하는 것이다.

## 주요 기능
- **Segmentation & Reassembly**: 데이터를 세그먼트로 나누고 재조립
- **Port Addressing**: 프로세스 식별을 위한 포트 번호 사용
- **Connection Control**: 연결 지향 / 비연결 지향 통신 제공
- **Flow Control**: 송수신 속도 조절
- **Error Control**: 데이터 손실 및 오류 복구
- **Congestion Control**: 네트워크 혼잡 관리

## 대표 프로토콜 - [[TCP&UDP]]
- **TCP (Transmission Control Protocol)**: 신뢰성 보장, 연결 지향
- **UDP (User Datagram Protocol)**: 비연결, 빠르지만 신뢰성 낮음

---

## 트리 구조

```
Transport Layer
├── 기능
│   ├── Segmentation & Reassembly
│   ├── Port Addressing
│   ├── Connection Control
│   ├── Flow Control
│   ├── Error Control
│   └── Congestion Control
│
├── 프로토콜
│   ├── TCP
│   │   ├── 연결 지향 (3-way handshake)
│   │   ├── 신뢰성 보장
│   │   ├── 흐름 제어 (Sliding Window)
│   │   └── 혼잡 제어
│   │
│   └── UDP
│       ├── 비연결 지향
│       ├── 빠른 전송
│       └── 낮은 오버헤드
│
└── 핵심 개념
    ├── Port
    ├── Socket (IP + Port)
    ├── Segment
    └── End-to-End Communication
```