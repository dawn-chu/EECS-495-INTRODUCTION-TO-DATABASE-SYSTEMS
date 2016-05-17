//
//  main.cpp
//  project3_db
//
//  Created by weihanchu on 11/9/15.
//  Copyright © 2015 weihanchu. All rights reserved.
//



#include <mysql.h>
#include <iostream>
#include <string>
#include <ctime>


void login();
void Transcript(std::string);
void Details(std::string);
void personaldetails(std::string);
void enroll(std::string);
void enroll_right2015(std::string,std::string);
void enroll_right2016(std::string,std::string);
void withdraw(std::string);
void withdraw_right(std::string,std::string,int,std::string);

using namespace std;

MYSQL *conn;

string int2str(int n)
{
    char t[256];
    string s;
    sprintf (t,"%d",n);
    s=t;
    return s;
}

string student_semester_decide(int x)
{
    if(x >= 9 && x <= 12)
        return "Q1";
    else if (x >= 1 && x <= 3)
        return "Q2";
    else if (x >= 4 && x <= 6)
        return "Q3";
    else if (x >= 7 && x <= 8)
        return "Q4";
    else if (x > 12)
        return student_semester_decide(x - 12);
    else
        return "0";
}

void student_menu(string student_id)
{   string option;
    string logout = "logout";
    string student_semester;
    int year;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    year = now ->tm_year + 1900;
    student_semester = student_semester_decide( now -> tm_mon );
    
    mysql_close(conn);
    conn = mysql_init ( NULL );
    mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
    
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    string course = "SELECT * FROM transcript where StudId = " + student_id + " and year = '"+ int2str(year) + "' and semester= '"+ student_semester + "' and grade is null;";
    mysql_query(conn, course.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    
    cout << "---------------------------------" << endl;
    cout << "|        CURRENT COURSES :      |" << endl;
    cout << "---------------------------------" << endl;
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if(row != NULL)
        {
            cout << "| Course : " << row[1] << endl;
            cout << "| Year   : " << row[3] << endl;
        }
    }
    cout << "---------------------------------"<<endl;
    
    cout << "Please select one of the following option :" << endl;
    cout << "Enter 1 for Transcript" << endl;
    cout << "Enter 2 for Enroll" << endl;
    cout << "Enter 3 for Withdraw" << endl;
    cout << "Enter 4 for Personal Details" << endl;
    cout << "Enter 5 for Logout" << endl;
    cout << "Your option is :" ;
    cin >> option;
    if ( option == "1" )
    {
        Transcript(student_id);
    }
    else if(option=="2")
    {
        enroll(student_id);
    }
    else if ( option == "3")
    {
        withdraw(student_id);
    }
    else if ( option == "4")
    {
        personaldetails(student_id);
    }
    else if ( option == "5")
    {
        login();
    }
    else
    {
        cout << "*** You need to enter a valid option! ***" << endl << endl;
        student_menu(student_id);
    }
    
    
    
    
    
    
}



void Transcript(string student_id)
{   string Transcript_option;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    string transcripts = "SELECT * FROM transcript where StudId = " + student_id+";";
    mysql_query(conn, transcripts.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    cout << "---------------------------------"<< endl;
    cout << "|          TRANSCRIPT:          |"<< endl;
    cout << "---------------------------------"<< endl;
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if(row[4]!= NULL)
        {
            cout << "| Course  : " << row[1] << endl;
            cout << "| Grade   : " << row[4] << endl;
        }
        else
        {
            cout << "| Course  : " << row[1] << endl;
            cout << "| Grade   : null" << endl;
        }
    }
    cout << "---------------------------------"<<endl;
    mysql_free_result( res_set );
    
    cout << "Please select one of the following option:" << endl << "Enter 1 for Details" << endl << "Enter 2 for Menu"<<endl;
    cout << "Your option is : " ;
    cin>>Transcript_option;
    
    if ( Transcript_option == "1" )
    {
        Details(student_id);
    }
    else if ( Transcript_option == "2")
    {
        student_menu(student_id);
    }
    else
    {
        cout << "*** You need to enter a valid option! ***" << endl << endl;
        Transcript(student_id);
    }
}


