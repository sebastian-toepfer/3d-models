use <../parts/gehaeuse_oberseite.scad>
use <../parts/gehaeuse_unterseite.scad>

gesamt_hoehe    = gehaeuse_oberseite_dimension().z + gehaeuse_unterseite_dimension().z;
oberteil_hoehe  = gehaeuse_oberseite_dimension().z;
unterteil_hoehe = gehaeuse_unterseite_dimension().z;

translate([0, 0, gehaeuse_unterseite_dimension().z / 2]) {
    gehaeuse_oberseite();
}

translate([0, 0, gehaeuse_oberseite_dimension().z / - 2 + gehaeuse_unterseite_dimension().z]) {
    import("../../stl/board_pumpensteuerung_tht.stl");
}
translate([0, 0, gehaeuse_oberseite_dimension().z / - 2]) {
   gehaeuse_unterseite();
}

