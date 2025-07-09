// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <corner.template.scad>

psu_holder_corner_rechts();

module psu_holder_corner_rechts() {
    mirror([1, 0, 0]) {
        psu_holder_corner_template();
    }
}

