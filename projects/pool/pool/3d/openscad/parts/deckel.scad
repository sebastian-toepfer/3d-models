use <../libs/own/mirror_copy.fuc>

skimmer_abdeckung();

module skimmer_abdeckung(
    groesse                 = 213,
    dicke                   = 3.55,
    rundungen_radius        = 5,
    ring_aussen_durchmesser = 200,
    ring_wandstaerke        = 3,
    ring_hoehe              = 10
) {
    union() {
        grundplatte(
            dimension=[
                groesse,
                groesse,
                dicke
            ],
            rundungen_radius = rundungen_radius
        );
        translate([0, 0, (dicke + ring_hoehe)/ 2]) {
            ring(
                aussen_durchmesser = ring_aussen_durchmesser,
                wandstaerke = ring_wandstaerke,
                hoehe = ring_hoehe
            );
        }
    }

    module grundplatte(dimension=[100, 100, 5], rundungen_radius = 2) {
        difference() {
            hull() {
                mirror_copy([0, 1, 0]) {
                    mirror_copy([1, 0, 0]) {
                       translate([
                           dimension.x / 2 - rundungen_radius,
                           dimension.y / 2 - rundungen_radius,
                           0
                       ]) {
                           cylinder(
                               r = rundungen_radius,
                               h = dimension.z,
                               center = true
                           );
                       }
                    }
                }
            }

            cylinder(d = 20, h = dimension.z * 1.1, center = true);
        }
    }

    module ring(aussen_durchmesser = 20, wandstaerke = 1, hoehe = 10) {
       difference() {
           cylinder(d = aussen_durchmesser, h = hoehe, center = true);
           cylinder(
               d = aussen_durchmesser - wandstaerke * 2,
               h = hoehe * 1.1,
               center = true
           );
       }
    }
}

