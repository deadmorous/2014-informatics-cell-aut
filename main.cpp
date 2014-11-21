#include <QTextStream>
QTextStream cout(stdout);

const int N = 79;
int state[N];

void init()
{
    for (int i=0; i<N; ++i)
        state[i] = 0;
    state[N/2] = 1;
}

void printState()
{
    for (int i=0; i<N; ++i)
        cout << (state[i] ?   '*' :   ' ');
    cout << endl;
}

void nextState()
{
    const int Rule = 18;
    int next[N];
    for (int i=1; i<N-1; ++i)
    {
        int index = (state[i-1] << 2) | (state[i] << 1) | state[i+1];
        next[i] = Rule & (1 << index) ?   1 :   0;
    }
    for (int i=1; i<N-1; ++i)
        state[i] = next[i];
}

void run()
{
    const int MaxSteps = 25;
    for (int i=0; i<MaxSteps; ++i) {
        printState();
        nextState();
    }
}

int main(int argc, char *argv[])
{
    init();
    run();
    return 0;
}
