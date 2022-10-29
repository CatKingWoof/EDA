#include <iostream>
#include <string>
#include<vector>
#include<fstream>
#include <sstream>
#include<ctime>
#include <random>
using namespace std;

class node;
class gain_table;
class net{
public:
    int ID;
    vector <node*> node_part_A;
    vector <node*> node_part_B;
    vector <int> node_connect;
    int num_A;
    int num_B;
    //bool lock_A;
    //bool lock_B;
    bool critical;
    net():ID(0),num_A(0),num_B(0),critical(0){}
    void initial(int net_num);
    void check_critical();
};

class node{
    friend class net;
public:
    int ID;
    int gain;
    int num_net;
    bool part;
    bool lock;
    vector <net*> connect_net;
    node():ID(0),gain(0),lock(0),part(0),num_net(0){}
    void initial(int node_num,bool p);
    void set_gain();
    void move(gain_table* bucket_list_A,gain_table* bucket_list_B);

};

class bucket{
public:
    vector<node*> chain;
};


class gain_table{
public:
    int Pmax;
    //int node_num;
    int max_gain;
    int node_num;
    vector <bucket> table;
    gain_table(int P):Pmax(P),max_gain(0),node_num(0)
    {
        table.resize(2*Pmax+1);
    }
    void make_table(node* n);
    void erase_table(node* n);
    void check_max();
    node* max_gain_node();
};
class step{
public:
    int cell;
    int gain;
    int gain_acc;
    step():cell(0),gain(0),gain_acc(0){}
};
class step_table{
public:
    vector<step> step_list;
};


