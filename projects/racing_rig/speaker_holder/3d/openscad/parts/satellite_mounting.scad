use <../libs/own/mirror_copy.fuc>
use <../sharedparts/screw/hole.scad>
use <../things/playseat/monitorstand/playseat_monitor_stand_bein.scad>

satellite_mounting();

module satellite_mounting(rahmenstaerke = 5, rahmentiefe = 20, rahmenhoehe = 15) {
    difference() {
        basis(
            dimension = [
                playseat_monitor_stand_bein_breite() + rahmenstaerke,
                rahmenhoehe * 3.5,
                rahmentiefe
            ]
        );

        union() {
            befestigung_aussparrung(
                gesamt_tiefe      = rahmenhoehe * 3.5,
                rahmen_tiefe      = rahmenhoehe,
                auflage           = 5,
                befestigung_hoehe = 5
            );
            befestigung_schraub_loecher(
                tiefe = rahmenhoehe + 5,
                hoehe = rahmentiefe
            );

            halterung_loch(hoehe = rahmentiefe);
            translate([0, 0, (10 - rahmentiefe - 0.01) / 2]) {
                halterung_aussparrung(
                    rahmen_dimension = [
                        playseat_monitor_stand_bein_breite() + rahmenstaerke,
                        rahmenhoehe,
                        10.1
                    ]
                );
            }
        }
    }

    module basis(dimension = [10, 10, 10]) {
        cube([dimension.x, dimension.y, dimension.z], center = true);
    }

    module befestigung_aussparrung(
        gesamt_tiefe      = 15,
        rahmen_tiefe      = 10,
        auflage           = 1,
        befestigung_hoehe = 1
    ) {
        translate([
                0,
                0,
                playseat_monitor_stand_bein_dimension().y / 2
                  - playseat_monitor_stand_bein_wandstaerke()
        ]) {
            union() {
                difference() {
                    cube([
                            playseat_monitor_stand_bein_dimension_innen().x + 0.01,
                            gesamt_tiefe * 1.1,
                            playseat_monitor_stand_bein_dimension_innen().y + 0.01
                        ],
                        center = true
                    );

                    translate([
                        0,
                        0,
                        playseat_monitor_stand_bein_dimension().y / -2
                          + playseat_monitor_stand_bein_wandstaerke()
                          + befestigung_hoehe / 2
                    ]) {
                        cube([
                                playseat_monitor_stand_bein_breite_luecke(),
                                rahmen_tiefe + auflage * 2,
                                befestigung_hoehe
                            ],
                            center = true
                        );
                    }
                }

                rotate([270, 0, 0]) {
                    playseat_monitor_stand_bein();
                }
            }
        }
    }

    module befestigung_schraub_loecher(
        tiefe = 10,
        hoehe = 20,
        schraube_durchmesser = 5,
        schrauben_kopf_durchmesser = 10,
        schrauben_kopf_laenge = 2.5,
    ) {
        mirror_copy([0, 1, 0]) {
            translate([0, tiefe, 0]) {
                rotate([180, 0, 0]) {
                    screw_hole(
                            length = hoehe - schrauben_kopf_laenge * 2,
                            diameter = schraube_durchmesser,
                            head_diameter = schrauben_kopf_durchmesser,
                            head_length = schrauben_kopf_laenge,
                            center = true
                    );
                }
            }
        }
    }

    module halterung_loch(hoehe = 10) {
        cylinder(
            d = 5.6,
            h = hoehe * 1.1,
            center = true
        );
    }

    module halterung_aussparrung(rahmen_dimension = [10, 10, 10]) {
        cube([
                rahmen_dimension.x * 1.1,
                rahmen_dimension.y,
                rahmen_dimension.z + 0.01
            ],
            center = true
        );
    }
}

