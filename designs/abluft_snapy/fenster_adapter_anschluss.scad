// part to remix https://www.thingiverse.com/thing:4148187
use <../../libs/own/mirror_copy.scad>

module runde_box(hoehe = 20, breite = 40, tiefe = 20) {
    union() {
        mirror_copy() {
            translate([breite / 2 - tiefe / 2, 0, 0]) {
                cylinder(d = tiefe, h = hoehe, center = true);
            }
        }
        cube([breite - tiefe, tiefe, hoehe], center = true);
    }
}

module fenster_adapter_anschluss(hoehe = 30, breite = 130, tiefe = 30, wandstaerke = 1, ueberlappung = 5) {
    difference() {
        union() {
            translate([0, 0, - ueberlappung / 2]) {
                runde_box(hoehe = hoehe - ueberlappung, breite = breite, tiefe = tiefe);
            }
            runde_box(hoehe = hoehe, breite = breite - wandstaerke * 2, tiefe = tiefe - wandstaerke * 2);
        }
        translate([0, 0, 0]) {
            runde_box(hoehe = hoehe + ueberlappung * 2, breite = breite - wandstaerke * 4, tiefe = tiefe - wandstaerke * 4);
        }
    }
}

fenster_adapter_anschluss(hoehe = 20);