int main(int argc,char *argv[])
{
    ifstream ifs;
    ofstream ofs;
    int total_net;
    int total_node;
    int a;
    int l,g,walk;
    int total_A=0,total_B=0;
    string s,dir;
    net* tmp_net;
    node* tmp_node;
    step tmp_step;
    step_table iteration;
    clock_t start,stop;
    clock_t check_time[2];
    start = clock();
	std::srand(std::time(nullptr));
    ifs.open(argv[1]);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } else
    {
        ifs>>total_net;
        ifs>>total_node;
        //initial node
		vector<node> nodelist;
		nodelist.resize(total_node+1);
        for(int i=1;i<=total_node;i++)
        {
            bool p=0;
			
            if(i%2==0)
            {
                p=1;
                total_B+=1;
            }
            else
                total_A+=1;
			
			/*if(i<=total_node/2)
			{
                p=1;
                total_B+=1;
            }
            else
                total_A+=1;*/
			
			

            nodelist[i].initial(i,p);
        }

        //initial net
		vector<net> netlist;
		netlist.resize(total_net+1);
        getline(ifs,s);
        for(int i=1;i<=total_net;i++)
        {

            getline(ifs,s);
            stringstream ss(s);

            while(getline(ss, dir, ' ')){
                int a=stoi(dir);
                netlist[i].node_connect.push_back(a);
                nodelist[a].num_net+=1;
                nodelist[a].connect_net.push_back(&netlist[i]);
                if(nodelist[a].part==0)
                {
                    netlist[i].node_part_A.push_back(&nodelist[a]);
                    netlist[i].num_A+=1;
                }
                else
                {
                    netlist[i].node_part_B.push_back(&nodelist[a]);
                    netlist[i].num_B+=1;
                }

            }
            netlist[i].initial(i);
        }
        node current_node[total_node+1];
        for(int i=0;i<=total_node;i++)
        {
            current_node[i]=nodelist[i];
        }
        //create gain list
        int Pmax=0;
            for(int i=1;i<=total_node;i++)
            {

                if(nodelist[i].num_net>Pmax)
                    Pmax=nodelist[i].num_net;
            }
            gain_table A(Pmax);
            gain_table B(Pmax);

        //iteration
        while(1)
        {

            for(int i=1;i<=total_net;i++)
            {
                netlist[i].check_critical();

            }

            for(int i=1;i<=total_node;i++)
            {
                nodelist[i].set_gain();
                //cout<<nodelist[i].gain<<" ";
                if(nodelist[i].part==0)
                {
                    A.make_table(&nodelist[i]);
                }
                else
                {
                    B.make_table(&nodelist[i]);
                }

            }
            //cout<<"a_num"<<total_A;
            //cout<<"b_num"<<total_B;
            A.check_max();
            B.check_max();
            /*cout<<"P:"<<Pmax;
            cout<<A.table[2].chain[0]->ID;
            cout<<"A:"<<A.max_gain<<" B:"<<B.max_gain;*/
            //find max gain node
            if((float(total_A)-1.0)<0.45*float(total_node)-1.0)
            {
                //cout<<"a";
                tmp_node=B.max_gain_node();
                total_B-=1;
                total_A+=1;
                B.erase_table(tmp_node);
            }
            else if((float(total_A)+1.0)>0.55*float(total_node)+1.0)
            {
                //cout<<"b";
                tmp_node=A.max_gain_node();
                total_A-=1;
                total_B+=1;
                A.erase_table(tmp_node);
            }
            else if((A.max_gain>B.max_gain))
            {
                //cout<<"c";
                tmp_node=A.max_gain_node();
                total_A-=1;
                total_B+=1;
                A.erase_table(tmp_node);
            }
            else
            {
                //cout<<"d";
                tmp_node=B.max_gain_node();
                total_B-=1;
                total_A+=1;
                B.erase_table(tmp_node);
            }
            //cout<<tmp_node->ID;
            tmp_step.cell=tmp_node->ID;
            tmp_step.gain=tmp_node->gain;
            tmp_step.gain_acc+=tmp_node->gain;
            iteration.step_list.push_back(tmp_step);
            //move
            tmp_node->move(&A,&B);
            //cout<<endl;



            for(int k=0;k<total_node-1;k++)
            {

                A.check_max();
                B.check_max();
                //cout<<"A:"<<A.node_num<<" B:"<<B.node_num<<endl;
                if((float(total_A)-1.0)<0.45*float(total_node)-1.0)
                {
                    //cout<<"a";
                    tmp_node=B.max_gain_node();
                    total_B-=1;
                    total_A+=1;
                    B.erase_table(tmp_node);
                }
                else if((float(total_A)+1.0)>0.55*float(total_node)+1.0)
                {
                    //cout<<"b";
                    tmp_node=A.max_gain_node();
                    total_A-=1;
                    total_B+=1;
                    A.erase_table(tmp_node);
                }
                else if(A.max_gain>B.max_gain||B.node_num<=0)
                {
                    //cout<<"c";
                    tmp_node=A.max_gain_node();
                    total_A-=1;
                    total_B+=1;
                    A.erase_table(tmp_node);
                }
                else
                {
                    //cout<<"d";
                    tmp_node=B.max_gain_node();
                    total_B-=1;
                    total_A+=1;
                    B.erase_table(tmp_node);
                }
                //cout<<tmp_node->ID;
                tmp_step.cell=tmp_node->ID;
                tmp_step.gain=tmp_node->gain;
                tmp_step.gain_acc+=tmp_node->gain;
                iteration.step_list.push_back(tmp_step);
                //move
                tmp_node->move(&A,&B);


                //cout<<endl;
				stop = clock();
                if((double(stop - start) / CLOCKS_PER_SEC)>29.5)  break ;
            }
			//cout<<"qwerqwer";
            //iterate_end
			/*stop = clock();
			if((double(stop - start) / CLOCKS_PER_SEC)>29)  break ;*/
			l=0;
            l=iteration.step_list.size();
			
            g=0;
            walk=0;
            for(int i=0;i<l;i++)
            {
                if(iteration.step_list[i].gain_acc>g)
                {
                    g=iteration.step_list[i].gain_acc;
                    walk=i+1;

                }
            }
            
			//cout<<l<<"walk:"<<walk<<"g:"<<g<<endl;
            for(int i=0;i<walk;i++)
            {
                current_node[iteration.step_list[i].cell].part=!current_node[iteration.step_list[i].cell].part;
            }
            total_A=0;
            total_B=0;
            for(int i=1;i<=total_node;i++)
            {
                nodelist[i]=current_node[i];
                if(current_node[i].part==0)
                {
                    total_A+=1;
                }
                else
                {
                    total_B+=1;
                }
            }

            for(int i=1;i<=total_net;i++)
            {
				if(netlist[i].node_part_A.size()>0)
				{
					netlist[i].node_part_A.clear();
				}
                if(netlist[i].node_part_B.size()>0)
				{
					netlist[i].node_part_B.clear();
				}
                
                netlist[i].num_A=0;
                netlist[i].num_B=0;
                for(int j=0;j<int(netlist[i].node_connect.size());j++)
                {
                    a=netlist[i].node_connect[j];
                    if(nodelist[a].part==0)
                    {
                        netlist[i].node_part_A.push_back(&nodelist[a]);
                        netlist[i].num_A+=1;
                    }
                    else
                    {
                        netlist[i].node_part_B.push_back(&nodelist[a]);
                        netlist[i].num_B+=1;
                    }
                }

            }
            //refresh gain list
                for(int i=0;i<A.table.size();i++)
                {
                    A.table[i].chain.clear();
                }
                for(int i=0;i<B.table.size();i++)
                {
                    B.table[i].chain.clear();
                }
            A.max_gain=0;
            B.max_gain=0;
            A.node_num=0;
            B.node_num=0;
			if(iteration.step_list.size()>0)
			{
				iteration.step_list.clear();
			}
            tmp_step.gain_acc=0;
            stop = clock();
            if((double(stop - start) / CLOCKS_PER_SEC)>29.5)  break ;

        }
        ofs.open("output.txt");
        for(int i=1;i<=total_node;i++)
        {
            ofs<<current_node[i].part<<endl;
        }
		stop = clock();
		cout<<double(stop - start) / CLOCKS_PER_SEC;

    }



    return 0;
}
void node::initial(int node_num,bool p){
    ID=node_num;
    part=p;

}

