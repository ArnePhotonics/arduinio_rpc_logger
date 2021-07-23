import os
from subprocess import call

clang_paths = ('C:\\Program Files\\LLVM\\bin\\clang-format.exe', 'C:\\Program Files (x86)\\LLVM\\bin\\clang-format.exe', '/usr/bin/clang-format')
for path in clang_paths:
        if os.path.isfile(path):
                clang_format_path = path
                break


exclude_dirs = [
	"mcu/libs/RPC-ChannelCodec",
    "Arduino-Makefile",
    "qt",
	"mcu/modules/rpc/src/client/generated_app",
    "mcu/modules/rpc/src/server/generated_app",
	"mcu/modules/rpc/include/rpc_transmission/client",
	"mcu/modules/rpc/include/rpc_transmission/server/generated_general",
]

allowed_file_suffixes = [
	".c",
	".h",
	".hpp",
	".cpp",
    ".ino"
]

def isExcluded(filenameToTest):
	filenameToTest = filenameToTest.replace(os.sep, "/") 
	extension = os.path.splitext(filenameToTest)[1].lower()
	extensionMatches = False
	if extension in allowed_file_suffixes:
		pass
	else:
		return True

	for exclude_dir in exclude_dirs:
		if filenameToTest.startswith( ""+exclude_dir ):
			#print(filenameToTest)
			return True
			
	return False

for subdir, dirs, files in os.walk("mcu/"):
    for file in files:
        #print os.path.join(subdir, file)
        filepath = subdir + os.sep + file
        if isExcluded(filepath):
                pass
        else:
                print (filepath)
                #print(clang_format_path)
                call([clang_format_path, "-i",filepath])
			
