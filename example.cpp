#include "rposition.h"

int main(int argc, char *argv[])
{
    Position3D pos;

    // Set Cartesian coordinates
    pos.set_cartesian(1.0, 1.0, 1.0);

    // Get and print Spherical coordinates
    double r, theta, phi;
    pos.get_spherical(r, theta, phi);
    // std::cout << "Spherical coordinates after setting Cartesian (1,1,1):\n";
    // std::cout << "r: " << r << ", theta: " << theta << ", phi: " << phi << "\n";

    pos.print();
    // Set Spherical coordinates
    pos.set_spherical(2.0, 1.0, 1.0);

    // Get and print Cartesian coordinates
    double x, y, z;
    pos.get_cartesian(x, y, z);
    // std::cout << "Cartesian coordinates after setting Spherical (2,1,1):\n";
    // std::cout << "x: " << x << ", y: " << y << ", z: " << z << "\n";

    pos.print();

    // Set Geographic coordinates (latitude, longitude, radius)
    std::cout << "\n--- Geographic Coordinates Demo ---\n";
    pos.set_geographic(51.46, -2.12, 6371.0); // Chippenham, Wiltshire, UK on Earth's surface
    std::cout << "After setting geographic (lat=51.46°, lon=-2.12°, r=6371km):\n";
    std::cout << "Latitude: " << pos.latitude_deg() << "°, Longitude: " << pos.longitude_deg() << "°\n";
    pos.get_cartesian(x, y, z);
    std::cout << "Cartesian: x=" << x << ", y=" << y << ", z=" << z << "\n";

    // Velocity and heading demo
    std::cout << "\n--- Velocity and Heading Demo (Flat Space) ---\n";
    pos.set_cartesian(0, 0, 0); // Start at origin
    std::cout << "Starting at origin (0, 0, 0)\n";

    // Set heading to 45 degrees (northeast) with speed of 10 units/sec
    pos.set_velocity_deg(10.0, 45.0, 0.0);
    std::cout << "Set velocity: speed=10, heading=45° (northeast), vertical=0°\n";

    // Update position after 5 seconds
    pos.update(5.0);
    pos.get_cartesian(x, y, z);
    std::cout << "After 5 seconds: x=" << x << ", y=" << y << ", z=" << z << "\n";
    std::cout << "Speed: " << pos.speed() << ", Heading: " << pos.heading_deg() << "°\n";

    // Change heading to north (90 degrees) and update for 3 more seconds
    pos.set_heading_deg(90.0);
    std::cout << "\nChanged heading to 90° (north)\n";
    pos.update(3.0);
    pos.get_cartesian(x, y, z);
    std::cout << "After 3 more seconds: x=" << x << ", y=" << y << ", z=" << z << "\n";

    // Spherical surface movement demo
    std::cout << "\n--- Movement on Sphere Surface (Earth) ---\n";
    pos.set_geographic(51.46, -2.12, 6371.0); // Start at Chippenham
    std::cout << "Starting at Chippenham: lat=" << pos.latitude_deg()
              << "°, lon=" << pos.longitude_deg() << "°, r=" << pos.r() << " km\n";

    // Move east at 100 km/h for 1 hour
    pos.set_velocity_deg(100.0, 0.0, 0.0); // heading 0° = east
    std::cout << "\nMoving east at 100 km/h for 1 hour\n";
    pos.update_on_sphere(1.0);
    std::cout << "New position: lat=" << pos.latitude_deg()
              << "°, lon=" << pos.longitude_deg() << "°, r=" << pos.r() << " km\n";

    // Move north at 100 km/h for 1 hour
    pos.set_heading_deg(90.0); // heading 90° = north
    std::cout << "\nMoving north at 100 km/h for 1 hour\n";
    pos.update_on_sphere(1.0);
    std::cout << "New position: lat=" << pos.latitude_deg()
              << "°, lon=" << pos.longitude_deg() << "°, r=" << pos.r() << " km\n";

    return 0;
}