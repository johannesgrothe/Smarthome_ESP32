import os
from datetime import date

file_name = "flash_info.h"

print("Generating '{}' file to define flashing info...".format(file_name))

data_str = "#pragma once\n"

flash_time = '#define SW_FLASH_TIME "{}"\n'.format(date.today())
data_str += flash_time

git_commit = '#define SW_GIT_COMMIT "{}"\n'.format(os.popen("git rev-parse HEAD").read().strip("\n"))
data_str += git_commit

git_branch = '#define SW_GIT_BRANCH "{}"\n'.format(os.popen("git for-each-ref --format='%(upstream:short)' $(git symbolic-ref -q HEAD)").read().strip("\n"))
data_str += git_branch

print("Saving...")

out_file = open(file_name, "w")
out_file.write(data_str)


print("Done.")

