#include <QTextStream>
#include <QVector>
#include <exception>
QTextStream cout(stdout);
QTextStream cerr(stderr);

class Error
{
public:
    explicit Error(const QString& what) : m_what(what) {}
    virtual ~Error() {}
    virtual QString what() const { return m_what; }
private:
    QString m_what;
};

class CellAut
{
public:
    explicit CellAut(int rule = 18, int size = 79)
    {
        setRule(rule);
        resize(size);
        init();
    }
    int rule() const
    {
        return m_rule;
    }
    void setRule(int rule)
    {
        if (rule < 0   ||   rule > 0xff)
            throw Error("Rule is out of range");
        m_rule = rule;
    }
    int size() const
    {
        return m_size;
    }
    void resize(int size)
    {
        int MaxSize = 1000;
        if (size < 3)
            throw Error("State size is too small");
        if (size > MaxSize)
            throw Error("State size is too large");
        m_size = size;
        m_state.resize(size);
        m_nextState.resize(size);
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
        if (bits > m_size)
            throw Error("Initial state exceeds state vector size");
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
    try
    {
        // Parse command line parameters
        int rule = 18, steps = 25, initstate = 1, size = 79;
        int *parg = 0;
        for (int i=1; i<argc; ++i)
        {
            QString arg = argv[i];
            bool isInt;
            int value = arg.toInt(&isInt);
            if (isInt) {
                if (!parg)
                    throw Error("Incorrect command line arguments");
                *parg = value;
                parg = 0;
            }
            if (parg)
                throw Error("Incorrect command line arguments");
            if (arg == "-r")
                parg = &rule;
            else if (arg == "-s")
                parg = &steps;
            else if (arg == "-i")
                parg = &initstate;
            else if (arg == "-z")
                parg = &size;
            // QString programName = QString(argv[0]).split(QRegExp("\\\\|/")).last();
            // throw Error("Usage: " + programName + " rule steps initstate size");
        }

        // Run simulation
        CellAut ca(rule, size);
        ca.init(initstate);
        ca.run(steps);
    }
    catch (Error& e)
    {
        cerr << "ERROR: " << e.what() << endl;
        return -1;
    }
    return 0;
}
