# Liberty Recomp - Online Multiplayer Guide

This guide explains how to set up and use online multiplayer in Liberty Recompiled.

## Overview

Liberty Recomp uses **Nebula VPN** to enable online multiplayer by creating a virtual LAN between players over the internet. This allows players who are not on the same local network to connect and play together as if they were on the same LAN.

**How it works:**
1. One player creates a "network" and becomes the **lighthouse** (coordinator)
2. Other players join the network using the shared credentials
3. Nebula creates encrypted tunnels between all players
4. The game's existing LAN multiplayer code works unchanged

## Quick Start

### Option A: Join an Existing Network

If someone has already created a network and shared the credentials with you:

1. **Main Menu → Multiplayer → Online Setup**
2. Select **"Join Existing Network"**
3. Enter the network details:
   - **CA Certificate**: Paste the certificate shared by the host
   - **Lighthouse Address**: The host's public IP:port (e.g., `203.0.113.45:4242`)
   - **Your Virtual IP**: Choose an unused IP (e.g., `192.168.100.2/24`)
4. Click **Connect**
5. Return to **Multiplayer** and browse for games

### Option B: Create a New Network (Host)

To host a network for your friends:

1. **Main Menu → Multiplayer → Online Setup**
2. Select **"Create New Network"**
3. Configure:
   - **Network Name**: A name for your network
   - **Your Public IP**: Auto-detected or enter manually
   - **Port**: `4242` (default, must be open on your router)
4. Click **Create**
5. **Share the CA certificate** with your friends (Export button)
6. Return to **Multiplayer** and host a game

### Port Forwarding (Host Only)

The lighthouse host must open **UDP port 4242** on their router:

1. Find your router's admin page (usually `192.168.1.1` or `192.168.0.1`)
2. Navigate to Port Forwarding / NAT settings
3. Add a rule:
   - **Port**: 4242
   - **Protocol**: UDP
   - **Internal IP**: Your computer's local IP
4. Save and apply

## Network Architecture

```
                          INTERNET
                              │
              ┌───────────────┼───────────────┐
              │               │               │
              ▼               ▼               ▼
        ┌──────────┐   ┌──────────┐   ┌──────────┐
        │ Player A │   │ Player B │   │ Player C │
        │  (Host)  │   │          │   │          │
        │ 192.168  │   │ 192.168  │   │ 192.168  │
        │ .100.1   │   │ .100.2   │   │ .100.3   │
        └────┬─────┘   └────┬─────┘   └────┬─────┘
             │              │              │
             └──────────────┼──────────────┘
                            │
                   Virtual LAN Network
                    192.168.100.0/24
```

- **Lighthouse (Host)**: Has public IP, coordinates peer discovery
- **Clients**: Connect through lighthouse, then directly to each other
- **P2P Mesh**: After initial connection, traffic flows directly between peers

## Detailed Setup

### Creating Certificates (Advanced)

For manual setup or scripting:

```bash
# Navigate to Nebula directory
cd ~/.local/share/LibertyRecomp/nebula/

# Generate CA (do once per network)
./nebula-cert ca -name "MyGameNetwork" -duration 8760h

# Generate lighthouse certificate
./nebula-cert sign -name "lighthouse" -ip "192.168.100.1/24" -groups "lighthouse"

# Generate player certificates
./nebula-cert sign -name "player2" -ip "192.168.100.2/24"
./nebula-cert sign -name "player3" -ip "192.168.100.3/24"
```

**Important:** Keep `ca.key` private! Only share `ca.crt` with trusted players.

### Configuration Files

Configuration files are stored in:

| Platform | Location |
|----------|----------|
| Windows | `%LOCALAPPDATA%\LibertyRecomp\nebula\` |
| macOS | `~/Library/Application Support/LibertyRecomp/nebula/` |
| Linux | `~/.local/share/LibertyRecomp/nebula/` |

Key files:
- `ca.crt` - Certificate Authority (shared)
- `ca.key` - CA private key (keep secret!)
- `<name>.crt` - Node certificate
- `<name>.key` - Node private key
- `config.yml` - Nebula configuration

### Starting Nebula Manually

```bash
# Start Nebula (requires sudo on macOS/Linux for TUN device)
sudo ./nebula -config config.yml
```

## Troubleshooting

### Cannot Connect to Lighthouse

1. **Check port forwarding**: Verify UDP 4242 is open on the host's router
2. **Check firewall**: Ensure firewall allows Nebula
3. **Verify IP address**: Confirm the lighthouse's public IP is correct
4. **Test connectivity**: `nc -vzu <lighthouse-ip> 4242`

### Players Can't See Each Other's Games

1. **Wait for connection**: Allow 10-30 seconds for NAT traversal
2. **Check virtual IPs**: Each player must have a unique IP in the range
3. **Verify certificates**: All players must use the same CA certificate
4. **Check Nebula status**: Ensure Nebula shows "Connected" status

### High Latency

1. **Direct connection failed**: If behind carrier-grade NAT, traffic may route through lighthouse
2. **Geographic distance**: Latency increases with physical distance
3. **Enable relay**: If direct P2P fails, enable relay mode in config

### Permission Errors (macOS/Linux)

Nebula requires root/admin privileges to create TUN devices:

```bash
# macOS: Grant TUN permissions or run with sudo
sudo ./nebula -config config.yml