void Details(string student_id)
{   string course_number;
    cout << "Please enter a course number :" << endl;
    cin >> course_number;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string courseinformation="SELECT t.UoSCode,UoSName,t.Year,t.Semester,Enrollment,MaxEnrollment,Name,Grade,StudId FROM faculty f, transcript t, unitofstudy un,uosoffering uo where StudId = " + student_id+" and t.UoSCode= '"+course_number+"'and t.UoSCode=un.UoSCode and t.UoSCode=uo.UoSCode and uo.InstructorId=f.Id and t.Year=uo.Year and t.Semester=uo.Semester ;";
    mysql_query(conn, courseinformation.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    
    if(numrows==1)
    {
        row = mysql_fetch_row(res_set);
        if(row[7]!= NULL)
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "|               COURSE DETAILS :                    |" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << "| Coursenumber:" << row[0] << endl;
            cout << "| Title       :" << row[1] << endl;
            cout << "| Year        :" << row[2] << endl;
            cout << "| Quarter     :" << row[3] << endl;
            cout << "| Emrollnumber:" << row[4] << endl;
            cout << "| MaxEmroll   :" << row[5] << endl;
            cout << "| Lecturer    :" << row[6] << endl;
            cout << "| Grade       :" << row[7] << endl;
            cout << "----------------------------------------------------" << endl;
            cout << "Enter 1 for another course detail" << endl << "Enter 2 to Transcript" << endl;
            cout << "Your option is : ";
            string see;
            cin >> see;
            if( see == "1" )
            {
                Details(student_id);
            }
            else if ( see == "2" )
            {
                Transcript(student_id);
            }
            else
            {
                cout << "*** Invalid option! ***" << endl;
                Details(student_id);
            }
        }
        else
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "|               COURSE DETAILS :                    |" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << "| Coursenumber:" << row[0] << endl;
            cout << "| Title       :" << row[1] << endl;
            cout << "| Year        :" << row[2] << endl;
            cout << "| Quarter     :" << row[3] << endl;
            cout << "| Emrollnumber:" << row[4] << endl;
            cout << "| MaxEmroll   :" << row[5] << endl;
            cout << "| Lecturer    :" << row[6] << endl;
            cout << "| Grade       : null" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << " Enter 1 for another course detail" << endl << " Enter 2 to Transcript" << endl;
            cout << " Your option is : ";
            string see;
            cin >> see;
            if ( see == "1" )
            {
                Details(student_id);
            }
            else if ( see == "2" )
            {
                Transcript(student_id);
            }
            else
            {
                cout << "*** Invalid option! ***" << endl;
                Details(student_id);
            }
        }
    }
    else {
        cout << "*** Please enter a valid course number! ***" << endl;
        Details(student_id);
    }
    mysql_free_result( res_set );
}



void personaldetails(string student_id)
{
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string pd_id="SELECT * FROM student where Id="+student_id+";";
    mysql_query(conn,pd_id.c_str());
    res_set = mysql_store_result(conn);
    row = mysql_fetch_row( res_set );
    cout << "-----------------------------------" << endl;
    cout << "|        PERSONAL DETAILS :       |" << endl;
    cout << "-----------------------------------" << endl;
    cout << "| Id      :"<<row[0]<<endl;
    cout << "| Name    :"<<row[1]<<endl;
    cout << "| Password:"<<row[2]<<endl;
    cout << "| Address :"<<row[3]<<endl;
    cout << "-----------------------------------"<<endl;
    cout << "Enter 1 to change the password"<<endl<<"Enter 2 to change the address"<<endl<<"Enter 3 to menu"<<endl;
    cout << "Your option is : ";
    string change;
    cin >> change;
    if(change == "1")
    {
        cout << "Enter a new password : "<<endl;
        string newpassword;
        cin >> newpassword;
        string revise1 = "UPDATE student SET student.Password = '"+newpassword+"' WHERE student.Id = " + student_id+";";
        mysql_query(conn,"START TRANSACTION;");
        mysql_query(conn, revise1.c_str());
        mysql_query(conn,"COMMIT;");
        res_set = mysql_store_result(conn);
        cout << "*** The password is changed successfully! ***" << endl;
        personaldetails( student_id );
    }
    else if( change == "2")
    {
        cout << "Enter a new address" << endl;
        string newaddress;
        cin >> newaddress;
        string revise2 = "UPDATE student SET student.Address = '"+newaddress+"' WHERE student.Id = " + student_id+";";
        mysql_query(conn,"START TRANSACTION;");
        mysql_query(conn, revise2.c_str());
        mysql_query(conn,"COMMIT;");
        res_set = mysql_store_result(conn);
        cout << "*** The address is changed successfully! ***" << endl;
        personaldetails(student_id);
    }
    else if (change == "3")
    {
        student_menu(student_id);
    }
    else
    {
        cout << "*** You need to enter a valid option! ***" << endl;
        personaldetails(student_id);
    }
}






