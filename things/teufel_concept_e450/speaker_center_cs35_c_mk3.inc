use <../../libs/own/mirror_copy.fuc>
use <../../libs/own/sector_arc.fuc>

module speaker_center_cs35_c_mk3() {
  hoehe_vorn         = 101; //schraeg
  hoehe_hinten       = 71;  //schraeg
  tiefe              = 95;
  breite             = 225;
  radius_abrundungen = 5;

  rotate([0, 90, 0]) {
    rotate([0, 0, -8.973]) {
      difference() {
        linear_extrude(height = breite, center = true) {
          difference() {
            polygon(points = [
                [hoehe_hinten / -2, tiefe / 2], [hoehe_hinten / 2, tiefe / 2],
                [hoehe_vorn / 2, tiefe / -2], [hoehe_vorn / -2, tiefe / -2]
            ]);
            mirror_copy([1, 0, 0]) {
              //mit welcher funktion kommen wir hier auf die ausgewuerfelte 4?
              translate([hoehe_hinten / -2 + 4, tiefe / 2 - radius_abrundungen, 0]) {
                arc(radius = radius_abrundungen, angles = [90, 180], width = radius_abrundungen);
              };
            };
          };
        };
        mirror_copy([0, 0, 1]) {
          translate([0, tiefe/ -2 + radius_abrundungen, breite/ 2 - radius_abrundungen]) {
            rotate([0, 90, 0]) {
              linear_extrude(height = hoehe_vorn * 1.1, center = true) {
                arc(radius = radius_abrundungen, angles = [180, 270], width = radius_abrundungen);
              };
            };
          };
        };
      };
    };
  };
}
