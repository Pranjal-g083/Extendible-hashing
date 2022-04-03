// Program to implement extendible hashing
#include<bits/stdc++.h>
using namespace std;
// bucket for each hash value
class bucket{
    private:
    int n;
    int pointer;
    vector<int > v;
    public:
    //constructor
    bucket(int n)
    {
        this->n=n;
        v.assign(n,-1);
        pointer=0;
    }
    void insert(int value)
    {
        v[pointer]=value;
        pointer++;
//        cout<<value<<" "<<pointer<<endl;
    }
    bool search(int value)
    {
        for(int i=0;i<pointer;i++)
        {
            if(v[i]==value)
            {
                return 1;
            }
        }
        return 0;
    }
    bool deleteval(int value)
    {
        bool flag=false;
        for(int i=0;i<pointer;i++)
        {
            if(v[i]==value)
            {
                v[i]=-1;
                flag=true;
            }
        }
        if(flag)
        {
            // remove all -1
            for(int i=0;i<pointer;i++)
            {
                if(v[i]==-1)
                {
                    for(int j=i;j<pointer-1;j++)
                    {
                        v[j]=v[j+1];
                    }
                    pointer--;
                    break;
                }
            }
        }
        return flag;
    }
    bool isfull()
    {
        return pointer==n;
    }
    bool isempthy()
    {
        return pointer==0;
    }
    vector<int > get_vector()
    {
        return v;
    }
    int size()
    {
        return pointer;
    }
};
// extendible hashing class
class Extendible_Hashing
{
    private:
    vector<bucket *> v;
    vector<int> local_depth;
    int n,siz;  //global depth
    int global_depth;
    int numberofbuckets;
    // hashfunction
    int hash_function(int key,int num)
    {
        return key%num;
    }
    public:
    // constructor
    Extendible_Hashing(int n,int sz)
    {
        this->n=(1<<n);
        v.assign((1<<n),NULL);
        siz=sz;
        cout<<siz<<" "<<this-> n<<endl;
        for(int i=0;i<(1<<n);i++)
        {
            v[i]=new bucket(siz);
        }
        local_depth.assign((1<<n),n);
        global_depth=n;
    }

    // function to insert key value pair
    void insert_key_value_pair(int value)
    {
        int hash_value;
        for(int i=1;i<=global_depth;i++)
        {
            if(local_depth[(value&((1<<i)-1))]==i)
            {
                hash_value=(value&((1<<i)-1));
                break;
            }
        }
        cout<<hash_value<<endl;
        while(v[hash_value]->isfull())
        {
            if(local_depth[hash_value]==global_depth)
            {
                v.resize(v.size()*2,NULL);
                n*=2;
                global_depth++;
                local_depth.resize(v.size());
                vector<int >var;
                if(v[hash_value])
                var=v[hash_value]->get_vector();
                v[hash_value]=new bucket(siz);
                int num=0;
                if(hash_value>=n/2)
                num=hash_value-n/2;
                else
                num=hash_value+n/2;
                v[num]=new bucket(siz);
                for(int i=0;i<var.size();i++)
                {
                    int hash_value=hash_function(var[i],n);
//                    cout<<hash_value<<" ";
                    v[hash_value]->insert(var[i]);
                    local_depth[hash_value]=global_depth;
                }
                hash_value=hash_function(value,n);
            }
            else
            {
                vector<int >var;
                int num=0;
                
                if(v[hash_value])
                var=v[hash_value]->get_vector();
                v[hash_value]=new bucket(siz);
                v[num]=new bucket(siz);
                int hash=local_depth[hash_value]+1;
                if(hash_value>=(1<<(hash-1)))
                num=hash_value-(1<<(hash-1));
                else
                num=hash_value+(1<<(hash-1));
                local_depth[hash_value]=hash;
                local_depth[num]=hash;
                for(int i=0;i<var.size();i++)
                {
                    int hash_value=hash_function(var[i],1<<hash);
//                    cout<<hash_value<<" ";
                    v[hash_value]->insert(var[i]);
                    
                }
                hash_value=hash_function(value,1<<hash);
                
            }
        }
        // else
        {
//            cout<<hash_value<<" ";

            v[hash_value]->insert(value);
        }
    }
    // deleting key value pair
    void delete_key_value_pair(int value)
    {
        int hash_value;
        for(int i=1;i<=global_depth;i++)
        {
            if(local_depth[(value&((1<<i)-1))]==i)
            {
                hash_value=(value&((1<<i)-1));
                break;
            }
        }
        if(v[hash_value]->deleteval(value))
        {
            if(v[hash_value]->isempthy())
            {
                local_depth[hash_value]--;
                
                if(hash_value>=1<<local_depth[hash_value])
                {
                    v[hash_value]=NULL;
                    local_depth[hash_value-1<<local_depth[hash_value]]--;
                }
                else
                {
                    v[hash_value]=v[hash_value+1<<local_depth[hash_value]];
                    local_depth[hash_value+1<<local_depth[hash_value]]--;
                    v[hash_value+1<<local_depth[hash_value]]=NULL;
                }
            }
        }
        else
        {
            cout<<"No such value present in the hash table"<<endl;
        }
    }
    //function to display the contents
    void display()
    {
        cout<<global_depth<<endl;
        numberofbuckets=0;
        for(int i=0;i<v.size();i++)
        {
            if(v[i])
            {
                numberofbuckets++;
            }
        }
        cout<<numberofbuckets<<endl;
        for(int i=0;i<v.size();i++)
        {
            if(v[i])
            {
                cout<<v[i]->size();
//                cout<<endl;
                vector<int> v1;
                v1=v[i]->get_vector();
//                for(auto x:v1)
//                    cout<<v1[i]<<" ";
//                cout<<endl;
                cout<<" "<<local_depth[i]<<endl;
            }
        }
    }
    // search a value in hash table
    bool search (int key)
    {
        int hash_value;
        for(int i=1;i<=global_depth;i++)
        {
            if(local_depth[(key&((1<<i)-1))]==i)
            {
                hash_value=(key&((1<<i)-1));
                break;
            }
        }
        if(v[hash_value]->search(key))
        {
            return true;
        }
        else
        {
            return false;
        }

    }

};

int main()
{
    int gd,sz;
    cin>>gd>>sz;
    Extendible_Hashing e(gd,sz);
    int n;
    while(n!=6)
    {
        cin>>n;
        if(n>1&&n<=4)
        {
            int value;
            cin>>value;
            if(n==2)
            e.insert_key_value_pair(value);
            else if(n==3)
            cout<<e.search(value)<<endl;
            else
            e.delete_key_value_pair(value);
        }
        else if(n==5)
        e.display();
        else
        break;
    }
}