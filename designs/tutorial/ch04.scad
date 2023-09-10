//https://en.wikibooks.org/wiki/OpenSCAD_Tutorial/Chapter_4
$fa = 1;
$fs = 0.4;

base_height         = 10;
base_length         = 100;
top_height          = 14;
top_length          = 75;
top_offset          = 5;
track               = 40;
body_roll           = -5;

wheels_turn         = 20;

front_wheel_radius        = 10;
front_side_spheres_radius = 50;
front_hub_thickness       = 4;
front_cylinder_radius     = 2;

rear_wheel_radius        = 12;
rear_side_spheres_radius = 30;
rear_hub_thickness       = 8;
rear_cylinder_radius     = 3;

module wheel(wheel_radius=10, side_spheres_radius=50, hub_thickness=4, cylinder_radius=2) {
    cylinder_height=2*wheel_radius;
        difference() {
            // Wheel sphere
            sphere(r=wheel_radius);
            // Side sphere 1
            translate([0,side_spheres_radius + hub_thickness/2,0])
                sphere(r=side_spheres_radius);
            // Side sphere 2
            translate([0,- (side_spheres_radius + hub_thickness/2),0])
                sphere(r=side_spheres_radius);
            // Cylinder 1
            translate([wheel_radius/2,0,0])
                rotate([90,0,0])
                cylinder(h=cylinder_height,r=cylinder_radius,center=true);
            // Cylinder 2
            translate([0,0,wheel_radius/2])
                rotate([90,0,0])
                cylinder(h=cylinder_height,r=cylinder_radius,center=true);
            // Cylinder 3
            translate([-wheel_radius/2,0,0])
                rotate([90,0,0])
                cylinder(h=cylinder_height,r=cylinder_radius,center=true);
            // Cylinder 4
            translate([0,0,-wheel_radius/2])
                rotate([90,0,0])
                cylinder(h=cylinder_height,r=cylinder_radius,center=true);
    }
}

module body(base_height=10, top_height=14, base_length=60, top_length=30, width=20, top_offset=5) {
    // Car body base
    cube([base_length,width,base_height],center=true);
    // Car body top
    translate([top_offset,0,base_height/2+top_height/2 - 0.001])
        cube([top_length,width,top_height],center=true);
}

module axle(track=35, radius=2) {
    rotate([90,0,0])
        cylinder(h=track,r=radius,center=true);
}

//body
rotate([body_roll,0,0]) {
    body();
}

//front left wheel    
translate([-20,-track/2,0])
    rotate([0, 0, wheels_turn])
        wheel(front_wheel_radius, front_side_spheres_radius, front_hub_thickness, front_cylinder_radius);
//front right wheel
translate([-20,track/2,0])
    rotate([0, 0, wheels_turn])
        wheel(front_wheel_radius, front_side_spheres_radius, front_hub_thickness, front_cylinder_radius);

//front axle
translate([-20, 0, 0])
    axle();
//rear left wheel
translate([20,-track/2,0])
    wheel(rear_wheel_radius, rear_side_spheres_radius, rear_hub_thickness, rear_cylinder_radius);
//rear right wheel
translate([20,track/2,0])
    wheel(rear_wheel_radius, rear_side_spheres_radius, rear_hub_thickness, rear_cylinder_radius);
//rear axle
translate([20, 0, 0])
  axle();
