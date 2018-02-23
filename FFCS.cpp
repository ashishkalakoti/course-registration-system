#include<iostream>
#include<fstream>
#include<conio.h>
#include<stdlib.h>
using namespace std;

struct Student //details of student in Student structure
{
    string name,registration_No,registered_Courses[10],waitlist_Courses[10];
    int total_credits,registered,waitlist;
    Student()//default constructor
    {

        total_credits=registered=waitlist=0;
        for(int i=0;i<10;i++)
            registered_Courses[i]=waitlist_Courses[i]="0";
    }
    Student *next;
}*Student_head=NULL;

struct Waitlist//Waitlist of every course code (generally acts as queue)
{
    string registration_No;
    Waitlist *next;
}*Waitlist_head=NULL;

struct Course//Details of course in Course structure
{
    string course_Code,course_Name,faculty_Name,Slot,register_registration_no[100];
    int credit,seats_Available,waiting,counter;
    Waitlist *waitlist;
    void course_Get()//get details of course and write them to "courses.txt"
    {
        cout<<"Course code:";
        cin>>course_Code;
        cout<<"\nCourse name:";
        cin>>course_Name;
        cout<<"\nFaculty name:";
        cin>>faculty_Name;
        cout<<"\nSlot:";
        cin>>Slot;
        cout<<"\nCredit:";
        cin>>credit;
        cout<<"\nSeats available:";
        cin>>seats_Available;
        counter=0;
        waiting=0;
        fstream file("courses.txt",ios::out|ios::app);
        file<<course_Code<<" "<<course_Name<<" "<<faculty_Name<<" "<<Slot<<" "<<credit<<" "<<seats_Available<<" ";
        file.close();
    }
    void course_View()//View course details
    {
        cout<<"Course code: "<<course_Code<<"\n"<<"Course name: "<<course_Name<<"\n";
        cout<<"Faculty name: "<<faculty_Name<<"\n"<<"Slot: "<<Slot<<"\n"<<"Credits: "<<credit<<"\n";
        cout<<"No. of seats: "<<seats_Available<<"\n"<<"Waiting list no.: "<<waiting<<"\n";
    }
    void Register(string reg_no)//Register a course as Student
    {
        int check=0;
        Student *student=new Student;
        if(Student_head==NULL)
        {
            cout<<"NOT FOUND\n";
        }
        else
            for(student=Student_head;student!=NULL;student=student->next)
                if(student->registration_No==reg_no)
                    break;
            if(student==NULL)
            {
                cout<<"NOT FOUND\n";
            }
            for(int i=0;i<student->registered;i++)
                if(course_Code==student->registered_Courses[i])
                    check=1;
            if(check==0)
                for(int i=0;i<student->waitlist;i++)
                    if(student->waitlist_Courses[i]==course_Code)
                        check=1;
            if(check==1)
                cout<<"You have already registered this course\n";
            if(student!=NULL&&check==0)
            {
                if(counter<seats_Available)
                {
                    student->registered_Courses[student->registered++]=course_Code;
                    student->total_credits+=credit;
                }
                else
                    student->waitlist_Courses[student->waitlist++]=course_Code;
            }
        if(counter<seats_Available&&check==0)//Seats are available
        {
            register_registration_no[counter]=reg_no;
            counter++;
            cout<<"You have successfully registered for the course "<<course_Code<<endl;
        }
        else if(counter>=seats_Available&&check==0) //Registering in the waitlist
        {
            Waitlist *temp=new Waitlist;
            waiting++;
            cout<<"YOU ARE REGISTERING IN THE WAITLIST\n";
            temp->registration_No=reg_no;
            temp->next=NULL;
            if(Waitlist_head==NULL)
            {
                Waitlist_head=temp;
                waitlist=temp;
            }
            else
            {
                Waitlist *temp1=new Waitlist;
                for(temp1=Waitlist_head;temp1->next!=NULL;temp1=temp1->next)
                {}
                temp1->next=temp;
            }
            temp->next=NULL;
            cout<<"\nYou are at waitlist no. "<<waiting<<endl;
        }
    }
    void drop(string reg_no)//drop a course as student
    {
        int check=0,i;
        for(i=0;i<counter;i++)
            if(register_registration_no[i]==reg_no)
                break;
        if(i!=counter)//registration no. found in Registered list
        {
            check=1;
            for(;i<counter-1;i++)
                register_registration_no[i]=register_registration_no[i+1];
            if(Waitlist_head!=NULL)
            {
                register_registration_no[i]=Waitlist_head->registration_No;
                cout<<register_registration_no[i]<<" has been registered to "<<course_Code<<endl;
                Waitlist_head=Waitlist_head->next;
                waiting--;
            }
            else
                counter--;
        }
        else//registration no. not found in Registered list
        {
            if(Waitlist_head==NULL)
                check=0;
            else
            {
                Waitlist *waitlist=new Waitlist;
                for(waitlist=Waitlist_head;waitlist!=NULL;waitlist=waitlist->next)
                {
                    if(waitlist->registration_No==reg_no)
                    {
                        check=2;
                        break;
                    }
                }
                if(check==2)
                {
                    waiting--;
                    Waitlist *temp=new Waitlist;
                    if(Waitlist_head==waitlist)
                        Waitlist_head=waitlist->next;
                    else
                    {
                        for(temp=Waitlist_head;temp->next!=waitlist;temp=temp->next){}
                        temp->next=waitlist->next;
                        temp=waitlist;
                    }
                    delete(temp);
                }
            }
            if(check==0)
                cout<<"You have not registered in this course\n";
        }
        if(check==1)//registration no found in registered list
        {
            Student *student=new Student;
            for(student=Student_head;student!=NULL;student=student->next)
            {
                if(student->registration_No==reg_no)
                    break;
            }
            for(i=0;i<student->registered;i++)
            {
                if(student->registered_Courses[i]==course_Code)
                    break;
            }
            student->registered_Courses[i]="0";
            student->total_credits-=credit;
        }
        else if(check==2)//registration no found in waitlist
        {
            Student *student=new Student;
            for(student=Student_head;student!=NULL;student=student->next)
            {
                if(student->registration_No==reg_no)
                    break;
            }
            for(i=0;i<student->waitlist;i++)
            {
                if(student->waitlist_Courses[i]==course_Code)
                    break;
            }
            student->waitlist_Courses[i]="0";
        }
        if(check==1||check==2)
            cout<<"\nYou have successfully dropped the course "<<course_Code<<endl;
    }
    void view_registered()//view list of registered students of any course.
    {
        if(counter==0)
        {
            cout<<"No registered students\n";
            return;
        }
        for(int i=0;i<counter;i++)
            cout<<register_registration_no[i]<<endl;
    }
    void view_waitlist()//view list of waitlisted students of any course
    {
        if(counter<seats_Available)
            cout<<"EMPTY WAITLIST\n";
        else
        {
            Waitlist *temp=new Waitlist;
            for(temp=Waitlist_head;temp!=NULL;temp=temp->next)
                cout<<temp->registration_No<<endl;
        }
    }
    Course *next;
}*Course_head=NULL;

