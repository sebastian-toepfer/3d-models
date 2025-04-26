include <../libs/own/mirror_copy.fuc>
use <gehaeuse_unterseite.scad>

gehaeuse_oberseite();

module gehaeuse_oberseite(
    breite = 105,
    hoehe = 90,
    tiefe = 50.6,
    wandstaerke = 2,
    center = true
) {
    difference() {
        korpus(breite = breite, hoehe = hoehe, tiefe = tiefe, center = center);
        translate([0, 0, - wandstaerke - 0.01]) {
            korpus(
                breite = breite - wandstaerke * 2,
                hoehe = hoehe - wandstaerke * 2,
                tiefe = tiefe + 0.02,
                center = center
            );
        }

        translate([0, 0, tiefe / -2 - 4.0]) {
            gehaeuse_unterseite();
        }

        mirror_copy([0, 1, 0]) {
            mirror_copy() {
                translate([
                    (breite - wandstaerke * 2) / 2 + 0.25,
                    hoehe / 4,
                    -25.25
                ]) {
                    cube([0.5 + 0.01, 5, 5], center = true);
                }
            }
        }
    }

    module korpus(breite = 10, hoehe = 5, tiefe = 10, center = true) {
        rotate([0, 90, 0]) {
            linear_extrude(height = breite, center = center) {
                mirror_copy([0, 1, 0]) {
                    polygon(
                      points = [
                        [tiefe / 2, 0],
                        [tiefe / 2, hoehe / 2],
                        [tiefe / 2 - 20, hoehe / 2],
                        [tiefe / 2 - 20, hoehe / 2 - 13.175],
                        [tiefe / 2 - 20 - 20, hoehe / 2 - 13.175],
                        [tiefe / 2 - 20 - 20, hoehe / 2 - 13.175 - 9.25],
                        [tiefe / 2 * -1, hoehe / 2 - 13.175 - 9.25 ],
                        [tiefe / 2 * -1, 0],
                      ]
                    );
                }
            }
        }
    }
}

