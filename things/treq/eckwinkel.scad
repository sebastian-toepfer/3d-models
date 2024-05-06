use <../../libs/own/mirror_copy.fuc>

module eckwinkel(seitlicher_verdrehschutz = false, unterer_verdrehschutz = false) {
    groesse      = 36;
    tiefe        = 38;
    wand_staerke = 4.5;
  
    translate([seitlicher_verdrehschutz ? 1 : 0, 0, unterer_verdrehschutz ? 1 : 0]) {
        rotate([270, 0, 0]) {
            difference() {
                union() {
                    linear_extrude(height = tiefe, center = true) {
                        polygon(points = [
                                [groesse / - 2, groesse / + 2],
                                [groesse / + 2, groesse / + 2],
                                [groesse / + 2, groesse / + 2 - wand_staerke],
                                [groesse / - 2 + wand_staerke, groesse / - 2],
                                [groesse / - 2, groesse / - 2]
                        ]);
                    }
                    if (seitlicher_verdrehschutz) {
                        mirror_copy([0, 1, 0]) {
                            translate([(tiefe + 2) / -2, groesse / 2 - 2, 0]) {
                                rotate([0, 0, 270]) {
                                    linear_extrude(height = 8, center = true) {
                                        polygon(points = [
                                                [0, 0], [0, 2], [4, 2]
                                        ]);
                                    }
                                }
                            }
                        }
                    }

                    if (unterer_verdrehschutz) {
                        mirror_copy([1, 0, 0]) {
                            translate([groesse / 2 - 2, (tiefe + 2) / 2, 0]) {
                                rotate([0, 0, 180]) {
                                    linear_extrude(height = 8, center = true) {
                                        polygon(points = [
                                                [0, 0], [0, 2], [4, 2]
                                        ]);
                                    }
                                }
                            }
                        }
                    }
                }
                union() {
                    translate([wand_staerke, - wand_staerke, 0]) {
                        cube([groesse, groesse, tiefe - wand_staerke * 2], center = true);
                    }
                    translate([0, (groesse - wand_staerke)/ 2, 0]) {
                        cube([groesse - wand_staerke * 4, wand_staerke * 1.1, wand_staerke * 2], center = true);
                    }
                    translate([(groesse - wand_staerke)/ - 2, 0, 0]) {
                        cube([wand_staerke * 1.1, groesse - wand_staerke * 4, wand_staerke * 2], center = true);
                    }
                }
            }
        }
    }
}

