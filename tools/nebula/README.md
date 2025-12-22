# Nebula VPN Integration for Liberty Recomp

Nebula is used to enable online multiplayer by creating a virtual LAN between players over the internet. This allows the existing LAN multiplayer code to work unchanged for online play.

## Overview

[Nebula](https://github.com/slackhq/nebula) is a scalable overlay networking tool with a focus on performance, simplicity, and security. It creates encrypted tunnels between hosts, making remote players appear to be on the same local network.

**Key Benefits:**
- **No port forwarding required** for most players (UDP hole punching)
- **End-to-end encryption** using Noise protocol
- **Peer-to-peer connections** when possible (low latency)
- **Lighthouse relay** as fallback for restrictive NATs
- **Cross-platform** - Windows, macOS, Linux

## Architecture

```
┌─────────────┐         ┌─────────────┐         ┌─────────────┐
│   Player A  │◄───────►│  Lighthouse │◄───────►│   Player B  │
│ 192.168.100.2│        │ 192.168.100.1│        │ 192.168.100.3│
└─────────────┘         └─────────────┘         └─────────────┘
       │                                                │
       └────────────────────────────────────────────────┘
                    Direct P2P Connection
                 (after NAT hole punching)
```

1. **Lighthouse** - Coordinator node with public IP, helps peers discover each other
2. **Clients** - Player nodes that connect through the lighthouse

## Quick Start

### Building from Source

Requires Go 1.21 or later:

```bash
# Build for current platform
./build.sh

# Or with CMake
cmake -B build -DNEBULA_BUILD_FROM_SOURCE=ON
cmake --build build
```

### Using Pre-built Binaries

Download from [Nebula Releases](https://github.com/slackhq/nebula/releases) and place in:
- `bin/windows-amd64/` for Windows
- `bin/darwin-amd64/` or `bin/darwin-arm64/` for macOS
- `bin/linux-amd64/` for Linux

## Network Setup

### 1. Create Network (Host/Lighthouse)

The player hosting the network needs a publicly accessible IP and port.

```bash
# Generate CA certificate (do once per network)
./nebula-cert ca -name "LibertyRecomp-MyNetwork"

# Generate lighthouse certificate
./nebula-cert sign -name "lighthouse" -ip "192.168.100.1/24"

# Generate player certificates
./nebula-cert sign -name "player2" -ip "192.168.100.2/24"
./nebula-cert sign -name "player3" -ip "192.168.100.3/24"
```

Share `ca.crt` and player certificates with other players (keep `ca.key` private!).

### 2. Configure Lighthouse

Edit `config/lighthouse.yml`:
- Set paths to certificates
- Set your public IP
- Open UDP port 4242 on your router

### 3. Configure Clients

Each player edits `config/client.yml`:
- Set paths to their certificates
- Set lighthouse public IP:port

### 4. Start Nebula

```bash
# On lighthouse
sudo ./nebula -config lighthouse.yml

# On clients
sudo ./nebula -config client.yml
```

## Integration with Liberty Recomp

The game integrates Nebula through the `NebulaManager` class which handles:
- Certificate generation and management
- Configuration file creation
- Starting/stopping the Nebula service
- Connection status monitoring

### In-Game Setup

1. **Main Menu → Multiplayer → Online Setup**
2. Choose "Create Network" or "Join Network"
3. Follow the wizard to configure certificates
4. Start Nebula and connect to other players

## Firewall Configuration

### Required Ports

| Port | Protocol | Direction | Purpose |
|------|----------|-----------|---------|
| 4242 | UDP | Inbound | Nebula (lighthouse only) |
| Any | UDP | Outbound | Nebula hole punching |

### Windows Firewall

```powershell
# Allow Nebula (run as Administrator)
netsh advfirewall firewall add rule name="Nebula VPN" dir=in action=allow protocol=UDP localport=4242
```

### macOS

```bash
# Nebula requires TUN device permissions
# The first run may prompt for System Extension approval
```

### Linux

```bash
# Allow UDP port 4242
sudo ufw allow 4242/udp

# Or with iptables
sudo iptables -A INPUT -p udp --dport 4242 -j ACCEPT
```

## Troubleshooting

### Connection Issues

1. **Check lighthouse is reachable**: `ping <lighthouse-public-ip>`
2. **Verify port is open**: `nc -vzu <lighthouse-ip> 4242`
3. **Check logs**: Run nebula with `-l debug` flag
4. **Symmetric NAT**: If behind carrier-grade NAT, enable relay mode

### Certificate Errors

- Ensure `ca.crt` matches between all nodes
- Check certificate hasn't expired (default: 1 year)
- Verify IP addresses don't conflict

### Performance

- Nebula adds ~5-15ms latency overhead
- Use MTU 1300 to avoid fragmentation
- Enable `punchy` for faster NAT traversal

## License

Nebula is licensed under the MIT License. See `src/LICENSE` for details.

## Resources

- [Nebula GitHub](https://github.com/slackhq/nebula)
- [Nebula Documentation](https://nebula.defined.net/docs/)
- [Liberty Recomp Online Multiplayer Guide](../../docs/ONLINE_MULTIPLAYER.md)
