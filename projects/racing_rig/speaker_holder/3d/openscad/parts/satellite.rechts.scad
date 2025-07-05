use <satellite.template.scad>

satellite_rechts();

module satellite_rechts() {
   mirror([1, 0, 0]) {
       satellite_template();
   }
}

