#!/usr/bin/env python3
"""
XEX Constructor Table Finder
Analyzes Xbox 360 XEX files to locate C++ constructor tables (.ctors section)
"""

import struct
import sys
from pathlib import Path

# XEX2 Header structure
XEX2_MAGIC = b'XEX2'

# Optional header IDs we care about
XEX_HEADER_ENTRY_POINT = 0x00010100
XEX_HEADER_BASE_ADDRESS = 0x00010201
XEX_HEADER_IMPORT_LIBRARIES = 0x000103FF
XEX_HEADER_EXECUTION_INFO = 0x00040006
XEX_HEADER_TLS_INFO = 0x00020104
XEX_HEADER_DEFAULT_STACK_SIZE = 0x00020200
XEX_HEADER_PE_MODULE_NAME = 0x000183FF

def read_u32_be(data, offset):
    """Read big-endian 32-bit unsigned int"""
    return struct.unpack('>I', data[offset:offset+4])[0]

def read_u16_be(data, offset):
    """Read big-endian 16-bit unsigned int"""
    return struct.unpack('>H', data[offset:offset+2])[0]

def parse_xex_header(data):
    """Parse XEX2 header and optional headers"""
    if data[:4] != XEX2_MAGIC:
        raise ValueError("Not a valid XEX2 file")
    
    # XEX2 Header structure (big-endian)
    module_flags = read_u32_be(data, 0x04)
    pe_data_offset = read_u32_be(data, 0x08)
    reserved = read_u32_be(data, 0x0C)
    security_info_offset = read_u32_be(data, 0x10)
    optional_header_count = read_u32_be(data, 0x14)
    
    print(f"=== XEX2 Header ===")
    print(f"Module Flags: 0x{module_flags:08X}")
    print(f"PE Data Offset: 0x{pe_data_offset:08X}")
    print(f"Security Info Offset: 0x{security_info_offset:08X}")
    print(f"Optional Header Count: {optional_header_count}")
    print()
    
    # Parse optional headers (start at offset 0x18)
    headers = {}
    offset = 0x18
    
    for i in range(optional_header_count):
        header_id = read_u32_be(data, offset)
        header_data = read_u32_be(data, offset + 4)
        
        # Check if it's inline data or an offset
        # If lower byte of header_id is 0xFF, it's an offset to more data
        # If lower byte is 0x00-0x01, it's inline data
        key_size = header_id & 0xFF
        key_id = header_id & 0xFFFFFF00
        
        if key_size == 0xFF:
            # Offset to data block
            headers[key_id] = {'type': 'offset', 'offset': header_data, 'raw_id': header_id}
        elif key_size == 0x00:
            # No additional data
            headers[key_id] = {'type': 'none', 'value': header_data, 'raw_id': header_id}
        elif key_size == 0x01:
            # 4 bytes inline
            headers[key_id] = {'type': 'inline', 'value': header_data, 'raw_id': header_id}
        else:
            # key_size * 4 bytes of data at offset
            headers[key_id] = {'type': 'sized', 'size': key_size * 4, 'offset': header_data, 'raw_id': header_id}
        
        offset += 8
    
    return headers, pe_data_offset, security_info_offset

def find_ctors_in_pe(data, pe_offset, base_address):
    """Search for constructor tables in the PE data"""
    print(f"\n=== Searching for Constructor Tables ===")
    print(f"PE Data at offset 0x{pe_offset:08X}")
    print(f"Base Address: 0x{base_address:08X}")
    
    # The PE data in XEX is usually compressed/encrypted
    # Let's look at the raw bytes around the known addresses
    
    # Known addresses from the code:
    # Array 1: 0x820214FC to 0x82021508 (guest address)
    # Array 2: 0x82020010 to 0x820214F8 (guest address)
    
    # These are virtual addresses relative to base 0x82000000
    # So offsets would be:
    # Array 1: 0x214FC to 0x21508
    # Array 2: 0x10 to 0x214F8
    
    # Let's search for patterns that look like constructor tables
    # Constructor tables typically contain:
    # - A series of function pointers (addresses starting with 0x82)
    # - Possibly terminated by 0 or -1
    
    print(f"\n=== Scanning for function pointer arrays ===")
    
    # Search through the file for sequences of big-endian addresses
    # that look like 0x82XXXXXX (valid code addresses)
    
    potential_tables = []
    
    # Scan in 4-byte chunks
    for scan_offset in range(pe_offset, min(len(data) - 16, pe_offset + 0x100000), 4):
        # Read 4 consecutive potential pointers
        vals = []
        valid_count = 0
        for j in range(4):
            val = read_u32_be(data, scan_offset + j*4)
            vals.append(val)
            # Check if it looks like a code address
            if 0x82000000 <= val <= 0x82FFFFFF:
                valid_count += 1
        
        # If we found 4 consecutive valid-looking pointers, this might be a table
        if valid_count >= 3:
            # Check if we haven't already recorded this area
            already_found = False
            for pt in potential_tables:
                if abs(pt['offset'] - scan_offset) < 32:
                    already_found = True
                    break
            
            if not already_found:
                # Count how many consecutive valid pointers
                count = 0
                for check_off in range(scan_offset, min(len(data) - 4, scan_offset + 8000), 4):
                    val = read_u32_be(data, check_off)
                    if 0x82000000 <= val <= 0x82FFFFFF or val == 0 or val == 0xFFFFFFFF:
                        count += 1
                        if val == 0 or val == 0xFFFFFFFF:
                            break
                    else:
                        break
                
                if count >= 4:
                    potential_tables.append({
                        'offset': scan_offset,
                        'count': count,
                        'first_values': vals
                    })
    
    print(f"Found {len(potential_tables)} potential function pointer tables:")
    for i, pt in enumerate(potential_tables[:20]):  # Show first 20
        print(f"  [{i}] Offset 0x{pt['offset']:08X}, ~{pt['count']} entries")
        print(f"      First values: {' '.join(f'0x{v:08X}' for v in pt['first_values'])}")
    
    if len(potential_tables) > 20:
        print(f"  ... and {len(potential_tables) - 20} more")
    
    return potential_tables

