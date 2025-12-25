# Liberty Recomp - Online Multiplayer

Online multiplayer in Liberty Recompiled is **seamless** - it works just like the original Xbox 360 game, but **better**. The default configuration requires **no setup** - just start a multiplayer session and play with friends over the internet.

> **🎮 64-Player Support**: Liberty Recomp supports up to **64 players** per session - 4× the original Xbox 360 limit of 16!

## How It Works

Liberty Recomp uses **GameNetworkingSockets** (Valve's networking library) for peer-to-peer gameplay with a configurable **session tracker** for matchmaking. This enables:

- **Xbox Live-style matchmaking** - Quick Match, Custom Match with filters, session browsing
- **No port forwarding required** - NAT traversal via STUN/TURN
- **Encrypted connections** - AES-256-GCM encryption
- **Low latency** - Direct peer-to-peer when possible
- **Automatic fallback** - TURN relay if direct connection fails

```
┌─────────────────────────────────────────────────────────────────────┐
│                     Session Tracker Backends                        │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐     │
│  │ Community Server│  │    Firebase     │  │    LAN Only     │     │
│  │   (Default)     │  │  (Self-Hosted)  │  │  (Local Play)   │     │
│  └────────┬────────┘  └────────┬────────┘  └────────┬────────┘     │
│           │                    │                    │               │
│           └────────────────────┼────────────────────┘               │
│                                │                                    │
│                    ┌───────────▼───────────┐                        │
│                    │   GameNetworkingSockets│                        │
│                    │      (P2P Data)        │                        │
│                    └───────────────────────┘                        │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Quick Start

### For Players (Default - Community Server)

**No setup required!** Just:
1. Launch Liberty Recomp
2. Go to Multiplayer menu
3. Choose Quick Match or Custom Match
4. Play!

The community server handles matchmaking automatically.

### For Private Communities (Firebase)

Want to run your own private matchmaking server? See the **Firebase** backend option below.

### For LAN Parties

Playing on a local network without internet? Use **LAN mode** for automatic local discovery.

---

## Multiplayer Backends

### Community Server (Default)

The recommended option for most players. Uses the Liberty Recomp community-hosted REST API.

**Features:**
- Works out of the box
- Full Quick Match / Custom Match support
- Session browsing with filters (game mode, map area)
- Lobby codes for private sessions

**Config:**
```toml
[Multiplayer]
MultiplayerBackend = "Community"
```

### Firebase (Self-Hosted)

For private communities who want complete control over their matchmaking.

**Features:**
- Host your own matchmaking server
- Full control over player access
- Same Xbox Live-style features

**Setup:**
1. Create a Firebase project at [console.firebase.google.com](https://console.firebase.google.com)
2. Enable Realtime Database
3. Configure security rules (see below)
4. Add credentials to config

**Config:**
```toml
[Multiplayer]
MultiplayerBackend = "Firebase"
FirebaseProjectId = "your-project-id"
FirebaseApiKey = "AIzaSy..."
```

**Security Rules:**
```json
{
  "rules": {
    "sessions": {
      "$sessionId": {
        ".read": true,
        ".write": true,
        "players": {
          "$peerId": {
            ".write": true
          }
        }
      }
    }
  }
}
```

### LAN Only

For offline/local network play without internet matchmaking.

**Features:**
- UDP broadcast discovery on local network
- No internet connection required
- Automatic session discovery
- Works behind any firewall

**Config:**
```toml
[Multiplayer]
MultiplayerBackend = "LAN"
# LANBroadcastPort = 3074  # Optional, default is 3074
```

---

## GTA IV Game Modes

All original multiplayer modes are supported:

| Mode | Description |
|------|-------------|
| **Free Mode** | Open world sandbox (up to **64 players** - was 16 on Xbox 360) |
| **Deathmatch** | Free-for-all combat |
| **Team Deathmatch** | Team-based combat |
| **Mafiya Work** | Complete jobs for Petrovic |
| **Team Mafiya Work** | Team-based Mafiya Work |
| **Car Jack City** | Steal and deliver vehicles |
| **Team Car Jack City** | Team-based Car Jack City |
| **Race** | Vehicle racing |
| **GTA Race** | Racing with weapons |
| **Cops 'n' Crooks** | Asymmetric cops vs criminals |
| **Turf War** | Territory control |
| **Hangman's NOOSE** | Cooperative extraction |
| **Deal Breaker** | Cooperative mission |
| **Bomb Da Base II** | Cooperative mission |

---

## Network Architecture

| Component | Purpose |
|-----------|---------|
| **Session Tracker** | Lobby management & matchmaking |
| **STUN (Google)** | NAT type detection & hole punching |
| **TURN (OpenRelay)** | Relay for restrictive NATs |
| **GameNetworkingSockets** | Reliable/unreliable P2P messaging |

### Virtual Network

When in a multiplayer session, players are assigned virtual IPs in the `192.168.100.0/24` range:

| Player | Virtual IP |
|--------|------------|
| Host | 192.168.100.1 |
| Player 2 | 192.168.100.2 |
| Player 3 | 192.168.100.3 |
| ... | ... |

The game's original networking code sees these as LAN addresses.

---

## Troubleshooting

### Can't Find Sessions

1. **Check your internet connection** - Community server requires internet
2. **Try LAN mode** - If on local network, switch to LAN backend
3. **Check backend config** - Ensure `MultiplayerBackend` is set correctly

### Can't Connect to Other Players

1. **Wait longer** - NAT traversal can take 5-15 seconds
2. **Try a different network** - Some corporate/school networks block P2P
3. **Disable VPN** - VPNs can interfere with NAT traversal

### High Latency

- **Direct P2P**: 5-50ms overhead (depends on distance)
- **TURN relay**: 50-200ms overhead (fallback mode)

If latency is consistently high, you may be using TURN relay. This happens when:
- Both players are behind symmetric NAT
- One player is behind carrier-grade NAT (CGNAT)

### Connection Drops

- **Intermittent network** - Check your WiFi/Ethernet stability
- **Firewall** - Ensure outbound UDP is allowed
- **Timeout** - Connections drop after 30s of no response

---

## Technical Details

### STUN Servers Used

```
stun:stun.l.google.com:19302
stun:stun1.l.google.com:19302
stun:stun.cloudflare.com:3478
```

### TURN Servers Used

```
turn:openrelay.metered.ca:80 (UDP)
turn:openrelay.metered.ca:443 (TCP)
Username: openrelayproject
Password: openrelayproject
```

These are free public TURN servers provided by [metered.ca](https://www.metered.ca/tools/openrelay/) for testing.

### Ports

| Port | Protocol | Direction | Purpose |
|------|----------|-----------|---------|
| 443 | TCP | Outbound | Session tracker API |
| 19302 | UDP | Outbound | Google STUN |
| 3478 | UDP | Outbound | Cloudflare STUN |
| 80, 443 | UDP/TCP | Outbound | OpenRelay TURN |
| 3074 | UDP | Both | LAN broadcast (LAN mode only) |
| Random | UDP | Both | P2P data (after NAT traversal) |

No inbound ports need to be opened.

---

## Configuration Reference

### config.toml Options

```toml
[Multiplayer]
# Backend selection: "Community", "Firebase", or "LAN"
MultiplayerBackend = "Community"

# Community server URL (hidden, uses default)
# CommunityServerURL = "https://liberty-sessions.libertyrecomp.com"

# Firebase credentials (only for Firebase backend)
# FirebaseProjectId = "your-project-id"
# FirebaseApiKey = "AIzaSy..."

# LAN broadcast port (only for LAN backend)
# LANBroadcastPort = 3074

# Your display name in multiplayer
# PlayerName = "Player"
```

---

## Security

- **End-to-end encryption**: All P2P traffic uses AES-256-GCM
- **No IP exposure**: Your real IP is only visible to STUN/TURN servers, not other players
- **Session isolation**: Players can only see sessions they search for or have codes for

---

## Performance Tips

1. **Wired connection** - Ethernet is more stable than WiFi
2. **Close bandwidth-heavy apps** - Streaming/downloads increase latency
3. **Nearby host** - The host's location affects everyone's latency
4. **Smaller lobbies** - Fewer players = less bandwidth per player

---

## Comparison with Original

| Feature | Xbox 360 Original | Liberty Recomp |
|---------|-------------------|----------------|
| **Max Players** | 16 | **64** (4× increase!) |
| LAN Play | ✅ | ✅ (unchanged) |
| System Link | ✅ (local only) | ✅ (over internet via LAN mode) |
| Xbox Live | ❌ (discontinued) | ✅ (Community Server) |
| Quick Match | ✅ | ✅ |
| Custom Match | ✅ | ✅ |
| Session Browsing | ✅ | ✅ |
| Lobby Codes | ✅ | ✅ |
| Port Forward Required | No | No |

---

## Resources

- [GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets)
- [Firebase Setup (for self-hosting)](FIREBASE_SETUP.md)
