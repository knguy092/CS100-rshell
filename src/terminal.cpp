#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
bool CONTROL=true;
bool prev=true;

//****************************************************
//STAT FUNCTION--MILESTONE#3
//****************************************************
bool stat_function(string command)
{
    try
    {
        vector<string> commandstring;
        string flag, pathname;                              //VARIABLE to store flags and the pathname
        char_separator<char> sepspaces(" ");                //removing spaces using boost libraries
        tokenizer<char_separator<char> > token_spaces(command, sepspaces);  //
        BOOST_FOREACH(string t, token_spaces)               //recompiling the string with exactly one space between
        {                                                   //the words in the string::replicates terminal behaviour
            commandstring.push_back(t);    
        }
        if(commandstring.at(0)=="test" && ( commandstring.at(1)=="-e" || commandstring.at(1)=="-f" || commandstring.at(1)=="-d") ) //CASE 1.0: test -[flags] [pathname]
        {
            flag = commandstring.at(1); 
            pathname = commandstring.at(2);
        }
        else if(commandstring.at(0)=="test" && commandstring.size()==2)                 //CASE 1.1: test [pathname]
        {
            flag="-e";
            pathname=commandstring.at(1);
        }
        else if(commandstring.at(0)=="[")                                               
        {
            bool check_closing_square=false;
            if(commandstring.at(2)=="]" || commandstring.at(3)=="]")// checks for invalid commands or syntax:: without the closing brackets
            {
                check_closing_square=true;
                if(!check_closing_square)
                    return false;
            }
            
            if(commandstring.at(1)=="-e" || commandstring.at(1)=="-f" || commandstring.at(1)=="-d")//CASE 2.0: [-[flags] pathname ] "SQUARE BRACKET OPERATOR"
            {
                flag=commandstring.at(1);
                pathname=commandstring.at(2);
            }
            else                            //CASE 2.0: [ pathname ] "SQUARE BRACKET OPERATOR WITH DEFAULT FLAG -e"
            {
                flag="-e";
                pathname=commandstring.at(1);
            }
            
        }
        else
            return false;       //returns command failure for ever other case
        
        struct stat* c1 = new struct stat;                  //DECLARE a new stat struct for use with stat function
        char * path =  new char [pathname.size()+1];
        for(unsigned i=0; i < pathname.size();i++)          //converts the string to char* 
            path[i]=pathname.at(i);
        path[pathname.size()]='\0';                         //last element of char* a should be a null pointer to signify end and prevent seg faults
        //TEST FEATURE:: TO CHECK THE CORRECTNESS OF THE PATH :: UNCOMMENT TO CHECK IT OUT
        //cout<<path<<endl;
        
        int returnval = stat(path, c1);                     //CRUX_OF_THE_FUNCTION: accepts the path in c-string and passing the stat struct by reference 
        if(returnval == -1){                                //which writes the details of the file in the stat struct c1.
            cout<<"(False)"<<endl;                          //Stat function returns a non zero value for runtime failure
            return false;
        }
        
        int isdir = (c1->st_mode & S_IFDIR);                //checking if flags for S_IFDIR and S_IFREG are set
        int isreg = (c1->st_mode & S_IFREG);                //PRO TIP: 711 are octal numbers, not hex? weird, innit?
        
        if(flag=="-e")                                      //different cases for inferring the type of file.
        {
            cout<<"(True)"<<endl;
            return true;
        }
        else if(flag=="-f" && isreg!=0)
        {
            cout<<"(True)"<<endl;
            return true;
        }
        else if(flag=="-d" && isdir!=0)
        {
            cout<<"(True)"<<endl;
            return true;
        }
        else
        {
            cout<<"(False)"<<endl;
            return false;
        }
        return false;
    }
    catch(std::exception e)                             //entering an improper test command would often result in the improper initialization of vector
    {                                                   //so, I've used exception handling to prevent the program from crashing due to inputs like "[-f./pathname]""
        cout<<"-rshell: invalid test command syntax"<<endl;
        return false;
    }
}
//STAT taken from man pages and examples
//****************************************************

