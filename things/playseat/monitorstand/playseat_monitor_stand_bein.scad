use <../../../libs/own/sector_arc.fuc>
use <../../../libs/own/mirror_copy.fuc>

playseat_monitor_stand_bein();

module playseat_monitor_stand_bein() {
    breite      = 56;
    tiefe       = 34;
    hoehe       = 780;
    wandstaerke = 2.6;
  
    color("silver") {
        difference() {
            cube([breite, tiefe, hoehe], center = true);

            union() {
                cube([breite - wandstaerke * 2, tiefe - wandstaerke * 2, hoehe * 1.1], center = true);
                translate([0, 10, 0]) {
                    cube([26, tiefe, hoehe * 1.1], center = true);
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

