module screw_hole(length =10, diameter = 4, head_diameter = 0, head_length = 0, center = false) {
    union() {
        cylinder(h = length, d = diameter, center = center);
        translate([0, 0, (length + head_length) / (center ? 2 : 1)]) {
            cylinder(h = head_length + 0.1, d = head_diameter, center = center);
        }
    }
}

