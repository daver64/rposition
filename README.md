# rposition

A header-only C++ library for 3D position representation with seamless conversion between Cartesian, spherical, and geographic coordinate systems.

## Features

- **Multiple Coordinate Systems**
  - Cartesian (x, y, z)
  - Spherical (radius, inclination, azimuth)
  - Geographic (latitude, longitude, radius)

- **Automatic Synchronization**
  - Set coordinates in any system, query in any other
  - Internal conversions handled automatically

- **Velocity & Heading**
  - Speed and heading properties with degree/radian support
  - Movement in flat 3D space or on spherical surfaces
  - Time-based position updates

- **Movement Operations**
  - Direct translation
  - Forward movement
  - Yaw and pitch adjustments
  - Spherical surface navigation

## Usage

Include the header in your project:

```cpp
#include "rposition.h"
```

### Basic Examples

#### Cartesian Coordinates

```cpp
Position3D pos;
pos.set_cartesian(1.0, 1.0, 1.0);

// Query individual components
double x = pos.x();
double y = pos.y();
double z = pos.z();

// Or get all at once
double x, y, z;
pos.get_cartesian(x, y, z);
```

#### Spherical Coordinates

```cpp
Position3D pos;
pos.set_spherical(2.0, 1.0, 1.0);  // (radius, theta, phi)

double r, theta, phi;
pos.get_spherical(r, theta, phi);
```

#### Geographic Coordinates

```cpp
Position3D pos;
pos.set_geographic(51.46, -2.12, 6371.0);  // Chippenham, UK

std::cout << "Latitude: " << pos.latitude_deg() << "°\n";
std::cout << "Longitude: " << pos.longitude_deg() << "°\n";
```

#### Velocity and Movement

```cpp
Position3D pos;
pos.set_cartesian(0, 0, 0);

// Set velocity: speed, heading (0°=East, 90°=North), vertical angle
pos.set_velocity_deg(10.0, 45.0, 0.0);

// Update position after 5 seconds
pos.update(5.0);
```

#### Movement on Spherical Surface

```cpp
Position3D pos;
pos.set_geographic(51.46, -2.12, 6371.0);  // Start at Chippenham

// Move east at 100 km/h for 1 hour
pos.set_velocity_deg(100.0, 0.0, 0.0);
pos.update_on_sphere(1.0);

std::cout << "New position: " << pos.latitude_deg() << "°, " 
          << pos.longitude_deg() << "°\n";
```

## Coordinate Systems

### Cartesian
- **x, y, z**: Standard 3D Cartesian coordinates

### Spherical (Mathematical Convention)
- **r**: Radius (distance from origin)
- **θ (theta)**: Inclination angle (0 = +Z axis, π = -Z axis)
- **φ (phi)**: Azimuth angle (counterclockwise from +X axis)

### Geographic
- **Latitude**: -90° (South Pole) to +90° (North Pole)
- **Longitude**: -180° to +180° (or 0° to 360°)
- **Radius**: Distance from center (e.g., 6371 km for Earth's surface)

### Heading
- **0°**: East (+X direction)
- **90°**: North (+Y direction)
- **180°**: West (-X direction)
- **270°**: South (-Y direction)

## API Reference

### Constructors

- `Position3D()` - Default constructor (origin)
- `Position3D(double x, double y, double z)` - Cartesian constructor
- `Position3D(double r, double theta, double phi, bool spherical)` - Spherical constructor

### Cartesian Accessors

- `double x()`, `double y()`, `double z()` - Get individual components
- `void set_cartesian(double x, double y, double z)` - Set position
- `void get_cartesian(double& x, double& y, double& z)` - Get all components

### Spherical Accessors

- `double r()`, `double theta()`, `double phi()` - Get individual components
- `void set_spherical(double r, double theta, double phi)` - Set position
- `void get_spherical(double& r, double& theta, double& phi)` - Get all components

### Geographic Accessors

- `double latitude_deg()` - Get latitude in degrees
- `double longitude_deg()` - Get longitude in degrees
- `void set_geographic(double lat, double lon, double radius)` - Set position

### Velocity & Heading

- `double speed()`, `double heading()`, `double vertical_angle()` - Get velocity components
- `double heading_deg()`, `double vertical_angle_deg()` - Get angles in degrees
- `void set_speed(double speed)` - Set speed only
- `void set_heading(double rad)` / `void set_heading_deg(double deg)` - Set heading
- `void set_velocity(double speed, double heading, double v_angle)` - Set all (radians)
- `void set_velocity_deg(double speed, double heading, double v_angle)` - Set all (degrees)

### Movement

- `void translate(double dx, double dy, double dz)` - Move by offset
- `void move_forward(double distance)` - Move in current spherical direction
- `void yaw(double dphi)` - Rotate horizontally
- `void pitch(double dtheta)` - Rotate vertically
- `void update(double dt)` - Update position in flat 3D space based on velocity
- `void update_on_sphere(double dt)` - Update position on spherical surface (maintains radius)

### Utility

- `void align_heading_to_phi()` - Sync heading with azimuth angle
- `void align_heading_to_spherical()` - Sync heading and vertical angle with spherical coords
- `void print()` - Print all coordinate representations

## Building

Compile with C++11 or later:

```bash
g++ example.cpp -o example -I./include
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.