#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <string>

using namespace std;

// -------------------- Ввід з перевіркою --------------------

int readInt(const string& prompt, int minVal, int maxVal) {
    for (;;) {
        cout << prompt << flush;
        long long v;
        if (cin >> v && v >= minVal && v <= maxVal) return (int)v;

        cout << "Помилка вводу. Потрібно ціле в діапазоні [" << minVal << "; " << maxVal << "].\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

long double readLongDouble(const string& prompt, long double minVal, long double maxVal) {
    for (;;) {
        cout << prompt << flush;
        long double v;
        if (cin >> v && v >= minVal && v <= maxVal) return v;

        cout << "Помилка вводу. Потрібно дійсне в діапазоні [" << (double)minVal << "; " << (double)maxVal << "].\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// -------------------- Завдання 1 (табл.1): Figure = 10 --------------------

static bool inSquare(long double x, long double y, long double a) {
    return (x >= -a/2 && x <= a/2 && y >= -a && y <= 0);
}

static bool inCircle(long double x, long double y, long double a) {
    long double r = a / 2.0L;
    long double cy = -r;          // центр (0, -a/2)
    long double dx = x;
    long double dy = y - cy;      // y - (-r) = y + r
    return (dx*dx + dy*dy <= r*r);
}

void task1() {
    cout << "\n--- Завдання 1 (Figure = 10) ---\n";

    long double a = readLongDouble("Введiть a (a > 0): ", 1e-12L, 1e12L);
    int n = readInt("Введiть кiлькiсть точок n (1..100000): ", 1, 100000);

    int cnt = 0;

    for (int i = 1; i <= n; i++) {
        cout << "Точка #" << i << " (x y): " << flush;
        long double x, y;
        while (!(cin >> x >> y)) {
            cout << "Помилка вводу. Повторiть (x y): " << flush;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (!inSquare(x, y, a)) continue;

        // Діагональ через (-a/2, -a) та (a/2, 0): y = x - a/2
        long double yDiag = x - a/2.0L;
        bool insideCircle = inCircle(x, y, a);

        bool ok = false;

        // Прийнята інтерпретація області для Figure=10:
        // x <= 0: нижче/на діагоналі в межах квадрата
        // x > 0 : вище/на діагоналі в межах квадрата та поза колом
        if (x <= 0) {
            if (y <= yDiag) ok = true;
        } else {
            if (y >= yDiag && !insideCircle) ok = true;
        }

        if (ok) cnt++;
    }

    cout << "Кiлькiсть точок, що належать областi (Figure=10): " << cnt << "\n";
}

// -------------------- Завдання 2 (табл.2): варіант 17 --------------------
// S = -1 + sum_{k=0..n} [ (-1)^k * t^(2k) / (2k)! ],  t = -pi/2 + x^(1/4)
// Для непарних варіантів: вивести значення кожного 4-го елемента.

void task2() {
    cout << "\n--- Завдання 2 (табл.2, варiант 17) ---\n";

    long double x = readLongDouble("Введiть x (x >= 0): ", 0.0L, 1e18L);
    int n = readInt("Введiть n (0..100000): ", 0, 100000);

    const long double PI = acosl(-1.0L);
    long double t = -PI / 2.0L + powl(x, 0.25L);

    long double term = 1.0L;            // k=0
    long double sum  = -1.0L + term;    // -1 + term0

    cout << fixed << setprecision(15);

    // Нумерація елементів: 1-й відповідає k=0 (term=1), 2-й — k=1, ...
    if (1 % 4 == 0) {
        cout << "Елемент #1 (k=0): " << term << "\n";
    }

    for (int k = 0; k < n; k++) {
        long double denom = (long double)(2*k + 1) * (long double)(2*k + 2);
        term = term * (-1.0L) * (t*t) / denom;   // term_{k+1}
        sum += term;

        int k1 = k + 1;
        int elemIndex = k1 + 1;
        if (elemIndex % 4 == 0) {
            cout << "Елемент #" << elemIndex << " (k=" << k1 << "): " << term << "\n";
        }
    }

    cout << "S = " << sum << "\n";
}

// -------------------- Завдання 3 (табл.3): варіант 10 --------------------
// u_n = (n! - 3^n) / n^n
// Критерій зупинки для збіжного: |u_n| < e
// Обчислення через логарифми для уникнення переповнення.

void task3() {
    cout << "\n--- Завдання 3 (табл.3, варiант 10) ---\n";

    long double e = readLongDouble("Введiть e (1e-20 .. 1e-5): ", 1e-20L, 1e-5L);
    int nMax = readInt("Введiть nMax (1..100000): ", 1, 100000);

    long double sum = 0.0L;

    cout << fixed << setprecision(15);

    for (int n = 1; n <= nMax; n++) {
        long double ln_fact = lgammal((long double)n + 1.0L);

        long double ln_a = ln_fact - (long double)n * logl((long double)n);
        long double a = expl(ln_a); // n!/n^n

        long double ln_b = (long double)n * (logl(3.0L) - logl((long double)n));
        long double b = expl(ln_b); // (3/n)^n

        long double u = a - b;
        sum += u;

        cout << "n=" << setw(6) << n
             << "  u_n=" << setw(22) << (double)u
             << "  sum=" << setw(22) << (double)sum << "\n";

        if (fabsl(u) < e) {
            cout << "Зупинка: |u_n| < e.\n";
            break;
        }
    }

    cout << "Пiдсумкова сума (за виконаними членами): " << (double)sum << "\n";
}

// -------------------- Меню --------------------

int main() {
    ios::sync_with_stdio(false);
    cin.tie(&cout);

    int menu;
    do {
        cout << "\n=============================\n";
        cout << "ЛР5 меню:\n";
        cout << " 1 - Завдання 1 (Figure=10)\n";
        cout << " 2 - Завдання 2 (табл.2, вар.17)\n";
        cout << " 3 - Завдання 3 (табл.3, вар.10)\n";
        cout << " -1 - Вихiд\n";
        cout << "Ваш вибiр: " << flush;

        if (!(cin >> menu)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Помилка вводу. Спробуйте ще раз.\n";
            continue;
        }

        switch (menu) {
            case 1: task1(); break;
            case 2: task2(); break;
            case 3: task3(); break;
            case -1: cout << "Вихiд...\n"; break;
            default: cout << "Невiрний пункт меню!\n"; break;
        }
    } while (menu != -1);

    return 0;
}
