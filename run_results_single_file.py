import os
import glob
import shutil

def get_latest_ade_file(base_path):
    ade_files = glob.glob(os.path.join(base_path, "*.ade"))
    if not ade_files:
        print("No .ade files found in the current directory.")
        return None

    latest_file = max(ade_files, key=os.path.basename)
    return latest_file

# Set the base directory to the current directory
ade_base_dir = os.getcwd()

# Get the latest .ade file
latest_ade_file = get_latest_ade_file(ade_base_dir)

# Check if the user wants to use the latest file or provide another one
if latest_ade_file:
    print(f"The latest .ade file found is: {latest_ade_file}")
    use_latest = input("Do you want to use the latest file? (yes/no): ").strip().lower()

    if use_latest == 'no':
        chosen_file = input("Please enter the name of the .ade file you want to use: ").strip()
        chosen_file_path = os.path.join(ade_base_dir, chosen_file)
        if os.path.exists(chosen_file_path):
            ade_file_to_use = chosen_file_path
        else:
            print(f"The file {chosen_file_path} does not exist. Exiting.")
            exit(1)
    else:
        ade_file_to_use = latest_ade_file
else:
    print("No .ade files found.")
    exit(1)

# Hardcoded values for numDetectors and channels
numDetectors = 23
channel_MCP_1 = 28
channel_X1 = 24
channel_X2 = 25
channel_Y1 = 26
channel_Y2 = 27

# Print the hardcoded values
print(f"Using hardcoded values:")
print(f"Number of detectors: {numDetectors}")
print(f"Channel MCP_1: {channel_MCP_1}")
print(f"Channel X1: {channel_X1}")
print(f"Channel X2: {channel_X2}")
print(f"Channel Y1: {channel_Y1}")
print(f"Channel Y2: {channel_Y2}")

# Generate conversion command
conversion_command = f"~/abcd/convert/./ade2root {ade_file_to_use} ."
print(f"Executing: {conversion_command}")
os.system(conversion_command)

# Prepare directory for raw_root and bychannels
raw_root_dir = os.path.join(ade_base_dir, "raw_root/")
bychannels_dir = os.path.join(ade_base_dir, "bychannels/")
os.makedirs(raw_root_dir, exist_ok=True)
os.makedirs(bychannels_dir, exist_ok=True)

# Find .root files after conversion
root_files = glob.glob(os.path.join(ade_base_dir, "*.root"))
if root_files:
    root_file = root_files[0]
    print(f"Found .root file: {root_file}")
else:
    print(f"No .root files found in {ade_base_dir}. Exiting.")
    exit(1)

# Move the .root file to raw_root directory
destination = os.path.join(raw_root_dir, os.path.basename(root_file))
if os.path.exists(destination):
    os.remove(destination)
shutil.move(root_file, destination)
print(f"Moved {root_file} to {raw_root_dir}")

# Generate and execute ROOT command
output_filename = os.path.basename(root_file).replace("events", "by_channels")
output_file = os.path.join(bychannels_dir, output_filename)
command = f'root -l -q \'Tree_generation_vectorized.C("{destination}", "{output_file}", {numDetectors}, {channel_MCP_1}, {channel_X1}, {channel_X2}, {channel_Y1}, {channel_Y2})\''
print(f"Executing command: {command}")
os.system(command)

# Check for .root files in bychannels_dir
bychannel_files = glob.glob(os.path.join(bychannels_dir, "*.root"))
if bychannel_files:
    bychannel_file = bychannel_files[0]
    print(f"Found by_channels .root file: {bychannel_file}")
else:
    print(f"No by_channels .root files found in {bychannels_dir}. Exiting.")
    exit(1)

# Plot the results using ToF_analysis_pos_sensitive_vectorized.C
plot_command = f'root -l \'ToF_analysis_pos_sensitive_vectorized.C("{bychannel_file}")\''
print(f"Executing plot command: {plot_command}")
os.system(plot_command)
