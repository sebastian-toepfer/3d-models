use <../../../parts/cable/channel.scad>
use <../../../parts/screw/hole.scad>
use <../../../things/playseat/monitorstand/playseat_monitor_stand_bein.scad>
use <../../../things/teufel_concept_e450/speaker_satellite_cs25_fcr_mk3.scad>

links                             = true;

satellite(links = links);

module satellite(
    rahmenhoehe = 14,
    rahmenstaerke = 5,
    links = true
) {
    if (links) {
        satellite_links(
            rahmenhoehe = rahmenhoehe,
            rahmenstaerke = rahmenstaerke
        );
    } else {
        satellite_rechts(
            rahmenhoehe = rahmenhoehe,
            rahmenstaerke = rahmenstaerke
        );
    }
}

module satellite_rechts(
        rahmenhoehe = 14,
        rahmenstaerke = 5
) {
    mirror([1, 0, 0]) {
        satellite_links(
            rahmenhoehe = rahmenhoehe
        );
    }
}

module satellite_links(
        rahmenhoehe = 14,
        rahmenstaerke = 5
) {
    hinter_abstand_stand_lautsprecher =
        (
          speaker_center_cs25_fcr_mk3_breite_vorn()
          - speaker_center_cs25_fcr_mk3_breite_hinten()
        ) / 2;
    //ohne ueberhaenge ... aber warum
    max_gesamt_breite = speaker_center_cs25_fcr_mk3_breite_vorn()
        + playseat_monitor_stand_bein_breite();
    max_gesamt_tiefe = speaker_center_cs25_fcr_mk3_tiefe()
        + rahmenstaerke;
    breite_hinten = playseat_monitor_stand_bein_breite()
        + speaker_center_cs25_fcr_mk3_breite_hinten()
        + hinter_abstand_stand_lautsprecher;
    x = (
          max_gesamt_tiefe
          * sin(speaker_center_cs25_fcr_mk3_winkel())
        )
        / sin(90)
        + rahmenstaerke
        / 2;

    difference() {
        union() {
            beinbefestigung(
                rahmenhoehe = rahmenhoehe,
                rahmenstaerke = rahmenstaerke,
                max_gesamt_breite = max_gesamt_breite,
                breite_hinten = breite_hinten
            );
            speakerrahmen(
                rahmenhoehe = rahmenhoehe,
                rahmenstaerke = rahmenstaerke,
                max_gesamt_breite = max_gesamt_breite,
                max_gesamt_tiefe = max_gesamt_tiefe,
                x = x
            );
        }
        rechte_abschraegungen(
            rahmenhoehe = rahmenhoehe,
            rahmenstaerke = rahmenstaerke,
            max_gesamt_breite = max_gesamt_breite,
            x = x
        );
        befestigung(
            rahmenstaerke = rahmenstaerke,
            max_gesamt_breite = max_gesamt_breite
        );
        kabelfuehrung(
            rahmenhoehe = rahmenhoehe,
            rahmenstaerke = rahmenstaerke,
            max_gesamt_breite = max_gesamt_breite,
            breite_hinten = breite_hinten
        );
        bein(
            rahmenstaerke = rahmenstaerke,
            max_gesamt_breite = max_gesamt_breite
        );
        speaker(
            rahmenstaerke = rahmenstaerke,
            max_gesamt_breite = max_gesamt_breite
        );
    }
}

module beinbefestigung(
    rahmenhoehe = 10,
    rahmenstaerke = 10,
    max_gesamt_breite = 10,
    breite_hinten = 10
) {
    translate([
        (max_gesamt_breite - rahmenstaerke) / -2,
        (speaker_center_cs25_fcr_mk3_tiefe() - rahmenhoehe) / 2,
        0
    ]) {
        cube([rahmenstaerke, rahmenhoehe, rahmenhoehe], center = true);
    }

    translate([
        (max_gesamt_breite - breite_hinten - 1) / -2,
        speaker_center_cs25_fcr_mk3_tiefe() / 2,
        0
    ]) {
        cube([
                breite_hinten + 1,
                rahmenstaerke * 2,
                rahmenhoehe
            ],
            center = true
        );
    }
}

