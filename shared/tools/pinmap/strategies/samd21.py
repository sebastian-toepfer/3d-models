class SAMD21Strategy:
    def generate_header(self, assignment):
        lines = [
            "//generated file, do not edit!",
            "#ifndef PINMAP_H",
            "#define PINMAP_H",
            "",
            "#include \"gpio/pin_config.h\"",
            "",
            "#ifdef __cplusplus",
            "extern \"C\" {",
            "#endif",
            ""
        ]

        for name in assignment:
            lines.append(f"extern const pin_config_t {name}_pin_config;")

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
            "#include \"gpio/samd21/pin_config.h\"",
            ""
        ]

        for name, schematic_pin in assignment.items():
            pin, port, pin_mask, pin_index = pinmap[schematic_pin]
            lines.append(f"const pin_config_t {name}_pin_config = {{")
            lines.append(f"    .platform_config = &(PinConfig_SAMD21){{")
            lines.append(f"        .group = &PORT->Group[{ord(port) - ord('A')}], .pin_mask = 1 << {pin_mask}, .pin_index = {pin_index}}}}};\n")

        return "\n".join(lines)