void add_Course(Course *course)//Add a new course in "courses.dat"
{
    course->course_Get();
    if(Course_head==NULL)
        Course_head=course;
    else
    {
        Course *temp=new Course;
        for(temp=Course_head;temp->next!=NULL;temp=temp->next)
        {}
        temp->next=course;
    }
    course->next=NULL;
}
void drop_Course(Course *course,string reg_no)//drop a course as Student
{
    course->drop(reg_no);
}
void student_Details(string reg_no)//view details of student
{
    Student *student=new Student;
    if(Student_head==NULL)
        cout<<"No students added\n";
    else
    {
        for(student=Student_head;student;student=student->next)
        {
            if(student->registration_No==reg_no)
                break;
        }
        if(student==NULL)
            cout<<"Registration no. not found\n";
        else
        {
            int i;
            cout<<"Details of "<<reg_no<<":\n";
            cout<<"Name: "<<student->name;
            cout<<"\nTotal credits: "<<student->total_credits;
            if(student->registered>0)
            {
                cout<<"\nRegistered courses are:\n";
                for(i=0;i<student->registered;i++)
                    if(student->registered_Courses[i]!="0")
                        cout<<student->registered_Courses[i]<<endl;
            }
            else
            {
                cout<<"\nNo registered courses\n";
            }
            if(student->waitlist>0)
            {
                cout<<"\nWaitlisted courses are:\n";
                for(i=0;i<student->waitlist;i++)
                    if(student->waitlist_Courses[i]!="0")
                        cout<<student->waitlist_Courses[i]<<endl;
            }
            else
            {
                cout<<"\nNo waitlist courses\n";
            }
        }
    }
}

