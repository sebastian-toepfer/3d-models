include <../libs/own/mirror_copy.fuc>
use <gehaeuse_unterseite.scad>

function gehaeuse_oberseite_dimension() = [
   105,
   90,
   50.6
];

gehaeuse_oberseite();

module gehaeuse_oberseite(
    dimension   = gehaeuse_oberseite_dimension(),
    wandstaerke = 2,
    center      = true
) {
    difference() {
        korpus(dimension = dimension, center = center);
        translate([0, 0, - wandstaerke - 0.01]) {
            korpus(
                dimension = [
                    dimension.x - wandstaerke * 2,
                    dimension.y - wandstaerke * 2,
                    dimension.z + 0.02
                ],
                center = center
            );
        }

        translate([0, 0, dimension.z / -2 - 4.0]) {
            gehaeuse_unterseite();
        }

        mirror_copy([0, 1, 0]) {
            mirror_copy() {
                translate([
                    (dimension.x - wandstaerke * 2) / 2 + 0.25,
                    dimension.y / 4,
                    -25.25
                ]) {
                    cube([0.5 + 0.01, 5, 5], center = true);
                }
            }
        }
    }

    module korpus(dimension = [10, 5, 10], center = true) {
        rotate([0, 90, 0]) {
            linear_extrude(height = dimension.x, center = center) {
                mirror_copy([0, 1, 0]) {
                    polygon(
                      points = [
                        [dimension.z / 2, 0],
                        [dimension.z / 2, dimension.y / 2],
                        [dimension.z / 2 - 20, dimension.y / 2],
                        [dimension.z / 2 - 20, dimension.y / 2 - 13.175],
                        [dimension.z / 2 - 20 - 20, dimension.y / 2 - 13.175],
                        [dimension.z / 2 - 20 - 20, dimension.y / 2 - 13.175 - 9.25],
                        [dimension.z / 2 * -1, dimension.y / 2 - 13.175 - 9.25 ],
                        [dimension.z / 2 * -1, 0],
                      ]
                    );
                }
            }
        }
    }
}

