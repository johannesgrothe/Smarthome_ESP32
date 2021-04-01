import os
import re
import argparse
import json
from datetime import datetime

file_name = "static_config.h"
file_path = os.path.join("src", file_name)


def parse_args():
    """Parses the arguments given to the script"""
    parser = argparse.ArgumentParser()
    parser.add_argument('--config_path', type=str, help='Path of the json the config should be generated from')
    parser.add_argument('--config', type=str, help='The complete config as string')
    return parser.parse_args()


def main(config: dict):
    """Generates the config"""

    print("Generating '{}' file to define static config...".format(file_name))

    data_template: [str]
    start_lines: [str] = []
    end_lines: [str] = []
    out_lines: [str] = []

    gadget_repeat_block: [str] = []

    with open("generate_static_config_template.h", "r") as file:
        data_template = file.readlines()

    block_status = 0
    for line in data_template:
        if block_status == 0:
            if line.strip() == "/*<gadget_block_start>*/":
                block_status = 1
            else:
                start_lines.append(line)
        elif block_status == 1:
            if line.strip() == "/*<gadget_block_end>*/":
                block_status = 2
            else:
                gadget_repeat_block.append(line)
        else:
            end_lines.append(line)

    out_lines = start_lines

    for gadget_data in config["gadgets"]:
        for line in gadget_repeat_block:
            replaced_line: str
            if line.strip().startswith("/*<pin_block>*/"):
                for port_name in gadget_data["ports"]:
                    replaced_line = line\
                        .replace("/*<pin_block>*/", "")\
                        .replace("/*<pin_index>*/", port_name[4:]) \
                        .replace("/*<pin_value>*/", str(gadget_data["ports"][port_name]))

                    out_lines.append(replaced_line)
            elif line.strip().startswith("/*<remote_block>*/"):
                for remote_type in gadget_data["remotes"]:
                    remote_index: int
                    if remote_type == "gadget":
                        remote_index = 0
                    elif remote_type == "code":
                        remote_index = 1
                    elif remote_type == "event":
                        remote_index = 2
                    else:
                        continue
                    replaced_line = line\
                        .replace("/*<remote_block>*/", "") \
                        .replace("/*<remote_index>*/", str(remote_index)) \
                        .replace("/*<remote_value>*/", "true" if gadget_data["remotes"][remote_type] else "false")

                    out_lines.append(replaced_line)
            else:
                replaced_line = line\
                    .replace("/*<gadget_type>*/", str(gadget_data["type"]))\
                    .replace("/*<gadget_name>*/", gadget_data["name"])\
                    .replace("/*<gadget_json>*/", json.dumps(gadget_data["config"]).replace('"', '\\"'))\
                    .replace("/*<code_json>*/", json.dumps(gadget_data["codes"]).replace('"', '\\"'))
                out_lines.append(replaced_line)

    out_lines += end_lines

    print("Saving...")

    with open(file_path, "w") as out_file:
        out_file.writelines(out_lines)

    print("Done.")


if __name__ == "__main__":
    args = parse_args()

    if args.config:
        try:
            loaded_config = json.loads(args.config)
            main(loaded_config)
        except ValueError:
            print("Given JSON could not be decoded")

    elif args.config_path:
        with open(args.config_path, "r") as file:
            try:
                loaded_config = json.load(file)
                print(f"Generating config from '{args.config_path}'")
                main(loaded_config)
            except ValueError:
                print("JSON from file path could not be decoded")
    else:
        print("No config name passed, deleting file")
        try:
            os.remove(file_path)
        except FileNotFoundError:
            pass
