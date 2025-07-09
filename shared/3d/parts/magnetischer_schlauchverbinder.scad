// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
//inspiriert durch: https://www.thingiverse.com/thing:5358837 
use <../libs/own/mirror_copy.fuc>
use <../libs/own/polar_pattern.fuc>

schlauch_durchmesser = 75;
schlauch_wandstaerke = 1.5;
wandstaerke          = 1.5;
ueberlappung         = 4.5;
magnet_durchmesser   = 10;
magnet_dicke         = 3;

magnetischer_schlauchverbinder(
    schlauch_durchmesser = schlauch_durchmesser,
    schlauch_wandstaerke = schlauch_wandstaerke,
    wandstaerke          = wandstaerke,
    ueberlappung         = ueberlappung,
    magnet_durchmesser   = magnet_durchmesser,
    magnet_dicke         = magnet_dicke
);

module magnetischer_schlauchverbinder(
    schlauch_durchmesser = 75,
    schlauch_wandstaerke = 1.5,
    wandstaerke          = 1.5,
    ueberlappung         = 4.5,
    magnet_durchmesser   = 10,
    magnet_dicke         = 3
) {
    kegel_hoehe  = 25;
    gesamt_hoehe = kegel_hoehe * 2 + ueberlappung;
    union() {
        roehre(laenge = gesamt_hoehe, durchmesser = schlauch_durchmesser - schlauch_wandstaerke * 2, wandstaerke = wandstaerke, center = true);
        translate([0, 0, gesamt_hoehe / 2 - kegel_hoehe - ueberlappung]) {
            befestigungsring(hoehe = kegel_hoehe, durchmesser = schlauch_durchmesser - schlauch_wandstaerke * 2 - 0.0001) {
                cylinder(h = magnet_dicke, d = magnet_durchmesser);
            }
        }
    }

    module roehre(laenge = 10, durchmesser = 20, wandstaerke = 1, center = false) {
        difference() {
            cylinder(h = laenge, r = durchmesser / 2, center = center);
            cylinder(h = laenge + 0.2, r = durchmesser / 2 - wandstaerke, center = center);
        }
    }

    module befestigungsring(hoehe = 20, durchmesser = 20, magnet_anzahl = 6) {
        difference() {
            radius = durchmesser / 2;
            hoehe_gerade = hoehe * 0.333;
            hoehe_schraege = hoehe - hoehe_gerade;
            aussen_radius = radius + hoehe_schraege;
            union() {
              cylinder(h = hoehe_schraege, r1 = radius, r2 = aussen_radius, center = false);
              translate([0, 0, hoehe_schraege - 0.0001]) {
                  cylinder(h = hoehe_gerade, r = aussen_radius);
              }
            }
            aussparrungen_hoehe = hoehe * 2.5;
            cylinder(h = aussparrungen_hoehe, r = radius, center = true);
            durchmesser_aussparrung = PI * aussen_radius * 2 / 12 * 1.2;
            polar_pattern(magnet_anzahl) {
                rotate(90) {
                    translate([durchmesser / 2 + durchmesser_aussparrung * 0.55, 0, 0]) {
                        cylinder(h = aussparrungen_hoehe, d = durchmesser_aussparrung, center = false);
                    }
                }
                if ($children > 0) {
                    translate([durchmesser / 2 + (aussen_radius - radius) / 2, 0, hoehe_schraege + hoehe_gerade + 0.01]) {
                        rotate([0, 180, 0]) {
                            children(0);
                        }
                    }
                }
            }
        }
    }
}