int main()
{
    int choice,pos;
    string password,registration_no;
    char ch;
    fstream file("courses.txt",ios::in);//read "courses.txt" for details of courses
    if(file.is_open())
    {
        while(!file.eof())
        {
            Course *course=new Course;
            file>>course->course_Code>>course->course_Name>>course->faculty_Name>>course->Slot>>course->credit>>course->seats_Available;
            if(course->course_Code=="0")
                break;
            if(Course_head==NULL)
                Course_head=course;
            else
            {
                Course *temp=new Course;
                for(temp=Course_head;temp->next;temp=temp->next){}
                temp->next=course;
            }
            course->next=NULL;
        }
    }
    l:do
    {
    system("cls");
    cout<<"1. ADMIN LOGIN 2. STUDENT LOGIN 3 . EXIT\n";
    cin>>choice;
    if(choice==1)//ADMIN LOGIN
    {
        password="\0";
        cout<<"Enter password:";
        const char ENTER=13;
        while((ch=_getch())!=ENTER)
        {
            password+=ch;
            cout<<'*';
        }
        if (password=="FFCS")//pre-defined password for admin
            cout<<"\nACCESS GRANTED\n";
        else
        {
            cout<<"\nAUTHENTICATION FAILURE\n";
            getch();
            goto l;
        }
    }
    else if(choice==2)//STUDENT LOGIN
    {
        string reg_no,pwd;
        cout<<"Registration no:";
        cin>>registration_no;
        cout<<"\nEnter password:";
        const char ENTER=13;
        password="\0";
        while((ch=_getch())!=ENTER)
        {
            password+=ch;
            cout<<'*';
        }
        fstream file("student.txt",ios::in);
        if(!file.is_open())
        {
            cout<<"\nAUTHENTICATION FAILURE\n";
            getch();
            goto l;
        }
        while(!file.eof())
        {
            file>>reg_no>>pwd;
            if(reg_no==registration_no&&pwd==password)
            {
                cout<<"\nACCESS GRANTED\n";
                break;
            }
        }
        if(file.eof())
        {
            cout<<"\nAUTHENTICATION FAILURE\n";
            getch();
            goto l;
        }
        file.close();
    }
    else if(choice==3)//EXIT
        return 0;
    if(choice==1)
    {
    do//functions of ADMIN
    {
        cout<<"\n\n1. Add Course\n2. View all course details\n3. View registered list\n4. View waitlist\n5. Add a student\n6. Search student's details\n7. Format data\n0. LOGOUT\n";
        cin>>choice;
        if(choice==1)//Add course
        {
            Course *course = new Course;
            add_Course(course);
        }
        else if(choice==2)//View course details
        {
            Course *temp=new Course;
            if(Course_head)
            {
                string course_Code;
                cout<<"Course code:\n";
                for(temp=Course_head;temp!=NULL;temp=temp->next)
                    cout<<temp->course_Code<<endl;
                cout<<"Enter course code for full details:";
                cin>>course_Code;
                cout<<"\n";
                for(temp=Course_head;temp->course_Code!=course_Code;temp=temp->next){}
                temp->course_View();
            }
            else
                cout<<"COURSE LIST EMPTY\n";
        }
        else if(choice==3)//View registered list of any course
        {
            string course_Code;
            Course *temp=new Course;
            cout<<"Enter course code:";
            cin>>course_Code;
            for(temp=Course_head;temp!=NULL;temp=temp->next)
                if(course_Code==temp->course_Code)
                     break;
            if(temp!=NULL)
                temp->view_registered();
            else
                cout<<course_Code<<" not found\n";
        }
        else if(choice==4)//view waitlist of any course
        {
            string course_Code;
            Course *temp=new Course;
            cout<<"Enter course code:";
            cin>>course_Code;
            for(temp=Course_head;temp!=NULL;temp=temp->next)
                if(course_Code==temp->course_Code)
                    break;
            if(temp!=NULL)
                temp->view_waitlist();
            else
                cout<<course_Code<<" not found\n";
        }
        else if(choice==5)//Add a student to "student.txt"
        {
            password="\0";
            fstream file("student.txt",ios::out|ios::app);
            cout<<"Registration no:";
            cin>>registration_no;
            cout<<"\nSet new password:";
            const char ENTER=13;
            while((ch=_getch())!=ENTER)
            {
                password+=ch;
                cout<<'*';
            }
            cout<<"\n";
            file<<registration_no<<" "<<password<<" ";
            Student *student=new Student;
            cout<<"\nEnter name:";
            cin>>student->name;
            student->registration_No=registration_no;
            if(Student_head==NULL)
                Student_head=student;
            else
            {
                Student *temp=new Student;
                for(temp=Student_head;temp->next;temp=temp->next){}
                temp->next=student;
            }
            student->next=NULL;
        }
        else if(choice==6)//View details of a student
        {
            cout<<"Enter registration no:";
            cin>>registration_no;
            student_Details(registration_no);
        }
        else if(choice==7)//Delete files
        {
            remove("courses.txt");
            remove("student.txt");
            cout<<"DATA HAS BEEN FORMATTED\n";
        }
    }
    while(choice!=0);
    }
    else if(choice==2)//STUDNET LOGIN
    do
    {
        cout<<"\n\n1. Register course\n2. Drop course\n3. View details\n0. LOGOUT\n";
        cin>>choice;
        if(choice==1)//Register course
        {
            string course_Code;
            Course *temp=new Course;
            cout<<"Enter course code:";
            cin>>course_Code;
            for(temp=Course_head;temp!=NULL;temp=temp->next)
                  if(course_Code==temp->course_Code)
                       break;
            if(temp!=NULL)
                  temp->Register(registration_no);
            else
                  cout<<course_Code<<" not found\n";
        }
        else if(choice==2)//Drop course
        {
            string course_Code;
            Course *temp=new Course;
            cout<<"Enter course code:";
            cin>>course_Code;
            for(temp=Course_head;temp!=NULL;temp=temp->next)
                  if(course_Code==temp->course_Code)
                       break;
            if(temp==NULL)
                cout<<course_Code<<" not found\n";
            else
                drop_Course(temp,registration_no);
        }
        else if(choice==3)//View details of course
        {
            student_Details(registration_no);
        }
    }while(choice!=0);
    }while(choice==0);
}
