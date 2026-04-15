#!/usr/bin/env python3
"""
extract_trophy_trp.py — Extract PNG trophy icons from a PS3/PS4 TROPHY.TRP

Usage:
    python3 extract_trophy_trp.py <TROPHY.TRP> [--out <output_dir>]

Default output dir: res/ps4/trophy00/  (replaces placeholder PNGs)

The TROPHY.TRP for GTA IV (USA, BLUS30127) has NP Comm ID: NPWR00263_00
To obtain it:
  - Run GTA IV in RPCS3 with a PSN account connected; RPCS3 auto-downloads
    and installs it to: ~/Library/Application Support/rpcs3/dev_hdd0/home/00000001/trophy/NPWR00263_00/
  - Or locate the installed TROPHY.TRP at:
    PS3_GAME/TROPDIR/NPWR00263_00/TROPHY.TRP  (on disc copies that include it)
"""

import struct, os, sys, argparse, shutil

TRP_MAGIC = 0xDCA24D00
# Header is 96 bytes on PS4 (version 3). PS3 / version 1 also uses 96 in practice —
# orbis-pub-cmd and OpenOrbis create-trp both emit a 96-byte header.
HEADER_SIZE = 96
ENTRY_SIZE  = 64  # fixed entry record size (see header entrySize field @ +0x14)

def extract_trp(trp_path: str, out_dir: str) -> list[tuple[str, int]]:
    with open(trp_path, 'rb') as f:
        data = f.read()

    magic = struct.unpack_from('>I', data, 0)[0]
    if magic != TRP_MAGIC:
        raise ValueError(f"Not a TROPHY.TRP (magic={magic:#010x})")

    version    = struct.unpack_from('>I', data, 4)[0]
    file_size  = struct.unpack_from('>Q', data, 8)[0]
    num_files  = struct.unpack_from('>I', data, 16)[0]
    entry_size = struct.unpack_from('>I', data, 20)[0]

    print(f"TRP v{version}  |  {num_files} entries  |  {len(data)//1024} KB")

    os.makedirs(out_dir, exist_ok=True)
    extracted = []

    for i in range(num_files):
        off = HEADER_SIZE + i * entry_size
        name_raw     = data[off : off + 32]
        name         = name_raw.split(b'\x00')[0].decode('ascii', errors='replace')
        entry_offset = struct.unpack_from('>Q', data, off + 32)[0]
        entry_len    = struct.unpack_from('>Q', data, off + 40)[0]

        out_path = os.path.join(out_dir, name)
        with open(out_path, 'wb') as f:
            f.write(data[entry_offset : entry_offset + entry_len])

        extracted.append((name, entry_len))
        print(f"  {name:30s}  {entry_len:>8} bytes  → {out_path}")

    return extracted

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    default_out = os.path.join(script_dir, 'trophy00')

    parser = argparse.ArgumentParser(description='Extract TROPHY.TRP contents')
    parser.add_argument('trp', help='Path to TROPHY.TRP')
    parser.add_argument('--out', default=default_out,
                        help=f'Output directory (default: {default_out})')
    args = parser.parse_args()

    if not os.path.isfile(args.trp):
        print(f"ERROR: File not found: {args.trp}", file=sys.stderr)
        sys.exit(1)

    files = extract_trp(args.trp, args.out)
    pngs = [n for n, _ in files if n.endswith('.PNG')]
    print(f"\nDone. Extracted {len(pngs)} PNG files to: {args.out}")
    print("These will be used by create-trp when building the PS4 PKG.")

if __name__ == '__main__':
    main()
