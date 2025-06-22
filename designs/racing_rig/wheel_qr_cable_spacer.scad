use <../../libs/own/polar_pattern.fuc>

difference() {
    cylinder(d = 81.81, h = 3.7, center = true);
    cylinder(d = 55, h = 4, center = true);

    polar_pattern(anzahl = 6, angle = 360) {
        translate([0, 70 / 2, 0]) {
            cylinder(d = 6, h = 4, center = true);
        }
    }

    translate([20, -30, 0]) {
        rotate([0, 0, 135]) {
            cube([30, 4, 4], center = true);
        }
    }
}

