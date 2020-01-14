#include<iostream>
#include<vector>
using namespace std;

int main()
{
    int i;
    int ar[]={1,2,3,4};
    vector<int> v(ar,ar+4);
    for(i = 0;i<v.size();i++)
    {
        cout<<"v[i] = "<<v[i]<<endl;
    }

    for(auto& e:v)
    {
        cout<<e<<" ";
    }
    cout<<endl;
    return 0;
}
