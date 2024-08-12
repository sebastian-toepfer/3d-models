$fa = 1;
$fs = 0.4;

module body(base_height=10, top_height=14, base_length=60, top_length=30, width=20, top_offset=5, top=true, front_bumper=false, rear_bumper=false) {
    // Car body base
    cube([base_length,width,base_height],center=true);
    // Car body top
    if (top) {
        translate([top_offset,0,base_height/2+top_height/2 - 0.001]) {
            cube([top_length,width,top_height],center=true);
        }
    }
    // front bumper
    if (front_bumper) {
        color("blue") {
            translate([base_length/-2,0,0])rotate([90,0,0]) {
                cylinder(h=width - base_height,r=base_height/2,center=true);
                translate([0,0,(width - base_height)/2])
                    sphere(r=base_height/2);
                translate([0,0,-(width - base_height)/2])
                    sphere(r=base_height/2);
            }
        }
    }
    // Rear bumper
    if (rear_bumper) {
        color("blue") {
            translate([base_length/2,0,0])rotate([90,0,0]) {
                cylinder(h=width - base_height,r=base_height/2,center=true);
                translate([0,0,(width - base_height)/2])
                    sphere(r=base_height/2);
                translate([0,0,-(width - base_height)/2])
                    sphere(r=base_height/2);
            }
        }
    }
}

module simple_wheel(wheel_radius = 10, wheel_width = 6) {
    rotate([90, 0, 0]) {
        cylinder(h = wheel_width, r = wheel_radius, center = true);
    }
}

module complex_wheel(wheel_radius=10, side_spheres_radius=50, hub_thickness=4, cylinder_radius=2) {
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

module spoked_wheel(radius=12, width=5, thickness=5, number_of_spokes=7, spoke_radius=1.5) {
    rotate([90, 0, 0]) {
        // Ring
        inner_radius = radius - thickness/2;
        difference() {
            cylinder(h=width,r=radius,center=true);
            cylinder(h=width + 1,r=inner_radius,center=true);
        }
        // Spokes
        spoke_length = radius - thickness/4;
        for (angle=[0:360/number_of_spokes:359]) {
            rotate([0, 90, angle]) {
                cylinder(r = spoke_radius, h = spoke_length);
            }
        }
   }
}

module axle(track=35, radius=2) {
    rotate([90,0,0])
        cylinder(h=track,r=radius,center=true);
}

module axle_wheelset(track=35, radius=2, wheel_turn = 0) {
    translate([0,track/2,0])
        rotate([0, 0, wheel_turn]) {
            children(0);
        }
        axle(track=track, radius=radius);
        translate([0,-track/2,0])
            rotate([0, 0, wheel_turn]) {
                children(1);
            }
}

