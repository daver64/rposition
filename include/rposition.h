#pragma once
#include <cmath>
#include <iostream>

inline double normalize_angle(double a)
{
    const double TWO_PI = 6.283185307179586;
    while (a < 0)
        a += TWO_PI;
    while (a >= TWO_PI)
        a -= TWO_PI;
    return a;
}

inline double clamp(double v, double mn, double mx)
{
    return (v < mn ? mn : (v > mx ? mx : v));
}

class Position3D
{
private:
    // Cartesian
    double x_, y_, z_;

    // Spherical (mathematical)
    // r  = radius
    // theta = inclination (0=+Z, π=-Z)
    // phi   = azimuth
    double r_;
    double theta_;
    double phi_;

    // Velocity/Heading
    double speed_;          // units per time (e.g., m/s, km/h)
    double heading_;        // horizontal direction (radians, 0=+X, counterclockwise)
    double vertical_angle_; // vertical angle (radians, 0=horizontal, +π/2=up, -π/2=down)

    // Guard flag to prevent infinite recursion during coordinate system conversions
    // (e.g., cartesian → spherical → cartesian)
    bool updating_ = false;

    //
    // INTERNAL CONVERSIONS
    //

    // Cartesian → spherical
    void update_spherical_from_cartesian()
    {
        if (updating_)
            return;
        updating_ = true;

        r_ = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);

        if (r_ == 0)
        {
            theta_ = 0;
            phi_ = 0;
        }
        else
        {
            theta_ = std::acos(clamp(z_ / r_, -1.0, 1.0)); // inclination
            phi_ = std::atan2(y_, x_);
        }

        phi_ = normalize_angle(phi_);

        updating_ = false;
    }

    // Spherical → Cartesian
    void update_cartesian_from_spherical()
    {
        if (updating_)
            return;
        updating_ = true;

        double sin_theta = std::sin(theta_);
        x_ = r_ * sin_theta * std::cos(phi_);
        y_ = r_ * sin_theta * std::sin(phi_);
        z_ = r_ * std::cos(theta_);

        updating_ = false;
    }

