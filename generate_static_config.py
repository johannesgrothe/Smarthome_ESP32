import os
import re
import argparse
import json
from datetime import datetime

file_name = "static_config.h"
file_path = os.path.join("src", "storage", file_name)


def parse_args():
    """Parses the arguments given to the script"""
    parser = argparse.ArgumentParser()
    parser.add_argument('--config_path', type=str, help='Path of the json the config should be generated from')
    parser.add_argument('--config', type=str, help='The complete config as string')
    return parser.parse_args()


def generate_string_config(config: dict):
    print("Generating '{}' file to define static string config...".format(file_name))

    data_template: [str]
    out_lines: [str] = []

    config_str = json.dumps(config).replace('"', r'\"')

    with open("static_string_config_template.h", "r") as file_h:
        data_template = file_h.readlines()

    for line in data_template:
        buf_line = line.replace("/*<config_str>*/", config_str)
        out_lines.append(buf_line)

    print("Saving...")

    with open(file_path, "w") as out_file:
        out_file.writelines(out_lines)

    print("Done.")


if __name__ == "__main__":
    args = parse_args()

    if args.config:
        try:
            loaded_config = json.loads(args.config)
            generate_string_config(loaded_config)
        except ValueError:
            print("Given JSON could not be decoded")

    elif args.config_path:
        with open(args.config_path, "r") as file:
            try:
                loaded_config = json.load(file)
                print(f"Generating config from '{args.config_path}'")
                generate_string_config(loaded_config)
            except ValueError:
                print("JSON from file path could not be decoded")
    else:
        print("No config name passed, deleting file")
        try:
            os.remove(file_path)
        except FileNotFoundError:
            pass
