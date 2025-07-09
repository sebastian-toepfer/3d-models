// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <../../../libs/own/sector_arc.fuc>
use <../../../libs/own/mirror_copy.fuc>

function playseat_monitor_stand_bein_dimension_innen() = [
    playseat_monitor_stand_bein_dimension().x - playseat_monitor_stand_bein_wandstaerke() * 2,
    playseat_monitor_stand_bein_dimension().y - playseat_monitor_stand_bein_wandstaerke() * 2,
    playseat_monitor_stand_bein_dimension().z - playseat_monitor_stand_bein_wandstaerke() * 2
];

function playseat_monitor_stand_bein_dimension() = [
    playseat_monitor_stand_bein_breite(),
    playseat_monitor_stand_bein_tiefe(),
    playseat_monitor_stand_bein_hoehe()
];

function playseat_monitor_stand_bein_breite() = 56;
function playseat_monitor_stand_bein_tiefe() = 34;
function playseat_monitor_stand_bein_hoehe() = 780;
function playseat_monitor_stand_bein_breite_luecke() = 26;
function playseat_monitor_stand_bein_wandstaerke() = 2.6;

module playseat_monitor_stand_bein() {
    breite      = playseat_monitor_stand_bein_breite();
    tiefe       = playseat_monitor_stand_bein_tiefe();
    hoehe       = playseat_monitor_stand_bein_hoehe();
    wandstaerke = playseat_monitor_stand_bein_wandstaerke();
  
    color("silver") {
        difference() {
            cube([breite, tiefe, hoehe], center = true);

            union() {
                cube([breite - wandstaerke * 2, tiefe - wandstaerke * 2, hoehe * 1.1], center = true);
                translate([0, 10, 0]) {
                    cube([playseat_monitor_stand_bein_breite_luecke(), tiefe, hoehe * 1.1], center = true);
                }
                translate([0, tiefe / -2, hoehe / 2 - 30]) {
                    rotate([90, 0, 90]) {
                        linear_extrude(height = breite * 1.1, center = true) {
                            polygon(points = [
                                    [  0, 40], [ 40, 40],
                                    [ 40,  0], [ 30,  0],
                                    [ 15, 30], [  0, 30]
                            ]);
                        }
                    }
                }
            }

            mirror_copy([0, 1, 0]) {
                mirror_copy() {
                    linear_extrude(height = hoehe, center = true) {
                        translate([breite / 2 - 5, tiefe / 2 - 5]) {
                            arc(radius = 5, angles = [0, 90], width = 5 * 1.1);
                        }
                    }
                }
            }
        }
    }
}

