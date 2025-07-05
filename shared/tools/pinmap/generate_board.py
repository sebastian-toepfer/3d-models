#!/bin/python3

import os
import csv
import sexpdata
import argparse

from strategies import get_strategy

# Resolve base directory of the script
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

def extract_pin_assignment(netlist_file, mcu_ref):
    with open(netlist_file, "r") as f:
        content = f.read()

    data = sexpdata.loads(content)
    pin_assignment = {}

    IGNORED_NETS = ["3V3", "AREF", "+5V", "5V", "VIN", "-5V", "GND"]

    for item in data:
        if isinstance(item, list) and item and item[0] == sexpdata.Symbol("nets"):
            for net in item[1:]:
                if isinstance(net, list) and net and net[0] == sexpdata.Symbol("net"):
                    net_name = None
                    for e in net:
                        if isinstance(e, list) and e and e[0] == sexpdata.Symbol("name"):
                            net_name = e[1]
                            break
                    if not net_name:
                        continue

                    for e in net:
                        if isinstance(e, list) and e and e[0] == sexpdata.Symbol("node"):
                            ref = None
                            pin = None
                            for sub in e:
                                if isinstance(sub, list) and sub:
                                    if sub[0] == sexpdata.Symbol("ref"):
                                        ref = sub[1]
                                    if sub[0] == sexpdata.Symbol("pin"):
                                        pin = sub[1]
                            if ref == mcu_ref and pin:
                                if net_name.startswith("unconnected-"):
                                    continue
                                if any(net_name.strip("/").upper() == ign.upper() for ign in IGNORED_NETS):
                                    continue
                                clean_name = net_name.lstrip("/")
                                pin_assignment[clean_name] = pin
    return pin_assignment


def load_pinmap(filename):
    pinmap = {}
    with open(filename, newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            pinmap[row["schematic_pin"]] = (int(row["pin"]), row["port"], int(row["pin_mask"]), int(row["pin_index"]))
    return pinmap

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate board.h and board.c from KiCad netlist")
    parser.add_argument("--netlist", required=True, help="Path to KiCad netlist (.xml S-expression format)")
    parser.add_argument("--out-h", required=True, help="Path to output board.h")
    parser.add_argument("--out-c", required=True, help="Path to output board.c")
    parser.add_argument("--platform", required=True, help="Target platform name (e.g., samd21, stm32)")
    parser.add_argument("--mcu-ref", required=True, help="MCU reference designator in schematic (e.g., MCU1)")

    args = parser.parse_args()

    pin_assignment = extract_pin_assignment(args.netlist, args.mcu_ref)

    if not pin_assignment:
        print("[Error] No relevant MCU pins found in netlist.")
        exit(1)

    pinmap = load_pinmap(os.path.join(BASE_DIR, "config", f"pinmap_{args.platform}.csv"))
    strategy = get_strategy(args.platform)

    os.makedirs(os.path.dirname(args.out_h), exist_ok=True)
    os.makedirs(os.path.dirname(args.out_c), exist_ok=True)

    with open(args.out_h, "w") as header:
        header.write(strategy.generate_header(pin_assignment))

    with open(args.out_c, "w") as source:
        source.write(strategy.generate_source(pin_assignment, pinmap))

