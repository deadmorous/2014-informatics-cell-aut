#include <QTextStream>
#include <QVector>
QTextStream cout(stdout);
QTextStream cerr(stderr);

class CellAut
{
public:
    explicit CellAut(int rule = 18, int size = 79) :
        m_rule(rule),
        m_size(size),
        m_state(size),
        m_nextState(size)
    {
        init();
    }
    int rule() const
    {
        return m_rule;
    }
    int size() const
    {
        return m_size;
    }
    const int *state() const
    {
        return m_state.data();
    }
    void print() const
    {
        for (int i=0; i<m_size; ++i)
            cout << (m_state[i] ?   '*' :   ' ');
        cout << endl;
    }
    void init(int state = 1<<16)
    {
        m_state.fill(0);
        int bits = 0;
        for (int s=state; s; s>>=1)
            ++bits;
        int stateIndex = (m_size+bits)/2;
        for (int s=state; s; s>>=1)
            m_state[stateIndex--] = s & 1 ?   1 :   0;
    }
    void next()
    {
        for (int i=1; i<m_size-1; ++i)
        {
            int index = (m_state[i-1] << 2) | (m_state[i] << 1) | m_state[i+1];
            m_nextState[i] = m_rule & (1 << index) ?   1 :   0;
        }
        qSwap(m_state, m_nextState);
    }
    void run(int genCount)
    {
        for (int gen=0; gen<genCount; ++gen) {
            print();
            next();
        }
    }

private:
    int m_rule;
    int m_size;
    QVector<int> m_state;
    QVector<int> m_nextState;
};

int main(int argc, char *argv[])
{
    // Parse command line parameters
    if (argc != 5)
    {
        QString programName = QString(argv[0]).split(QRegExp("\\\\|/")).last();
        cerr << "Usage: " << programName << " rule steps initstate size" << endl;
        return -1;
    }
    int rule = atoi(argv[1]);
    int steps = atoi(argv[2]);
    int initstate = atoi(argv[3]);
    int size = atoi(argv[4]);

    // Run simulation
    CellAut ca(rule, size);
    ca.init(initstate);
    ca.run(steps);
    return 0;
}
