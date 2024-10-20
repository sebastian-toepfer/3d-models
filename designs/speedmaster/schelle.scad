layer_height = 0.16;
wall_thickness = 1.2;

innen_durchmesser = 25;
laenge            = 35;

difference() {
    union() {
        cylinder(
            d = innen_durchmesser + wall_thickness * 2,
            h = laenge,
            center = true
        );
        translate([0, 0, (laenge + layer_height * 2) / -2]) {
            cylinder(
                d = innen_durchmesser + wall_thickness * 6,
                h = layer_height * 2,
                center = true
            );
        }
    }

    union() {
        cylinder(d = innen_durchmesser, h = laenge * 1.1, center = true);

        translate([innen_durchmesser / 2, 0, 0]) {
            cube([10, 5, laenge * 1.1], center = true);
        }
    }
}

