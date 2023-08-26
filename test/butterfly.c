#include <math.h>
#include <complex.h>
#include <turtle.h>

double complex expo(double complex x)
{
    double complex e = 1.0;
    for (int i = 1; i < 30; i++)
    {
        e += cpow(x, i) / tgamma(i + 1);
    }
    return e;
}

double rd(double i)
{
    return 100 * (1 - cos(i) * sin(3 * i));
}

int main(void)
{
    width(4);
    color("red");

    double teta[100];
    for (int i = 0; i < 100; i++)
    {
        teta[i] = i * 2 * M_PI / 99;
    }

    up();
    goto(100, 0);
    down();

    for (int i = 1; i < 100; i++)
    {
        double r = rd(teta[i]);
        double complex z = r * expo(teta[i] * I);
        double x = creal(z);
        double y = cimag(z);
        goto(x, y);
    }

    goto(100, 0);
    width(1);
    const char *col[] = {"blue", "purple", "green"};

    int n = 0;
    for (int i = 0; i < 100; i++)
    {
        double r = rd(teta[i]);
        double complex z = r * expo(teta[i] * I);
        double x = creal(z);
        double y = cimag(z);
        color(col[n % 3]);
        n++;
        goto(x, y);
        goto(0, 0);
    }

    show();
    return 0;
}