# Liberty Recomp - Multiplayer Backend Setup (Developer Guide)

This guide is for **developers** setting up the multiplayer backend infrastructure for Liberty Recomp.

## Overview

Liberty Recomp supports three multiplayer backends:

| Backend | Use Case | Setup Required |
|---------|----------|----------------|
| **Community Server** | Default for all players | Deploy REST API |
| **Firebase** | Private communities | Firebase project |
| **LAN** | Local network play | None (built-in) |

---

## Community Server Setup (Default Backend)

The community server is a simple REST API that handles session tracking for all players using the default configuration.

### API Specification

The community server must implement these endpoints:

#### `POST /api/sessions` - Create Session
```json
// Request
{
  "hostPeerId": "peer_abc123...",
  "hostName": "PlayerOne",
  "gameMode": 0,
  "mapArea": 0,
  "maxPlayers": 16,
  "currentPlayers": 1,
  "isPrivate": false,
  "lobbyCode": ""
}

// Response
{
  "sessionId": "session_xyz789...",
  "lobbyCode": "ABC123"
}
```

#### `GET /api/sessions` - List/Search Sessions
```
GET /api/sessions?gameMode=0&mapArea=0&notFull=true&public=true&limit=20
```
```json
// Response
[
  {
    "sessionId": "session_xyz789...",
    "hostPeerId": "peer_abc123...",
    "hostName": "PlayerOne",
    "gameMode": 0,
    "mapArea": 0,
    "maxPlayers": 16,
    "currentPlayers": 3,
    "isPrivate": false,
    "lobbyCode": ""
  }
]
```

#### `GET /api/sessions?lobbyCode=ABC123` - Find by Code
```json
// Response
{
  "sessionId": "session_xyz789...",
  "hostPeerId": "peer_abc123...",
  ...
}
```

#### `POST /api/sessions/{sessionId}/join` - Join Session
```json
// Request
{
  "peerId": "peer_def456...",
  "playerName": "PlayerTwo"
}

// Response
{
  "hostPeerId": "peer_abc123..."
}
```

#### `POST /api/sessions/{sessionId}/leave` - Leave Session
```json
// Request
{
  "peerId": "peer_def456..."
}
```

#### `PUT /api/sessions/{sessionId}` - Update Session
```json
// Request
{
  "currentPlayers": 4
}
```

#### `POST /api/sessions/{sessionId}/heartbeat` - Keep Alive
```json
// Request
{}
```

#### `DELETE /api/sessions/{sessionId}` - Close Session

### Game Mode Values

| Value | Mode |
|-------|------|
| 0 | Free Mode |
| 1 | Deathmatch |
| 2 | Team Deathmatch |
| 3 | Mafiya Work |
| 4 | Team Mafiya Work |
| 5 | Car Jack City |
| 6 | Team Car Jack City |
| 7 | Race |
| 8 | GTA Race |
| 9 | Cops 'n' Crooks |
| 10 | Turf War |
| 11 | Deal Breaker |
| 12 | Hangman's NOOSE |
| 13 | Bomb Da Base II |
| 14 | Party Mode |

### Map Area Values

| Value | Area |
|-------|------|
| 0 | All of Liberty City |
| 1 | Broker |
| 2 | Dukes |
| 3 | Bohan |
| 4 | Algonquin |
| 5 | Alderney |

### Deployment Options

#### Option 1: Cloudflare Workers (Free Tier)
```javascript
// worker.js - Example implementation
const sessions = new Map();

export default {
  async fetch(request, env) {
    const url = new URL(request.url);
    const path = url.pathname;
    
    if (request.method === 'POST' && path === '/api/sessions') {
      const body = await request.json();
      const sessionId = crypto.randomUUID();
      const lobbyCode = generateLobbyCode();
      
      sessions.set(sessionId, {
        ...body,
        sessionId,
        lobbyCode: body.isPrivate ? lobbyCode : '',
        createdAt: Date.now(),
        lastHeartbeat: Date.now()
      });
      
      return Response.json({ sessionId, lobbyCode });
    }
    
    // ... implement other endpoints
  }
};

function generateLobbyCode() {
  const chars = 'ABCDEFGHJKLMNPQRSTUVWXYZ23456789';
  let code = '';
  for (let i = 0; i < 6; i++) {
    code += chars[Math.floor(Math.random() * chars.length)];
  }
  return code;
}
```

