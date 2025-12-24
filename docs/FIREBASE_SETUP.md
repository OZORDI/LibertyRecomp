# Firebase Setup Guide for Liberty Recomp Multiplayer

This guide walks you through setting up Firebase Realtime Database for online multiplayer signaling in Liberty Recompiled.

## Overview

Liberty Recomp uses **Firebase Realtime Database** as a signaling server for peer-to-peer multiplayer. Firebase handles:
- **Lobby creation and discovery** - Players create/join lobbies with 6-character codes
- **ICE candidate exchange** - WebRTC connection negotiation
- **Player presence** - Tracking who's in each lobby

**Cost**: Firebase's free tier (Spark plan) is sufficient for small-to-medium player bases:
- 1 GB storage
- 10 GB/month download
- 100 simultaneous connections

---

## Step 1: Create a Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)

2. Click **"Create a project"** (or "Add project")

3. Enter a project name (e.g., `libertyrecomp-multiplayer`)

4. **Disable Google Analytics** (not needed for multiplayer)

5. Click **"Create project"** and wait for it to finish

---

## Step 2: Create Realtime Database

1. In your Firebase project, go to **Build → Realtime Database**

2. Click **"Create Database"**

3. Choose a location closest to your players:
   - `us-central1` - United States
   - `europe-west1` - Belgium
   - `asia-southeast1` - Singapore

4. Start in **"Test mode"** (we'll secure it later)

5. Click **"Enable"**

Your database URL will look like:
```
https://your-project-id-default-rtdb.firebaseio.com/
```

---

## Step 3: Get Your Project Credentials

1. Go to **Project Settings** (gear icon → Project settings)

2. Under **"Your apps"**, click **"Add app"** → **Web** (</> icon)

3. Enter an app nickname (e.g., `LibertyRecomp`)

4. **Don't** check "Firebase Hosting"

5. Click **"Register app"**

6. Copy these values from the config shown:
   ```javascript
   const firebaseConfig = {
     apiKey: "AIzaSy...",           // ← Copy this
     projectId: "your-project-id",   // ← Copy this
     databaseURL: "https://your-project-id-default-rtdb.firebaseio.com",
   };
   ```

---

## Step 4: Configure Security Rules

1. Go to **Realtime Database → Rules**

2. Replace the default rules with:

```json
{
  "rules": {
    "lobbies": {
      "$lobbyCode": {
        // Anyone can read lobby info
        ".read": true,
        
        // Only allow creating new lobbies (not overwriting)
        ".write": "!data.exists() || auth != null",
        
        "players": {
          "$peerId": {
            // Players can add/remove themselves
            ".write": true
          }
        },
        
        "signaling": {
          // Anyone in the lobby can send/receive signaling
          ".read": true,
          ".write": true,
          
          // Auto-delete old messages after 1 hour
          ".indexOn": ["timestamp"]
        }
      }
    }
  }
}
```

3. Click **"Publish"**

---

## Step 5: Configure Liberty Recomp

### Option A: Config File (Recommended)

Create or edit `config.toml` in your Liberty Recomp data directory:

**Windows:** `%LOCALAPPDATA%\LibertyRecomp\config.toml`
**macOS:** `~/Library/Application Support/LibertyRecomp/config.toml`
**Linux:** `~/.local/share/LibertyRecomp/config.toml`

Add these lines:

```toml
[multiplayer]
firebase_project_id = "your-project-id"
firebase_api_key = "AIzaSy..."
```

### Option B: Environment Variables

Set these environment variables before launching:

```bash
export LIBERTY_FIREBASE_PROJECT_ID="your-project-id"
export LIBERTY_FIREBASE_API_KEY="AIzaSy..."
```

---

## Step 6: Test Your Setup

1. Launch Liberty Recomp

2. Start a multiplayer game - it will automatically create a lobby

3. Check Firebase Console → Realtime Database → Data
   - You should see a new entry under `/lobbies/`

4. Have a friend join using your lobby code

---

## Troubleshooting

### "Failed to create lobby"

- **Check project ID**: Must match exactly (case-sensitive)
- **Check API key**: Copy the full key including `AIzaSy...`
- **Check database URL**: Should end in `.firebaseio.com`
- **Check rules**: Make sure you published the security rules

### "Lobby not found"

- **Check the code**: Lobby codes are 6 characters, all uppercase
- **Lobby expired**: Lobbies auto-delete after 1 hour of inactivity

### Connection Issues

The P2P connection uses:
- **STUN**: Google's free STUN servers for NAT traversal
- **TURN**: OpenRelay's free TURN servers as fallback

If you're behind a very restrictive NAT (like carrier-grade NAT), connections may fail. Solutions:
1. Try a different network (e.g., mobile hotspot)
2. Use a VPN
3. Host your own TURN server (advanced)

---

## Database Structure

For reference, here's the Firebase data structure:

```
/lobbies/
  /{lobbyCode}/
    hostId: "peer_abc123..."
    hostName: "PlayerOne"
    maxPlayers: 16
    createdAt: 1703376000000
    /players/
      /peer_abc123.../
        name: "PlayerOne"
        joinedAt: 1703376000000
      /peer_def456.../
        name: "PlayerTwo"
        joinedAt: 1703376030000
    /signaling/
      /{messageId}/
        type: 0
        from: "peer_abc123..."
        to: "peer_def456..."
        payload: "{...}"
        timestamp: 1703376031000
```

---

## Cost Optimization

To stay within Firebase's free tier:

1. **Auto-delete old lobbies**: Add a Cloud Function or scheduled job to delete lobbies older than 1 hour

2. **Limit signaling messages**: The code already limits polling to 500ms intervals

3. **Monitor usage**: Check Firebase Console → Usage for your current consumption

### Optional: Auto-Cleanup Cloud Function

Deploy this Cloud Function to auto-delete old lobbies:

```javascript
const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp();

exports.cleanupLobbies = functions.pubsub
  .schedule('every 1 hours')
  .onRun(async (context) => {
    const db = admin.database();
    const now = Date.now();
    const oneHourAgo = now - (60 * 60 * 1000);
    
    const snapshot = await db.ref('lobbies').once('value');
    const updates = {};
    
    snapshot.forEach((lobby) => {
      const createdAt = lobby.child('createdAt').val();
      if (createdAt && createdAt < oneHourAgo) {
        updates[lobby.key] = null;
      }
    });
    
    if (Object.keys(updates).length > 0) {
      await db.ref('lobbies').update(updates);
      console.log(`Deleted ${Object.keys(updates).length} old lobbies`);
    }
    
    return null;
  });
```

---

## Security Best Practices

1. **Never share your API key publicly** - It's okay in your app, but don't post it on GitHub

2. **Enable App Check** (optional) - Prevents abuse from non-app clients

3. **Monitor for abuse** - Check Firebase Console → Usage regularly

4. **Rate limit** (advanced) - Add security rules to limit writes per user

---

## Resources

- [Firebase Realtime Database Docs](https://firebase.google.com/docs/database)
- [Firebase Security Rules](https://firebase.google.com/docs/database/security)
- [Firebase Pricing](https://firebase.google.com/pricing)
- [GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets)

---

## Need Help?

If you're having trouble:
1. Check the [Liberty Recomp Discord](#) for community support
2. Open an issue on [GitHub](https://github.com/user/LibertyRecomp/issues)
3. Make sure you followed each step exactly - Firebase is case-sensitive!
