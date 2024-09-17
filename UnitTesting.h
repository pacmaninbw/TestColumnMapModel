#ifndef UNITTESTING_H
#define UNITTESTING_H

const int MIN_DBG_LEVEL = 0;
const int MAX_DBG_LEVEL = 5;

class UnitTesting
{
public:
    UnitTesting();
    ~UnitTesting();
    void getUserTestConfiguration();
    int executeAllUnitTests();

private:
    void separaterLine(void);
    void getDebugLevel(void);
    void allowExceptions(void);
    void getContinueAfterFail(void);
    // Copied from another Stack Overflow answer.
    bool is_number(const std::string& s)
    {
        return !s.empty() && std::find_if(s.begin(), 
            s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }
    // Copied from accepted answer https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
    static char asciitolower(char in)
    {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }
};

#endif