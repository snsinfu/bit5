import io;
import math;

int main()
{
    for (int i = 0; i <= 10; i++) {
        double x = double(i - 5) / 5;
        println(sigmoid(x));
    }
}