# Linux: Same, or use capabilities
sudo setcap cap_net_admin=+ep ./nebula
./nebula -config config.yml
```

### Windows Firewall

Add firewall rule for Nebula:

```powershell
# Run as Administrator
netsh advfirewall firewall add rule name="Nebula VPN" dir=in action=allow protocol=UDP localport=4242
netsh advfirewall firewall add rule name="Nebula VPN Out" dir=out action=allow protocol=UDP
```

## Security Considerations

- **Certificates**: Only share `ca.crt` and individual node certificates with trusted players
- **CA Key**: Never share `ca.key` - anyone with it can create valid certificates
- **Network isolation**: Nebula traffic is encrypted end-to-end using the Noise protocol
- **IP spoofing**: Impossible due to certificate-based authentication

## Performance

| Metric | Typical Value |
|--------|---------------|
| Added latency (P2P) | 5-15ms |
| Added latency (Relayed) | 30-100ms |
| Bandwidth overhead | ~5% |
| Connection time | 5-30 seconds |

## Command Reference

| Action | In-Game | Command Line |
|--------|---------|--------------|
| Start Nebula | Multiplayer → Online → Connect | `nebula -config config.yml` |
| Stop Nebula | Multiplayer → Online → Disconnect | `pkill nebula` |
| Check status | Multiplayer → Online → Status | `nebula -config config.yml -test` |
| Generate CA | Online Setup → Create Network | `nebula-cert ca -name "Name"` |
| Generate cert | Automatic | `nebula-cert sign -name "name" -ip "IP"` |

## FAQ

**Q: Do all players need to port forward?**  
A: No, only the lighthouse host. Other players connect through NAT hole punching.

**Q: Can I play with more than 16 players?**  
A: The virtual network supports 253 players (192.168.100.2 - 192.168.100.254). Game limits may apply.

**Q: Does this work on mobile/console?**  
A: No, Nebula only runs on Windows, macOS, and Linux.

**Q: Is my real IP exposed?**  
A: Your real IP is only visible to the lighthouse host. Other players only see your virtual IP.

**Q: Can I use this with other VPNs?**  
A: Generally yes, but may cause routing conflicts. Disable other VPNs while using Nebula.

## In-Game Troubleshooting

The game includes a built-in diagnostics tool accessible from **Multiplayer → Online Setup → Diagnostics**.

### Diagnostics Page Features

| Check | What It Tests | Common Fixes |
|-------|---------------|---------------|
| Nebula Status | Binary availability | Run `tools/nebula/build.sh` |
| Config Valid | Configuration exists | Create/join network first |
| Connection | Lighthouse reachable | Check port forwarding, firewall |
| Peers | Other players visible | Wait 10-30s, check certificates |

### Status Indicator Colors

| Color | Meaning | Action |
|-------|---------|--------|
| 🟢 Green | Connected | Ready to play |
| 🟡 Yellow | Connecting | Wait for connection |
| 🔴 Red | Error/Disconnected | Check diagnostics |
| ⚪ Gray | Not configured | Set up network first |

### Common Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| "Binaries not found" | Nebula not built | Run `tools/nebula/build.sh --download` |
| "Failed to generate CA" | Permission denied | Run as admin/root |
| "Connection timeout" | Port blocked | Forward UDP 4242 |
| "Certificate mismatch" | Wrong CA cert | Get correct cert from host |
| "TUN device error" | Missing permissions | Run with elevated privileges |

### Log Files

Nebula logs are stored in:

| Platform | Location |
|----------|----------|
| Windows | `%LOCALAPPDATA%\LibertyRecomp\nebula\logs\` |
| macOS | `~/Library/Application Support/LibertyRecomp/nebula/logs/` |
| Linux | `~/.local/share/LibertyRecomp/nebula/logs/` |

### Network Test Commands

```bash
# Test if lighthouse is reachable (replace with actual IP)
nc -vzu 203.0.113.45 4242

# Check Nebula process status
ps aux | grep nebula

# View Nebula logs in real-time
tail -f ~/.local/share/LibertyRecomp/nebula/logs/nebula.log
```

## Resources

- [Nebula GitHub](https://github.com/slackhq/nebula)
- [Nebula Documentation](https://nebula.defined.net/docs/)
- [Liberty Recomp Discord](#) - Get help from the community
- [Port Forwarding Guide](https://portforward.com/)