void enroll(string student_id)
{
    string student_semester,student_semester1;
    int year,year1;
    time_t t = time(0);
    struct tm * now = localtime( &t );
    year = now -> tm_year + 1900;
    student_semester = student_semester_decide( now -> tm_mon);
    year1 = now -> tm_year + 1900 + 1;
    student_semester1 = student_semester_decide( now -> tm_mon + 3);
    
    
    mysql_close(conn);
    conn = mysql_init ( NULL );
    mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
    
    
    cout << "You can only enroll the class of the current year and quarter or the following quarter." << endl;
    cout << "Please enter the year and quarter you want to enroll class" << endl;
    cout << "Year : " ;
    string whichyear;
    cin >> whichyear;
    cout << "Quarter :" ;
    string whichquarter;
    cin >> whichquarter;
    
    if ( (whichyear == int2str(year)) && (whichquarter == student_semester) )
    {
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        
        string enroll1="CALL enrollclass ("+student_id+","+ int2str(year) +",'"+student_semester+"');" ;
        mysql_query(conn, enroll1.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        
        cout << "-----------------------------------" << endl;
        cout << "|             ENROLL:             |" << endl;
        cout << "-----------------------------------" << endl;
        cout << " Course can enroll : " << endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row(res_set);
            if(row[0]!= NULL)
            {
                cout << "|" << row[0] << "|" << endl;
                
            }
        }
        cout << "-----------------------------------" << endl;
        mysql_free_result( res_set );
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        MYSQL_RES *res_set4;
        MYSQL_ROW row2;
        
        string cannotenroll="CALL cannotenroll ("+student_id+","+ int2str(year) +",'"+student_semester+"');" ;
        mysql_query(conn, cannotenroll.c_str());
        res_set4 = mysql_store_result(conn);
        int numrows4 = (int)mysql_num_rows(res_set4);
        cout << " Course cannot enroll : " << endl;
        for (int i = 0; i < numrows4; i++)
        {
            row2 = mysql_fetch_row(res_set4);
            if(row2[0] != NULL)
            {
                cout << "|" << row2[0] << "|" << endl;
            }
        }
        cout << "-----------------------------------" << endl;
        mysql_free_result( res_set4 );
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        
        cout << "Enter 1 to enroll a class" << endl;
        cout << "Enter 2 to see the preclass of the course you cannot enroll" << endl;
        cout << "Your option is : ";
        
        string enroll_or_not;
        cin >> enroll_or_not;
        if ( enroll_or_not == "1" )
        {
            cout << "Please enter the course you want to enroll" << endl;
            string chosecourse1;
            cin >> chosecourse1;
            
            MYSQL_RES *res_set1;
            string enrollchoice = "CALL whetherrightclass('"+chosecourse1+"',"+student_id+","+ int2str(year) +",'"+student_semester+"');" ;
            mysql_query(conn, enrollchoice.c_str());
            res_set1 = mysql_store_result(conn);
            int numrows1 = (int)mysql_num_rows(res_set1);
            
            if ( numrows1 == 1)
            {
                mysql_free_result( res_set1 );
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
                
                enroll_right2015(chosecourse1,student_id);
            }
            
            else {
                cout << "***       Invalid command!      ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                enroll( student_id);
            }
        }
        else if ( enroll_or_not == "2" )
        {
            cout << "Please enter the course you cannot enroll to see the pre-class" << endl;
            string chosecourse2;
            cin >> chosecourse2;
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
            
            
            MYSQL_RES *res_set2;
            string needpreclass = "CALL needpreclass('"+chosecourse2+"',"+student_id+","+ int2str(year) +",'"+student_semester+"');" ;
            mysql_query(conn, needpreclass.c_str());
            res_set2 = mysql_store_result(conn);
            int numrows2 = (int)mysql_num_rows(res_set2);
            
            
            if ( numrows2 == 1)
            {
                cout << "Pre-class of this course" << endl;
                
                mysql_free_result( res_set2 );
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
                
                
                
                MYSQL_RES *res_set3;
                string preclass="CALL preclass('"+chosecourse2+"');" ;
                mysql_query(conn, preclass.c_str());
                res_set3 = mysql_store_result(conn);
                int numrows3 = (int)mysql_num_rows(res_set3);
                
                for (int i = 0; i < numrows3; i++)
                {
                    row = mysql_fetch_row(res_set3);
                    if(row[0] != NULL)
                    {
                        cout << row[0] << endl;
                    }
                }
                mysql_free_result( res_set3 );
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
                
                cout << "-----------------------------------" << endl;
                cout << "Enter 1 to go back to enroll menu" << endl;
                cout << "Enter 2 to go back to student menu" << endl;
                cout << "Your option is : ";
                
                string enroll_option;
                cin >> enroll_option;
                if ( enroll_option == "1" )
                {
                    enroll(student_id);
                }
                else if ( enroll_option == "2" )
                {
                    student_menu(student_id);
                }
                else {
                    cout << "***     Invalid command!     ***" << endl;
                    cout << "*** Go back to student menu! ***" << endl << endl;
                    student_menu(student_id);
                    
                }
                
                
            }
            else {
                cout << "***       Invalid course number!      ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                enroll( student_id);
            }
        }
        else
        {
            cout << "***       Invalid command!      ***" << endl;
            cout << "*** Go back to the enroll menu! ***" << endl << endl;
            enroll( student_id);
            
        }
    }
    else if ( (whichyear == int2str(year1)) && (whichquarter == student_semester1) )
    {
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        
        string enroll1="CALL enrollclass ("+student_id+","+ int2str(year1) +",'"+student_semester1+"');" ;
        mysql_query(conn, enroll1.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        
        cout << "-----------------------------------" << endl;
        cout << "|             ENROLL:             |" << endl;
        cout << "-----------------------------------" << endl;
        cout << " Course can enroll : " << endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row(res_set);
            if(row[0]!= NULL)
            {
                cout << "|" << row[0] << "|" << endl;
                
            }
        }
        cout << "-----------------------------------" << endl;
        mysql_free_result( res_set );
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        MYSQL_RES *res_set4;
        MYSQL_ROW row2;
        
        string cannotenroll="CALL cannotenroll ("+student_id+","+ int2str(year1) +",'"+student_semester1+"');" ;
        mysql_query(conn, cannotenroll.c_str());
        res_set4 = mysql_store_result(conn);
        int numrows4 = (int)mysql_num_rows(res_set4);
        cout << " Course cannot enroll : " << endl;
        for (int i = 0; i < numrows4; i++)
        {
            row2 = mysql_fetch_row(res_set4);
            if(row2[0] != NULL)
            {
                cout << "|" << row2[0] << "|" << endl;
            }
        }
        cout << "-----------------------------------" << endl;
        mysql_free_result( res_set4 );
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        
        cout << "Enter 1 to enroll a class" << endl;
        cout << "Enter 2 to see the preclass of the course you cannot enroll" << endl;
        cout << "Your option is : ";
        
        string enroll_or_not;
        cin >> enroll_or_not;
        if ( enroll_or_not == "1" )
        {
            cout << "Please enter the course you want to enroll" << endl;
            string chosecourse1;
            cin >> chosecourse1;
            
            MYSQL_RES *res_set1;
            string enrollchoice = "CALL whetherrightclass('"+chosecourse1+"',"+student_id+","+ int2str(year1) +",'"+student_semester1+"');" ;
            mysql_query(conn, enrollchoice.c_str());
            res_set1 = mysql_store_result(conn);
            int numrows1 = (int)mysql_num_rows(res_set1);
            
            if ( numrows1 == 1)
            {
                mysql_free_result( res_set1 );
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
                
                enroll_right2016(chosecourse1,student_id);
            }
            else
            {
                cout << "***       Invalid command!      ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                enroll( student_id);
            }
        }
        else if ( enroll_or_not == "2" )
        {
            cout << "Please enter the course you cannot enroll to see the pre-class" << endl;
            string chosecourse2;
            cin >> chosecourse2;
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
            
            
            MYSQL_RES *res_set2;
            string needpreclass = "CALL needpreclass('"+chosecourse2+"',"+student_id+","+ int2str(year1) +",'"+student_semester1+"');" ;
            mysql_query(conn, needpreclass.c_str());
            res_set2 = mysql_store_result(conn);
            int numrows2 = (int)mysql_num_rows(res_set2);
            
            
            if ( numrows2 == 1)
            {
                cout << "Pre-class of this course" << endl;
                
                mysql_free_result( res_set2 );
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
                
                
                
                MYSQL_RES *res_set3;
                string preclass="CALL preclass('"+chosecourse2+"');" ;
                mysql_query(conn, preclass.c_str());
                res_set3 = mysql_store_result(conn);
                int numrows3 = (int)mysql_num_rows(res_set3);
                
                for (int i = 0; i < numrows3; i++)
                {
                    row = mysql_fetch_row(res_set3);
                    if(row[0] != NULL)
                    {
                        cout << row[0] << endl;
                    }
                }
                mysql_free_result( res_set3 );
                
                cout << "-----------------------------------" << endl;
                cout << "Enter 1 to go back to enroll menu" << endl;
                cout << "Enter 2 to go back to student menu" << endl;
                cout << "Your option is : ";
                
                string enroll_option;
                cin >> enroll_option;
                if ( enroll_option == "1" )
                {
                    enroll(student_id);
                }
                else if ( enroll_option == "2" )
                {
                    student_menu(student_id);
                }
                else {
                    cout << "***     Invalid command!     ***" << endl;
                    cout << "*** Go back to student menu! ***" << endl << endl;
                    student_menu(student_id);
                    
                }
                
            }
            else
            {
                cout << "***    Invalid course number!   ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                enroll( student_id);
                
            }
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
            
            cout << "-----------------------------------" << endl;
            cout << "Enter 1 to go back to enroll menu" << endl;
            cout << "Enter 2 to go back to student menu" << endl;
            cout << "Your option is : ";
            
            string enroll_option;
            cin >> enroll_option;
            if ( enroll_option == "1" )
            {
                enroll(student_id);
            }
            else if ( enroll_option == "2" )
            {
                student_menu(student_id);
            }
            else {
                cout << "***     Invalid command!     ***" << endl;
                cout << "*** Go back to student menu! ***" << endl << endl;
                student_menu(student_id);
            }
            
        }
        else {
            cout << "***      Invalid command!       ***" << endl;
            cout << "*** Go back to the enroll menu! ***" << endl << endl;
            enroll( student_id);
        }
    }
    else
    {
        cout << "*** Please enter valid year and quarter! ***" <<endl << endl;
        enroll(student_id);
    }
}


