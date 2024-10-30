loewetv_klappen_befestigung();

module loewetv_klappen_befestigung() {
    $fn = 50;
    dicke         = 2;
    tv_laenge     = 12;
    tv_breite     = 11.4;
    klappe_breite = 15.8;
    //verschieben damit es mittig ist (ist so einfacher gewesen :()
    translate([0, 0, (tv_laenge + dicke) / -2]) {
        difference() {
            rotate([0, 270, 0]) {
                union() {
                    translate([0, 2.5, 0]) {
                        cylinder(d = dicke, h = klappe_breite, center = true);
                        translate([tv_laenge / 2, 0, 0]) {
                            cube([tv_laenge, dicke, tv_breite], center = true);
                        }
                        //die 7.38 verstehe ich gar nicht :(
                        //die 1.3 auch nicht :(
                        translate([7.38, 1.3, 0]) {
                            cube([3.75, 0.75, 3.5], center = true);
                        }
                    }
                    cube([dicke, 5, klappe_breite], center = true);
                    translate([0, -2.5, 0]) {
                        cylinder(d = dicke, h = klappe_breite + 2, center = true);
                    }
                }
            }

            translate([0, 0, (6.5 - dicke) / 2]) {
                cube([3.5, (5 + dicke) * 1.1, 6.51], center = true);
            }
        }
    }
}