void node::set_gain(){
    int l=connect_net.size();
    for(int i=0;i<l;i++)
    {
        if(connect_net[i]->critical)
        {
            //cout<<connect_net[i]->ID<<" "<<connect_net[i]->num_A<<" "<<connect_net[i]->num_B<<" "<<endl;
            if(part==0)
            {
                if(connect_net[i]->num_A==1)
                {
                    gain+=1;
                    /*if(ID==4656)
                    {
                        cout<<"k"<<connect_net[i]->ID;
                    }*/


                }
                if(connect_net[i]->num_B==0)
                {
                    gain-=1;
                    //cout<<connect_net[i]->ID<<"bbb";
                    /*if(ID==4656)
                    {
                        cout<<"m"<<connect_net[i]->ID;
                    }*/
                }
            }
            else if(part==1)
            {
                if(connect_net[i]->num_B==1)
                {
                    gain+=1;
                    //cout<<connect_net[i]->ID<<"ccc";
                    /*if(ID==4656)
                    {
                        cout<<"n"<<connect_net[i]->ID<<" ";
                    }*/
                }
                if(connect_net[i]->num_A==0)
                {
                    gain-=1;
                    //cout<<connect_net[i]->ID<<"ddd";
                    /*if(ID==4656)
                    {
                        cout<<"g"<<connect_net[i]->ID<<" ";
                    }*/
                }
            }
        }
    }
}

void net::check_critical(){
    if((num_A==0||num_A==1)||(num_B==0||num_B==1))
        critical=1;
    else
        critical=0;
}

void net::initial(int net_num){
    ID=net_num;
}

void gain_table::make_table(node* n){
    int g=n->gain;
    node_num+=1;
    //cout<<"before:"<<table[g+Pmax].chain.size()<<" ";
    /*if(n->ID==4656)
        cout<<" "<<g<<" "<<n->num_net<<"  "<<n->ID;*/
    table[g+Pmax].chain.push_back(n);
    //cout<<"after:"<<table[g+Pmax].chain.size()<<" ";
    /*if(n->ID==4656)
        cout<<"a";*/



}

void gain_table::check_max(){
    for(int i=Pmax*2;i>=0;i--)
    {
        if(int(table[i].chain.size())!=0)
        {
            max_gain=i;
            break;
        }
    }
	
}

void gain_table::erase_table(node* n){
    int g=n->gain;
    node_num-=1;
    //cout<<"before:"<<table[g+Pmax].chain.size()<<" ";
    for(int j=0;j<int(table[g+Pmax].chain.size());j++)
    {
        if(table[g+Pmax].chain[j]->ID==n->ID)
        {
            table[g+Pmax].chain.erase(table[g+Pmax].chain.begin()+j);
        }
    }
    //cout<<"after:"<<table[g+Pmax].chain.size()<<" ";
    /*if(n->ID==4656)
        cout<<"b";*/
}


