from . import samd21

PLATFROM_ALIASES = {
    "mkrwan1310": "samd21",
}

def get_strategy(platform):
    platform = PLATFROM_ALIASES.get(platform, platform);

    if platform == "samd21":
        return samd21.SAMD21Strategy()
    else:
        raise ValueError(f"Unsupported platform: {platform}")

