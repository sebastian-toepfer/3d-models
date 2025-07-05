use <../libs/own/mirror_copy.fuc>

deckel();

module deckel(
    breite_innen = 134,
    tiefe_innen  = 64,
    hoehe        = 50,
    wandstaerke  = 2
) {
    difference() {
        union() {
            cube(
                    [
                        breite_innen + 4.5 * 2 + wandstaerke * 2,
                        tiefe_innen + wandstaerke * 4,
                        wandstaerke * 2
                    ],
                    center = true
            );

            mirror_copy([0, 1, 0]) {
                mirror_copy() {
                    translate([
                            breite_innen / 4,
                            (tiefe_innen + wandstaerke * 4) / 2,
                            wandstaerke * 2
                    ]) {
                        difference() {
                            cube([18, wandstaerke * 2, 7.5], center = true);
                            rotate([90, 0, 90]) {
                                linear_extrude(height = 18 * 1.1, center = true) {
                                    polygon(points = [
                                            [ wandstaerke * 1.1, (7.5 - 4)   / 2],
                                            [ wandstaerke,       (7.5 * 1.1) / -2],
                                            [ -0.2,              (7.5 * 1.1) / -2]
                                    ]);
                                }
                            }
                        }
                    }
                }
            }
        }

        translate([0, 0, wandstaerke]) {
            cube(
                    [
                        breite_innen + 4.5 * 2,
                        tiefe_innen + wandstaerke * 2,
                        wandstaerke * 2
                    ],
                    center = true
            );
        }

        mirror_copy([0, 1, 0]) {
            mirror_copy() {
                translate([breite_innen / 4, tiefe_innen / 2 + wandstaerke, 5]) {
                    rotate([45, 0, 0]) {
                        cube([20, sqrt(8), sqrt(8)], center = true);
                    }
                }
            }
        }
    }
}

