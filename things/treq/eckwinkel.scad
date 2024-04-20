module eckwinkel() {
    groesse      = 36;
    tiefe        = 38;
    wand_staerke = 4.5;
  
    rotate([270, 0, 0]) {
        difference() {
            linear_extrude(height = tiefe, center = true) {
                polygon(points = [
                        [groesse / - 2, groesse / + 2],
                        [groesse / + 2, groesse / + 2],
                        [groesse / + 2, groesse / + 2 - wand_staerke],
                        [groesse / - 2 + wand_staerke, groesse / - 2],
                        [groesse / - 2, groesse / - 2]
                ]);
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

