#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include<ctime>
#include <random>
using namespace std;
class Net{
public:
int src_x,src_y,tar_x,tar_y,net_num;
int min_dist;
bool in_Q,find;
int net_l,dir;
string name;
vector <int> point_x;
vector <int> point_y;
void Initial(string s,int n,int sx,int sy,int tx,int ty);
};

class Grid{
public:
int x,y;
bool b;
int c;
void Initial(int i,int j);
};

int main(int argc,char *argv[])
{

	ifstream ifs;
    ofstream ofs;
	ifs.open(argv[1]);
	ofs.open(argv[2]);
	string s;
	int row_n,col_n,block_n,net_n;
	int left_x,right_x,up_y,down_y;
	int src_x,src_y,tar_x,tar_y;
	bool find_all=0;
	int first_time=1;
	int round_grid=0,best_grid=0;
	vector <Net*> connected_net,temp_net;
	clock_t start,stop;
    clock_t check_time[2];
	start = clock();
	std::srand(std::time(nullptr));

	if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } else
    {

		ifs>>s;
		ifs>>s;
		row_n=atoi(s.c_str());
		ifs>>s;
		ifs>>s;
		col_n=atoi(s.c_str());
		ifs>>s;
		ifs>>s;
		block_n=atoi(s.c_str());
		Grid grid [col_n][row_n];
		bool init_map[col_n][row_n];
		for(int j=0;j<row_n;j++)
		{
			for(int i=0;i<col_n;i++)
			{
				grid[i][j].Initial(i,j);
                init_map[i][j]=0;
			}
		}


        for(int t=0;t<block_n;t++)
		{
            ifs>>s;
            left_x=atoi(s.c_str());
            ifs>>s;
            right_x=atoi(s.c_str());
            ifs>>s;
            down_y=atoi(s.c_str());
            ifs>>s;
            up_y=atoi(s.c_str());

            for(int j=down_y;j<=up_y;j++)
            {
                for(int i=left_x;i<=right_x;i++)
                {
                    grid[i][j].b=1;
                    init_map[i][j]=1;
                }
            }

		}


		ifs>>s;
		ifs>>s;
		net_n=atoi(s.c_str());
		Net net[net_n],best_net[net_n];
		string n_name;
		for(int t=1;t<=net_n;t++)
		{
		    ifs>>s;
			n_name=s;
            ifs>>s;
            src_x=atoi(s.c_str());
            ifs>>s;
            src_y=atoi(s.c_str());
            ifs>>s;
            tar_x=atoi(s.c_str());
            ifs>>s;
            tar_y=atoi(s.c_str());

            net[t-1].Initial(n_name,t,src_x,src_y,tar_x,tar_y);
			init_map[src_x][src_y]=1;
            init_map[tar_x][tar_y]=1;
            grid[src_x][src_y].b=1;
			grid[tar_x][tar_y].b=1;
		}
		/*for(int j=row_n-1;j>=0;j--)
		{
			for(int i=0;i<col_n;i++)
			{
				cout<<grid[i][j].b;
			}
			cout<<endl;
		}
		cout<<endl;*/

        //net ordering
		int min;
		Net* min_net;

		for(int l=0;l<net_n;l++)
        {

            for(int t=0;t<net_n;t++)
            {
                if(net[t].in_Q==0)
                {
                    min=net[t].min_dist;
                    min_net=&net[t];
                }

            }

            for(int t=0;t<net_n;t++)
            {
                if(net[t].min_dist<min&&net[t].in_Q==0)
                {
                    min_net=&net[t];
                    min=min_net->min_dist;
                }
            }

            connected_net.push_back(min_net);
            min_net->in_Q=1;

        }

        //find path
        queue <Grid*> grid_Q;
        Grid* current_grid;
		while(1)
		{
			//cout<<"a";
			stop = clock();
			if((double(stop - start) / CLOCKS_PER_SEC)>90.0)  break ;
			while(find_all==0)
			{

				for(int i=0;i<net_n;i++)
				{

					src_x=connected_net[i]->src_x;
					src_y=connected_net[i]->src_y;
					tar_x=connected_net[i]->tar_x;
					tar_y=connected_net[i]->tar_y;
					grid[src_x][src_y].c=1;
					grid_Q.push(&grid[src_x][src_y]);
					//cout<<"a";
					while(grid_Q.size()!=0)
					{

						current_grid=grid_Q.front();
						grid_Q.pop();

						if(current_grid->x==tar_x&&current_grid->y-1==tar_y)
						{
							connected_net[i]->find=1;
							connected_net[i]->net_l=current_grid->c+1;
							//connected_net[i]->dir=1;
							break;
						}
						else if(current_grid->x+1==tar_x&&current_grid->y==tar_y)
						{
							connected_net[i]->find=1;
							connected_net[i]->net_l=current_grid->c+1;
							//connected_net[i]->dir=4;
							break;
						}
						else if(current_grid->x-1==tar_x&&current_grid->y==tar_y)
						{
							connected_net[i]->find=1;
							connected_net[i]->net_l=current_grid->c+1;
							//connected_net[i]->dir=3;
							break;
						}
						else if(current_grid->x==tar_x&&current_grid->y+1==tar_y)
						{
							connected_net[i]->find=1;
							connected_net[i]->net_l=current_grid->c+1;
							//connected_net[i]->dir=2;
							break;
						}

						if(current_grid->y-1>=0&&grid[current_grid->x][current_grid->y-1].b==0&&grid[current_grid->x][current_grid->y-1].c==0)
						{

							grid_Q.push(&grid[current_grid->x][current_grid->y-1]);
							grid[current_grid->x][current_grid->y-1].c=current_grid->c+1;

						}
						if(current_grid->x+1<col_n&&grid[current_grid->x+1][current_grid->y].b==0&&grid[current_grid->x+1][current_grid->y].c==0)
						{

							grid_Q.push(&grid[current_grid->x+1][current_grid->y]);
							grid[current_grid->x+1][current_grid->y].c=current_grid->c+1;

						}

						if(current_grid->x-1>=0&&grid[current_grid->x-1][current_grid->y].b==0&&grid[current_grid->x-1][current_grid->y].c==0)
						{
							grid_Q.push(&grid[current_grid->x-1][current_grid->y]);
							grid[current_grid->x-1][current_grid->y].c=current_grid->c+1;

						}
						if(current_grid->y+1<row_n&&grid[current_grid->x][current_grid->y+1].b==0&&grid[current_grid->x][current_grid->y+1].c==0)
						{

							grid_Q.push(&grid[current_grid->x][current_grid->y+1]);
							grid[current_grid->x][current_grid->y+1].c=current_grid->c+1;

						}

					}

					/*cout<<endl;
					for(int j=row_n-1;j>=0;j--)
					{
						for(int i=0;i<col_n;i++)
						{
							cout<<grid[i][j].c<<" ";
						}
						cout<<endl;
					}
					cout<<endl;*/
					/*cout<<"b";
					cout<<grid[15][2].c;*/
					//trace
					int trace_c=connected_net[i]->net_l-1;
					int trace_x=connected_net[i]->tar_x;
					int trace_y=connected_net[i]->tar_y;
					bool point=0;
					int r;
					if(connected_net[i]->find==1)
					{
						//cout<<"c";
						if(i==net_n-1)
						{
							find_all=1;
						}
						r= int(rand()) % 4  ;
						if(r==0)
						{
							if(grid[trace_x][trace_y+1].c==trace_c)
							{
								connected_net[i]->dir=1;
							}
							else if(grid[trace_x-1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=4;
							}
							else if(grid[trace_x+1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=3;
							}
							else if(grid[trace_x][trace_y-1].c==trace_c)
							{
								connected_net[i]->dir=2;
							}
						}
						else if(r==1)
						{
							if(grid[trace_x][trace_y-1].c==trace_c)
							{
								connected_net[i]->dir=1;
							}
							else if(grid[trace_x-1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=4;
							}
							else if(grid[trace_x+1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=3;
							}
							else if(grid[trace_x][trace_y+1].c==trace_c)
							{
								connected_net[i]->dir=2;
							}
						}
						else if(r==2)
						{
							if(grid[trace_x][trace_y+1].c==trace_c)
							{
								connected_net[i]->dir=1;
							}
							else if(grid[trace_x+1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=4;
							}
							else if(grid[trace_x-1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=3;
							}
							else if(grid[trace_x][trace_y-1].c==trace_c)
							{
								connected_net[i]->dir=2;
							}
						}
						else
						{
							if(grid[trace_x][trace_y-1].c==trace_c)
							{
								connected_net[i]->dir=1;
							}
							else if(grid[trace_x+1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=4;
							}
							else if(grid[trace_x-1][trace_y].c==trace_c)
							{
								connected_net[i]->dir=3;
							}
							else if(grid[trace_x][trace_y+1].c==trace_c)
							{
								connected_net[i]->dir=2;
							}
						}
						/*cout<<"d"<<r;
						cout<<endl;
					for(int j=row_n-1;j>=0;j--)
					{
						for(int i=0;i<col_n;i++)
						{
							cout<<grid[i][j].c<<" ";
						}
						cout<<endl;
					}
					cout<<endl;*/
						while(trace_x!=src_x||trace_y!=src_y)
						{
							//cout<<"e"<<trace_x<<" "<<trace_y<<" "<<trace_c;
							//cout<<r;
							grid[trace_x][trace_y].b=1;
							if(r==0)
							{
								if(grid[trace_x][trace_y+1].c==trace_c&&trace_y+1<row_n)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=1)
									{
										connected_net[i]->dir=1;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_y+=1;

								}
								else if(grid[trace_x-1][trace_y].c==trace_c&&trace_x-1>=0)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=4)
									{
										connected_net[i]->dir=4;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_x-=1;
								}
								else if(grid[trace_x+1][trace_y].c==trace_c&&trace_x+1<col_n)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=3)
									{
										connected_net[i]->dir=3;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_x+=1;
								}
								else if(grid[trace_x][trace_y-1].c==trace_c&&trace_y-1>=0)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=2)
									{
										connected_net[i]->dir=2;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_y-=1;
								}
							}
							else if(r==1)
							{
								if(grid[trace_x][trace_y-1].c==trace_c&&trace_y-1>=0)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=1)
									{
										connected_net[i]->dir=1;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_y-=1;

								}
								else if(grid[trace_x-1][trace_y].c==trace_c&&trace_x-1>=0)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=4)
									{
										connected_net[i]->dir=4;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_x-=1;
								}
								else if(grid[trace_x+1][trace_y].c==trace_c&&trace_x+1<col_n)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=3)
									{
										connected_net[i]->dir=3;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_x+=1;
								}
								else if(grid[trace_x][trace_y+1].c==trace_c&&trace_y+1<row_n)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=2)
									{
										connected_net[i]->dir=2;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_y+=1;
								}
							}
							else if(r==2)
							{
								if(grid[trace_x][trace_y+1].c==trace_c&&trace_y+1<row_n)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=1)
									{
										connected_net[i]->dir=1;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_y+=1;

								}
								else if(grid[trace_x+1][trace_y].c==trace_c&&trace_x+1<col_n)
								{
									//cout<<"no";
									trace_c-=1;

									if(connected_net[i]->dir!=4)
									{
										connected_net[i]->dir=4;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_x+=1;
								}
								else if(grid[trace_x-1][trace_y].c==trace_c&&trace_x-1>=0)
								{
									//cout<<"yes";
									trace_c-=1;

									if(connected_net[i]->dir!=3)
									{
										connected_net[i]->dir=3;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_x-=1;
								}
								else if(grid[trace_x][trace_y-1].c==trace_c&&trace_y-1>=0)
								{
									trace_c-=1;
									point=0;
									if(connected_net[i]->dir!=2)
									{
										connected_net[i]->dir=2;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);

									}
									trace_y-=1;

								}
							}
							else
							{
								if(grid[trace_x][trace_y-1].c==trace_c&&trace_y-1>=0)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=1)
									{
										connected_net[i]->dir=1;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);

									}
									trace_y-=1;

								}
								else if(grid[trace_x+1][trace_y].c==trace_c&&trace_x+1<col_n)
								{
									trace_c-=1;


									if(connected_net[i]->dir!=4)
									{
										connected_net[i]->dir=4;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);

									}
									trace_x+=1;
								}
								else if(grid[trace_x-1][trace_y].c==trace_c&&trace_x-1>=0)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=3)
									{
										connected_net[i]->dir=3;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);

									}
									trace_x-=1;
								}
								else if(grid[trace_x][trace_y+1].c==trace_c&&trace_y+1<row_n)
								{
									trace_c-=1;

									if(connected_net[i]->dir!=2)
									{
										connected_net[i]->dir=2;
										connected_net[i]->point_x.push_back(trace_x);
										connected_net[i]->point_y.push_back(trace_y);
									}
									trace_y+=1;
								}
							}




						}


					}


					for(int j=0;j<row_n;j++)
					{
						for(int i=0;i<col_n;i++)
						{
							grid[i][j].c=0;
						}
					}
					while(grid_Q.empty()==0)
					{
						grid_Q.pop();
					}



					//not found
					if(connected_net[i]->find==0)
					{

						/*for(int j=row_n-1;j>=0;j--)
						{
							for(int i=0;i<col_n;i++)
							{
								cout<<grid[i][j].b;
							}
							cout<<endl;
						}
						cout<<endl;
						cout<<connected_net[i]->net_num;
						for(int i=0;i<net_n;i++)
						{
							cout<<"net"<<connected_net[i]->net_num<<" ";
						}
						cout<<endl;*/
						for(int j=0;j<row_n;j++)
						{
							for(int i=0;i<col_n;i++)
							{
								grid[i][j].b=init_map[i][j];
							}
						}
						for(int i=0;i<net_n;i++)
						{
							net[i].dir=0;
							net[i].find=0;
							net[i].net_l=0;
							net[i].point_x.clear();
							net[i].point_y.clear();
							//net[i].usage=0;
						}
						int k= int(rand()) % i;
						Net* nf_net=connected_net[k];
						connected_net.erase(connected_net.begin()+k);
						connected_net.insert(connected_net.begin(),nf_net);
						nf_net=connected_net[i];
						connected_net.erase(connected_net.begin()+i);
						connected_net.insert(connected_net.begin(),nf_net);
						break;

					}

				}
			}
			for(int j=0;j<row_n;j++)
            {
                for(int i=0;i<col_n;i++)
                {
                    grid[i][j].b=init_map[i][j];
                }
            }
			find_all=0;
			for(int i=0;i<net_n;i++)
			{
			round_grid+=net[i].net_l-2;
			}

			if(first_time||round_grid<best_grid)
			{
				cout<<round_grid;
				best_grid=round_grid;
				for(int i=0;i<net_n;i++)
				{
					best_net[i]=net[i];
				}
			}
			first_time=0;
			round_grid=0;
			temp_net.clear();
			while(connected_net.size()>0)
			{
				int idx = rand() % connected_net.size();
				Net* t_net=connected_net[idx];
				temp_net.push_back(t_net);
				connected_net.erase(connected_net.begin()+idx);
			}
			for(int i=0;i<temp_net.size();i++)
			{
				connected_net.push_back(temp_net[i]);
			}
			for(int i=0;i<net_n;i++)
			{
			    net[i].dir=0;
                net[i].find=0;
                net[i].net_l=0;
                net[i].point_x.clear();
                net[i].point_y.clear();
			}



		}

        for(int i=0;i<net_n;i++)
        {
            ofs<<best_net[i].name<<" "<<best_net[i].net_l-2<<endl<<"begin"<<endl;
            ofs<<best_net[i].src_x<<" "<<best_net[i].src_y;
            int l=best_net[i].point_x.size();
            for(int j=l-1;j>=0;j--)
            {
                ofs<<" "<<best_net[i].point_x[j]<<" "<<best_net[i].point_y[j]<<endl;
                ofs<<best_net[i].point_x[j]<<" "<<best_net[i].point_y[j];
            }
            ofs<<" "<<best_net[i].tar_x<<" "<<best_net[i].tar_y<<endl<<"end"<<endl;

        }



	}
	return 0;
}


void Grid::Initial(int i,int j){
	x=i;
	y=j;
	b=0;
	c=0;
}

void Net::Initial(string s,int n,int sx,int sy,int tx,int ty){
    //usage=-1;
	name=s;
    net_num=n;
    src_x=sx;
    src_y=sy;
    tar_x=tx;
    tar_y=ty;
    in_Q=0;
    find=0;
    net_l=0;
    min_dist=abs(tar_x-src_x)+abs(tar_y-src_y);
}


