


#include <getopt.h>
#include <iostream>
//#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <sstream>
#include <queue>
#include <map>
#include "TableEntry.h"
using namespace std;

bool quiet = 0;
const TableEntry helper( EntryType & in)
    {
    int d = 0;
    switch(in)
    {
        case EntryType::Int:
        {
            int value = 0;
            cin >> value;
            return TableEntry(value);
        }
        case EntryType::Double:
        {
            double value = 0;
            cin >> value;
            return TableEntry(value);
        }
        case EntryType::String:
        {
            string value;
            cin >> value;
            return TableEntry(value);
        }
        case EntryType::Bool:
        {

            bool value = 0;
            cin >> value;
            return TableEntry(value);

        }
    }

    return TableEntry(d);
}
    

class Table
{

    public:

    string name;
    unordered_map<string, size_t> columnNames;
    vector<EntryType> columnTypes;
    vector <vector <TableEntry>> table;
    size_t numColums = 0;
    size_t numRows = 0;
    map <TableEntry, vector<size_t>> BSTindex;
    unordered_map<TableEntry, vector<size_t>> HASHindex;
    size_t IndexCol = 0;
    string IndexName;

    void PRINTALL(vector <string> & in )
    {
        if(!quiet)
        {
            for(size_t i  =0; i < in.size(); i++)
            {
            cout << in[i] << " " ;
            }
            cout << '\n';
            for(size_t i = 0; i < numRows; i++)
            {
                for(size_t j = 0; j < in.size(); j++)
                {
                    cout << table[i][columnNames[in[j]]] << " ";
                }
                cout << '\n';
            }
        }
        cout << "Printed " << numRows << " matching rows from " << name << '\n';
        
    }

    unordered_map<TableEntry,vector<size_t>> TEMP (string & col)
    {
        unordered_map <TableEntry,vector<size_t>> temp;
        size_t c = columnNames[col];
        for(size_t i = 0; i < numRows; i ++)
        {
            temp[table[i][c]].push_back(i);
        }
        return temp;
    }

    void PRINTWHERE(string & col, char & op ,vector <string> & print)
    {
        size_t idx = columnNames[col];
        EntryType a = columnTypes[idx];
        TableEntry b (helper(a));
        size_t count =0;
        if(!quiet)
        {
            for(size_t i  =0; i < print.size(); i++)
            {
                cout << print[i] << " " ;
            }
            cout << "\n";
        }
        
        if(!BSTindex.empty() && IndexCol == idx)
        {
            switch(op)
            {
                case '=':
                {
                    if(!quiet)
                    {
                        for(size_t i = 0; i < BSTindex[b].size(); i++)
                        {
                        
                            for(size_t j = 0; j < print.size(); j++)
                            {
                                cout << table[BSTindex[b][i]][columnNames[print[j]]] << " " ;
                            
                            }
                            cout << "\n";
                            count++;
                        
                        }
                    }else{
                        count = BSTindex[b].size();
                    }
                    
                    break;
                }
                case '<':
                {
                    auto lower = BSTindex.lower_bound(b);
                    for(auto i = BSTindex.begin(); i !=lower; i++ )
                    {
                        if(!quiet)
                        {
                            for(size_t value: i->second)
                            {
                                for(size_t j = 0; j< print.size(); j++)
                                {
                                    cout << table[value][columnNames[print[j]]] << " " ;
                                    
                                }
                                cout << "\n";
                                count ++;
                                
                            }
                        }else{
                            count += i->second.size();
                        }
                    }
                    break;
                }
                case '>':
                {
                    //auto lower = lower_bound(BSTindex.begin(), BSTindex.end(), b);
                    auto upper = BSTindex.upper_bound(b);
                    for(auto i = upper; i != BSTindex.end(); i++)
                    {
                        if(!quiet)
                        {
                            for(size_t value: i ->second)
                            {
                                
                                
                                for(size_t j = 0; j< print.size(); j++)
                                {
                                cout << table[value][columnNames[print[j]]] << " " ;
                                    
                                }
                                cout << "\n";
                                count++;
                                
                            }
                        }else{
                            count += i->second.size();
                        }
                        
                    }
                    break;
                    
                }
            }
        }else{
            switch(op)
            {
                case '=':
                {
                    if (!HASHindex.empty() && IndexCol == idx){
                        //TableEntry c (col);
                        if(!quiet)
                        {
                            for(size_t i = 0; i < HASHindex[b].size(); i++)
                            {
                                for(size_t j = 0; j < print.size(); j++)
                                {
                                    cout << table[HASHindex[b][i]][columnNames[print[j]]] << " ";
                                
                                }
                                cout << "\n";
                                count++;
                            }
                        }else{
                            count = HASHindex[b].size();
                        }
                        
                    }else{
                        for(size_t i = 0; i < numRows; i ++)
                        {
                            if(table[i][idx]== b)
                            {
                                if(!quiet)
                                {
                                    for(size_t j = 0; j < print.size(); j++)
                                    {
                                        cout << table[i][columnNames[print[j]]] << " ";
                                    
                                    }
                                    cout << "\n";
                                }
                                
                                count++;
                                
                            }
                        } 
                    }
                    break;
                }
                case '<':
                {
                    for(size_t i = 0; i < numRows; i ++)
                    {
                        if(table[i][idx] < b)
                        {
                            if(!quiet)
                            {
                                for(size_t j = 0; j < print.size(); j++)
                                {
                                    cout << table[i][columnNames[print[j]]] << " ";
                                
                                }
                                cout << "\n";
                            }
                            count++;
                            
                        }
                    } 
                    break;  
                }  
                case '>':
                {
                    for(size_t i = 0; i < numRows; i ++)
                    {
                        if(table[i][idx] > b)
                        {
                            if(!quiet)
                            {
                                for(size_t j = 0; j < print.size(); j++)
                                {
                                cout << table[i][columnNames[print[j]]] << " ";
                                }
                                cout << "\n";
                            }
                            count++;
                            
                        }
                    } 
                    break;  
                }
            }
           
        }
        cout << "Printed " << count << " matching rows from " << name << '\n';
    }


