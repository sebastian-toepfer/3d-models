include <../libs/own/mirror_copy.fuc>

function pcb_dimension() = [
    100,
    85,
    1.6
];

function gehaeuse_unterseite_dimension(pcb_dimension = pcb_dimension(), wandstaerke = 1) = [
   ///5.08 entspricht einer TE -> wir wollen 1/4 TE's nutzen
   ceil((pcb_dimension.x * 1.01 + wandstaerke * 4) / (5.08 / 4)) * (5.08 / 4),
   pcb_dimension.y * 1.01 + wandstaerke * 4,
   7.8
];

gehaeuse_unterseite();

module gehaeuse_unterseite(
    pcb_dimension = pcb_dimension(),
    wandstaerke   = 1,
    ueberhang     = 5
) {
    dimension   = gehaeuse_unterseite_dimension(pcb_dimension, wandstaerke);
    difference() {
        korpus(dimension = dimension, wandstaerke = wandstaerke, ueberhang = ueberhang);
        translate([0, 0, dimension.z / 2]) {
            innenraum(
                dimension = [
                  pcb_dimension.x * 1.01,
                  pcb_dimension.y * 1.01,
                  ueberhang + 0.1
                ],
                board_befestigung = 10
            );
        }
        scale([1.01, 1.01, 1.01]) {
            translate([0, 0, (ueberhang + dimension.z) / 2 - pcb_dimension.z]) {
                import("../../stl/board_pumpensteuerung_tht.stl");
            }
        }
        //die 5 sind die hutschiene ... das muss noch besser gehen!!
        translate([0, 0, (ueberhang + dimension.z - 5) / -2]) {
            hutschienen_befestigung(dimension = dimension);
        }
    }

    module korpus(dimension = [10, 10, 10], wandstaerke = 1, ueberhang = 2) {
        union() {
            cube(
                [
                    dimension.x - wandstaerke * 2 - 0.3,
                    dimension.y - wandstaerke * 2 - 0.3,
                    dimension.z + ueberhang
                ],
                center = true
            );
            translate([0, 0, ueberhang / -2]) {
                cube(dimension, center = true);

                translate([0, 0, dimension.z / 2 + 1.5]) {
                    mirror_copy([0, 1, 0]) {
                        mirror_copy() {
                            translate([
                                dimension.x / -2  + wandstaerke - 1,
                                dimension.y / 4,
                                0
                            ]) {
                                rotate([270, 270, 0]) {
                                    linear_extrude(3.5, center = true) {
                                        polygon(
                                            points = [
                                                [0, 0],
                                                [0, 1],
                                                [1, 1],
                                            ]
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    module innenraum(dimension = [10, 10, 10], board_befestigung = 8) {
        difference() {
            cube(
                [
                    dimension.x,
                    dimension.y,
                    dimension.z
                ],
                center = true
            );

            mirror_copy([0, 1, 0]) {
                mirror_copy() {
                    translate([
                        (dimension.x - board_befestigung) / 2,
                        (dimension.y - board_befestigung) / 2,
                        - 1 
                    ]) {
                        difference() {
                            cube(
                                [
                                  board_befestigung,
                                  board_befestigung,
                                  dimension.z - 1.3
                                ],
                                center = true
                            );
                            translate([1, 1, 0.1]) {
                                cylinder(d = 3.5, h = dimension.z, center = true);
                            }
                        }
                    }
                }
            }
        }
    }

    module hutschienen_befestigung(dimension = [10, 10, 10]) {
        hutschienen_breite = 36;
        hutschienen_hoehe  = 5.02;
        feder_breite       = 11;
        // aussparrung feder ...
        translate([0, -17.5, (dimension.z - 5) / -2]) {
            mirror_copy() {
                difference() {
                    linear_extrude(hutschienen_hoehe, center = true) {
                        polygon(
                            points = [
                              [ 0, 0.1],
                              [ 15, 0.1],
                              [ 10, -15],
                              [ feder_breite / 2 + 1.5, -15],
                              [ feder_breite / 2 + 1.5, -5],
                              [ 0, -5]
                            ]
                        );
                    }
                    translate([13.5, -5, 0]) {
                        cylinder(d = 4, h = hutschienen_hoehe, center = true);
                    }
                }
            }
            translate([0, -15, 0]) {
                cube([feder_breite, 5, hutschienen_hoehe], center = true);
            }
        }
 
        //federfuehrungsaussparrung
        translate([0, (dimension.y - hutschienen_breite) / -4, 0.4]) {
            cube([feder_breite, hutschienen_breite + (dimension.y - hutschienen_breite) / 2 + 0.01, 3.5], center = true);
        }

        //hutschienenaussparrung ...
        translate([0, 0, (dimension.z - 5) / -2]) {
            difference() {
                cube([dimension.x * 1.1, hutschienen_breite, hutschienen_hoehe], center = true);
                mirror_copy() {
                    for( i = [10:20:dimension.x / 2]) {
                        translate([i, hutschienen_breite / 2 - 3.5 , -2.5]) {
                            rotate([90, 0, 90]) {
                                linear_extrude(3.5, center = true) {
                                    polygon(
                                        points = [
                                            [0, 0],
                                            [3.5, 0],
                                            [3.5, 4],
                                            [0, 1]
                                        ]
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
