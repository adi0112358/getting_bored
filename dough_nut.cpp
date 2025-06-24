#include <iostream>
#include <cmath>
#include <cstring>
#include <unistd.h> 

int main() {
    float A = 0, B = 0;
    const int width = 80, height = 22;
    const float theta_step = 0.07, phi_step = 0.02;
    const float R1 = 1, R2 = 2, K2 = 5;
    const float K1 = 15;

    while (true) {
        char output[width * height];
        float zbuffer[width * height];
        std::memset(output, ' ', width * height);
        std::memset(zbuffer, 0, sizeof(zbuffer));

        for (float theta = 0; theta < 2 * M_PI; theta += theta_step) {
            for (float phi = 0; phi < 2 * M_PI; phi += phi_step) {
                float sinA = sin(A), cosA = cos(A);
                float sinB = sin(B), cosB = cos(B);
                float costheta = cos(theta), sintheta = sin(theta);
                float cosphi = cos(phi), sinphi = sin(phi);

                float circlex = R2 + R1 * costheta;
                float circley = R1 * sintheta;

                float x = circlex * (cosB * cosphi + sinA * sinB * sinphi) - circley * cosA * sinB;
                float y = circlex * (sinB * cosphi - sinA * cosB * sinphi) + circley * cosA * cosB;
                float z = K2 + cosA * circlex * sinphi + circley * sinA;
                float ooz = 1 / z;

                int xp = (int)(width / 2 + K1 * ooz * x);
                int yp = (int)(height / 2 - K1 * ooz * y);
                int idx = xp + yp * width;

                float L = cosphi * costheta * sinB - cosA * costheta * sinphi -
                          sinA * sintheta + cosB * (cosA * sintheta - costheta * sinA * sinphi);

                if (idx >= 0 && idx < width * height && L > 0) {
                    if (ooz > zbuffer[idx]) {
                        zbuffer[idx] = ooz;
                        const char* chars = ".,-~:;=!*#$@";
                        output[idx] = chars[int(L * 8) % 12];
                    }
                }
            }
        }

        std::cout << "\x1b[H";
        for (int i = 0; i < width * height; i++)
            std::cout << (i % width ? output[i] : '\n');

        A += 0.04;
        B += 0.02;
        usleep(30000);
    }
}