    void DEL(string & in)
    {
        size_t idx  = columnNames[in];
        char op;
        cin >> op;
        EntryType type = columnTypes[idx];
        switch(op)
        {
            case '=':
            {
                ComparatorEqual comp( idx, type);
                table.erase(remove_if(table.begin(), table.end(), comp), table.end());
                break;
            }
            case '<':
            {
                ComparatorLess comp( idx, type);
                table.erase(remove_if(table.begin(), table.end(), comp), table.end());
                break;
            }
            case '>':
            {
                ComparatorGreater comp1( idx, type);
                table.erase(remove_if(table.begin(), table.end(), comp1), table.end());
                break; 
            }

        }
        size_t deletedN = numRows - table.size();
        numRows = table.size();  
        if(!BSTindex.empty()){
            string t = "bst";
            INDEX(t, IndexName);
        }else if(!HASHindex.empty())
        {
            string t = "hash";
            INDEX(t, IndexName);
        }

        
        cout << "Deleted " << deletedN << " rows from " << name << '\n';
             
    }

    void INDEX(string & type, string & col)
    {
        
        if(!BSTindex.empty())
        {
            BSTindex.clear();
        }else if (!HASHindex.empty())
        {
            HASHindex.clear();
        }
        IndexName = col;
        size_t c = columnNames[col];
        IndexCol = c;
        if (type == "hash")
        {
            for(size_t i=0; i < numRows; i++)
            {
                HASHindex[table[i][c]].push_back(i);
            }
        }else if(type == "bst")
        {
            for(size_t i = 0; i < numRows; i++)
            {
                BSTindex[table[i][c]].push_back(i);
            }
        }
        

    }

    class ComparatorEqual
    {
        public:
        size_t index = 0;
        EntryType v;
        vector<TableEntry> value;
        
        ComparatorEqual(size_t & i,  EntryType & a){index = i; v =a; value.emplace_back(helper(v));} 
        bool operator()( vector <TableEntry> & vec)
        {
            if(vec[index] == value[0])
            {
                return true;
            }
            return false;

        }
        friend class Table;
        private:
    };

    class ComparatorLess
    {
        public:
        size_t index = 0;
        EntryType v;
        vector<TableEntry> value;
        ComparatorLess(size_t  & i,  EntryType & a){index = i; v =a; value.emplace_back(helper(v));} 
        bool operator()( vector <TableEntry> & vec)
        {
            if(vec[index] < value[0])
            {
                return true;
            }
            return false;

        }
        friend class Table;
        private:
    };

