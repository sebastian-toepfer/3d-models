use <../../../parts/cable/channel.scad>
use <../../../parts/screw/hole.scad>
use <../../../things/playseat/monitorstand/playseat_monitor_stand_bein.scad>
use <../../../things/teufel_concept_e450/speaker_satellite_cs25_fcr_mk3.scad>

rahmenstaerke                     = 5;
rahmenhoehe                       = 14;

lautsprecher_tiefe                = 95;
lautsprecher_breite_vorn          = 101;
lautsprecher_breite_hinten        = 71;
lautsprecher_hoehe                = 140;

monitor_stand_breite              = 56;
monitor_stand_tiefe               = 34;

hinter_abstand_stand_lautsprecher = (lautsprecher_breite_vorn - lautsprecher_breite_hinten) / 2;

//ohne ueberhaenge ... aber warum
max_gesamt_breite                 = lautsprecher_breite_vorn + monitor_stand_breite;
breite_hinten                     = monitor_stand_breite + lautsprecher_breite_hinten + hinter_abstand_stand_lautsprecher;

winkel_ls = 90 - atan(lautsprecher_tiefe / ((lautsprecher_breite_vorn - lautsprecher_breite_hinten) / 2));
x = ((lautsprecher_tiefe + rahmenstaerke) * sin(winkel_ls)) / sin(90) + rahmenstaerke / 2;
l = sqrt(pow(x, 2) - 2 * x * (lautsprecher_tiefe + rahmenstaerke) * cos(90 - winkel_ls) + pow(lautsprecher_tiefe + rahmenstaerke, 2));

difference() {

    union() {
        translate([(max_gesamt_breite - rahmenstaerke) / -2, (lautsprecher_tiefe - rahmenhoehe) / 2, 0]) {
            cube([rahmenstaerke, rahmenhoehe, rahmenhoehe], center = true);
        }

        translate([(max_gesamt_breite - breite_hinten - 1) / -2, lautsprecher_tiefe / 2, 0]) {
            cube([breite_hinten + 1, rahmenstaerke * 2, rahmenhoehe], center = true);
        }

        translate([x * -1, 0, 0]) {
            rotate([0, 0, - winkel_ls]) {
                cube([rahmenstaerke, l, rahmenhoehe], center = true);
            }
        }

        translate([(max_gesamt_breite - lautsprecher_breite_vorn - rahmenstaerke) / 2, 0, 0]) {
            cube([rahmenstaerke * 3, lautsprecher_tiefe + rahmenstaerke, rahmenhoehe], center = true);
        }

        translate([(max_gesamt_breite - lautsprecher_breite_vorn - rahmenstaerke) + x, 0, 0]) {
            rotate([0, 0, winkel_ls]) {
                cube([rahmenstaerke, l, rahmenhoehe], center = true);
            }
        }

        difference() {
            translate([(max_gesamt_breite - lautsprecher_breite_vorn - rahmenstaerke) / 2, lautsprecher_tiefe / -2, 0]) {
                cube([(lautsprecher_breite_vorn + rahmenstaerke * 2), rahmenstaerke * 2, rahmenhoehe], center = true);
            }

            //wie berechnet man hier die 0.85
            translate([(x + rahmenstaerke + 0.85) * - 1, - rahmenstaerke, 0]) {
                rotate([0, 0, - winkel_ls]) {
                    cube([rahmenstaerke, lautsprecher_tiefe + rahmenstaerke, rahmenhoehe * 1.1], center = true);
                }
            }
        }
    }

    //wie berechnet man hier die 0.85
    translate([(max_gesamt_breite - lautsprecher_breite_vorn - rahmenstaerke) + x + rahmenstaerke + 0.85, - rahmenstaerke, 0]) {
        rotate([0, 0, winkel_ls]) {
            cube([rahmenstaerke, (lautsprecher_tiefe + rahmenstaerke) * 2, rahmenhoehe * 1.1], center = true);
        }
    }

    translate([(max_gesamt_breite - monitor_stand_breite - rahmenstaerke) / -2, rahmenstaerke * 1.25 + monitor_stand_tiefe, 0]) {
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

    union() {
        kabel_durchmesser = 2.5;
        mitte_stand = (max_gesamt_breite - monitor_stand_breite - rahmenstaerke) / -2;
        mitte_lautsprecher = 25;
        abstand_mitte_stand_mitte_lautsprecher = abs(mitte_stand - mitte_lautsprecher);

        translate([
                (max_gesamt_breite - breite_hinten + 5) / -2 ,
                (lautsprecher_tiefe + rahmenstaerke + kabel_durchmesser - 0.9) / 2,
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
                (lautsprecher_tiefe + rahmenstaerke + kabel_durchmesser - 0.9) / 2,
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

    translate([(max_gesamt_breite - rahmenstaerke - monitor_stand_breite) / -2, (lautsprecher_tiefe - monitor_stand_tiefe - rahmenstaerke) / 2, 0]) {
        playseat_monitor_stand_bein();
    }

    translate([(max_gesamt_breite - lautsprecher_breite_vorn - rahmenstaerke) / 2, 0, lautsprecher_hoehe / 2]) {
        speaker_satellite_cs25_fcr_mk3();
    }
}

