import os
import subprocess
Import("env")

print("Current CLI targets", COMMAND_LINE_TARGETS)
print("Current Build targets", BUILD_TARGETS)
print("=== Running extra_script.py ===")


# generate the protobuff files before compilation
print("=== Generating protobuff files ===")
script_path = os.path.join(env['PROJECT_DIR'], "utils", "nanoPb_generate.bat")
subprocess.call(script_path)
print("=== Done generating protobuff files ===")
