// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <../libs/own/mirror_copy.fuc>
use <../sharedparts/screw/hole.scad>
use <../things/playseat/monitorstand/playseat_monitor_stand_bein.scad>
use <../things/teufel_concept_e450/speaker_satellite_cs25_fcr_mk3.scad>

satellite_template();

module satellite_template(
    rahmenhoehe = 14,
    rahmenstaerke = 5,
) {
    tiefe = rahmenstaerke * 2;
    hoehe = rahmenhoehe;

    gesamt_breite =
        speaker_satellite_cs25_fcr_mk3_breite_vorn()
        + playseat_monitor_stand_bein_breite();

    union() {
        translate([
            (
              gesamt_breite
              - playseat_monitor_stand_bein_breite()
            ) / -2,
            (
              speaker_satellite_cs25_fcr_mk3_tiefe()
              - playseat_monitor_stand_bein_tiefe()
              - rahmenstaerke
            ) / 2,
            0
        ]) {
            beinbefestigung(
                tiefe = tiefe,
                hoehe = hoehe,
                ueberhang = rahmenstaerke / 2
            );
        }

        breite_verbindung =
            gesamt_breite
            - playseat_monitor_stand_bein_breite()
            - speaker_satellite_cs25_fcr_mk3_breite_hinten()
            - rahmenstaerke * 3;
        tiefe_verbindung = rahmenstaerke * 2;

        translate([
            (
              gesamt_breite
              - playseat_monitor_stand_bein_breite()
              - speaker_satellite_cs25_fcr_mk3_breite_hinten()
            ) / -2,
            (
              speaker_satellite_cs25_fcr_mk3_tiefe()
              - tiefe_verbindung
              + rahmenstaerke * 2
            ) / 2,
            0
        ]) {
            verbindungsstueck(
                dimension = [
                    breite_verbindung,
                    tiefe_verbindung,
                    hoehe
                ]
            );
        }

        translate([
            (
              gesamt_breite
              - speaker_satellite_cs25_fcr_mk3_breite_vorn()
            ) / 2,
            0,
            0
        ]) {
            speaker_rahmen(
                tiefe = tiefe,
                hoehe = hoehe,
                rahmenstaerke = rahmenstaerke
            );
        }
    }

    module beinbefestigung(tiefe = 20, hoehe = 10, ueberhang = 2) { 
        difference() {
            translate([
                0,
                (
                  playseat_monitor_stand_bein_dimension().y / 2
                  + (
                    tiefe
                    - playseat_monitor_stand_bein_wandstaerke() * 2
                    + 0.0001 
                  ) / 2
                ),
                0,
            ]) {
                cube([
                        playseat_monitor_stand_bein_dimension().x + ueberhang * 2,
                        tiefe,
                        hoehe
                    ],
                    center = true
                );
            }

            translate([
                0,
                (
                  tiefe
                  * 1.1
                  + 2.75
                  + playseat_monitor_stand_bein_wandstaerke()
                  + 0.01
                ),
                0
            ]) {
                rotate([90, 0, 180]) {
                    screw_hole(
                            length = tiefe * 1.1,
                            diameter = 4.5,
                            head_diameter = 10,
                            head_length = 2.5,
                            center = true
                    );
                }
            }
            playseat_monitor_stand_bein();
        }
    }

    module verbindungsstueck(
        dimension = [10, 10, 10]
    ) {
        difference() {
            cube(dimension, center = true);
            // welche formel ...
            // y = 10 => 14
            // y   20 => 13
            // y   40 => 11
            // y   80 =>  9
            translate([14, 0, 0]) {
                rotate([0, 0, - speaker_satellite_cs25_fcr_mk3_winkel()]) {
                    cube([
                            dimension.x,
                            sqrt(
                                pow(dimension.x, 2)
                                + pow(dimension.y, 2)
                            ) * 1.1,
                            dimension.z * 1.1
                        ],
                        center = true
                    );
                }
            }
        }
    }

    module speaker_rahmen(
        tiefe = 20,
        hoehe = 10,
        rahmenstaerke = 2
    ) {
        difference() {
            speaker_rahmen_grundstruktur(
                tiefe = tiefe,
                hoehe = hoehe,
                rahmenstaerke = rahmenstaerke
            );
            translate([0, 0, speaker_satellite_cs25_fcr_mk3_hoehe() / 2]) {
                speaker_satellite_cs25_fcr_mk3();
            }
        }
    }

    module speaker_rahmen_grundstruktur(
        tiefe = 20,
        hoehe = 10,
        rahmenstaerke = 2
    ) {
        x = (
              speaker_satellite_cs25_fcr_mk3_tiefe()
              * sin(speaker_satellite_cs25_fcr_mk3_winkel())
            )
            / sin(90);
        mirror_copy([1, 0, 0]) {
            difference() {
                union() {
                    translate([
                        (
                          speaker_satellite_cs25_fcr_mk3_breite_vorn()
                          - x
                        ) / -2,
                        0,
                        0
                    ]) {
                        rotate([
                            0,
                            0,
                            speaker_satellite_cs25_fcr_mk3_winkel() * -1
                        ]) {
                            cube([rahmenstaerke * 2, 100, hoehe], center = true);
                        }
                    }

                    translate([
                        (
                          speaker_satellite_cs25_fcr_mk3_breite_hinten() / 2
                          + rahmenstaerke
                        ) / -2,
                        speaker_satellite_cs25_fcr_mk3_tiefe() / 2,
                        0
                    ]) {
                        cube([
                                (
                                  speaker_satellite_cs25_fcr_mk3_breite_hinten() / 2
                                  + rahmenstaerke
                                ),
                                tiefe,
                                hoehe
                            ],
                            center = true
                        );
                    }

                    translate([rahmenstaerke / -2, 0, 0]) {
                        cube([
                                rahmenstaerke,
                                speaker_satellite_cs25_fcr_mk3_tiefe(),
                                hoehe
                            ],
                            center = true
                        );
                    }

                    translate([
                        (
                          speaker_satellite_cs25_fcr_mk3_breite_vorn() / 2
                          + rahmenstaerke
                        ) / -2,
                        speaker_satellite_cs25_fcr_mk3_tiefe() / -2,
                        0
                    ]) {
                        cube([
                                (
                                  speaker_satellite_cs25_fcr_mk3_breite_vorn() / 2
                                  + rahmenstaerke
                                ),
                                tiefe,
                                hoehe
                            ],
                            center = true
                        );
                    }
                }

                translate([
                    (
                      speaker_satellite_cs25_fcr_mk3_breite_vorn()
                      - x + 12 
                    ) / -2,
                    0,
                    0
                ]) {
                    rotate([
                        0,
                        0,
                        speaker_satellite_cs25_fcr_mk3_winkel() * -1
                    ]) {
                        cube([5, 100 * 1.1, hoehe * 1.1], center = true);
                    }
                }
            }

        }
    }
}

