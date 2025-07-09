// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
schlauch_durchmesser                  = 75;
schlauch_wand_staerke                 = 1.5;

schlauch_anschluss_durchmesser        = 90;
schlauch_anschluss_durchmesser_aussen = 100;
wand_staerke                          = 1.5;

hoehe                                 = 60;

difference() {
    body(
        schlauch_durchmesser - schlauch_wand_staerke * 2,
        schlauch_anschluss_durchmesser,
        schlauch_anschluss_durchmesser_aussen,
        hoehe
    );
    body(
        schlauch_durchmesser - schlauch_wand_staerke * 2 - wand_staerke * 2,
        schlauch_anschluss_durchmesser - wand_staerke * 2,
        schlauch_anschluss_durchmesser - wand_staerke * 2,
        hoehe + 0.1
    );
}

module body(von_durchmesser = 10, auf_durchmesser_innen = 20, auf_durchmesser_aussen = 30, gesamt_hoehe = 30) {
    union() {
        translate([0, 0, gesamt_hoehe / -3]) {
            cylinder(d = von_durchmesser, h = gesamt_hoehe / 3, center = true);
        }
        cylinder(d1 = von_durchmesser, d2 = auf_durchmesser_aussen, h = gesamt_hoehe / 3, center = true);
        translate([0, 0, gesamt_hoehe / 3]) {
            difference() {
                cylinder(d = auf_durchmesser_aussen, h = gesamt_hoehe / 3, center = true);
                if (auf_durchmesser_innen < auf_durchmesser_aussen) {
                    translate([0, 0, gesamt_hoehe / 12]) {
                        difference() {
                            cylinder(d = auf_durchmesser_aussen + 0.2, h = gesamt_hoehe / 6 + 0.2, center = true);
                            cylinder(d = auf_durchmesser_innen, h = gesamt_hoehe / 6 + 0.2, center = true);
                        }
                    }
                }
            }
        }
    }
}

