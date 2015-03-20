#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

const float pi = M_PI;

// chain of oscillators
class osc {
private:
	int n;
	float dt, dx;
	float* xi;
	float* v;
public:
	osc(float Dt, float L, int mode, float Dx = 0) {
		dt = Dt;
		if (Dx == 0)
			dx = Dt;
		else
			dx = Dx;
		n = int(L / dx) + 1;
		xi = new float[n];
		v = new float[n];
		// set xi and v at the same time instance
		if (mode == 0) {
			// peak-like xi(x)
			for (int i = 0; i < n; ++i) {
				float k = pi / ((n - 1) * dx);
				xi[i] = 6.0 / (20 * k) * pow(sin(k * i * dx), 20);
				v[i] = -6 * pow(sin(k * i * dx), 19) * cos(k * i * dx);
			}
		} else {
			// n-th mode
			for (int i = 0; i < n; ++i) {
				v[i] = sin(pi * mode * i / (n - 1));
				xi[i] = 0;
			}
		}
		// halfstep back for xi
		for (int i = 1; i < n - 1; ++i)
			xi[i] -= v[i] * 0.5 * dt;
	}
	~osc() {
		delete[] xi;
		delete[] v;
	}
	void advance() {
		// main loop
		for (int i = 1; i < n - 1; ++i)
			xi[i] += v[i] * dt;
		for (int i = 1; i < n - 1; ++i)
			v[i] += dt / (dx * dx) * (xi[i + 1] - 2 * xi[i] + xi[i - 1]);
	}
	// displays v by pseudographics
	void show() {
		const char c[] = { '.', ':', '=', '?', '%', '$', '@', '#' };
		const char s[] = { ' ', '-' };
		for (int i = 0; i < n; ++i) {
			int j = (int) (8 * fabs(v[i]));
			if (j < 8)
				cout << setw(1) << s[v[i] < 0] << setw(1) << c[j];
			else
				cout << setw(1) << s[v[i] < 0] << setw(1) << c[7];
		}
		cout << '\n';
	}
};

int main() {
	// 1-st mode
	osc u(0.1, 2, 1);
	// 2-nd mode
	osc v(0.1, 2, 2);
	// (n-1)/2 mode
	osc y(0.1, 2.1, 10);
	// wave travelling from left to right
	osc w(0.1, 2, 0);
	// computes and plots the evolution
	for (int i = 0; i < 40; ++i) {
		u.advance();
		u.show();
	}
	/*
	 // 1-st mode, dt>dx
	 osc us(0.107, 2, 1, 0.1);
	 // computes and plots the evolution
	 for (int i = 0; i < 40; ++i) {
	 us.advance();
	 us.show();
	 }
	 */
	/*
	 // wave travelling from left to right, dt<dx
	 osc ws(0.05, 2, 0, 0.1);
	 // computes and plots the evolution
	 for (int i = 0; i < 80; ++i) {
	 ws.advance();
	 ws.show();
	 }
	 */
	return 0;
}
