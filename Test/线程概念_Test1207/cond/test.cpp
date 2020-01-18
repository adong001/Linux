#include<iostream>
#include<vector>
using namespace std;

int main()
{
    int i = 0;
    vector<int> v;
    for(int i = 0;i < 10;i++)
    {
        v.push_back(i);
    }

    for(auto& e: v)
    {
        cout<<e<<" "; 
    }
        
    cout<<endl;
    return 0;
}
