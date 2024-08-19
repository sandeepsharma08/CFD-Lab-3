#include <iostream>
#include <vector>

using namespace std;

// Function to perform Thomas Algorithm (Tridiagonal Matrix Algorithm)
void thomasAlgorithm(const vector<double>& a, const vector<double>& b, const vector<double>& c, vector<double>& d) {
    int n = d.size();
    vector<double> c_star(n);
    vector<double> d_star(n);

    // Forward sweep
    c_star[0] = c[0] / b[0];
    d_star[0] = d[0] / b[0];

    for (int i = 1; i < n; i++) {
        double m = 1.0 / (b[i] - a[i] * c_star[i - 1]);
        c_star[i] = c[i] * m;
        d_star[i] = (d[i] - a[i] * d_star[i - 1]) * m;
    }

    // Back substitution
    d[n - 1] = d_star[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        d[i] = d_star[i] - c_star[i] * d[i + 1];
    }
}

int main() {
    // Given lambda value
    double lambda = 0.020875;

    // Spatial step size
    double dx = 1.0;
    // Time step size
    double dt = 1.0;
    // Number of spatial points (including boundary points)
    int nx = 6;

    // Initialize temperature vector
    vector<double> u_old(nx, 0.0);
    vector<double> u_new(nx, 0.0);

    // Initial condition
    u_old[0] = 100.0; // u(0,0) = 100
    u_old[5] = 50.0;  // u(5,0) = 50

    // Crank-Nicolson parameters
    double r = lambda; // Given lambda as r

    // Coefficients of the tridiagonal system
    vector<double> a(nx - 2, -r / 2.0); // Lower diagonal
    vector<double> b(nx - 2, 1.0 + r);  // Main diagonal
    vector<double> c(nx - 2, -r / 2.0); // Upper diagonal

    // Time stepping
    int timeSteps = 1; // One time step to find u(1,1), u(2,1), u(3,1), u(4,1)

    for (int t = 0; t < timeSteps; t++) {
        // Right-hand side vector
        vector<double> d(nx - 2);

        // Internal points only
        for (int i = 1; i < nx - 1; i++) {
            d[i - 1] = (1.0 - r) * u_old[i] + (r / 2.0) * (u_old[i - 1] + u_old[i + 1]);
        }

        // Adjust for boundary conditions
        d[0] += r / 2.0 * u_old[0];     // Left boundary
        d[nx - 3] += r / 2.0 * u_old[nx - 1]; // Right boundary

        // Solve tridiagonal system
        thomasAlgorithm(a, b, c, d);

        // Update new values
        for (int i = 1; i < nx - 1; i++) {
            u_new[i] = d[i - 1];
        }

        // Apply boundary conditions
        u_new[0] = 100.0; // u(0,1) = 100
        u_new[nx - 1] = 50.0; // u(5,1) = 50

        // Update for next iteration
        u_old = u_new;
    }

    // Output the results for u(1,1), u(2,1), u(3,1), u(4,1)
    cout << "u(1,1) = " << u_new[1] << endl;
    cout << "u(2,1) = " << u_new[2] << endl;
    cout << "u(3,1) = " << u_new[3] << endl;
    cout << "u(4,1) = " << u_new[4] << endl;

    return 0;
}
