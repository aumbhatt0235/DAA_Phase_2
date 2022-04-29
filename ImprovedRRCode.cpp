#include<iostream>
using namespace std;


struct process
{
	int pno;
	int at;
	int bt;
	int wt;
	int tat;
	int ct;
	int rt;
};


//Global variables
int n;
struct process p[100];

void input_processes()
{
	cout<<"\nEnter the number of processes : ";
	cin >> n;
	
	for(int i=0;i<n;i++)
	{
		cout << "\nEnter process number:";
		cin >> p[i].pno;
		cout << "Arrival time: ";
		cin >> p[i].at;
		cout << "Burst time : ";
		cin >> p[i].bt;
		p[i].rt = p[i].bt;
		p[i].wt = 0;
		p[i].ct = p[i].at;
	}
}

void classical_rr(int timeQuantam)
{
	//Local variables
	int i,j,flag;
	struct process q[100],r[100];
	int rbeg = 0;
	int rend = 0;
	float l,h;
	l=h=0;
	int count = 0;
	int time = 0;
	int tq = timeQuantam;// 2;
	int time_exec ;
	float awt = 0,att = 0;
	//Copying to another array

	cout<<"\n Classical Round Robin Result: \n -------------------------------\n  Gantt chart:\n\t";

	for(i=0;i<n;i++)
	{
		q[i] = p[i];
	}
	
	flag = 1;
	while(count!=n)
	{		
			//Appends the processes to the ready queue-->In the beginning
			if(flag == 1)
			{
				for(i=0;i<n;i++)
				{
					if(q[i].at>=l && q[i].at<=h)
					{
						r[rend] = q[i];
						rend++;
					}
				}
				if(rend!=0)
				{
					flag --;
				}
				else
				{
					l=h;
					h++;
					continue;
				}
			}
		
		//Execute the process present at rbeg
		cout<<"("<<time<<")"<<"___"<<r[rbeg].pno<<"___";
		
		
		//Calculating parameters
		time_exec = (r[rbeg].rt>tq)?tq:r[rbeg].rt;
		r[rbeg].rt -=time_exec;
		r[rbeg].wt += time - r[rbeg].ct;
		time +=time_exec;
		r[rbeg].ct = time;	
		
		//Update l and h
		l = h+0.001;
		h = h + time_exec; 
		
		//Append again
		for(i=0;i<n;i++)
		{
			if(q[i].at>=l && q[i].at<=h)
			{
				r[rend] = q[i];
				rend++;
			}
		}
		
		if(r[rbeg].rt == 0)
		{
			count++;
			awt += r[rbeg].wt;
			r[rbeg].tat = r[rbeg].ct - r[rbeg].at;
			att += r[rbeg].tat;
			rbeg++;
			continue;
		}
		else
		{
			r[rend] = r[rbeg];
			rend++;
			rbeg++;
		}
	}
	cout << "("<<time<<")";
	
	cout << "\n\n  Average Waiting Time    : "<<(awt)/n;
	cout << "\n  Average Turnaround Time : "<<(att)/n<<endl<<endl;
}

void improved_rr()
{
	
	//Local variables
	int i,j,flag;
	struct process q[100],r[100];
	int rbeg = 0;
	int rend = 0;
	float l,h;
	l=h=0;
	int count = 0;
	int time = 0;
	int tq = 25;
	int time_exec ;
	float awt = 0,att = 0;
	
	//Additional IRR local variables
	struct process temp;//Used for swapping
	float median;//Used for storing median value 
	int mid;
	int oqt;
	int n_temp = 0;//Keeps track of number of distinct processes
	int exec_temp = 0;//Keeps track of number of processes executed

	cout<<"\n Improved Round Robin Result: \n -------------------------------\n  Gantt chart:\n\t";
	
	//Copying to another array
	for(i=0;i<n;i++)
	{
		q[i] = p[i];
	}
	
	flag = 1;
	while(count!=n)
	{		
			//Appends the processes to the ready queue-->In the beginning
			if(flag == 1)
			{
				for(i=0;i<n;i++)
				{
					if(q[i].at>=l && q[i].at<=h)
					{
						r[rend] = q[i];
						rend++;
						n_temp++;
					}
				}
				if(rend!=0)
				{
					flag --;
				}
				else
				{
					l=h;
					h++;
					continue;
				}
			}
		
		//Sort the processes in the ready queue based on bt
		for(i=rbeg;i<rend;i++)
		{
			for(j=rbeg;j<rend-1;j++)
			{
				if(r[j].bt>r[j+1].bt)
				{
					temp = r[j];
					r[j] = r[j+1];
					r[j+1] = temp;
				}
			}
		}
			
		if(exec_temp == 0)
		{
				//Find the median
				if((rend-rbeg)%2 == 0)
				{
					mid = (rend+rbeg)/2;
					median = 0.5*(r[mid].bt+r[mid-1].bt);
				}
				else
				{
				mid = (rend+rbeg)/2;
				median = r[mid].bt;
				}
			
				//Find the optimal time quantum		
				tq = (median + r[rend -1].bt)/2;	
		}	
		
		//Execute the process present at rbeg
		cout<<"("<<time<<")"<<"___"<<r[rbeg].pno<<"___";
		exec_temp = (exec_temp+1)%(n_temp);
		
		//cout<<"Executing : "<<r[rbeg].pno<<" From  : "<<time<<" to :";
		
		//Calculating parameters
		time_exec = (r[rbeg].rt>tq)?tq:r[rbeg].rt;
		r[rbeg].rt -=time_exec;
		r[rbeg].wt += time - r[rbeg].ct;
		time +=time_exec;
		r[rbeg].ct = time;	

		//Update l and h
		l = h+0.001;
		h = h + time_exec; 
		
		//Append again
		for(i=0;i<n;i++)
		{
			if(q[i].at>=l && q[i].at<=h)
			{
				r[rend] = q[i];
				rend++;
			}
		}
		
		if(r[rbeg].rt == 0)
		{
			count++;
			awt += r[rbeg].wt;
			r[rbeg].tat = r[rbeg].ct - r[rbeg].at;
			att += r[rbeg].tat;
			rbeg++;
			continue;
		}
		else
		{
			r[rend] = r[rbeg];
			rend++;
			rbeg++;
		}
	}
	cout<<"("<<time<<")";
	
	cout << "\n\n  Average Waiting Time    : "<<(awt)/n;
	cout << "\n  Average Turnaround Time : "<<(att)/n<<endl<<endl;

}
int main()
{
	int originalTimeQuantam = 0;

	// Input data for all processes.
	input_processes();

	cout << "\nEnter the Original Time Quantam (For classic RR scheduling) : ";
	cin >> originalTimeQuantam;
	
	//Classical Round Robin Scheduling
	classical_rr(originalTimeQuantam);
	
	//Improved Round Robin Scheduling
	improved_rr();


	return 0;
}