void enroll_right2015 ( string chosecourse ,string student_id)
{
    string student_semester;
    int year;
    time_t t = time(0);
    struct tm * now = localtime ( &t );
    year = now -> tm_year + 1900 ;
    student_semester = student_semester_decide ( now -> tm_mon );
    
    
    
    MYSQL_RES *res_set;
    
    string enroll2 = "CALL addclass ("+student_id+",'"+chosecourse+"',"+ int2str(year) +",'"+student_semester+"');";
    mysql_query(conn, enroll2.c_str());
    res_set = mysql_store_result(conn);
    mysql_free_result( res_set );
    
    cout << "***  Successfully enrolled!  ***" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Enter 1 to go back to enroll menu" << endl;
    cout << "Enter 2 to go back to student menu" << endl;
    cout << "Your option is : ";
    
    string enroll_option;
    cin >> enroll_option;
    if ( enroll_option == "1" )
    {
        enroll(student_id);
    }
    else if ( enroll_option == "2" )
    {
        student_menu(student_id);
    }
    else {
        cout << "***     Invalid command!     ***" << endl;
        cout << "*** Go back to student menu! ***" << endl << endl;
        student_menu(student_id);
        
    }
}

void enroll_right2016 ( string chosecourse ,string student_id)
{
    string student_semester;
    int year;
    time_t t = time(0);
    struct tm * now = localtime ( &t );
    year = now -> tm_year + 1900 + 1;
    student_semester = student_semester_decide ( now -> tm_mon + 3);
    
    
    
    MYSQL_RES *res_set;
    
    string enroll2 = "CALL addclass ("+student_id+",'"+chosecourse+"',"+ int2str(year) +",'"+student_semester+"');";
    mysql_query(conn, enroll2.c_str());
    res_set = mysql_store_result(conn);
    mysql_free_result( res_set );
    
    cout << "***  Successfully enrolled!  ***" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Enter 1 to go back to enroll menu" << endl;
    cout << "Enter 2 to go back to student menu" << endl;
    cout << "Your option is : ";
    
    string enroll_option;
    cin >> enroll_option;
    if ( enroll_option == "1" )
    {
        enroll(student_id);
    }
    else if ( enroll_option == "2" )
    {
        student_menu(student_id);
    }
    else {
        cout << "***     Invalid command!     ***" << endl;
        cout << "*** Go back to student menu! ***" << endl << endl;
        student_menu(student_id);
        
    }
}