    class ComparatorGreater
    {
        public:
        size_t index = 0;
        EntryType v;
        vector<TableEntry> value;
        //TableEntry ba (helper(v)) ;
        ComparatorGreater(size_t & i,  EntryType & a){index = i; v =a; value.emplace_back(helper(v));} 
        bool operator()( vector <TableEntry> & vec)
        {
            if(vec[index] > value[0])
            {
                return true;
            }
            return false;

        }
        friend class Table;
        private:
    };

    private:

};


class SQL
{
    public:


    void getOptions(int argc, char * argv[])
    {
        opterr= false;
        int choice;
        int index = 0;

        option long_options[] = {
            {"help", no_argument, nullptr, 'h'},
            {"quiet", no_argument, nullptr, 'q'},
            
        };

        while((choice = getopt_long(argc, argv, "hq",long_options,&index )) != -1)
        {
            switch(choice)
            {
                case 'h':
                    exit(0);
                    break;
                case 'q':
                    quiet = true;
                    break;

            }
        }

    }

    void run()
    {
        do{

            cout << "% " ;
            cin >> cmd;
            check();

        }while(cmd != "QUIT");
        cout << "Thanks for being silly!\n";
    }

    void check()
    {
        if(cmd[0] == 'C')
        {
            CREATE();
        }else if (cmd[0] == '#')
        {
            COMMENT();
        }else if (cmd[0] == 'R')
        {
            REMOVE();
        }else if (cmd[0] == 'I')
        {
            INSERT();
        }else if (cmd[0] == 'P')
        {
            PRINT();
        }else if (cmd[0] == 'D')
        {
            DELETE();
        }else if (cmd[0] == 'G')
        {
            GENERATE();
        }else if (cmd[0] == 'J')
        {
            JOIN();
        }else if (cmd[0] == 'Q'){
           return;
        }else{
            cout << "Error: unrecognized command\n" ;
            getline(cin, cmd);
            return;
        }
    }

    void JOIN()
    {
        size_t count  =0 ;
        string junk;
        string Table1;
        string Table2;
        string col1;
        string col2;
        size_t N = 0;
        vector <pair<string, bool>> colPrint;
        cin >>Table1;
        cin >> junk;
        cin >> Table2;
        cin >> junk;
        cin >> col1;
        cin >> junk;
        cin >> col2;
        cin >> junk;
        cin >> junk;
        cin >> N;
        colPrint.reserve(N);

        if(query.find(Table1) == query.end())
        {
            cout << "Error during JOIN: " << Table1 << " does not name a table in the database\n";
            getline(cin, junk);
            return;
        }
        if(query.find(Table2) == query.end())
        {
            cout << "Error during JOIN: " << Table2 << " does not name a table in the database\n";
            getline(cin, junk);
            return;
        }

        if(query[Table1].columnNames.find(col1) == query[Table1].columnNames.end())
        {
            cout << "Error during JOIN: " << col1 << " does not name a column in " << Table1 << "\n";
            getline(cin, junk);
            return;
        }

        if(query[Table2].columnNames.find(col2) == query[Table2].columnNames.end())
        {
            cout << "Error during JOIN: " << col2 << " does not name a column in " << Table2 << "\n";
            getline(cin, junk);
            return;
        }



        for(size_t i = 0; i < N; i++)
        {
            int temp = 0;
            pair<string,size_t> read;
            cin >> read.first;
            cin >> temp;
            if(temp ==1)
            {
                read.second = 0;
                if(query[Table1].columnNames.find(col1) == query[Table1].columnNames.end())
                {
                    cout << "Error during JOIN: " << read.first << " does not name a column in " << Table1 << "\n";
                    getline(cin, junk);
                    return;
                }
            }else if(temp ==2)
            {
                read.second =1;
                if(query[Table2].columnNames.find(col2) == query[Table2].columnNames.end())
                {
                    cout << "Error during JOIN: " << read.first << " does not name a column in " << Table2 << "\n";
                    getline(cin, junk);
                    return;
                }

            }
            colPrint.push_back(read);
        }
        
        unordered_map<TableEntry, vector<size_t>> index = query[Table2].TEMP(col2);
        size_t c1 = query[Table1].columnNames[col1];
        if(!quiet)
        {
            for(size_t i = 0; i < N ; i++)
            {
                cout << colPrint[i].first << " ";
            }
            cout << "\n";
        }
        
        for(size_t i = 0; i < query[Table1].numRows; i++)
        {
            TableEntry match(query[Table1].table[i][c1]);
            if(index.find(match) != index.end())
            {
                if(!quiet)
                {
                    for(size_t j = 0; j < index[match].size(); j++)
                    {
                    
                        for(size_t t = 0; t < N; t++)
                        {
                            if(colPrint[t].second)
                            {
                                cout << query[Table2].table[index[match][j]][query[Table2].columnNames[colPrint[t].first]] << " ";
                            }else{
                                cout << query[Table1].table[i][query[Table1].columnNames[colPrint[t].first]] << " ";
                            
                            }
                        }
                        count ++;
                        cout << "\n";
                    }
                }else{
                    count += index[match].size();
                }
                
            }
        }

        cout << "Printed " << count << " rows from joining " << Table1 << " to " << Table2 ;
        cout << "\n";
    }
   

