use <../../../libs/own/mirror_copy.fuc>
use <../../../parts/screw/hole.scad>
use <../../../things/playseat/monitorstand/playseat_monitor_stand_bein.scad>

rahmenstaerke        = 5;
rahmenhoehe          = 15;
rahmentiefe          = 20;

satellite_mounting();

module satellite_mounting() {
    difference() {
        basis();
        union() {
            mirror_copy([0, 1, 0]) {
                aussparrung_befestigung();
            }
            aussparrung_halter();
            mirror_copy([1, 0, 0]) {
                aussparrung_seite();
            }
            aussparrung_mitte();
            aussparrung_bein();
        }
    }
}

module basis() {
    cube([
            playseat_monitor_stand_bein_breite() + rahmenstaerke,
            rahmenhoehe * 3.5,
            rahmentiefe
        ],
        center = true
    );
}

module aussparrung_befestigung() {
    translate([0, rahmenhoehe + 5, rahmenstaerke / 2]) {
        cube([
                playseat_monitor_stand_bein_breite() - rahmenstaerke,
                rahmenhoehe,
                rahmentiefe - rahmenstaerke + 0.01
            ],
            center = true
        );
        translate([0, 0, rahmenstaerke * -1]) {
            rotate([180, 0, 0]) {
                screw_hole(
                        length = rahmenstaerke * 2,
                        diameter = 4.5,
                        head_diameter = 10,
                        head_length = 2.5,
                        center = true
                );
            }
        }
    }
}

module aussparrung_halter() {
    union() {
        translate([0, 0, -5]) {
            cube([
                    (playseat_monitor_stand_bein_breite() + rahmenstaerke) * 1.1,
                    rahmenhoehe,
                    10.01
                ],
                center = true
            );
        }

        translate([playseat_monitor_stand_bein_breite() / 2 + 0.001, 0, 0]) {
            cube([rahmenstaerke + 0.02, rahmenhoehe, rahmentiefe * 1.1], center = true);
        }
    }
}

module aussparrung_seite() {
    translate([19, 0, 5]) {
        cube([
                (playseat_monitor_stand_bein_breite() - playseat_monitor_stand_bein_breite_luecke()) / 2,
                rahmenhoehe * 3.6,
                rahmentiefe
            ],
            center = true
        );
    }
}

module aussparrung_mitte() {
    union() {
        translate([0, 0, 10 + 5 / 2]) {
            cube([
                    playseat_monitor_stand_bein_breite_luecke(),
                    rahmenhoehe * 3.6,
                    rahmentiefe - 5
                ],
                center = true
            );
        }
        cylinder(
            d = 5.6,
            h = rahmenstaerke * 3 + 1.1,
            center = true
        );
    }
}

module aussparrung_bein() {
    translate([0, 0, 10]) {
        rotate([270, 0, 0]) {
            playseat_monitor_stand_bein();
        }
    }
}

