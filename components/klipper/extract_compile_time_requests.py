#!/usr/bin/env python3
"""
Script to extract compile time requests from object files and generate compile_time_request.c

This script replicates the Makefile behavior:
1. Extract .compile_time_request sections from all object files
2. Concatenate them and convert null bytes to newlines
3. Run buildcommands.py to generate the final C file

Usage: extract_compile_time_requests.py <build_dir> <component_dir> <component_target> <tools_string>
"""

import sys
import os
import subprocess
import glob
import argparse


def find_object_files(build_dir, component_target):
    """Find all object files for the given component."""
    patterns = [
        os.path.join(build_dir, 'CMakeFiles', component_target + '.dir', '**', '*.c.obj'),
        os.path.join(build_dir, 'CMakeFiles', component_target + '.dir', '**', '*.obj'),
        os.path.join(build_dir, 'CMakeFiles', component_target + '.dir', '**', '*.o'),
    ]
    
    obj_files = []
    for pattern in patterns:
        found = glob.glob(pattern, recursive=True)
        obj_files.extend(found)
    
    # Remove duplicates and ensure files exist
    unique_files = []
    seen = set()
    for f in obj_files:
        if f not in seen and os.path.exists(f):
            unique_files.append(f)
            seen.add(f)
    
    return unique_files


def extract_ctr_sections(obj_files, objcopy_cmd):
    """Extract .compile_time_request sections from object files."""
    ctr_data = b''
    
    for obj_file in obj_files:
        ctr_file = obj_file + '.ctr'
        try:
            # Extract the section using objcopy
            result = subprocess.run([
                objcopy_cmd, '-j', '.compile_time_request', '-O', 'binary',
                obj_file, ctr_file
            ], capture_output=True, text=True)
            
            # Read the extracted data if the file exists and has content
            if os.path.exists(ctr_file):
                if os.path.getsize(ctr_file) > 0:
                    with open(ctr_file, 'rb') as f:
                        data = f.read()
                        ctr_data += data
                        print(f'Extracted {len(data)} bytes from {os.path.basename(obj_file)}')
                # Clean up temporary file
                os.remove(ctr_file)
        except Exception as e:
            print(f'Error processing {obj_file}: {e}')
    
    return ctr_data


def process_ctr_data(ctr_data):
    """Process the raw CTR data like the Makefile: tr -s '\0' '\n'"""
    if not ctr_data:
        return b''
    
    # Convert null bytes to newlines
    ctr_text = ctr_data.replace(b'\0', b'\n')
    
    # Remove empty lines and strip (similar to tr -s behavior)
    try:
        lines = ctr_text.decode('utf-8', errors='ignore').strip().split('\n')
        lines = [line.strip() for line in lines if line.strip()]
        ctr_text = '\n'.join(lines).encode('utf-8')
    except Exception as e:
        print(f'Warning: Error processing CTR text: {e}')
        # Fallback to simple replacement
        ctr_text = ctr_data.replace(b'\0', b'\n')
    
    return ctr_text


