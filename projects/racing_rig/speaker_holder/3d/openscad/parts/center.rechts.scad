// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <center.template.scad>

center_rechts();

module center_rechts() {
    mirror([1, 0, 0]) {
        center_template();
    }
}

