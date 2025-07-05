use <../libs/own/polar_pattern.fuc>

solar_erdspies();

module solar_erdspies(
    innen_durchmesser  = 13.5,
    sockel_hoehe       = 20,
    wand_staerke       = 2,
    spies_laenge       = 90,
    unter_spitzenteil_breite = 5
) {
    aussen_durchmesser = innen_durchmesser + wand_staerke * 2;
    union() {
        translate([0, 0, (spies_laenge - sockel_hoehe) / 2]) {
            difference() {
                union() {
                    cylinder(h = sockel_hoehe, d = innen_durchmesser, center = true);
                    cylinder(h = wand_staerke * 2, d = aussen_durchmesser, center = true);
                }
                cylinder(h = sockel_hoehe * 1.1, d = innen_durchmesser - wand_staerke * 2, center = true);
            }
        }

        translate([0, 0, sockel_hoehe / -2]) {
            difference() {
                polar_pattern(anzahl = 4) {
                    rotate([90, 0, 0]) {
                        linear_extrude(height = wand_staerke * 1, center = true) {
                            polygon(points = [
                                    [(aussen_durchmesser) / 2 + wand_staerke / 2, spies_laenge /  2],
                                    [0,                                           spies_laenge /  2],
                                    [0,                                           spies_laenge / -2],
                                    [unter_spitzenteil_breite / 2,                spies_laenge / -2 + unter_spitzenteil_breite * 2],
                                    [6 / 2,                                       0]
                            ]);
                        }
                    }
                }
        
                translate([0, 0, (spies_laenge - unter_spitzenteil_breite * 2) / -2]) {
                    difference() {
                        cylinder(h = unter_spitzenteil_breite * 2 * 1.1, d = unter_spitzenteil_breite * 1.1, center = true);
                        translate([0, 0, 0.1]) {
                            cylinder(h = unter_spitzenteil_breite * 2 * 1.1, d1 = 0, d2 = unter_spitzenteil_breite * 1.2, center = true);
                        }
                    }
                }
            }
        }
    }
}

