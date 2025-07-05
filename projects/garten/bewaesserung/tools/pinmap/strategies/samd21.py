class SAMD21Strategy:
    def generate_header(self, assignment):
        lines = [
            "//generated file, do not edit!",
            "#ifndef PINMAP_H",
            "#define PINMAP_H",
            "",
            "#include \"digital_output_pin.h\"",
            "",
            "#ifdef __cplusplus",
            "extern \"C\" {",
            "#endif",
            ""
        ]

        for name in assignment:
            lines.append(f"extern const digital_pin_config_t {name}_pin_config;")

        lines += [
            "",
            "#ifdef __cplusplus",
            "}",
            "#endif",
            "",
            "#endif"
        ]

        return "\n".join(lines) + "\n"

    def generate_source(self, assignment, pinmap):
        lines = [
            "#include \"pinmap.h\"",
            "#include \"digital_output_pin_samd21.h\"",
            ""
        ]

        for name, schematic_pin in assignment.items():
            pin, port, pin_mask, pin_index = pinmap[schematic_pin]
            lines.append(f"const digital_pin_config_t {name}_pin_config = {{")
            lines.append(f"    .pin = {pin},")
            lines.append(f"    .platform_info = &(DigitalPinInfo_SAMD21){{")
            lines.append(f"        .group = &PORT->Group[{ord(port) - ord('A')}], .pin_mask = 1 << {pin_mask}, .pin_index = {pin_index}}}}};\n")

        return "\n".join(lines)