    void PRINT()
    {
        string read;
        string name;
        cin >> read;
        cin >> name;
        int N =0;
        cin >> N;
        vector <string> columnPrint;
        columnPrint.reserve(N);

        if(query.find(name) == query.end())
        {
            cout << "Error during PRINT: " << name << " does not name a table in the database\n";
            getline(cin, read);
            return;
        }

        for(int i = 0; i < N; i++)
        {
            cin >> read;
            columnPrint.push_back(read);
            if(query[name].columnNames.find(read) == query[name].columnNames.end())
            {
                cout << "Error during PRINT: " << read << " does not name a column in " << name << "\n";
                getline(cin, read);
                return;
            }
        }


        cin >> read;


        if (read == "ALL")
        {
            query[name].PRINTALL(columnPrint);
        }else if (read == "WHERE")
        {
            string colName;
            char operation;
            cin >> colName;
            cin >> operation;

            if(query[name].columnNames.find(colName) == query[name].columnNames.end())
            {
                cout << "Error during PRINT: " << colName << " does not name a column in " << name << "\n";
                getline(cin,read);
                return;
            }
            query[name].PRINTWHERE(colName,operation, columnPrint);
            // string value;
        }
    }

   // void PRINTALL(vector <string> & in)

   void GENERATE()
   {
        string read;
        string name;
        string idxType;
        string colName;
        cin >> read;
        cin >> name;
        cin >> idxType;
        cin >> read;
        cin >>read;
        cin >> colName;
        query[name].INDEX(idxType, colName);

        if(query.find(name) == query.end())
        {
            cout << "Error during GENERATE: " << name << " does not name a table in the database\n";
            getline(cin, read);
            return;
        }

        if(query[name].columnNames.find(colName) == query[name].columnNames.end())
        {
            cout << "Error during GENERATE: " << colName << " does not name a column in " << name << "\n";
            getline(cin,read);
            return;
        }

        if(idxType == "hash")
        {
            cout << "Created " << idxType << " index for table " << name << " on column " 
            << colName <<", with " << query[name].HASHindex.size() << " distinct keys\n";
        }else if(idxType == "bst")
        {
            cout << "Created " << idxType << " index for table " << name << " on column " 
            << colName <<", with " << query[name].BSTindex.size() << " distinct keys\n";
        }
        
   }

   void DELETE()
   {
        string read;
        cin >> read;
        string name;
        string Colname;
        cin >> name;
        cin >> read;
        cin >> Colname;
        if(query.find(name) == query.end())
        {
            cout << "Error during DELETE: " << name << " does not name a table in the database\n";
            getline(cin,read);
            return;
        }
        if(query[name].columnNames.find(Colname) == query[name].columnNames.end())
        {
            cout << "Error during DELETE: " << Colname << " does not name a column in " << name << "\n";
            getline(cin,read);
            return;
        }
        query[name].DEL(Colname);

   }

    void COMMENT()
    {
        string junk;
        getline(cin, junk);
        
    }

    void REMOVE()
    {
        string name;
        cin >> name;
        if(query.find(name) == query.end())
        {
            string junk;
            cout << "Error during REMOVE: " << name << " does not name a table in the database\n";
            getline(cin,junk);
            return;
        }
        query.erase(name);
        cout << "Table " << name << " removed\n";
    }

