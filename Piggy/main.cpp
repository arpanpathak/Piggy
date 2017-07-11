/*** Piggy,a data analysis library like pandas written in C++
 *** @author: arpanpathak
 ***/
#include <iostream>
#include <sstream>
#include <map>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <list>

using namespace std;
namespace pork
{
    class Frame
    {
        const int default_max_row=100;
        vector<vector<string> > data;
        public:
            Frame(vector<vector<string> > &data )
            {
                this->data=data;
            }
            friend ostream& operator << ( ostream &out , Frame &b )
            {
                out<<"[[ data_type='Piggy Data Bag'\n";
               int rows=0;
                for( vector<string> i: b.data )
                {
                    if(rows>b.default_max_row)
                    {
                        out<<"\t...."<<b.data.size()-rows<<" more rows\n";
                        break;
                    }
                    rows++;
                    for ( string j: i ) {
                        out<<"\t"<<j;
                    }
                    out<<"\n";
                }
                out<<"(rows="<<b.data.size()-1<<",cols="<<b.data[0].size()<<")\n]]\n";
                return out;
            }
    };
    class Piggy
    {
        string file_to_read,version;
        const int row_limit=1000000; // limiting no of rows to limit memory usage
        public:
            Piggy()
            {
                // First Constructor....
                version="0.0.1";
            }
            // delimiter must have to be a character..
            vector<string> split(string data,char delim)
            {
                stringstream ss(data);
                vector<string> output;
                string temp;
                while(getline(ss,temp,delim))
                    output.push_back(temp);
                return output;
            }
            Frame read_csv(string filename,char delim=',',bool ignore_header=false)
            {
                try
                {
                    ifstream f(filename);
                    if(!f)
                        throw 1;
                    string str;
                    int row=0;
                    vector<vector<string> > data;
                    while(getline(f,str)) {
                        data.push_back(split(str,delim));
                        row++;
                        if ( row>=row_limit ) break;
                    }
                    f.close();
                    return Frame(data);
                }catch(int ex)
                {
                    cout<<"Exception Occured!!! : "<<"CSV File Not Found...";
                }
            }
    };
};

// testing .....
using namespace pork;
int main()
{
    Piggy p;
    Frame data=p.read_csv("data.csv");
    cout<<data;
    return 0;
}
