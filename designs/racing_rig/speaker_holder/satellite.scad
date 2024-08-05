use <../../../parts/cable/channel.scad>
use <../../../parts/screw/hole.scad>
use <../../../things/playseat/monitorstand/playseat_monitor_stand_bein.scad>
use <../../../things/teufel_concept_e450/speaker_satellite_cs25_fcr_mk3.scad>

links                             = true;
rahmenstaerke                     = 5;
rahmenhoehe                       = 14;

hinter_abstand_stand_lautsprecher = (speaker_center_cs25_fcr_mk3_breite_vorn() - speaker_center_cs25_fcr_mk3_breite_hinten()) / 2;

//ohne ueberhaenge ... aber warum
max_gesamt_breite                 = speaker_center_cs25_fcr_mk3_breite_vorn() + playseat_monitor_stand_bein_breite();
max_gesamt_tiefe                  = speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke;
breite_hinten                     = playseat_monitor_stand_bein_breite() + speaker_center_cs25_fcr_mk3_breite_hinten() + hinter_abstand_stand_lautsprecher;

winkel_ls = speaker_center_cs25_fcr_mk3_winkel();
x = (max_gesamt_tiefe * sin(winkel_ls)) / sin(90) + rahmenstaerke / 2;
l = sqrt(pow(x, 2) - 2 * x * max_gesamt_tiefe * cos(90 - winkel_ls) + pow(max_gesamt_tiefe, 2));

mirror(links ? [0, 0, 0] : [1, 0, 0]) {
    difference() {
        union() {
            beinbefestigung();
            speakerrahmen();
        }
        rechte_abschraegungen();
        befestigung();
        kabelfuehrung();
        bein();
        speaker();
    }
}

module beinbefestigung() {
    translate([(max_gesamt_breite - rahmenstaerke) / -2, (speaker_center_cs25_fcr_mk3_tiefe() - rahmenhoehe) / 2, 0]) {
        cube([rahmenstaerke, rahmenhoehe, rahmenhoehe], center = true);
    }

    translate([(max_gesamt_breite - breite_hinten - 1) / -2, speaker_center_cs25_fcr_mk3_tiefe() / 2, 0]) {
        cube([breite_hinten + 1, rahmenstaerke * 2, rahmenhoehe], center = true);
    }
}

module speakerrahmen() {
    translate([x * -1, 0, 0]) {
        rotate([0, 0, - winkel_ls]) {
            cube([rahmenstaerke, l, rahmenhoehe], center = true);
        }
    }

    translate([(max_gesamt_breite - speaker_center_cs25_fcr_mk3_breite_vorn() - rahmenstaerke) / 2, 0, 0]) {
        cube([rahmenstaerke * 2, speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke, rahmenhoehe], center = true);
    }

    translate([(max_gesamt_breite - speaker_center_cs25_fcr_mk3_breite_vorn() - rahmenstaerke) + x, 0, 0]) {
        rotate([0, 0, winkel_ls]) {
            cube([rahmenstaerke, l, rahmenhoehe], center = true);
        }
    }

    difference() {
        translate([(max_gesamt_breite - speaker_center_cs25_fcr_mk3_breite_vorn() - rahmenstaerke) / 2, speaker_center_cs25_fcr_mk3_tiefe() / -2, 0]) {
            cube([(speaker_center_cs25_fcr_mk3_breite_vorn() + rahmenstaerke * 2), rahmenstaerke * 2, rahmenhoehe], center = true);
        }

        //wie berechnet man hier die 0.85
        translate([(x + rahmenstaerke + 0.85) * - 1, - rahmenstaerke, 0]) {
            rotate([0, 0, - winkel_ls]) {
                cube([rahmenstaerke, speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke, rahmenhoehe * 1.1], center = true);
            }
        }
    }
}

module rechte_abschraegungen() {
    //wie berechnet man hier die 0.85
    translate([(max_gesamt_breite - speaker_center_cs25_fcr_mk3_breite_vorn() - rahmenstaerke) + x + rahmenstaerke + 0.85, - rahmenstaerke, 0]) {
        rotate([0, 0, winkel_ls]) {
            cube([rahmenstaerke, (speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke) * 2, rahmenhoehe * 1.1], center = true);
        }
    }
}

module befestigung() {
    translate([(max_gesamt_breite - playseat_monitor_stand_bein_breite() - rahmenstaerke) / -2, rahmenstaerke * 1.25 + playseat_monitor_stand_bein_tiefe(), 0]) {
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

module kabelfuehrung() {
    union() {
        kabel_durchmesser = 2.5;
        mitte_stand = (max_gesamt_breite - playseat_monitor_stand_bein_breite() - rahmenstaerke) / -2;
        mitte_lautsprecher = 25;
        abstand_mitte_stand_mitte_lautsprecher = abs(mitte_stand - mitte_lautsprecher);

        translate([
                (max_gesamt_breite - breite_hinten + 5) / -2 ,
                (speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke + kabel_durchmesser - 0.9) / 2,
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
                (speaker_center_cs25_fcr_mk3_tiefe() + rahmenstaerke + kabel_durchmesser - 0.9) / 2,
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

module bein() {
    translate([(max_gesamt_breite - rahmenstaerke - playseat_monitor_stand_bein_breite()) / -2, (speaker_center_cs25_fcr_mk3_tiefe() - playseat_monitor_stand_bein_tiefe() - rahmenstaerke) / 2, 0]) {
        playseat_monitor_stand_bein();
    }
}

module speaker() {
    translate([(max_gesamt_breite - speaker_center_cs25_fcr_mk3_breite_vorn() - rahmenstaerke) / 2, 0, speaker_center_cs25_fcr_mk3_hoehe() / 2]) {
        speaker_satellite_cs25_fcr_mk3();
    }
}