    void INSERT()
    {
        bool active = 0;
        
        string read;
        string name;
        cin >> read;
        cin >> name;
        size_t N = 0;
        cin >> N;

        if(query.find(name) == query.end())
        {
            cout << "Error during INSERT: " << name << " does not name a table in the database\n";
            getline(cin,read);
            return;
        }

        size_t rowSize = query[name].numRows;
        size_t ColumnSize = query[name].numColums;
        query[name].table.resize(rowSize + N);
        cin >> read;

        if(!query[name].BSTindex.empty() || !query[name].HASHindex.empty())
        {
            active =1;
        }


        for(size_t  i =rowSize; i <rowSize + N; i++)
        {
            query[name].table[i].reserve(ColumnSize);
            for(size_t j = 0; j < ColumnSize; j++)
            {
                EntryType type = query[name].columnTypes[j];
                switch(type)
                {
                    case EntryType::Int:
                    {
                        int value = 0;
                        cin >> value;
                        if(active && j == query[name].IndexCol)
                        {
                            if(!query[name].BSTindex.empty())
                            {
                                TableEntry temp (value);
                                query[name].BSTindex[temp].push_back(i);
                            }else if(!query[name].HASHindex.empty())
                            {
                                TableEntry temp (value);
                                query[name].HASHindex[temp].push_back(i);
                            }
                        }
                        query[name].table[i].emplace_back(value);
                        break;
                    }
                        
                    case EntryType::Bool:
                    {
                        bool value1 = 0;
                        cin >> value1;
                        if(active && j == query[name].IndexCol)
                        {
                            if(!query[name].BSTindex.empty())
                            {
                                TableEntry temp (value1);
                                query[name].BSTindex[temp].push_back(i);
                            }else if(!query[name].HASHindex.empty())
                            {
                                TableEntry temp (value1);
                                query[name].HASHindex[temp].push_back(i);
                            }
                        }
                        query[name].table[i].emplace_back(value1);
                        break;
                    }
                       
                    case EntryType::String:
                    {
                        cin >> read;
                        if(active && j == query[name].IndexCol)
                        {
                            if(!query[name].BSTindex.empty())
                            {
                                TableEntry temp (read);
                                query[name].BSTindex[temp].push_back(i);
                            }else if(!query[name].HASHindex.empty())
                            {
                                TableEntry temp (read);
                                query[name].HASHindex[temp].push_back(i);
                            }
                        }
                        query[name].table[i].emplace_back(read);
                        break;
                    }
                        
                    case EntryType::Double:
                    {
                        double value2 = 0;
                        cin >> value2;
                        //cout << value2 << '\n';
                        if(active && j == query[name].IndexCol)
                        {
                            if(!query[name].BSTindex.empty())
                            {
                                TableEntry temp (value2);
                                query[name].BSTindex[temp].push_back(i);
                            }else if(!query[name].HASHindex.empty())
                            {
                                TableEntry temp (value2);
                                query[name].HASHindex[temp].push_back(i);
                            }
                        }
                       
                        query[name].table[i].emplace_back(value2);
                        break;
                    }
                       
                }
                
            }
        }

        cout << "Added " << N << " rows to " << name << " from position " << rowSize << " to " << rowSize + N -1<< '\n';
        query[name].numRows += N;

    }

    void CREATE()
    {
        string read;
        string name;
        Table NTable;
        int N;
        cin>> name;
        NTable.name = name;
        cin >> N;
        NTable.numColums = N;

        if(query.find(name) != query.end())
        {
            cout << "Error during CREATE: Cannot create already existing table " << name << "\n";
            getline(cin, read);
            return;
        }
        for(int i = 0; i < N; i++)
        {
            cin >> read;
            if(read == "int")
            {
                NTable.columnTypes.push_back(EntryType::Int);
            }else if (read == "bool")
            {
                NTable.columnTypes.push_back(EntryType::Bool);
            }else if (read== "double")
            {
                NTable.columnTypes.push_back(EntryType::Double);
            }else if (read == "string")
            {
                NTable.columnTypes.push_back(EntryType::String);
            }
            
        }

        cout << "New table " << NTable.name << " with column(s) ";
        for(int i = 0; i < N; i++)
        {
            cin >> read;
            NTable.columnNames[read] = i;
            cout << read << " ";
        }
        cout << "created\n";
        query.emplace(name,NTable);
       
    }

    private:
    string cmd;
    unordered_map <string, Table> query;
};

int main(int argc, char * argv[])
{
    std::ios_base::sync_with_stdio(false);
    cin >> std::boolalpha;
    cout << std::boolalpha;
    SQL program;
    program.getOptions(argc,argv);
    program.run();
}
