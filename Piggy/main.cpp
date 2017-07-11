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
#include <iomanip>
#include <string>
using namespace std;
namespace pork
{
    class Frame
    {
        const int default_max_row=100;
        vector<vector<string> > data;
        bool ignore_header;
        map<string,int> headers;
        public:
            Frame()
            {
                // blank constructor....
            }
            Frame( vector<vector<string> > &data ,bool ignore_header=false )
            {
                this->data=data;
                this->ignore_header=ignore_header;
                if(!ignore_header)
                for( unsigned int i=0 ; i<data[0].size() ; i++ )
                    headers[data[0][i]]=i;
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
                        out<<setw(20)<<j;
                    }
                    out<<"\n";
                }
                out<<"(rows="<<b.data.size()-1<<",cols="<<b.data[0].size()<<")\n]]\n";
                return out;
            }
            // get column based on integer index value....
            Frame operator [] (int idx)
            {
                vector<vector<string> > result;
                for(unsigned int i=0;i<data.size();i++)
                {
                    vector<string> s={ data[i][idx] };
                    result.push_back(s);
                }
                return Frame(result);
            }
            // get all the value of a column by column name...
            Frame operator [] ( string col )
            {

                return operator []( headers[col] );
            }
            // returns a string at particular position indexed by i,j
            string get( int i, int j )
            {
                return data[i][j];
            }
            double getVal( int i, int j )
            {
                stringstream ss( data[i][j] );
                double val;
                ss>>val;
                return val*1.00;
            }
            // returns no of rows of the data frame..
            int nrows()
            {
                if(!ignore_header)  return data.size() - 1;
                                    return data.size();
            }
            int ncols()
            {
                return data[0].size();
            }
            // get mean by colname
            double mean( string col )
            {
                map<string,int>::iterator it=headers.find(col);
                if( it==headers.end() )
                {
                    cout<<"ColumnNameNotFound Exception..\n";
                    return -INFINITY;
                }
                int idx=it->second;
                double m=0.0;
                for( unsigned int i=0 ; i < data.size() ; i++ )
                {
                    if( i==0 && !ignore_header )
                        continue;   // first row is header.. ignore it
                    m+=atof ( data[i][idx].c_str() );
                }
                return ignore_header? (m/data.size()*1.00) :
                                      (m/ ( data.size()-1)*1.00 );
            }
            // get mean by col index
            double mean( int col )
            {

                double m=0.0;
                try {
                    if( col>data[0].size()-1 ) throw 1;
                    for( unsigned int i=0 ; i < (unsigned) data.size() ; i++ )
                    {
                        if( i==0 && !ignore_header )
                            continue;   // first row is header.. ignore it
                        m+=atof ( data[i][col].c_str() );
                    }
                } catch( int ex ) {
                    cout<<"Column Index OutOfBound Exception...";
                    return -INFINITY;
                }
                return ignore_header? (m/data.size()*1.00) :
                                      (m/ ( data.size()-1)*1.00 );
            }
            Frame head( int n=5 )
            {
                vector< vector<string> > H;
                for( unsigned i=0; i< (ignore_header?n:n+1); i++ )
                    H.push_back( data[i] );
                return Frame(H);
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
                return Frame();
            }
    };
};
// testing .....
using namespace pork;
int main()
{
    Piggy p;
    Frame data=p.read_csv("data.csv");
    Frame col2=data["cgpa"];
    cout<<data;
    cout<<"Column 1\n";
    cout<<col2;
    // get value of particular cell ( row,col )
    cout<<"Value at (10,1) : ";
    cout<<data.getVal(10,1)<<endl;
    cout<<"Mean CGPA="<<data.mean("cgpa")<<endl;
    cout<<"Mean CGPA="<<data.mean("cgpaa")<<endl; // Invalid Column Name
    cout<<"Mean CGPA="<<data.mean(3); // Invalid Column Index
    cout<<"\nHead : -\n";
    Frame h=data.head();
    cout<<h;
    return 0;
}
