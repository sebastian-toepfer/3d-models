use <./speaker_common.scad>

module speaker_center_cs35_c_mk3() {
  hoehe_vorn         = 101; //schraeg
  hoehe_hinten       = 71;  //schraeg
  tiefe              = 95;
  breite             = 225;
  radius_abrundungen = 5;

  rotate([0, 90, 0]) {
    rotate([0, 0, -8.973]) {
      union() {
        color("black") {
          wall(hoehe_vorn, hoehe_hinten, tiefe, breite, 5, radius_abrundungen);
        }
        color("grey") {
          translate([0, 5 / -2, 0]) {
            inlay(hoehe_vorn - 5 * 2, hoehe_hinten - 5 * 2, tiefe - 5, breite);
          };
        };
      };
    };
  };
};