#### Option 2: Supabase (Free Tier)
- Create a Supabase project
- Create `sessions` table with columns matching the API spec
- Use Supabase REST API or Edge Functions

#### Option 3: Self-Hosted (Node.js/Express)
```javascript
const express = require('express');
const app = express();
app.use(express.json());

const sessions = new Map();

app.post('/api/sessions', (req, res) => {
  // Implementation
});

// ... other endpoints

app.listen(3000);
```

### Session Cleanup

Sessions should be cleaned up after inactivity. Recommended:
- Delete sessions with no heartbeat for 60 seconds
- Run cleanup every 30 seconds

```javascript
setInterval(() => {
  const now = Date.now();
  for (const [id, session] of sessions) {
    if (now - session.lastHeartbeat > 60000) {
      sessions.delete(id);
    }
  }
}, 30000);
```

### Default Server URL

The client defaults to:
```
https://liberty-sessions.libertyrecomp.com
```

Update `CommunityServerURL` in config to use a different server.

---

## Firebase Setup (Alternative Backend)

For private communities who want their own matchmaking server.

### Step 1: Create Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Click **"Create a project"**
3. Enter a project name (e.g., `my-gta4-server`)
4. **Disable Google Analytics** (not needed)
5. Click **"Create project"**

### Step 2: Create Realtime Database

1. Go to **Build → Realtime Database**
2. Click **"Create Database"**
3. Choose a location closest to your players
4. Start in **"Test mode"**
5. Click **"Enable"**

### Step 3: Get Credentials

1. Go to **Project Settings** (gear icon)
2. Under **"Your apps"**, click **Web** (</> icon)
3. Register an app
4. Copy `apiKey` and `projectId`

### Step 4: Configure Security Rules

Go to **Realtime Database → Rules** and set:

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

### Step 5: Configure Client

Players using your Firebase server need this config:

```toml
[Multiplayer]
MultiplayerBackend = "Firebase"
FirebaseProjectId = "your-project-id"
FirebaseApiKey = "AIzaSy..."
```

### Firebase Free Tier Limits

- 1 GB storage
- 10 GB/month download
- 100 simultaneous connections

Sufficient for small-to-medium communities.

---

## LAN Backend

No setup required. Built into the client.

Uses UDP broadcast on port 3074 (configurable) for local network discovery.

```toml
[Multiplayer]
MultiplayerBackend = "LAN"
LANBroadcastPort = 3074
```

---

## Testing

### Test Community Server Locally

1. Run your server on `localhost:3000`
2. Set config:
   ```toml
   [Multiplayer]
   MultiplayerBackend = "Community"
   CommunityServerURL = "http://localhost:3000"
   ```
3. Launch two instances of Liberty Recomp
4. Create session on one, join on other

### Test Firebase

1. Set up Firebase project
2. Configure credentials in config
3. Check Firebase Console → Realtime Database → Data for session entries

### Test LAN

1. Set both clients to LAN mode
2. Ensure both are on same network
3. Create session on one, search on other

---

## Monitoring

### Community Server Metrics

Track these for production:
- Active sessions count
- Sessions created/hour
- Average session duration
- API response times

### Firebase Monitoring

Use Firebase Console → Usage to monitor:
- Database reads/writes
- Bandwidth usage
- Concurrent connections

---

## Security Considerations

1. **Rate limiting** - Prevent session spam
2. **Input validation** - Sanitize all inputs
3. **HTTPS only** - Never run production over HTTP
4. **CORS** - Configure appropriately for your domain

---

## Resources

- [Cloudflare Workers Docs](https://developers.cloudflare.com/workers/)
- [Supabase Docs](https://supabase.com/docs)
- [Firebase Realtime Database](https://firebase.google.com/docs/database)