//****************************************************
//EXECUTE FUNCTION NEEDS WORK--CAN BE MADE A LOT SHORTER--WORKS FOR MILESTONE#1
//****************************************************
bool execute(char* a, int &t)   //This function executes the command by accetping a c-string
{                               //t is a testing variable -- will be utilized later
    string scom,sarg;           //string for accepting 'COMMAND' and 'ARGUMENTS'
    int status;                 //TESTING VARIABLE--TO BE USED LATER
    unsigned size=sizeof(a)/sizeof(char);   //FINDING THE SIZE OF char* a
    if(size==0)                             //if size is zero exit program right here
        return false;
    
    
    //flag to make sure only the 'COMMAND' part of the program goes into scom    
    bool flag=false;                        
    for(unsigned i=0;i<size || a[i]!='\0';i++)//converting a to string type variables scom and sarg
    {
        //this snipped makes sure that only the COMMAND goes into the scom part of the program
        if(a[i]==' ' && !flag)  
        {
            flag=true; continue; //once it encounters a space, the rest
        }
        if(flag)
        {
            sarg=sarg+a[i];     // of the string goues in to sarg
            continue;
        }
        scom=scom+a[i]; //as you can see scom only has the command part
    }
    unsigned scom_length=scom.length()+1;   //this extra line has to be put in because C++ does not allow 
    char * command = new char [scom_length];    //variables for declaring an array's size.
    command[scom.length()]='\0';        //the last character is NULL because the cstring needs a null character to signal end..This was a bugfix for execvp not executing 
    unsigned sarg_length=sarg.length()+1;   //the cstring properly
    char * arguments = new char [sarg_length];
    arguments[sarg.length()]= '\0';
    for(unsigned i =0;i<scom.length();i++)  //NOW we convert the scom string to a cstring by iterating through the string and  
    {                                       //assigning each character to its repective place. this complex method has been implemented
        command[i]=scom.at(i);              //because compilation was giving a "string deprecated when converting to char*" error
    }
    for(unsigned i =0;i<sarg.length();i++)
    {
        arguments[i]=sarg.at(i);
    }
    string commandtest=command;
    if(commandtest=="test" || commandtest=="[")
    {
        return stat_function(a);
    }
    char* final_command [3];  //the final_command pointer array goes into the execvp function, which is decribed inthe man
    if(sarg.length()==0)                //pages as follows: execvp(char* final_command[0], final_command )
    {
        final_command[0]=command;       //single word commands do not have arguments
        final_command[1]=NULL;          //hence, the 2nd and thrid positions are filled with null
        final_command[2]=NULL;
    }
    else
    {
        final_command[0]=command;       // multiword commands
        final_command[1]=arguments;
        final_command[2]=NULL;
    }
    //TEST HARNESS:::::cout<<final_command[0]<<" "<<final_command[1]<<" "<<final_command[2]<<endl;
    
    pid_t c_pid, pid;
    c_pid=fork();
    
    if(c_pid==-1)                       //failed fork
    {
        cout<<"fork failed: please try again"<<endl;
        perror("FORK:");
        return false;
    }
    else if(c_pid==0)
    {
        // if(final_command[0]=="exit")
        //     exit(0);
        t=execvp(final_command[0],final_command);
        printf("-rshell: %s: command not found\n", command);
        //cout<<"TEST FOR EXECBRANCH::exit code--> "<<t<<endl;
        exit(t);
    }    
    else
    {
        pid = wait(&status);
        //cout<<"TEST FOR EXECBRANCH::status code--> "<<status<<endl;
        //cout<<"TEST FOR EXECBRANCH::What does pid do? code--> "<<pid<<endl;
        //if (WIFEXITED(status)) {
        //    t =  WEXITSTATUS(status);
        //}
        if(pid){}
        if(status==0)
            return true;
        else 
            return false;
    }
}
//EXECVP taken from man pages and examples
//****************************************************

bool parses_string ( string text)
{
        string nostext;
        char_separator<char> sepspaces(" ");                //removing spaces using boost libraries
        tokenizer<char_separator<char> > token_spaces(text, sepspaces);  //
        bool first_token_added=false;
        BOOST_FOREACH(string t, token_spaces)               //recompiling the string with exactly one space between
        {                                                   //the words in the string::replicates terminal behaviour
            if(!first_token_added)
            {
                first_token_added=true;                      
                nostext=nostext+t;                          //The first token should not have spaces-->since the string is empty the next statement will
            }                                               //add a space before the COMMAND
            else
                nostext=nostext + " " + t;        
        }
        if(nostext=="exit")//********EXPERIMENTAL*****TURN BACK ON IF STOPS WORKING
             exit(0);       //required for extra space exit commands
        if(nostext.length()==0)                             //ANOTHER CASE FOR EMPTY string 
            return false;
        //nostext=nostext+'\0';
        char* final_command=new char[nostext.size() + 1];         //CREATING char* for passing to execute(char* , int&)
        memcpy(final_command, nostext.c_str(), nostext.size() + 1);
        int m;
        
        return execute(final_command, m);
}


