# Obsidian 볼트를 GitHub에 올려서 공유하는 가이드

이 가이드는 두 파트로 구성된다.
- **Part 1**: Obsidian 볼트 → GitHub 올리기 (일반)
- **Part 2**: 이 LLM Wiki 프로젝트를 GitHub로 공유하는 전략

---

## Part 1: Obsidian 볼트 GitHub 올리기

### Step 1 — Git 초기화

볼트 루트에서:

```bash
cd ~/path/to/your-vault
git init
git branch -M main
```

### Step 2 — .gitignore 설정

Obsidian은 내부 설정 파일을 `.obsidian/` 폴더에 저장한다. 대부분은 공유해도 되지만 워크스페이스 상태, 캐시는 제외한다.

```bash
cat > .gitignore << 'EOF'
# Obsidian 내부 파일
.obsidian/workspace
.obsidian/workspace.json
.obsidian/workspaces.json
.obsidian/cache

# 플러그인 캐시 (설치 목록은 공유, 캐시는 제외)
.obsidian/plugins/*/data.json

# OS 파일
.DS_Store
Thumbs.db

# 빌드 결과물 (필요시)
graphify-out/
EOF
```

`.obsidian/` 자체를 제외하지 않는 이유: 테마, 플러그인 목록, 단축키 설정 등은 공유하면 다른 사람이 동일한 환경을 바로 쓸 수 있다.

### Step 3 — GitHub 레포 생성

[github.com/new](https://github.com/new) 에서:
- Repository name: 볼트 이름 (예: `my-wiki`)
- **Public** or **Private** 선택
- README, .gitignore 체크 해제 (로컬에서 만들 것)

생성 후:

```bash
git remote add origin https://github.com/username/repo-name.git
```

### Step 4 — 첫 커밋 & 푸시

```bash
git add .
git commit -m "init: obsidian vault"
git push -u origin main
```

### Step 5 — Obsidian Git 플러그인 (자동 sync)

매번 터미널을 열지 않고 Obsidian 안에서 자동으로 커밋/푸시하려면:

1. Obsidian → Settings → Community Plugins → Browse
2. **"Obsidian Git"** 검색 → Install → Enable
3. 플러그인 설정:
   - `Vault backup interval (minutes)`: `10` (10분마다 자동 커밋)
   - `Auto pull interval (minutes)`: `10` (자동 pull)
   - `Commit message`: `vault backup: {{date}}`
4. 단축키 설정: `Ctrl+Shift+G` → Git 패널 열기

> GitHub 인증: HTTPS를 쓴다면 Personal Access Token (PAT)이 필요하다.
> GitHub → Settings → Developer settings → Personal access tokens → Generate new token
> scope: `repo` 체크. 생성된 토큰을 비밀번호 자리에 입력.

---

## Part 2: 이 LLM Wiki 프로젝트를 GitHub로 공유하기

### 볼트 구조와 공개 전략

```
claudewiki/
├── raw/           ← 개인 원본 노트. 공개 여부 선택 필요.
├── wiki/          ← AI가 컴파일한 지식베이스. 공개 핵심.
├── output/        ← 가이드, 샌드박스 코드. 공개 권장.
├── graphify-out/  ← 빌드 결과물. .gitignore 처리.
└── CLAUDE.md      ← 프로젝트 설명. 공개해도 됨.
```

**권장 전략**: `wiki/`와 `output/`은 공개, `raw/`는 선택.

---

### 옵션 A — raw/ 포함해서 전부 공개

공부한 과정을 투명하게 공유하고 싶을 때.

```gitignore
# .gitignore
graphify-out/
.obsidian/workspace
.obsidian/workspace.json
.obsidian/cache
.DS_Store
나의 핵심 맥락.md      # 개인 정보가 있는 파일만 제외
```

---

### 옵션 B — wiki/ 만 공개 (raw/ 제외)

원본 노트는 개인 보관, 정제된 지식베이스만 공유할 때. 대부분의 LLM Wiki 공개 케이스.

```gitignore
# .gitignore
raw/
graphify-out/
.obsidian/workspace
.obsidian/workspace.json
.obsidian/cache
.DS_Store
나의 핵심 맥락.md
```

---

### 옵션 C — 레포 두 개로 분리

원본 노트용 private 레포 + 지식베이스용 public 레포를 따로 관리.

```bash
# 현재 볼트를 private 레포로 (raw/ 포함)
git remote add origin https://github.com/username/claudewiki-private.git

# wiki/ 폴더만 별도 public 레포로
cd wiki/
git init
git remote add origin https://github.com/username/my-wiki-public.git
git push -u origin main
```

wiki/ 업데이트 시 두 곳 모두 push해야 하므로 관리가 번거롭다. 간단하게 가려면 옵션 B를 권장.

---

### README 작성

GitHub에서 방문자가 처음 보는 페이지. `wiki/index.md`를 기반으로 루트에 README.md를 만든다.

```markdown
# soggyfries's LLM Wiki

AI가 raw 소스를 컴파일해서 유지하는 지식베이스.

## 구조

| 폴더 | 내용 |
|------|------|
| `wiki/concepts/` | 개념 문서 (한 파일 = 한 개념) |
| `wiki/connections/` | 개념 간 연결·비교 |
| `output/guides/` | 가이드 문서 |
| `output/demos/` | 샌드박스 코드 |

## 주제

- 컴퓨터 네트워크 (TCP/IP, Transport Layer, RDT, UDP, DNS)
- Spring Boot (DataSource, JPA, HikariCP)
- OOP / C++ (Polymorphism, Virtual Function)
- AI 지식 관리 (LLM Wiki, RAG)

## 운영 방식

[Andrej Karpathy의 LLM Wiki 방법론](https://youtu.be/...)을 기반으로,
raw/ 폴더에 원본 노트를 쌓고 Claude Code가 wiki/로 정제·통합한다.

---
*AI-compiled knowledge base. Built with Claude Code + Obsidian.*
```

---

### CLAUDE.md 공개 시 주의사항

현재 `CLAUDE.md`에는 작업 워크플로우가 담겨있다. 공개해도 괜찮지만 개인 정보(이메일, 실명 등)는 미리 제거한다.

`나의 핵심 맥락.md` 같이 개인 정보가 들어있는 파일은 `.gitignore`에 명시적으로 추가한다.

---

### 최종 .gitignore (권장 — 옵션 B 기준)

```gitignore
# 개인 원본 노트
raw/

# Obsidian 상태 파일
.obsidian/workspace
.obsidian/workspace.json
.obsidian/workspaces.json
.obsidian/cache
.obsidian/plugins/*/data.json

# 빌드 결과물
graphify-out/

# OS
.DS_Store
Thumbs.db

# 개인 파일
나의 핵심 맥락.md
```

---

### 실행 순서 요약

```bash
# 1. 볼트 루트에서
cd ~/Desktop/_obsidian/claudewiki

# 2. git 초기화
git init && git branch -M main

# 3. .gitignore 생성 (위 내용 붙여넣기)

# 4. README.md 생성

# 5. 첫 커밋
git add .
git commit -m "init: LLM wiki - computer networks, spring, oop"

# 6. GitHub 레포 연결 후 푸시
git remote add origin https://github.com/username/repo.git
git push -u origin main

# 7. Obsidian Git 플러그인으로 이후 자동 sync
```