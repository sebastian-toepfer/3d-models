magnetic_tube_holder();

module magnetic_tube_holder(
   wandstaerke          = 3,
   schlauch_durchmesser = 10,
   magnet_durchmesser   = 10,
   magnet_dicke         = 3
) {
    union() {
        translate([0, 0, (wandstaerke * 1.5 + magnet_dicke) / 2]) {
            schlauch_schelle(
                wandstaerke = wandstaerke,
                schlauch_durchmesser = schlauch_durchmesser
            );
            translate([
                0,
                0,
                (
                  schlauch_durchmesser
                  + wandstaerke * 2.5
                  + magnet_dicke
                ) / -2
            ]) {
                magnet_gehaeuse(
                    wandstaerke = wandstaerke,
                    magnet_durchmesser = magnet_durchmesser,
                    magnet_dicke       = magnet_dicke
                );
            }
        }
    }
}

module schlauch_schelle(
   wandstaerke          = 1,
   schlauch_durchmesser = 5
) {
    difference() {
        rotate([90, 0, 0]) {
            difference() {
                cylinder(
                    d = schlauch_durchmesser + wandstaerke * 2,
                    h = wandstaerke,
                    center = true
                );
                cylinder(
                    d = schlauch_durchmesser,
                    h = wandstaerke * 1.1,
                    center = true
                );
            }
        }
        translate([
            0,
            0,
            wandstaerke
        ])
        cube([
                schlauch_durchmesser * 0.6,
                wandstaerke * 1.1,
                schlauch_durchmesser
            ],
            center = true
        );
    }
}

module magnet_gehaeuse(
   wandstaerke          = 1,
   magnet_durchmesser   = 5,
   magnet_dicke         = 1
) {
    difference() {
        cylinder(
            h = magnet_dicke + wandstaerke,
            d = magnet_durchmesser + wandstaerke * 2,
            center = true
        );
        translate([0, 0, wandstaerke / -2 + 0.04]) {
            cylinder(h = magnet_dicke + 0.1, d = magnet_durchmesser, center = true);
        }
    }
}

