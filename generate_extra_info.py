import os
import re
from datetime import datetime

file_name = "flash_info.h"

print("Generating '{}' file to define flashing info...".format(file_name))

data_str = "#pragma once\n// THIS IS AN AUTO_GENERATED FILE.\n\
// TOUCH IT IF YOU WANT, BUT BEWARE ANY CHANGES COULD BE OVERWRITTEN AT ANY TIME IN THE FUTURE.\n"


flash_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
flash_time_str = '#define SW_FLASH_TIME "{}"\n'.format(flash_time)
data_str += flash_time_str

git_commit = os.popen("git rev-parse HEAD").read().strip("\n")
git_commit_str = '#define SW_GIT_COMMIT "{}"\n'.format(git_commit)
data_str += git_commit_str


selected_branch = [x.strip('*').strip() for x in os.popen("git branch").read().split("\n") if x.startswith('*')]
git_branch = selected_branch[0] if len(selected_branch) == 1 else "None"
git_branch_str = '#define SW_GIT_BRANCH "{}"\n'.format(git_branch)
data_str += git_branch_str

print("Data:")
print("   Time: {}".format(flash_time))
print("   Commit: {}".format(git_commit))
print("   Branch: {}".format(git_branch))

print("Saving...")

out_file = open(file_name, "w")
out_file.write(data_str)

print("Done.")
