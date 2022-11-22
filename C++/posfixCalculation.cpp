// 后缀表达式的计算
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <string>
using namespace std;

void get_postfix(vector<string>& postf)
{
    postf.clear();
    string line;
    getline(cin, line);
    istringstream sin(line);
    string tmp;
    while (sin >> tmp)
    {
        postf.push_back(tmp);
    }
}

void init_op(map<string, int>& ops)
{
    ops.clear();
    ops["+"] = 100;
    ops["-"] = 100;
    ops["*"] = 200;
    ops["/"] = 200;
    ops["("] = 1000;
    ops[")"] = 0;
}

bool is_operator(const string& hs, const map<string, int>& ops)
{
    map<string, int>::const_iterator cit = ops.find(hs);\
    if (cit != ops.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

double cal_post(const vector<string>& postf, const map<string, int>& ops)
{
    stack<double> or_st;
    double operand = 0.0, a = 0.0, b = 0.0, c = 0.0;
    for (vector<string>::size_type i = 0; i != postf.size(); ++i)
    {
        if (!is_operator(postf[i], ops))
        {
            operand = static_cast<double>(atof(postf[i].c_str()));
            or_st.push(operand);
        }
        else
        {
            switch (postf[i][0])
            {
            case '+':
                b = or_st.top();
                or_st.pop();
                a = or_st.top();
                or_st.pop();
                c = a + b;
                or_st.push(c);
                break;
            case '-':
                b = or_st.top();
                or_st.pop();
                a = or_st.top();
                or_st.pop();
                c = a - b;
                or_st.push(c);
                break;
            case '*':
                b = or_st.top();
                or_st.pop();
                a = or_st.top();
                or_st.pop();
                c = a * b;
                or_st.push(c);
                break;
            case '/':
                b = or_st.top();
                or_st.pop();
                a = or_st.top();
                or_st.pop();
                c = a / b;
                or_st.push(c);
                break;
            default:
                break;
            }
        }
    }
    if (or_st.size() == 1)
    {
        return or_st.top();
    }
    else
    {
        return -10000000000000.0;
    }
}

int main()
{
    map<string, int> ops;
    init_op(ops);
    vector<string> postf;
    
    while (1)
    {
        get_postfix(postf);
        double ret = cal_post(postf, ops);
        cout << ret << endl << endl;
    }
    
    system("PAUSE");
    return 0;
}