//bool helper
bool parse_string ( string text)
{
    if(text.length()==0)                                    //empty string length , returns control back to call
        return true;                                            //This covers the caseof the empty prompt.
    string noctext; //command without the comments
    char * final_command;                                   
    
    char_separator<char> sepcomment("#");                   //SEPARATING THE COMMENTS FROM THE INPUT---strip it off
    tokenizer<char_separator<char> > token_comment(text, sepcomment);   //TO BE REVIEWED--WHEN QUOTATION MARKS COME INTO PLAY
    bool strip_comment=false;                               
    BOOST_FOREACH(string t, token_comment)
    {
        if(!strip_comment)
        {
            noctext=t;
            strip_comment=true;
        }
    }
    
    // cout<<"noctext"<<endl;
    bool i1=noctext.find("&&") == std::string::npos;        //checking for presence of &&
    bool i2=noctext.find("||") == std::string::npos;        //                         ||
    bool i3=noctext.find(";") == std::string::npos;         //                          ;
    // cout<<i1<<i2<<i3<<endl;
    if( i1 && i2 && i3 )                                    //CASE 1: single commands
    {
        string nostext;
        char_separator<char> sepspaces(" ");                //removing spaces using boost libraries
        tokenizer<char_separator<char> > token_spaces(noctext, sepspaces);  //
        bool first_token_added=false;
        BOOST_FOREACH(string t, token_spaces)               //recompiling the string with exactly one space between
        {                                                   //the words in the string::replicates terminal behaviour
            if(!first_token_added)
            {
                first_token_added=true;                      
                nostext=nostext+t;                          //The first token should not have spaces-->since the string is empty the next statement will
            }                                               //add a space before the COMMAND
            else
                nostext=nostext + " " + t;        
        }
        if(nostext=="exit")//********EXPERIMENTAL*****TURN BACK ON IF STOPS WORKING
             exit(0);       //required for extra space exit commands
        if(nostext.length()==0)                             //ANOTHER CASE FOR EMPTY string 
            return true;
        //nostext=nostext+'\0';
        final_command=new char[nostext.size() + 1];         //CREATING char* for passing to execute(char* , int&)
        memcpy(final_command, nostext.c_str(), nostext.size() + 1);
        int m;
        return execute(final_command, m);
        
    }
    else if ( i1 && i1 && !i3)                              //CASE 2: multiple commands chained only with ';'
    {
        string nosctext;                                    //using boost to split the string with semi colons
        char_separator<char> sepsemicolon(";");
        tokenizer<char_separator<char> > token_semicolon(noctext, sepsemicolon);
        BOOST_FOREACH(string t, token_semicolon)
        {
            return parse_string(t);                                //Each segmented string without semi colons is passed into
        }                                                   //parse_string() again----RECURSIVE
        
    }
    
    else if( (!i1 || !i2) && i3 )                       //CASE 3: multiple commands chained with connectors
    {
        vector<char> connector;
        
        for(unsigned i=0;i<noctext.length()-1;)         //we traverse the vector for connectors at several places within the program.
        {
            if(noctext.at(i)=='&' && noctext.at(i+1)=='&')  //each instance of && is found and then added to the connector vector
            {
                connector.push_back('&');
                i=i+2;
                continue;
            }
            if(noctext.at(i)=='|' && noctext.at(i+1)=='|')  //the same is done for ||. This connector vector will be later used to execute the commands
            {                                               //one by one and, decided the execurtion of the next command by taking into consideration
                connector.push_back('|');                   //the next connector that occurs and the result of the previous command (succesful run is 
                i=i+2;                                      //'true' and an unsuccessful run returns false)(see bool execute)
                continue;
            }
            else{
                i++;
            }
        }
        connector.push_back('\0'); // adding a null character to signify end of vector
        
        bool yflag=true;                                    //flag to signify first command execution--since the first command will set bool prev to true or false 
                                                            //to denote a successful run
        char_separator<char> sepnotamp("&|");                               //separating && and || using BOOST TOKENIZER
        tokenizer<char_separator<char> > token_notamp(noctext,sepnotamp);   //BUG:: CANNOT DIFFERENTIATE BETWEEN '&' and'&&''  and '|' and '||''
        unsigned i=0;
        BOOST_FOREACH(string t, token_notamp)
        {
            if(i>connector.size()-1)                        //safeguard program execution by not going OUT_OF_BOUNDS for vector <char> connector 
                break;
            if(yflag)                   
            {
                prev=parses_string(t);
                yflag=false;
                continue;
            }
            if(connector.at(i)=='&' && prev==true)          //CONDITION FOR AND:: prev should be true
            {
                prev=parses_string(t);
                i++;
            }
            else if(connector.at(i)=='|' && prev==false)  //CONDITION FOR OR:: prev should be false
            {
                prev=parses_string(t);
                i++;
            }
            else                                        //SECTION for skipped commands:: the command that shouldn't be executed because the prev 
            {                                           //and CONNECTOR do not satisfy CONDITION for execution
                i++;
                continue;   
            }
            
        }
        return prev;
    }
    else if( (!i1 || !i2) && !i3 )                      //CASE 4: multiple commands chained together with semicolons and connectors.
    {                                                   //TASK::TO SEPARATE all commands connected by semi colon.
        string nosctextNOTAMP;                                    //using boost to split the string with semi colons
        char_separator<char> sepsemicolonNOTAMP(";");
        tokenizer<char_separator<char> > token_semicolonNOTAMP(noctext, sepsemicolonNOTAMP);
        BOOST_FOREACH(string t, token_semicolonNOTAMP)
        {
            return parse_string(t);                                //Each segmented string without semi colons is passed into
        }                                                   //parse_string() again----RECURSIVE

    }
    return false;
}
//****************************************************
//PARSE_BRACKETS FUNCTION--MILESTONE#4
//****************************************************
bool parse_brackets (string text) {                 //PARSING BRACKETS::TO BE COMMENTED
    int pcounter = 0;
    int beginning = 0;
    int end = 0;
    bool yesp = false;
    bool endreached = false;
    bool first_command_no_p = false;
    bool nosecondpart = false;
    
    for (unsigned i = 0; i < text.size(); i++) 
    {
        if (!first_command_no_p && !yesp) {
            if (text.at(i) == '&' && text.at(i + 1) == '&') {
               first_command_no_p = true;
               end = i;
            }
            if (text.at(i) == '|' && text.at(i + 1) == '|') {
               first_command_no_p = true;
               end = i;
            }
        }
        if (!first_command_no_p && text.at(i) == '(')
        {
            if (pcounter == 0)
            {
                beginning = i + 1;
            }
            yesp = true;
            pcounter++;
        }
        if (!first_command_no_p && text.at(i) == ')') {
            if (pcounter == 1) 
            {
                end = i;
                endreached = true;
            }
            if (i == (text.size() - 1)) {
                nosecondpart = true;
            }
            pcounter--;
        }
        if (endreached)
        {
            i = text.size();
        }
    }
    
    if (!yesp && !first_command_no_p)
    {
        return parse_string(text);
    }
    
    string newtext = text.substr (beginning, (end - beginning));
    //cout << "first part: " << newtext << endl;

    bool returnvalue = parse_brackets(newtext);
    if (nosecondpart) {
        return returnvalue;
    }
    
    bool enable = true;
    for (unsigned i = end; i < text.size(); i++) {
        if (enable && text.at(i) == '&' && text.at(i + 1) == '&')
        {
            end = i + 2;
            enable = false;
        }
        if (enable && text.at(i) == '|' && text.at(i + 1) == '|')
        {
            if (returnvalue) {
                return true;
            }
            end = i + 2;
            enable = false;
        }
    }
    
    string temp = text.substr (end, text.size() - end);
    //cout << "second part: "<< temp << endl;
    
    return parse_brackets (temp);
}

int main()
{
    string t;               //accepting command line input 
    char* login=getlogin(); //getting the login
    char hostname[1024];    //hostname
    hostname[1023]='\0';    //CONVENTIONS FOR OBTAINING HOSTNAME
    int hostname_affirm=gethostname(hostname,30);
    while(CONTROL)
    {
        if(login==NULL || hostname_affirm!=0)   //checking if hostname is available
        cout<<"$ ";         //DISPLAYING THE DOLLAR PROMPT IF hostname is not available (like in cloud9)
        else
        cout<<login<<"@"<<hostname<<"$ ";   //display prompt if prompt is available
        getline(cin,t);
        if(t=="exit")
            exit(0);
        parse_brackets(t);
    }
    return 0;
}