module speakerrahmen(
    rahmenhoehe = 10,
    rahmenstaerke = 2,
    max_gesamt_breite = 10,
    max_gesamt_tiefe = 10,
    x = 10
    
) {
    l = sqrt(pow(x, 2)
        - 2
        * x
        * max_gesamt_tiefe
        * cos(90 - speaker_center_cs25_fcr_mk3_winkel())
        + pow(max_gesamt_tiefe, 2));

    translate([x * -1, 0, 0]) {
        rotate([0, 0, - speaker_center_cs25_fcr_mk3_winkel()]) {
            cube([rahmenstaerke, l, rahmenhoehe], center = true);
        }
    }

    translate([
        (
          max_gesamt_breite
          - speaker_center_cs25_fcr_mk3_breite_vorn()
          - rahmenstaerke
        ) / 2,
        0,
        0
    ]) {
        cube([
                rahmenstaerke * 2,
                speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke,
                rahmenhoehe
            ],
            center = true
        );
    }

    translate([
        (
          max_gesamt_breite
          - speaker_center_cs25_fcr_mk3_breite_vorn()
          - rahmenstaerke
        ) + x,
        0,
        0
    ]) {
        rotate([0, 0, speaker_center_cs25_fcr_mk3_winkel()]) {
            cube([rahmenstaerke, l, rahmenhoehe], center = true);
        }
    }

    difference() {
        translate([
            (
              max_gesamt_breite
              - speaker_center_cs25_fcr_mk3_breite_vorn()
              - rahmenstaerke
            ) / 2,
            speaker_center_cs25_fcr_mk3_tiefe() / -2,
            0
        ]) {
            cube([
                    (
                      speaker_center_cs25_fcr_mk3_breite_vorn()
                      + rahmenstaerke
                      * 2
                    ),
                    rahmenstaerke * 2,
                    rahmenhoehe
                ],
                center = true
            );
        }

        //wie berechnet man hier die 0.85
        translate([(x + rahmenstaerke + 0.85) * - 1, - rahmenstaerke, 0]) {
            rotate([0, 0, - speaker_center_cs25_fcr_mk3_winkel()]) {
                cube([
                        rahmenstaerke,
                        speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke,
                        rahmenhoehe * 1.1
                    ],
                    center = true
                );
            }
        }
    }
}

module rechte_abschraegungen(
    rahmenhoehe = 10,
    rahmenstaerke = 2,
    max_gesamt_breite = 10,
    x = 10
) {
    //wie berechnet man hier die 0.85
    translate([
        (
          (
            max_gesamt_breite
            - speaker_center_cs25_fcr_mk3_breite_vorn()
            - rahmenstaerke
          )
          + x
          + rahmenstaerke + 0.85
        ),
        - rahmenstaerke,
        0
    ]) {
        rotate([0, 0, speaker_center_cs25_fcr_mk3_winkel()]) {
            cube([
                    rahmenstaerke,
                    (
                      speaker_center_cs25_fcr_mk3_tiefe()
                      + rahmenstaerke
                    ) * 2,
                    rahmenhoehe * 1.1
                ],
                center = true
            );
        }
    }
}

module befestigung(rahmenstaerke = 2, max_gesamt_breite = 10) {
    translate([
        (
          max_gesamt_breite
          - playseat_monitor_stand_bein_breite()
          - rahmenstaerke
        ) / -2,
        rahmenstaerke * 1.25 + playseat_monitor_stand_bein_tiefe(),
        0
    ]) {
        rotate([90, 0, 180]) {
            screw_hole(
                    length = rahmenstaerke * 4,
                    diameter = 4.5,
                    head_diameter = 10,
                    head_length = 2.5,
                    center = true
            );
        }
    }
}

module kabelfuehrung(
    rahmenhoehe = 10,
    rahmenstaerke = 2,
    max_gesamt_breite = 10,
    breite_hinten = 10
) {
    union() {
        kabel_durchmesser = 2.5;
        mitte_stand = (
              max_gesamt_breite
              - playseat_monitor_stand_bein_breite()
              - rahmenstaerke
            ) / -2;
        mitte_lautsprecher = 25;
        abstand_mitte_stand_mitte_lautsprecher =
            abs(mitte_stand - mitte_lautsprecher);

        translate([
                (max_gesamt_breite - breite_hinten + 5) / -2 ,
                (
                  speaker_center_cs25_fcr_mk3_tiefe()
                  + rahmenstaerke
                  + kabel_durchmesser - 0.9
                ) / 2,
                0
        ]) {
            rotate([270, 0, 0]) {
                cable_channel(
                        length = abstand_mitte_stand_mitte_lautsprecher - 2,
                        cable_diameter = kabel_durchmesser,
                        cable_count = 2,
                        cover_thick = 1,
                        center = true
                );
            }
        }

        translate([
                25,
                (
                  speaker_center_cs25_fcr_mk3_tiefe()
                  + rahmenstaerke
                  + kabel_durchmesser - 0.9
                ) / 2,
                (rahmenhoehe - (kabel_durchmesser * 2 + (1 + 0.1) * 4)) / 2
        ]) {
            rotate([270, 90, 0]) {
                cable_channel(
                        length = rahmenhoehe,
                        cable_diameter = kabel_durchmesser,
                        cable_count = 2,
                        cover_thick = 1,
                        center = true
                );
            }
        }
    }
}

module bein(
    rahmenstaerke = 2,
    max_gesamt_breite = 10
) {
    translate([
        (
          max_gesamt_breite
          - rahmenstaerke
          - playseat_monitor_stand_bein_breite()
        ) / -2,
        (
          speaker_center_cs25_fcr_mk3_tiefe()
          - playseat_monitor_stand_bein_tiefe()
          - rahmenstaerke
        ) / 2,
        0
    ]) {
        playseat_monitor_stand_bein();
    }
}

module speaker(
    rahmenstaerke = 2,
    max_gesamt_breite = 10
) {
    translate([
        (
          max_gesamt_breite
          - speaker_center_cs25_fcr_mk3_breite_vorn()
          - rahmenstaerke
        ) / 2,
        0,
        speaker_center_cs25_fcr_mk3_hoehe() / 2
    ]) {
        speaker_satellite_cs25_fcr_mk3();
    }
}