node* gain_table::max_gain_node(){

        int l=table[max_gain].chain.size();
		if(l==0)
			return table[max_gain].chain[0];
		else
		{
			int r= int(rand()) % l  ;
			return table[max_gain].chain[r];
		}
        
}

void node::move(gain_table* bucket_list_A,gain_table* bucket_list_B){
    lock=1;
    for(int i=0;i<int(connect_net.size());i++)
    {

        if(connect_net[i]->critical)
        {

            if(part==0)
            {

                if(connect_net[i]->num_B==0)
                {
                    for(int j=0;j<int(connect_net[i]->node_part_A.size());j++)
                    {


                        //cout<<"a"<<connect_net[i]->node_part_A[j]->ID;
                        if(connect_net[i]->node_part_A[j]->lock==0)
                        {
                            bucket_list_A->erase_table(connect_net[i]->node_part_A[j]);
                            connect_net[i]->node_part_A[j]->gain+=1;
                            bucket_list_A->make_table(connect_net[i]->node_part_A[j]);
                        }


                        /*if(connect_net[i]->node_part_A[j]->ID==4656)
                        {
                            cout<<"a"<<" "<<connect_net[i]->ID<<" ";
                            cout<<connect_net[i]->node_part_A[j]->gain;
                             cout<<endl;

                        }*/


                    }

                }
                else if(connect_net[i]->num_B==1)
                {

                    //cout<<"e";
                    if(connect_net[i]->node_part_B[0]->lock==0)
                    {
                        bucket_list_B->erase_table(connect_net[i]->node_part_B[0]);
                        connect_net[i]->node_part_B[0]->gain-=1;
                        bucket_list_B->make_table(connect_net[i]->node_part_B[0]);
                    }


                    /*if(connect_net[i]->node_part_B[0]->ID==4656)
                    {
                       cout<<"b"<<" "<<connect_net[i]->ID<<" ";
                       cout<<connect_net[i]->node_part_B[0]->gain;
                        cout<<endl;
                    }*/


                }

                connect_net[i]->num_A-=1;
                connect_net[i]->num_B+=1;

                //erase node
                for(int j=0;j<int(connect_net[i]->node_part_A.size());j++)
                {
                    if(connect_net[i]->node_part_A[j]->ID==ID)
                    {
                        connect_net[i]->node_part_A.erase(connect_net[i]->node_part_A.begin()+j);
                        connect_net[i]->node_part_B.push_back(this);
                    }
                }
                //connect_net[i]->lock_B=1;


                if(connect_net[i]->num_A==0)
                {
                    for(int j=0;j<int(connect_net[i]->node_part_B.size());j++)
                    {
                        if(connect_net[i]->node_part_B[j]->lock==0)
                        {
                           bucket_list_B->erase_table(connect_net[i]->node_part_B[j]);

                            connect_net[i]->node_part_B[j]->gain-=1;

                            bucket_list_B->make_table(connect_net[i]->node_part_B[j]);
                        }


                        /*if(connect_net[i]->node_part_B[j]->ID==4656)
                        {
                           cout<<"c"<<" "<<connect_net[i]->ID<<" ";
                        cout<<connect_net[i]->node_part_B[j]->gain;
                        cout<<endl;

                        }*/

                    }
                }
                else if(connect_net[i]->num_A==1)
                {
                    //cout<<"d"<<connect_net[i]->node_part_A[0]->ID;
                    //cout<<"y";
                    if(connect_net[i]->node_part_A[0]->lock==0)
                    {
                        bucket_list_A->erase_table(connect_net[i]->node_part_A[0]);
                        connect_net[i]->node_part_A[0]->gain+=1;
                        bucket_list_A->make_table(connect_net[i]->node_part_A[0]);
                    }


                    //cout<<"d"<<connect_net[i]->node_part_A[0]->ID;
                    /*if(connect_net[i]->node_part_A[0]->ID==4656)
                    {
                        cout<<"d"<<" "<<connect_net[i]->ID<<" ";
                        cout<<connect_net[i]->node_part_A[0]->gain;
                        cout<<endl;

                    }*/

                }



            }else
            {

               if(connect_net[i]->num_A==0)
                {
                    for(int j=0;j<int(connect_net[i]->node_part_B.size());j++)
                    {
                        if(connect_net[i]->node_part_B[j]->lock==0)
                        {
                           bucket_list_B->erase_table(connect_net[i]->node_part_B[j]);
                            connect_net[i]->node_part_B[j]->gain+=1;
                            bucket_list_B->make_table(connect_net[i]->node_part_B[j]);
                        }

                        /*if(connect_net[i]->node_part_B[j]->ID==4656)
                        {
                            cout<<"e"<<" "<<connect_net[i]->ID<<" ";
                            cout<<connect_net[i]->node_part_B[j]->gain;
                            cout<<endl;

                        }*/

                    }

                }
                else if(connect_net[i]->num_A==1)
                {
                    if(connect_net[i]->node_part_A[0]->lock==0)
                    {
                        bucket_list_A->erase_table(connect_net[i]->node_part_A[0]);
                        connect_net[i]->node_part_A[0]->gain-=1;
                        bucket_list_A->make_table(connect_net[i]->node_part_A[0]);
                    }


                    /*if(connect_net[i]->node_part_A[0]->ID==4656)
                    {
                        cout<<"f"<<" "<<connect_net[i]->ID<<" ";
                        cout<<connect_net[i]->node_part_A[0]->gain;
                        cout<<endl;

                    }*/

                }

                connect_net[i]->num_B-=1;
                connect_net[i]->num_A+=1;

                //erase node
                for(int j=0;j<int(connect_net[i]->node_part_B.size());j++)
                {
                    if(connect_net[i]->node_part_B[j]->ID==ID)
                    {
                        connect_net[i]->node_part_B.erase(connect_net[i]->node_part_B.begin()+j);
                        connect_net[i]->node_part_A.push_back(this);
                    }
                }
                //connect_net[i]->lock_A=1;

                if(connect_net[i]->num_B==0)
                {
                    for(int j=0;j<int(connect_net[i]->node_part_A.size());j++)
                    {
                        if(connect_net[i]->node_part_A[j]->lock==0)
                        {
                           bucket_list_A->erase_table(connect_net[i]->node_part_A[j]);
                            connect_net[i]->node_part_A[j]->gain-=1;
                            bucket_list_A->make_table(connect_net[i]->node_part_A[j]);
                        }

                        /*if(connect_net[i]->node_part_A[j]->ID==4656)
                        {
                            cout<<"g"<<" "<<connect_net[i]->ID<<" ";
                            cout<<connect_net[i]->node_part_A[j]->gain;
                            cout<<endl;

                        }*/

                    }

                }
                else if(connect_net[i]->num_B==1)
                {
                    if(connect_net[i]->node_part_B[0]->lock==0)
                    {
                        bucket_list_B->erase_table(connect_net[i]->node_part_B[0]);
                        connect_net[i]->node_part_B[0]->gain+=1;
                        bucket_list_B->make_table(connect_net[i]->node_part_B[0]);
                    }

                    /*if(connect_net[i]->node_part_B[0]->ID==4656)
                    {
                        cout<<"h"<<" "<<connect_net[i]->ID<<" ";
                        cout<<connect_net[i]->node_part_B[0]->gain;
                        cout<<endl;

                    }*/

                }

            }


        }
        else
        {
            if(part==0)
            {
                for(int j=0;j<int(connect_net[i]->node_part_A.size());j++)
                {
                    if(connect_net[i]->node_part_A[j]->ID==ID)
                    {
                        connect_net[i]->node_part_A.erase(connect_net[i]->node_part_A.begin()+j);
                        connect_net[i]->node_part_B.push_back(this);
                    }
                }
            }
            else
            {
                for(int j=0;j<int(connect_net[i]->node_part_B.size());j++)
                {
                    if(connect_net[i]->node_part_B[j]->ID==ID)
                    {
                        connect_net[i]->node_part_B.erase(connect_net[i]->node_part_B.begin()+j);
                        connect_net[i]->node_part_A.push_back(this);
                    }
                }
            }
        }
    }
    part=!part;
}