def run_buildcommands(python_exe, script_path, tools_string, txt_file, output_file, dict_file):
    """Run the buildcommands.py script to generate the final C file."""
    
    # Fix the sys.path issue for msgproto import
    script_dir = os.path.dirname(script_path)
    klippy_dir = os.path.join(script_dir, '..', 'klippy')
    klippy_dir = os.path.abspath(klippy_dir)
    
    # Create a wrapper script that sets up the path correctly
    wrapper_script = f'''
import sys
import os
sys.path.insert(0, r"{klippy_dir}")
os.chdir(r"{script_dir}")
exec(open(r"{script_path}").read())
'''
    
    wrapper_file = output_file + '.wrapper.py'
    with open(wrapper_file, 'w') as f:
        f.write(wrapper_script)
    
    cmd = [
        python_exe, wrapper_file,
        '-d', dict_file,
        '-t', tools_string,
        txt_file,
        output_file
    ]
    
    print(f'Running buildcommands.py: {" ".join(cmd)}')
    
    try:
        result = subprocess.run(cmd, check=True, capture_output=True, text=True)
        print(f'Successfully generated {output_file}')
        if result.stdout:
            print(f'buildcommands.py output: {result.stdout}')
        
        # Clean up wrapper file
        if os.path.exists(wrapper_file):
            os.remove(wrapper_file)
        
        return True
    except subprocess.CalledProcessError as e:
        print(f'ERROR: buildcommands.py failed with exit code {e.returncode}')
        print(f'Command: {" ".join(cmd)}')
        if e.stdout:
            print(f'stdout: {e.stdout}')
        if e.stderr:
            print(f'stderr: {e.stderr}')
        
        # Clean up wrapper file
        if os.path.exists(wrapper_file):
            os.remove(wrapper_file)
        
        # Don't create fallback - let the build fail
        raise RuntimeError(f"buildcommands.py failed: {e.stderr}")
    except Exception as e:
        print(f'Unexpected error running buildcommands.py: {e}')
        
        # Clean up wrapper file
        if os.path.exists(wrapper_file):
            os.remove(wrapper_file)
        
        # Don't create fallback - let the build fail  
        raise RuntimeError(f"Failed to run buildcommands.py: {e}")


def create_fallback_file(output_file):
    """Create a minimal compile_time_request.c file as fallback."""
    # This function is now unused since we don't want fallbacks
    pass


def main():
    parser = argparse.ArgumentParser(description='Extract compile time requests and generate C file')
    parser.add_argument('build_dir', help='Build directory containing object files')
    parser.add_argument('component_dir', help='Component source directory')
    parser.add_argument('component_target', help='Component target name')
    parser.add_argument('tools_string', help='Semicolon-separated tool paths')
    parser.add_argument('--python', default=sys.executable, help='Python executable to use')
    parser.add_argument('--objcopy', help='objcopy command to use')
    
    args = parser.parse_args()
    
    # Extract objcopy from tools string if not provided
    if not args.objcopy:
        tools = args.tools_string.split(';')
        if len(tools) >= 4:  # CC;AS;LD;OBJCOPY;OBJDUMP;STRIP
            args.objcopy = tools[3]
        else:
            print('Error: objcopy not found in tools string and not provided separately')
            return 1
    
    print(f'Extracting compile time requests from {args.build_dir}')
    print(f'Component: {args.component_target}')
    print(f'Using objcopy: {args.objcopy}')
    
    # Find object files
    obj_files = find_object_files(args.build_dir, args.component_target)
    print(f'Found {len(obj_files)} object files:')
    for obj in obj_files:
        print(f'  {obj}')
    
    if not obj_files:
        print('Warning: No object files found')
    
    # Extract CTR sections
    ctr_data = extract_ctr_sections(obj_files, args.objcopy)
    
    # Process the data
    ctr_text = process_ctr_data(ctr_data)
    
    # Write compile_time_request.txt
    txt_file = os.path.join(args.build_dir, 'compile_time_request.txt')
    with open(txt_file, 'wb') as f:
        f.write(ctr_text)
    
    print(f'Generated {txt_file} with {len(ctr_text)} bytes')
    if ctr_text and len(ctr_text) < 500:
        print(f'Content preview: {ctr_text.decode("utf-8", errors="ignore")}')
    
    # Prepare paths for buildcommands.py
    script_path = os.path.join(args.component_dir, 'klipper', 'scripts', 'buildcommands.py')
    output_file = os.path.join(args.build_dir, 'compile_time_request.c')
    dict_file = os.path.join(args.build_dir, 'klipper.dict')
    
    # Check if buildcommands.py exists
    if not os.path.exists(script_path):
        raise RuntimeError(f'buildcommands.py not found at {script_path}')
    
    # Run buildcommands.py - this will raise an exception if it fails
    run_buildcommands(args.python, script_path, args.tools_string, 
                     txt_file, output_file, dict_file)
    
    print(f'Successfully generated {output_file}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
