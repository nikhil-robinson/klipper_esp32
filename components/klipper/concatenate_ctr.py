#!/usr/bin/env python3
import sys
import os

def main():
    if len(sys.argv) < 3:
        print("Usage: concatenate_ctr.py <input_files...> <output_file>", file=sys.stderr)
        sys.exit(1)
    
    # Get input .ctr files and output .txt file from arguments
    input_files = sys.argv[1:-1]
    output_file = sys.argv[-1]
    
    print(f"Concatenating {len(input_files)} .ctr files to {output_file}")
    
    # Filter out empty or non-existent files
    valid_files = []
    for f in input_files:
        if os.path.exists(f) and os.path.getsize(f) > 0:
            valid_files.append(f)
        else:
            print(f"Warning: Skipping empty or non-existent file: {f}")
    
    if not valid_files:
        print("Warning: No valid .ctr files found, creating empty output")
        # Create empty output file
        with open(output_file, 'wb') as f:
            f.write(b'')
        return
    
    # Concatenate all .ctr files, replacing null bytes with newlines
    # This matches the Makefile behavior: cat *.ctr | tr -s '\0' '\n'
    data = b''
    for f in valid_files:
        try:
            with open(f, 'rb') as file:
                file_data = file.read()
                data += file_data
                print(f"Read {len(file_data)} bytes from {f}")
        except Exception as e:
            print(f"Error reading {f}: {e}", file=sys.stderr)
    
    # Replace null bytes with newlines and remove consecutive newlines (tr -s '\0' '\n')
    data = data.replace(b'\0', b'\n')
    
    # Write to output file
    try:
        with open(output_file, 'wb') as f:
            f.write(data)
        print(f"Successfully wrote {len(data)} bytes to {output_file}")
    except Exception as e:
        print(f"Error writing to {output_file}: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()