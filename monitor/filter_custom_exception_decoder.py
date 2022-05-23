import os
import re
import sys
from typing import Optional

from platformio.compat import path_to_unicode
from platformio.exception import PlatformioException
from platformio.project.helpers import load_project_ide_data
from platformio.public import DeviceMonitorFilterBase


class ExceptionDecoder(DeviceMonitorFilterBase):
    NAME = "custom_exception_decoder"
    _buffer: str
    firmware_path: Optional[str]
    addr2line_path: Optional[str]
    enabled: bool

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        print(f"{self.__class__.__name__}-filter is loaded")
        self._buffer = ""

        self.firmware_path = None
        self.addr2line_path = None
        self.enabled = self.setup_paths()

        print(self.firmware_path)
        print(self.addr2line_path)

        if self.config.get("env:" + self.environment, "build_type") != "debug":
            print(
                """
    Please build project in debug configuration to get more details about an exception.
    See https://docs.platformio.org/page/projectconf/build_configurations.html
    
    """
            )

    def setup_paths(self):
        self.project_dir = path_to_unicode(os.path.abspath(self.project_dir))
        try:
            data = load_project_ide_data(self.project_dir, self.environment)
            self.firmware_path = data["prog_path"]
            if not os.path.isfile(self.firmware_path):
                sys.stderr.write(
                    "%s: firmware at %s does not exist, rebuild the project?\n"
                    % (self.__class__.__name__, self.firmware_path)
                )
                return False

            cc_path = data.get("cc_path", "")
            if "-gcc" in cc_path:
                path = cc_path.replace("-gcc", "-addr2line")
                if os.path.isfile(path):
                    self.addr2line_path = path
                    return True
        except PlatformioException as e:
            sys.stderr.write(
                "%s: disabling, exception while looking for addr2line: %s\n"
                % (self.__class__.__name__, e)
            )
            return False
        sys.stderr.write(
            "%s: disabling, failed to find addr2line.\n" % self.__class__.__name__
        )
        return False

    def extract_backtrace_info(self, trace: str) -> str:
        expr = r"0x[0-9a-f]{8}:0x[0-9a-f]{8}"
        addresses = " ".join(re.findall(expr, trace))
        command = f"{self.addr2line_path} -e {self.firmware_path} -a {addresses} -f -C"
        addr2line_result = os.popen(command).read().split("\n")
        buf_data = ""
        for i in range(len(addr2line_result)-1):
            if i % 3 == 0:
                line = f"{addr2line_result[i]} -> {addr2line_result[i + 1]}:\n   {addr2line_result[i + 2]}\n\n"
                buf_data += line

        return trace + "\n" + buf_data + "\n\n"

    def rx(self, text):
        out_data = ""
        for char in text:
            if char == "\n":
                out_data = self._buffer + "\n"
                self._buffer = ""
            else:
                self._buffer += char
        if out_data.startswith("Backtrace:"):
            out_data = self.extract_backtrace_info(out_data)

        return out_data

    def tx(self, text):
        print(f"Sent: {text}\n")
        return text
