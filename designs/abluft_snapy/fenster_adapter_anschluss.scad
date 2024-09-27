// part to remix https://www.thingiverse.com/thing:4148187
use <../../libs/own/mirror_copy.fuc>
use <../../libs/own/roehre.scad>

fenster_adapter_anschluss(
        schlauch_durchmesser = 75,
        box = [130, 30, 20],
        wand_staerke = 1
);

module fenster_adapter_anschluss(
        schlauch_durchmesser = 75,
        box = [130, 30, 20],
        wand_staerke = 1
) {
    abstand                     = ((schlauch_durchmesser - box[1]) * sin(55)) 
                                  / sin(35);
    befestigung_schlauch_laenge = 25;
    ueberlappung_fenser         = 5;

    union() {
        translate([
                    0,
                    (schlauch_durchmesser - box[1]) / -2,
                    (abstand + box[2]) / 2
        ]) {
            fenster_anschluss(
                    hoehe = box[2],
                    breite = box[0],
                    tiefe = box[1],
                    wandstaerke = wand_staerke,
                    ueberlappung = 5
            );
        }

        verbindung(
                schlauch_durchmesser = schlauch_durchmesser,
                box = box,
                wand_staerke = wand_staerke * 2,
                abstand = abstand
        );

        translate([0, 0, (abstand + befestigung_schlauch_laenge) / -2]) {
            roehre(
                    durchmesser = schlauch_durchmesser,
                    laenge = befestigung_schlauch_laenge,
                    wand_staerke = wand_staerke * 2
            );
        }
    }
}

module verbindung(
        schlauch_durchmesser = 75,
        box = [130, 30],
        abstand = 45,
        wand_staerke = 1
) {
    difference() {
        hull() {
            union() {
                translate([
                        0,
                        (schlauch_durchmesser - box[1]) / -2,
                        abstand / 2
                ]) {
                    runde_box(
                            hoehe = wand_staerke,
                            breite = box[0],
                            tiefe = box[1],
                            center = true
                    );
                }
                translate([0, 0, abstand / -2]) {
                   cylinder(
                           d = schlauch_durchmesser,
                           h = wand_staerke,
                           center = true
                   );
                }
            }
        }
        hull() {
            union() {
                translate([
                        0,
                        (
                            schlauch_durchmesser
                            - wand_staerke * 2
                            - box[1]
                            + wand_staerke
                            * 2
                        )
                        / -2,
                        abstand / 2 + 0.1
                ]) {
                    runde_box(
                            hoehe = wand_staerke,
                            breite = box[0] - wand_staerke * 2,
                            tiefe = box[1] - wand_staerke * 2,
                            center = true
                    );
                }
                translate([0, 0, abstand / -2 - 0.1]) {
                   cylinder(
                           d = schlauch_durchmesser - wand_staerke * 2,
                           h = wand_staerke,
                           center = true
                   );
                }
            }
        }
    }
}

module fenster_anschluss(
        hoehe = 30,
        breite = 130,
        tiefe = 30,
        wandstaerke = 1,
        ueberlappung = 5
) {
    difference() {
        union() {
            translate([0, 0, - ueberlappung / 2]) {
                runde_box(
                        hoehe = hoehe - ueberlappung,
                        breite = breite,
                        tiefe = tiefe,
                        center = true
                );
            }
            runde_box(
                    hoehe = hoehe,
                    breite = breite - wandstaerke * 2,
                    tiefe = tiefe - wandstaerke * 2,
                    center = true
            );
        }
        translate([0, 0, 0]) {
            runde_box(
                    hoehe = hoehe + ueberlappung * 2,
                    breite = breite - wandstaerke * 4,
                    tiefe = tiefe - wandstaerke * 4,
                    center = true
            );
        }
    }
}

module runde_box(
        hoehe = 20,
        breite = 40,
        tiefe = 20,
        center = false
) {
    union() {
        mirror_copy() {
            translate([breite / 2 - tiefe / 2, 0, 0]) {
                cylinder(d = tiefe, h = hoehe, center = center);
            }
        }
        cube([breite - tiefe, tiefe, hoehe], center = center);
    }
}

