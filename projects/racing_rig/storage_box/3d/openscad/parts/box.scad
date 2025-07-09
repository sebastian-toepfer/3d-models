// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <../things/treq/eckwinkel.scad>
use <../libs/own/mirror_copy.fuc>

box();

module box(
    breite_innen = 134,
    tiefe_innen  = 64,
    hoehe        = 50,
    wandstaerke  = 2
) {
    union() {
        difference() {
            //4.5 aus winkel ...
            cube([breite_innen + 4.5 * 2, tiefe_innen + wandstaerke * 2, hoehe + wandstaerke], center = true);
            translate([0, 0, wandstaerke]) {
                cube([breite_innen, tiefe_innen, hoehe + wandstaerke / 2], center = true);
            }
        }

        mirror_copy([0, 1, 0]) {
            mirror_copy() {
                translate([breite_innen / 4, tiefe_innen / 2 + wandstaerke, hoehe / 2 - 5]) {
                    rotate([45, 0, 0]) {
                        cube([20, sqrt(8), sqrt(8)], center = true);
                    }
                }
            }
        }
     
        mirror_copy() {
            translate([breite_innen / 2 + 36 / 2 , 0, (hoehe - 38) / -2 - wandstaerke]) {
                eckwinkel();
            }
        }
    }
}