public:
    //
    // CONSTRUCTORS
    //
    Position3D()
        : x_(0), y_(0), z_(0), r_(0), theta_(0), phi_(0),
          speed_(0), heading_(0), vertical_angle_(0) {}

    Position3D(double x, double y, double z)
        : x_(x), y_(y), z_(z), speed_(0), heading_(0), vertical_angle_(0)
    {
        update_spherical_from_cartesian();
    }

    Position3D(double r, double theta, double phi, bool spherical)
        : r_(r), theta_(theta), phi_(normalize_angle(phi)),
          speed_(0), heading_(0), vertical_angle_(0)
    {
        update_cartesian_from_spherical();
    }

    //
    // CARTESIAN ACCESSORS
    //
    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    void set_cartesian(double x, double y, double z)
    {
        x_ = x;
        y_ = y;
        z_ = z;
        update_spherical_from_cartesian();
    }

    void get_cartesian(double &x, double &y, double &z) const
    {
        x = x_;
        y = y_;
        z = z_;
    }

    //
    // SPHERICAL ACCESSORS
    //
    double r() const { return r_; }
    double theta() const { return theta_; }
    double phi() const { return phi_; }

    void set_spherical(double r, double theta, double phi)
    {
        r_ = r;
        theta_ = clamp(theta, 0.0, M_PI);
        phi_ = normalize_angle(phi);
        update_cartesian_from_spherical();
    }

    void get_spherical(double &r, double &theta, double &phi) const
    {
        r = r_;
        theta = theta_;
        phi = phi_;
    }

    //
    // GEOGRAPHIC SYSTEM (lat/long)
    // latitude  = -90° → +90°
    // longitude = -180° → +180°
    //

    double latitude_deg() const
    {
        // latitude = 90° - inclination*180/π
        return 90.0 - (theta_ * 180.0 / M_PI);
    }

    double longitude_deg() const
    {
        double lon = phi_ * 180.0 / M_PI;
        if (lon > 180.0)
            lon -= 360.0; // convert [0,360) → [-180,180)
        return lon;
    }

    void set_geographic(double latitude_deg, double longitude_deg, double radius)
    {
        // Clamp latitude
        latitude_deg = clamp(latitude_deg, -90.0, 90.0);

        r_ = radius;

        // Convert to radians
        double lat_rad = latitude_deg * (M_PI / 180.0);
        double lon_rad = longitude_deg * (M_PI / 180.0);

        // Convert to spherical
        theta_ = (M_PI / 2.0) - lat_rad; // inclination
        phi_ = normalize_angle(lon_rad); // azimuth

        update_cartesian_from_spherical();
    }

    //
    // MOVEMENT HELPERS
    //
    void translate(double dx, double dy, double dz)
    {
        x_ += dx;
        y_ += dy;
        z_ += dz;
        update_spherical_from_cartesian();
    }

    void move_forward(double distance)
    {
        double sin_theta = std::sin(theta_);
        x_ += distance * sin_theta * std::cos(phi_);
        y_ += distance * sin_theta * std::sin(phi_);
        z_ += distance * std::cos(theta_);
        update_spherical_from_cartesian();
    }

    void yaw(double dphi)
    {
        phi_ = normalize_angle(phi_ + dphi);
        update_cartesian_from_spherical();
    }

    void pitch(double dtheta)
    {
        theta_ = clamp(theta_ + dtheta, 0.0, M_PI);
        update_cartesian_from_spherical();
    }

    //
    // VELOCITY / HEADING
    //
    double speed() const { return speed_; }
    double heading() const { return heading_; }
    double vertical_angle() const { return vertical_angle_; }

    // Get heading in degrees (0° = East/+X, 90° = North/+Y)
    double heading_deg() const { return heading_ * 180.0 / M_PI; }
    double vertical_angle_deg() const { return vertical_angle_ * 180.0 / M_PI; }

    void set_speed(double speed)
    {
        speed_ = speed;
    }

    void set_heading(double heading_rad)
    {
        heading_ = normalize_angle(heading_rad);
    }

    void set_heading_deg(double heading_deg)
    {
        heading_ = normalize_angle(heading_deg * M_PI / 180.0);
    }

    void set_vertical_angle(double angle_rad)
    {
        vertical_angle_ = clamp(angle_rad, -M_PI / 2.0, M_PI / 2.0);
    }

    void set_vertical_angle_deg(double angle_deg)
    {
        vertical_angle_ = clamp(angle_deg * M_PI / 180.0, -M_PI / 2.0, M_PI / 2.0);
    }

    void set_velocity(double speed, double heading_rad, double vertical_angle_rad = 0.0)
    {
        speed_ = speed;
        heading_ = normalize_angle(heading_rad);
        vertical_angle_ = clamp(vertical_angle_rad, -M_PI / 2.0, M_PI / 2.0);
    }

    void set_velocity_deg(double speed, double heading_deg, double vertical_angle_deg = 0.0)
    {
        speed_ = speed;
        heading_ = normalize_angle(heading_deg * M_PI / 180.0);
        vertical_angle_ = clamp(vertical_angle_deg * M_PI / 180.0, -M_PI / 2.0, M_PI / 2.0);
    }

    // Update position based on current speed and heading for a given time delta
    void update(double dt)
    {
        if (speed_ == 0)
            return;

        double distance = speed_ * dt;
        double horizontal_distance = distance * std::cos(vertical_angle_);
        double vertical_distance = distance * std::sin(vertical_angle_);

        x_ += horizontal_distance * std::cos(heading_);
        y_ += horizontal_distance * std::sin(heading_);
        z_ += vertical_distance;

        update_spherical_from_cartesian();
    }

    // Update position on a sphere (maintains constant radius)
    // Speed is interpreted as angular velocity or arc distance
    void update_on_sphere(double dt)
    {
        if (speed_ == 0)
            return;

        double distance = speed_ * dt;

        // Convert heading to change in phi (longitude)
        // and vertical_angle to change in theta (latitude)
        // For small movements, approximate arc distance on sphere

        // Compute angular distances
        double angular_distance = distance / r_; // total angular distance

        // Decompose into horizontal and vertical components
        double d_phi = angular_distance * std::cos(vertical_angle_) * std::cos(heading_);
        double d_theta_component = angular_distance * std::cos(vertical_angle_) * std::sin(heading_);
        double d_theta_vertical = angular_distance * std::sin(vertical_angle_);

        // For movement on sphere surface:
        // heading_ represents direction in local tangent plane
        // We need to convert to changes in spherical coordinates

        // Simplified: heading 0° = east (increasing phi), 90° = north (decreasing theta)
        double sin_theta = std::sin(theta_);
        if (sin_theta < 0.0001)
            sin_theta = 0.0001; // avoid singularity at poles

        phi_ += (angular_distance * std::cos(vertical_angle_) * std::cos(heading_)) / sin_theta;
        theta_ -= angular_distance * std::cos(vertical_angle_) * std::sin(heading_);

        // Handle vertical movement (changes radius slightly or moves along theta)
        theta_ -= angular_distance * std::sin(vertical_angle_);

        // Keep theta in bounds
        theta_ = clamp(theta_, 0.0, M_PI);
        phi_ = normalize_angle(phi_);

        update_cartesian_from_spherical();
    }

    // Set heading to match current spherical phi angle
    void align_heading_to_phi()
    {
        heading_ = phi_;
    }

    // Set heading and vertical angle from spherical coordinates
    void align_heading_to_spherical()
    {
        heading_ = phi_;
        // Convert theta (inclination) to vertical angle
        // theta: 0=up(+Z), π/2=horizontal, π=down(-Z)
        // vertical_angle: +π/2=up, 0=horizontal, -π/2=down
        vertical_angle_ = M_PI / 2.0 - theta_;
    }

    //
    // DEBUG PRINT
    //
    void print() const
    {
        std::cout << "Cartesian: (" << x_ << ", " << y_ << ", " << z_ << ")\n";
        std::cout << "Spherical: r=" << r_
                  << " θ=" << theta_
                  << " φ=" << phi_ << "\n";
        std::cout << "Geographic: lat=" << latitude_deg()
                  << " lon=" << longitude_deg() << "\n";
    }
};