void withdraw(string student_id)
{
    
    string student_semester;
    string student_semester1;
    int year;
    int year1;
    time_t t = time(0);
    struct tm * now = localtime( &t );
    year = now -> tm_year + 1900;
    student_semester = student_semester_decide( now -> tm_mon);
    year1 = now -> tm_year + 1900 + 1;
    student_semester1 = student_semester_decide( now -> tm_mon + 3);
    
    mysql_close(conn);
    conn = mysql_init ( NULL );
    mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
    
    
    cout << "You can only withdraw the class of the current year and quarter or the following quarter." << endl;
    cout << "Please enter the year and quarter you want to withdraw class" << endl;
    cout << "Year : " ;
    string whichyear;
    cin >> whichyear;
    cout << "Quarter :" ;
    string whichquarter;
    cin >> whichquarter;
    
    if ( (whichyear == int2str(year)) && (whichquarter == student_semester) )
    {
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        
        string withdraw1 = "CALL withdrawclass ("+student_id+","+ int2str(year) +",'"+student_semester+"');" ;
        mysql_query(conn, withdraw1.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        
        if ( numrows != 0 )
        {
            cout << "-----------------------------------" << endl;
            cout << "|           WITHDRAW:             |" << endl;
            cout << "-----------------------------------" << endl;
            cout << " Course can withdraw : " << endl;
            
            for (int i = 0; i < numrows; i++)
            {
                row = mysql_fetch_row(res_set);
                if (row!= NULL)
                {
                    cout << "|" << row[0] << "|" << endl;
                }
            }
        }
        else
        {
            cout << "*** No course can withdraw! ***" << endl;
            cout << "*** Go back to student menu! ***" << endl << endl;
            student_menu(student_id);
        }
        
        mysql_free_result(res_set);
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        cout << "Please enter the course you want to withdraw" << endl;
        string chosecourse;
        cin >> chosecourse;
        
        MYSQL_RES *res_set1;
        string enrollchoice = "CALL whether_right_withdraw('"+chosecourse+"',"+student_id+","+ int2str(year) +",'"+student_semester+"');" ;
        mysql_query(conn, enrollchoice.c_str());
        res_set1 = mysql_store_result(conn);
        int numrows1 = (int)mysql_num_rows(res_set1);
        
        if ( numrows1 == 1)
        {
            mysql_free_result( res_set1 );
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
            
            withdraw_right(chosecourse,student_id,year,student_semester);
            
        }
        else
        {
            mysql_free_result(res_set1);
            cout << "***    Invalid course number!     ***" << endl;
            cout << "*** Go back to the withdraw menu! ***" << endl << endl;
            withdraw( student_id );
        }
        
    }
    
    else if ( (whichyear == int2str(year1)) && (whichquarter == student_semester1) )
    {
        MYSQL_RES *res_set2;
        MYSQL_ROW row;
        
        string withdraw1 = "CALL withdrawclass ("+student_id+","+ int2str(year1) +",'"+student_semester1+"');" ;
        mysql_query(conn, withdraw1.c_str());
        res_set2 = mysql_store_result(conn);
        int numrows2 = (int)mysql_num_rows(res_set2);
        
        if ( numrows2 != 0 )
        {
            cout << "-----------------------------------" << endl;
            cout << "|           WITHDRAW:             |" << endl;
            cout << "-----------------------------------" << endl;
            cout << " Course can withdraw : " << endl;
            
            for (int i = 0; i < numrows2; i++)
            {
                row = mysql_fetch_row(res_set2);
                if (row!= NULL)
                {
                    cout << "|" << row[0] << "|" << endl;
                }
            }
        }
        else
        {
            cout << "*** No course can withdraw! ***" << endl;
            cout << "*** Go back to student menu! ***" << endl << endl;
            student_menu(student_id);
        }
        
        mysql_free_result(res_set2);
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
        
        cout << "Please enter the course you want to withdraw" << endl;
        string chosecourse;
        cin >> chosecourse;
        
        MYSQL_RES *res_set3;
        string enrollchoice = "CALL whether_right_withdraw('"+chosecourse+"',"+student_id+","+ int2str(year1) +",'"+student_semester1+"');" ;
        mysql_query(conn, enrollchoice.c_str());
        res_set3 = mysql_store_result(conn);
        int numrows3 = (int)mysql_num_rows(res_set3);
        
        if ( numrows3 == 1)
        {
            mysql_free_result( res_set3 );
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
            
            withdraw_right (chosecourse,student_id,year1,student_semester1);
        }
        else
        {
            mysql_free_result(res_set3);
            cout << "***    Invalid course number!     ***" << endl;
            cout << "*** Go back to the withdraw menu! ***" << endl << endl;
            withdraw( student_id );
            
        }
        
    }
    else
    {
        
        cout << "*** Please enter valid year and quarter! ***" <<endl << endl;
        withdraw(student_id);
    }
}





void withdraw_right ( string chosecourse ,string student_id,int year,string student_semester)
{
    
    MYSQL_RES *res_set;
    
    string withdraw2="CALL decreaseclass ("+student_id+",'"+chosecourse+"',"+ int2str(year) +",'"+student_semester+"');";
    mysql_query(conn, withdraw2.c_str());
    res_set = mysql_store_result(conn);
    mysql_free_result( res_set );
    
    cout << "*** Successfully withdraw! ***" << endl << endl;
    
    
    MYSQL_RES *res_set10;
    string WARNING = "SELECT * FROM Warning ;";
    mysql_query(conn, WARNING.c_str());
    res_set10 = mysql_store_result(conn);
    int numrows10 = (int)mysql_num_rows(res_set10);
    
    if(numrows10!=0)
    {cout << "*** Warning: Enrollment number goes below 50% of the MaxEnrollment! ***" << endl<< endl;
        mysql_free_result(res_set10);}
    
    cout << "Enter 1 to withdraw another course " << endl << "Enter 2 to go back to memu" << endl;
    cout << "Your option is : ";
    string withdraw_option;
    cin >> withdraw_option;
    if ( withdraw_option == "1" )
    {
        withdraw(student_id);
    }
    else if ( withdraw_option == "2" )
    {
        student_menu(student_id);
    }
    else
    {
        cout << "*** Invalid command! ***" << endl;
        student_menu(student_id);
    }
}



void login()
{
    string student_id;
    string student_password;
    
    cout << "Please enter your studentid:" << endl;
    cin >> student_id;
    cout << "Please enter your password:" << endl;
    cin >> student_password;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string table_id= "SELECT * FROM student where Id="+student_id+";";
    mysql_query(conn,table_id.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    row = mysql_fetch_row( res_set );
    
    if ( numrows == 1 )
    {
        if(row[2] == student_password)
        {
            student_menu(student_id);
        }
        else
        {
            cout << "*** Please enter a valid password! ***" << endl << endl;
            login();
        }
    }
    else
    {
        cout<< "*** Please enter a valid id! ***" << endl << endl;
        login();
    }
    mysql_free_result(res_set);
}


int main ( int argc, char *argv[] )
{
    conn = mysql_init ( NULL );
    mysql_real_connect (conn,"localhost","root","","project3-nudb",3306,NULL,0);
    cout << "-------------------------------------------------------------" << endl;
    cout << "| ———Welcome to the Northwestern Caesar’s academic system—— |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    login();
}