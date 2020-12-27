import os
from datetime import datetime

file_name = "flash_info.h"

print("Generating '{}' file to define flashing info...".format(file_name))

data_str = "#pragma once\n// THIS IS AN AUTO_GENERATED FILE.\n// TOUCH IT IF YOU WANT, THE GENERATOR DOESNT CARE.\n\
// BUT HE HAS NO MERCY EITHER.\n"


flash_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
flash_time_str = '#define SW_FLASH_TIME "{}"\n'.format(flash_time)
data_str += flash_time_str

git_commit = os.popen("git rev-parse HEAD").read().strip("\n")
git_commit_str = '#define SW_GIT_COMMIT "{}"\n'.format(git_commit)
data_str += git_commit_str

git_branch = os.popen("git for-each-ref --format='%(upstream:short)' $(git symbolic-ref -q HEAD)").read().strip("\n")
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