def search_for_specific_addresses(data):
    """Search for the specific addresses we're looking for"""
    print(f"\n=== Searching for Known Addresses ===")
    
    # Search for the entry point address pattern
    target_addresses = [
        0x829A0860,  # Entry point
        0x829A7DC8,  # C++ constructor executor
        0x820214FC,  # Array 1 start (from code)
        0x82020010,  # Array 2 start (from code)
    ]
    
    for target in target_addresses:
        target_bytes_be = struct.pack('>I', target)
        target_bytes_le = struct.pack('<I', target)
        
        # Search big-endian
        pos = data.find(target_bytes_be)
        if pos != -1:
            print(f"Found 0x{target:08X} (BE) at file offset 0x{pos:08X}")
        
        # Search little-endian
        pos = data.find(target_bytes_le)
        if pos != -1:
            print(f"Found 0x{target:08X} (LE) at file offset 0x{pos:08X}")

def main():
    if len(sys.argv) < 2:
        print("Usage: xex_ctor_finder.py <path_to_xex>")
        sys.exit(1)
    
    xex_path = Path(sys.argv[1])
    if not xex_path.exists():
        print(f"Error: File not found: {xex_path}")
        sys.exit(1)
    
    print(f"Analyzing: {xex_path}")
    print(f"File size: {xex_path.stat().st_size} bytes")
    print()
    
    with open(xex_path, 'rb') as f:
        data = f.read()
    
    # Parse headers
    headers, pe_offset, security_offset = parse_xex_header(data)
    
    # Extract key info
    entry_point = None
    base_address = 0x82000000  # Default
    
    print("=== Optional Headers ===")
    for key_id, info in headers.items():
        if key_id == XEX_HEADER_ENTRY_POINT:
            entry_point = info.get('value')
            print(f"Entry Point: 0x{entry_point:08X}")
        elif key_id == XEX_HEADER_BASE_ADDRESS:
            base_address = info.get('value', 0x82000000)
            print(f"Base Address: 0x{base_address:08X}")
        elif key_id == XEX_HEADER_TLS_INFO:
            print(f"TLS Info: offset=0x{info.get('offset', 0):08X}")
    
    # Search for specific addresses
    search_for_specific_addresses(data)
    
    # Look for constructor tables
    tables = find_ctors_in_pe(data, pe_offset, base_address)
    
    # Detailed analysis of most promising tables
    if tables:
        print(f"\n=== Detailed Analysis of Top Candidates ===")
        for i, pt in enumerate(tables[:5]):
            print(f"\nTable {i} at file offset 0x{pt['offset']:08X}:")
            # Read more values
            values = []
            for j in range(min(pt['count'], 20)):
                val = read_u32_be(data, pt['offset'] + j*4)
                values.append(val)
            
            print(f"  Values: ")
            for j, v in enumerate(values):
                if v == 0:
                    print(f"    [{j}] 0x{v:08X} (NULL)")
                elif v == 0xFFFFFFFF:
                    print(f"    [{j}] 0x{v:08X} (-1/terminator)")
                elif 0x82000000 <= v <= 0x82FFFFFF:
                    print(f"    [{j}] 0x{v:08X} (code ptr)")
                else:
                    # Try to interpret as ASCII
                    try:
                        ascii_val = struct.pack('>I', v).decode('ascii', errors='replace')
                        print(f"    [{j}] 0x{v:08X} (data: '{ascii_val}')")
                    except:
                        print(f"    [{j}] 0x{v:08X} (unknown)")

if __name__ == '__main__':
    main()
